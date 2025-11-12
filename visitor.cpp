#include <iostream>
#include "ast.h"
#include "visitor.h"
#include <unordered_map>
using namespace std;
#include <typeinfo>

///////////////////////////////////////////////////////////////////////////////////

string BinaryExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string NumberExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string Program::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string IdExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string strExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
string PrintStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string AssignStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string IfStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string WhileStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string Body::accept(Visitor* visitor){
    return visitor->visit(this);
}


string VarDec::accept(Visitor* visitor){
    return visitor->visit(this);
}

string FcallExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}

string FunDec::accept(Visitor* visitor){
    return visitor->visit(this);
}

string ReturnStm::accept(Visitor* visitor){
    return visitor->visit(this);
}


///////////////////////////////////////////////////////////////////////////////////

int GenCodeVisitor::generar(Program* program) {

    program->accept(this);
    return 0;
}

string GenCodeVisitor::visit(Program* program) {
out << ".data\nprint_fmt: .string \"%ld \\n\""<<endl;
    out<<"fmt_str: .string \"%s\\n\""<<endl;


    for (auto dec : program->vdlist){
        dec->accept(this);
    }

    for (auto& [var, _] : memoriaGlobal) {
        out << var << ": .quad 0"<<endl;
    }

    out << ".text\n";
    for (auto dec : program->fdlist){
        dec->accept(this);
    }

    out << ".section .note.GNU-stack,\"\",@progbits"<<endl;
        return string{};
}

string GenCodeVisitor::visit(VarDec* stm) {
    for (auto var : stm->vars) {
        if (!entornoFuncion) {
            memoriaGlobal[var] = true;
        } else {
            memoria[var] = offset;
            offset -= 8;
        }
    }
        return string{};
}

string GenCodeVisitor::visit(strExp* str){
    out<<"movq $"<<str->value.size()+1<<", %rdi\n";
    out<< "call malloc@PLT\n";
    out<<"movq %rax,%rbx\n";
    for(char c:str->value){
        out<<"movb $'"<<c<<"',(%rbx)\n";
        out<<"addq $1,%rbx\n";

    }
    out<<"movb $0,(%rbx)\n";
    return string{};
}

string GenCodeVisitor::visit(NumberExp* exp) {
    out << " movq $" << exp->value << ", %rax"<<endl;
    return string{};
}

string GenCodeVisitor::visit(IdExp* exp) {
    if (memoriaGlobal.count(exp->value))
        out << " movq " << exp->value << "(%rip), %rax"<<endl;
    else
        out << " movq " << memoria[exp->value] << "(%rbp), %rax"<<endl;
    return string{};
}

string GenCodeVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    out << " pushq %rax\n";
    exp->right->accept(this);
    out << " movq %rax, %rcx\n popq %rax\n";

    switch (exp->op) {
        case PLUS_OP:  out << " addq %rcx, %rax\n"; break;
        case MINUS_OP: out << " subq %rcx, %rax\n"; break;
        case MUL_OP:   out << " imulq %rcx, %rax\n"; break;
        case LE_OP:
            out << " cmpq %rcx, %rax\n"
                      << " movl $0, %eax\n"
                      << " setle %al\n"
                      << " movzbq %al, %rax\n";
            break;
    }
    return string{};
}


string GenCodeVisitor::visit(AssignStm* stm) {
    stm->e->accept(this);
    if (memoriaGlobal.count(stm->id))
        out << " movq %rax, " << stm->id << "(%rip)"<<endl;
    else
        out << " movq %rax, " << memoria[stm->id] << "(%rbp)"<<endl;
            return string{};
}

string GenCodeVisitor::visit(PrintStm* stm) {
    string tipo=stm->e->accept(tc);
//    cout<<tc->tipos.size()<<endl;
//    cout<<"imprimendo: "<< tipo<<endl;
    stm->e->accept(this);
    if (tipo=="str")
        out<<
                            " leaq fmt_str(%rip), %rdi   \n"
                            "  movq %rax, %rsi   \n"
                            "call printf@PLT      \n"        ;


    else
        out <<
            " movq %rax, %rsi\n"
            " leaq print_fmt(%rip), %rdi\n"
            " movl $0, %eax\n"
            " call printf@PLT\n";

    return string{};
}



string GenCodeVisitor::visit(Body* b) {
    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
        return string{};
}

string GenCodeVisitor::visit(IfStm* stm) {
    int label = labelcont++;
    stm->condition->accept(this);
    out << " cmpq $0, %rax"<<endl;
    out << " je else_" << label << endl;
   stm->then->accept(this);
    out << " jmp endif_" << label << endl;
    out << " else_" << label << ":"<< endl;
    if (stm->els) stm->els->accept(this);
    out << "endif_" << label << ":"<< endl;
    return string{};
}

string GenCodeVisitor::visit(WhileStm* stm) {
    int label = labelcont++;
    out << "while_" << label << ":"<<endl;
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je endwhile_" << label << endl;
    stm->b->accept(this);
    out << " jmp while_" << label << endl;
    out << "endwhile_" << label << ":"<< endl;
    return string{};
}


string GenCodeVisitor::visit(ReturnStm* stm) {
    stm->e->accept(this);
    out << " jmp .end_"<<nombreFuncion << endl;
    return string{};
}

