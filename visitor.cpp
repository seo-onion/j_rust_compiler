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
string lenExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
string pushStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}
string BoolExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
string AssignPStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}
string accesExp::accept(Visitor* visitor) {
    return visitor->visit(this);
}
string FcallStm::accept(Visitor* visitor) {
    return visitor->visit(this);
}
string arrExp::accept(Visitor* visitor) {
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
    memoria.add_level();
    program->accept(this);
    memoria.remove_level();
    return 0;
}

string GenCodeVisitor::visit(Program* program) {
    memoria.add_level();
    out << ".data\nprint_fmt: .string \"%ld \\n\""<<endl;
    out<<R"(fmt_str: .string "%s\n")"<<endl;
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
    memoria.remove_level();
        return string{};
}

string GenCodeVisitor::visit(VarDec* stm) {
    for (auto var : stm->vars) {
        if (!entornoFuncion) {
            memoriaGlobal[var] = true;
        } else {
            if(tc->tipos[var]=="i64" or tc->tipos[var]=="bool") {
                memoria.add_var(var, offset);
                offset -= 8;
            }
            else{
                offset-=16;
                memoria.add_var(var, offset+8);
            }
        }
    }
        return string{};
}

string GenCodeVisitor::visit(accesExp* stm){
    string tipo=stm->accept(tc);
    int type_size=16;
    if(tipo=="bool" or tipo=="i64"){
        type_size=8;
    }
    int cont=1;
    int base=memoria.lookup(stm->variable);
    out << "movq " << base << "(%rbp), %r8"<<endl;    //variable
    for(Exp* e:stm->indexes){
        e->accept(this); //en el rax queda el offset en variables
        if(cont!=stm->indexes.size()){
            out<<"movq $16,%rbx\n";
        }else {
            out << "movq $" << type_size << ",%rbx\n";
        }
        out<<"imulq %rbx,%rax\n";
        out<<"addq %r8, %rax \n";
        out<<"movq (%rax),%r8\n";
        cont+=1;
    }
    if(type_size==16){
        out<<"movq 8(%rax),%rcx\n"; // tamanho del array o string de retorno
    }
    out<<"movq %rax,%rbx\n"; //puntero al valor esta en rbx
    out<<"movq %r8,%rax\n";
    return string{};
}

string GenCodeVisitor::visit(AssignPStm* stm) {
    stm->arr->accept(this);
    out<<"pushq %rbx\n";
    stm->e->accept(this);
    string t=stm->e->accept(tc);
    out<<"popq %rbx\n";
    out<<"movq %rax,(%rbx)\n";
    if(!(t=="bool" or t=="i64")){
        out<<"movq %rcx,8(%rbx)\n";
    }


    return string{};
}

string Typechecker::visit(AssignPStm* stm){
    string t1=stm->arr->accept(this);
    string t2=stm->e->accept(this);
    if(t1!=t2){
        throw runtime_error("asignacion invalida de : "+stm->arr->variable );
    }
    return "void";
}

string GenCodeVisitor::visit(NumberExp* exp) {
    out << " movq $" << exp->value << ", %rax"<<endl;
    return string{};
}
string GenCodeVisitor::visit(BoolExp* exp) {
    out << " movq $" << exp->value << ", %rax"<<endl;
    return string{};
}



string GenCodeVisitor::visit(BinaryExp* exp) {
    exp->left->accept(this);
    out << "pushq %rax\n";
    exp->right->accept(this);
    out << "movq %rax, %rcx\n popq %rax\n";
    switch (exp->op) {
        case AND_op:
            out<<"andq %rcx,%rax\n";
            break;
        case ORR_op:
            out<<"orq %rcx,%rax\n";
            break;
        case PLUS_OP:  out << "addq %rcx, %rax\n"; break;
        case MINUS_OP: out << "subq %rcx, %rax\n"; break;
        case MUL_OP:   out << "imulq %rcx, %rax\n"; break;
        case LE_OP:
            out << "cmpq %rcx, %rax\n"
                      << "movl $0, %eax\n"
                      << "setl %al\n"
                      << "movzbq %al, %rax\n";
            break;
        case LEEQ_OP:
            out << "cmpq %rcx, %rax\n"
                << "movl $0, %eax\n"
                << "setle %al\n"
                << "movzbq %al, %rax\n";
            break;
        case GR_OP:
            out << "cmpq %rcx, %rax\n"
                << "movl $0, %eax\n"
                << "setg %al\n"
                << "movzbq %al, %rax\n";
            break;
        case GREQ_OP:
            out << "cmpq %rcx, %rax\n"
                << "movl $0, %eax\n"
                << "setge %al\n"
                << "movzbq %al, %rax\n";
            break;


    }
    return string{};
}


string GenCodeVisitor::visit(AssignStm* stm) {
    stm->e->accept(this);
    if (memoriaGlobal.count(stm->id))
        out << "movq %rax, " << stm->id << "(%rip)"<<endl;
    else if(tc->tipos[stm->id]=="bool" or tc->tipos[stm->id]=="i64"){
        out << "movq %rax, " << memoria.lookup(stm->id) << "(%rbp)" << endl;
    }
    else {
        out << "movq %rcx, " << memoria.lookup(stm->id)+8 << "(%rbp)" << endl;
        out << "movq %rax, " << memoria.lookup(stm->id) << "(%rbp)" << endl;
    }
    return string{};
}

string GenCodeVisitor::visit(PrintStm* stm) {
    string tipo=stm->e->accept(tc);
    stm->e->accept(this);
    if (tipo=="str")
        out<<
        "leaq fmt_str(%rip), %rdi   \n"
        "movq %rax, %rsi   \n"
        "movl $0, %eax\n"
        "call printf@PLT      \n"        ;


    else
        out <<
            "movq %rax, %rsi\n"
            "leaq print_fmt(%rip), %rdi\n"
            "movl $0, %eax\n"
            "call printf@PLT\n";
    return string{};
}



string GenCodeVisitor::visit(Body* b) {
    memoria.add_level();
    for (auto dec : b->declarations){
        dec->accept(this);
    }
    for (auto s : b->StmList){
        s->accept(this);
    }
    memoria.remove_level();
        return string{};
}

string GenCodeVisitor::visit(IfStm* stm) {
    int label = labelcont++;
    if(stm->condition->type()=="BoolExp"){
        if(((BoolExp*)stm->condition)->value) {
            out << "# se compila directamente body del if porque, la condicion siempre es verdadera" << endl;
            stm->then->accept(this);
        }
        else if (stm->els) {
            out << "# se compila directamente body del else porque, la condicion siempre es falsa" << endl;
            stm->els->accept(this);
        }
        return string{};
    }
    stm->condition->accept(this);
    out << " cmpq $0, %rax" << endl;
    out << " je else_" << label << endl;
    stm->then->accept(this);
    out << " jmp endif_" << label << endl;
    out << " else_" << label << ":" << endl;
    if (stm->els) stm->els->accept(this);
    out << "endif_" << label << ":" << endl;
    return string{};
}

string GenCodeVisitor::visit(WhileStm* stm) {
    int label = labelcont++;
    if(stm->condition->type()=="BoolExp"){
        if(((BoolExp*)stm->condition)->value){
            out << "# Se compila directamente el cuerpo del while porque la consicion siempre es verdadera"<<endl;
            out << "while_" << label << ":"<<endl;
            stm->b->accept(this);
            out << "while_" << label << ":"<<endl;
        }
        else
            out << "# no se compila elbody del while porque la condicion siempre es falsa"<<endl;

        return string{};

    }
    out << "while_" << label << ":"<<endl;
    stm->condition->accept(this);
    out << "cmpq $0, %rax" << endl;
    out << "je endwhile_" << label << endl;
    stm->b->accept(this);
    out << "jmp while_" << label << endl;
    out << "endwhile_" << label << ":"<< endl;
    return string{};
}


string GenCodeVisitor::visit(ReturnStm* stm) {
    stm->e->accept(this);
    out << " jmp .end_"<<nombreFuncion << endl;
    return string{};
}
string GenCodeVisitor::visit(strExp* str){
    out<<"movq $"<<str->value.size()+1<<", %rdi\n";
    out<<"call malloc@PLT\n";
    int cont=0;
    for(char c:str->value){
        out<<"movb $'"<<c<<"',"<<cont<<"(%rax)\n";
        cont++;
    }
    out<<"movb $0,"<<cont<<"(%rax)\n";
    out<<"movq $"<<str->value.size()<<",%rcx\n";  //guardo el size en rcx
    return string{};
}
string GenCodeVisitor::visit(IdExp* exp) {
    if (memoriaGlobal.count(exp->value))
        out << "movq " << exp->value << "(%rip), %rax"<<endl;
    else {
        out << "movq " << memoria.lookup(exp->value) << "(%rbp), %rax" << endl;
        if(!(tc->tipos[exp->value]=="i64" or tc->tipos[exp->value]=="bool")) {
            out << "movq " << memoria.lookup(exp->value) + 8 << "(%rbp), %rcx" << endl;

        }
    }
    return string{};
}
string GenCodeVisitor::visit(lenExp* exp){
    exp->container->accept(this);
    out<<"mov %rcx,%rax\n";
    return string{};
}