string GenCodeVisitor::visit(FunDec* f) {
    entornoFuncion = true;
    memoria.clear();
    offset = -8;
    nombreFuncion = f->nombre;
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    out << ".globl " << f->nombre << endl;
    out << f->nombre <<  ":" << endl;
    out << " pushq %rbp" << endl;
    out << " movq %rsp, %rbp" << endl;
    out << " subq $" << tc->vars_per_funct[f->nombre]*8 << ", %rsp" << endl;
    int size = f->Pnombres.size();
    for (int i = 0; i < size; i++) {
        memoria[f->Pnombres[i]]=offset;
        out << " movq " << argRegs[i] << "," << offset << "(%rbp)" << endl;
        offset -= 8;
    }
    for (auto i: f->cuerpo->declarations){
        i->accept(this);
    }
    int reserva = -offset - 8;

    for (auto i: f->cuerpo->StmList){
        i->accept(this);
    }
    out << ".end_"<< f->nombre << ":"<< endl;
    out << "leave" << endl;
    out << "ret" << endl;
    entornoFuncion = false;

//    entornoFuncion = true;
//    memoria.clear();
//    offset = -8;
//    nombreFuncion = f->nombre;
//    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
//    out << ".globl " << f->nombre << endl;
//    out << f->nombre <<  ":" << endl;
//    out << " pushq %rbp" << endl;
//    out << " movq %rsp, %rbp" << endl;
//    int size = f->Pnombres.size();
//    for (int i = 0; i < size; i++) {
//        memoria[f->Pnombres[i]]=offset;
//        out << " movq " << argRegs[i] << "," << offset << "(%rbp)" << endl;
//        offset -= 8;
//    }
//    for (auto i: f->cuerpo->declarations){
//        i->accept(this);
//    }
//    int reserva = -offset - 8;
//    out << " subq $" << reserva << ", %rsp" << endl;
//    for (auto i: f->cuerpo->StmList){
//        i->accept(this);
//    }
//    out << ".end_"<< f->nombre << ":"<< endl;
//    out << "leave" << endl;
//    out << "ret" << endl;
//    entornoFuncion = false;
    return string{};
}

string GenCodeVisitor::visit(FcallExp* exp) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int size = exp->argumentos.size();
    for (int i = 0; i < size; i++) {
        exp->argumentos[i]->accept(this);
        out << " mov %rax, " << argRegs[i] <<endl;
    }
    out << "call " << exp->nombre << endl;
    return string{};
}

int Typechecker::generar(Program *program) {
    p=program;
    program->accept(this);

    return 0;
}

string Typechecker::visit(BinaryExp *exp) {
    string l=exp->left->accept(this);
    string r=exp->right->accept(this);
    if(l!="i64" or r!="i64"){
        throw runtime_error("Tipos invalidos para binary op"+Exp::binopToChar(exp->op));

    }
    switch (exp->op) {
        case PLUS_OP:  ;
        case MINUS_OP: ;
        case MUL_OP:  ;
        case DIV_OP:;
        case POW_OP:
            return "i64";
        default:
            return "bool";
    }
}

string Typechecker::visit(NumberExp *exp) {
    return "i64";
}
string Typechecker::visit(Program *program) {

    for(auto& vd: program->vdlist){
        vd->accept(this);
    }
    for(auto& vd: program->fdlist){
        vd->accept(this);
    }
    return "void";
}
string Typechecker::visit(IdExp *exp) {
    return tipos[exp->value];
}


string Typechecker::visit(PrintStm *stm) {
    return "void";
}

string Typechecker::visit(AssignStm *stm) {
    if(tipos[stm->id]=="undefined")
        tipos[stm->id]=stm->e->accept(this);
    else if(tipos[stm->id]!= stm->e->accept(this))
        throw runtime_error("Tipo invalido para la variable: "+stm->id);

    return "void";
}

string Typechecker::visit(WhileStm *stm) {
    stm->condition->accept(this);
    stm->b->accept(this);
    return "void";
}

string Typechecker::visit(IfStm *stm) {
    int a = locales;
    stm-> then -> accept(this);
    int b = locales;
    stm-> els  -> accept(this);
    int c = locales;
    locales = a + max(b-a,c-b);


    return "void";
}

string Typechecker::visit(Body *body) {
    for(auto& vd: body->declarations){
        vd->accept(this);
    }
    for(auto& stm: body->StmList){  //en un body si es que hay return debe estar al ultimo
        stm->accept(this);
    }

    return "void";
}

string Typechecker::visit(VarDec *vd) {
    locales += vd->vars.size();

    for(const string& v:vd->vars){
        tipos[v]=vd->type;
    }
    return "void";
}

string Typechecker::visit(FcallExp *fcall) {
    FunDec* f=nullptr;
    for(FunDec* fun:p->fdlist){
        if(fcall->nombre==fun->nombre)
            f=fun;
    }
    if(!f)
        throw runtime_error("Nombre equivocado de funcion: "+fcall->nombre);

    if(f->Ptipos.size()!=fcall->argumentos.size())
        throw runtime_error("Numero equivocado de argumentos para la funcion: "+fcall->nombre);
    string t;
    for(int i=0;i<fcall->argumentos.size();i++){
        t=fcall->argumentos[i]->accept(this);
        if( t!= f->Ptipos[i]){
            throw runtime_error("Tipo invalido en la llamada de funcion: "+fcall->nombre);

        }
    }
    return tipos[fcall->nombre];
}

string Typechecker::visit(ReturnStm *r) {
     string ret=r->e->accept(this);
     if(ret!=tipos[nombreFuncion]){
         throw runtime_error("Tipo de retorno invalid para la funcion: "+nombreFuncion);
     }
     return ret;
}

string Typechecker::visit(FunDec *fd) {
    int parametros = fd->Pnombres.size();
    tipos[fd->nombre]=fd->tipo;
    nombreFuncion=fd->nombre;

    locales = 0;
    fd->cuerpo->accept(this);
    vars_per_funct[fd->nombre] = parametros + locales;


    return fd->tipo;
}

string Typechecker::visit(strExp *str) {
    return "str";
}