string GenCodeVisitor::visit(pushStm* stm){
    int size=16;
    string t=stm->p->accept(tc);
    if(t=="bool" or t=="i64"){
        size=8;
    }
    if(stm->vector->type()=="IdExp"){
        int base=memoria.lookup(((IdExp*)stm->vector)->value);
        out<<"# le hago push a una variable del stack\n";
        out<<"movq %rbp,%rbx\n";
        out<<"addq $"<<base<<",%rbx\n";
        out<<"movq 8(%rbx),%rcx\n";
        out<<"movq (%rbx),%rax\n";
    }
    else
        stm->vector->accept(this);// en rax esta el puntero, en rcx el tamanho y en rbx la ubicacion del puntero
    int label=labelcont++;
    out<<"pushq %rax\n"
         "movq %rcx,%rdi\n"
         "addq $1,%rdi\n"
         "movq %rdi,8(%rbx)\n"
         "movq $"<<size<<",%rcx\n"
         "imulq %rcx,%rdi\n"
         "call malloc@PLT\n"
         "movq %rax,(%rbx)\n"  //hasta aqui ya se actualizo el tamanho del array pero no se copiaron los valores


         "popq %rax\n"         //en rax esta el arreglo que se debe copiar y rcx no tiene nigun valor util
         "movq 8(%rbx),%rcx\n"
         "movq (%rbx),%rbx\n"
         "movq $"<<size<<",%r8\n"
         "imulq %r8,%rcx\n"
         "subq $"<<size<<",%rcx\n"
         "addq %rax,%rcx\n"



         "for"<<label<<":\n"
         "cmpq %rax,%rcx\n"
         "je endfor"<<label<<"\n"
         "movq (%rax),%r8\n"
         "movq %r8,(%rbx)\n"
         "addq $8,%rax\n"
         "addq $8,%rbx\n"
         "jmp for"<<label<<"\n"
         "endfor"<<label<<":\n";
    stm->p->accept(this);// tengo el valor en el rax y el tamanho en rcx si es que tiene
    out<<"movq %rax,(%rbx)\n";
    if(size==16)
        out<<"movq %rcx,8(%rbx)\n";








    return string{};
}

string GenCodeVisitor::visit(arrExp* arr){
   // out<<"#se visita un array exp\n";
    int type_size=16;
    if(arr->tipo=="bool" or arr->tipo=="i64"){
        type_size=8;
    }
    out<<"pushq %rbx\n";
    out<<"movq $"<<arr->elements.size()*type_size<<", %rdi\n";
    out<<"call malloc@PLT\n";
    out<<"movq %rax,%rbx\n";
    int cont=0;
    for(Exp* exp:arr->elements){
        exp->accept(this);
        out<<"movq %rax,"<<cont<<"(%rbx)\n";
        if(type_size==16){
            out<<"movq %rcx,"<<cont+8<<"(%rbx)\n";
        }
        cont+=type_size;
    }
    out<<"movq $"<<arr->elements.size()<<",%rcx\n";  //guardo el size en rcx
    out<<"movq %rbx,%rax\n";
    out<<"popq %rbx\n";

    return string{};
}


string GenCodeVisitor::visit(FunDec* f) {
    entornoFuncion = true;
    memoria.clear();
    memoria.add_level();
    offset = -8;
    nombreFuncion = f->nombre;
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    out << ".globl " << f->nombre << endl;
    out << f->nombre <<  ":" << endl;
    out << "pushq %rbp" << endl;
    out << "movq %rsp, %rbp" << endl;
    out << "subq $" << tc->bytes_per_funct[f->nombre]<< ", %rsp" << endl;
    int size = 0;
    for(string t:f->Ptipos){
        if(t=="bool" or t=="i64")
            size+=1;
        else
            size+=2;
    }

    for (int i = 0; i < size; i++) {
        if(f->Ptipos[i]=="bool" or f->Ptipos[i]=="i64") {
            out << "movq " << argRegs[i] << "," << offset << "(%rbp)" << endl;
            memoria.add_var(f->Pnombres[i], offset);
            offset -= 8;
        }
        else{
            out << "movq " << argRegs[i] << "," << offset << "(%rbp)" << endl;
            offset-=8;
            memoria.add_var(f->Pnombres[i], offset);
            out << "movq " << argRegs[++i] << "," << offset << "(%rbp)" << endl;
            offset-=8;
        }
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
    memoria.remove_level();
    return string{};
}

string GenCodeVisitor::visit(FcallExp* exp) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int size = exp->argumentos.size();
    int j=0;
    for (int i = 0; i < size; i++) {
        string t=exp->argumentos[i]->accept(tc);
        if(t=="bool" or t=="i64"){
            exp->argumentos[i]->accept(this);
            out << "mov %rax, " << argRegs[j] <<endl;
            j++;
        }
        else{
            exp->argumentos[i]->accept(this);
            out << "mov %rcx, " << argRegs[j] <<endl;
            out << "mov %rax, " << argRegs[j+1] <<endl;
            j+=2;

        }
    }
    out << "call " << exp->nombre << endl;
    return string{};
}
string GenCodeVisitor::visit(FcallStm* stm) {
    vector<std::string> argRegs = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};
    int size = stm->argumentos.size();
    int j=0;
    for (int i = 0; i < size; i++) {
        string t=stm->argumentos[i]->accept(tc);
        if(t=="bool" or t=="i64"){
            stm->argumentos[i]->accept(this);
            out << "mov %rax, " << argRegs[j] <<endl;
            j++;
        }
        else{
            stm->argumentos[i]->accept(this);
            out << "mov %rcx, " << argRegs[j] <<endl;
            out << "mov %rax, " << argRegs[j+1] <<endl;
            j+=2;
        }
    }
    out << "call " << stm->nombre << endl;
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
    switch (exp->op) {
        case AND_op:
        case ORR_op:
            if(l!="bool" or r!="bool"){
                throw runtime_error("Tipos invalidos para binary op"+Exp::binopToChar(exp->op));
            }
            return "bool";
        case PLUS_OP:
        case MINUS_OP:
        case MUL_OP:
        case DIV_OP:
        case POW_OP:
            if(l!="i64" or r!="i64"){
                throw runtime_error("Tipos invalidos para binary op"+Exp::binopToChar(exp->op));
            }
            return "i64";
        default:
            if(l!="i64" or r!="i64"){
                throw runtime_error("Tipos invalidos para binary op"+Exp::binopToChar(exp->op));

            }
            return "bool";
    }
}

string Typechecker::visit(NumberExp *exp) {
    return "i64";
}

string Typechecker::visit(arrExp *exp) {
    exp->tipo=exp->elements.front()->accept(this);
    for(Exp* e:exp->elements){
        if(exp->tipo!=e->accept(this)){
            throw runtime_error("Tipo invalido para vector, se esperaba "+exp->tipo+" y se tiene: "+e->accept(this));
        }
    }
    return "Vec<"+exp->tipo+">";
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
    if(tipos[stm->id]=="undefined"){
        string t=stm->e->accept(this);
        tipos[stm->id]=t;
        if(t=="i64" or t=="bool")
            locales+=8;
        else
            locales+=16;

    }
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
    if(stm->els)
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
    int s;
    if(vd->type=="undefined") {
        s=0;
    }else if(vd->type=="bool" or vd->type=="i64"){
        s=8;
    }else {
        s=16;
    }
    locales += vd->vars.size()*s;
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

string Typechecker::visit(FcallStm *fcall) {
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
            cout<<t<<"  -  "<<f->Ptipos[i];

            throw runtime_error("Tipo invalido en la llamada de funcion: "+fcall->nombre);

        }
    }
    return "void";
}
string Typechecker::visit(ReturnStm *r) {
     string ret=r->e->accept(this);
     if(ret!=tipos[nombreFuncion]){
         throw runtime_error("Tipo de retorno invalid para la funcion: "+nombreFuncion);
     }
     return ret;
}

string Typechecker::visit(FunDec *fd) {
    int parametros=0;
    int i=0;
    for (string t: fd->Ptipos){
        if(t=="bool" or t=="i64")
            parametros+=8;
        else
            parametros += 16;
        tipos[fd->Pnombres[i]]=t;
        i+=1;

    }
    tipos[fd->nombre]=fd->tipo;
    nombreFuncion=fd->nombre;
    locales = 0;
    fd->cuerpo->accept(this);
    bytes_per_funct[fd->nombre] = parametros + locales;
    return fd->tipo;
}
string Typechecker::visit(lenExp* exp) {
    string t=exp->container->accept(this);
    if(t=="bool" or t=="i64")
        throw runtime_error(".len() se debe usar en un vector o string");
    return "i64";
}
string Typechecker::visit(strExp *str) {
    return "str";
}
string Typechecker::visit(BoolExp *str) {
    return "bool";
}
string Typechecker::visit(pushStm *stm) {
    string vt=stm->vector->accept(this);
    string t=stm->p->accept(this);

    if(vt.size()<=5 or vt.substr(4,vt.size()-5)!=t){
        //cout<<vt.substr(4,vt.size()-5)<<"  ---  "<<t<<endl;
        throw runtime_error("La expresion a pushear debe ser del tipo adecuado");
    }
    return "void";

}



string Typechecker::visit(accesExp* exp){
    int cont=0;
    string t{};
    for(Exp* index:exp->indexes){
        cont++;
        t=index->accept(this);
        if(t!="i64"){
            throw runtime_error("El indice a un vector debe ser i64");

        }
    }
    string type=tipos[exp->variable];
    string vec{};
    for(int i=0;i<cont;i++){
        vec=type.substr(0,4)+type.substr(type.size()-1);
        type=type.substr(4,type.size()-5);
        if(vec!="Vec<>"){
            throw runtime_error("No se puede acceder a un tipo que no sea vector");
        }
    }
    return type;

}