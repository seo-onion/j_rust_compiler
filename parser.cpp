#include <iostream>
#include "token.h"
#include "scanner.h"
#include "ast.h"
#include "parser.h"

using namespace std;

// =============================
// Métodos de la clase Parser
// =============================

Parser::Parser(Scanner *sc) : scanner(sc)
{
    previous = nullptr;
    current = scanner->nextToken();
    if (current->type == Token::ERR)
    {
        throw runtime_error("Error léxico");
    }
}

bool Parser::match(Token::Type ttype)
{
    if (check(ttype))
    {
        advance();

        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype)
{
    if (isAtEnd())
        return false;
    return current->type == ttype;
}

bool Parser::advance()
{
    if (!isAtEnd())
    {
        Token *temp = current;
        if (previous)
            delete previous;
        current = scanner->nextToken();
        previous = temp;

        if (check(Token::ERR))
        {
            throw runtime_error("Error lexico");
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd()
{
    return (current->type == Token::END);
}

// =============================
// Reglas gramaticales
// =============================

Program *Parser::parseProgram()
{
    Program *p = new Program();
    if (check(Token::LET))
    {
        p->vdlist.push_back(parseVarDec());
        while (check(Token::LET))
        {
            p->vdlist.push_back(parseVarDec());
        }
    }
    if (check(Token::FN))
    {
        p->fdlist.push_back(parseFunDec());
        while (check(Token::FN))
        {
            p->fdlist.push_back(parseFunDec());
        }
    }
    cout << "Parser exitoso" << endl;
    return p;
}

VarDec *Parser::parseVarDec()
{
    VarDec *vd = new VarDec();
    match(Token::LET);
    match(Token::LPAREN);
    match(Token::MUT);
    match(Token::ID);
    vd->vars.push_back(previous->text);
    if (match(Token::DDOTS))
    {
        match(Token::AMP);
        match(Token::ID);
        vd->type = previous->text;
    }
    else
        vd->type = "undefined";
    while (match(Token::COMA))
    {
        match(Token::ID);
        vd->vars.push_back(previous->text);
    }
    match(Token::RPAREN);
    match(Token::SEMICOL);
    return vd;
}

FunDec *Parser::parseFunDec()
{
    FunDec *fd = new FunDec();
    match(Token::FN);
    match(Token::ID);
    fd->nombre = previous->text;
    match(Token::LPAREN);
    match(Token::MUT);
    if(check(Token::ID)) {
        while(match(Token::ID)) {
            match(Token::MUT);
            fd->Pnombres.push_back(previous->text);
            match(Token::DDOTS);
            match(Token::ID);
            fd->Ptipos.push_back(previous->text);
            match(Token::COMA);
        }
    }
    match(Token::RPAREN);
    if (match(Token::ARROW))
    {
        match(Token::ID);
        fd->tipo = previous->text;
    }
    else
    {
        fd->tipo = "void";
    }
    match(Token::LCBRACE);
    fd->cuerpo = parseBody();
    match(Token::RCBRACE);
    return fd;
}

Body *Parser::parseBody()
{
    Body *b = new Body();
    if (check(Token::LET))
    {
        b->declarations.push_back(parseVarDec());
        while (check(Token::LET))
        {
            b->declarations.push_back(parseVarDec());
        }
    }
    b->StmList.push_back(parseStm());
    while (!check(Token::RCBRACE))
    {
        b->StmList.push_back(parseStm());
    }
    return b;
}

Stm *Parser::parseStm()
{
    Stm *a;
    Exp *e;
    string nom;
    Body *tb = nullptr;
    Body *fb = nullptr;
    if (match(Token::ID))
    {
        nom = previous->text;
        if (match(Token::LPAREN))
        {
            FcallStm *f = new FcallStm();
            f->nombre = nom;
            while (!match(Token::RPAREN))
            {
                f->argumentos.push_back(parseDE());

            }
            match(Token::SEMICOL);
            return f;
        }
        else if (match(Token::LBRACE))
        {

            AssignPStm *stm = new AssignPStm();
            accesExp *acc = new accesExp();
            acc->variable = nom;
            acc->indexes.push_back(parseDE());
            match(Token::RBRACE);
            while (match(Token::LBRACE))
            {
                acc->indexes.push_back(parseDE());
                match(Token::RBRACE);
            }
            if(match(Token::ASSIGN)) {
                stm->arr = acc;
                stm->e = parseDE();
                match(Token::SEMICOL);
                return stm;
            }else if(match(Token::DOT)){
                pushStm* push=new pushStm();
                push->vector=acc;
                match(Token::PUSH);
                match(Token::LPAREN);
                push->p=parseDE();
                match(Token::RPAREN);
                match(Token::SEMICOL);
                return push;
            }
        }
        else if(match(Token::DOT)){
            pushStm* push=new pushStm();
            push->vector=new IdExp(nom);
            match(Token::PUSH);
            match(Token::LPAREN);
            push->p=parseDE();
            match(Token::RPAREN);
            match(Token::SEMICOL);

            return push;
        }
        else
        {
            match(Token::ASSIGN);
            e = parseDE();
            match(Token::SEMICOL);
            return new AssignStm(nom, e);
        }
    }
    else if (match(Token::PRINT))
    {
        match(Token::EXCLAM);
        match(Token::LPAREN);
        match(Token::STRING);
        match(Token::COMA);
        e = parseDE();
        match(Token::RPAREN);
        match(Token::SEMICOL);
        return new PrintStm(e);
    }
    else if (match(Token::RETURN))
    {
        ReturnStm *r = new ReturnStm();
        match(Token::LPAREN);
        r->e = parseDE();
        match(Token::RPAREN);
        match(Token::SEMICOL);

        return r;
    }

    else if (match(Token::IF)) {
        e = parseDE();
        if (!match(Token::LCBRACE))
        {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::RCBRACE))
        {
            cout << "Error: se esperaba '}' después del body." << endl;
            exit(1);
        }
        fb = nullptr;
        if (match(Token::ELSE))
        {
            match(Token::LCBRACE);
            fb = parseBody();
            match(Token::RCBRACE);
        }

        a = new IfStm(e, tb, fb);
    }
    else if (match(Token::WHILE))
    {
        e = parseDE();
        if (!match(Token::LCBRACE))
        {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::RCBRACE))
        {
            cout << "Error: se esperaba '}' al final de la declaración." << endl;
            exit(1);
        }
        a = new WhileStm(e, tb);
    }

    else{
        cout<<current->text;
        throw runtime_error("Error sintáctico");
    }
    return a;
}

Exp *Parser::parseDE()
{
    Exp *l = parseCE();
    while (check(Token::AND) or check(Token::OR))
    {
        if (match(Token::OR))
        {
            BinaryOp op = ORR_op;
            Exp *r = parseCE();
            if (l->type() == "BoolExp" and r->type() == "BoolExp")
            {
                l = new BoolExp(((BoolExp *)l)->value or ((BoolExp *)r)->value);
            }
            else
                l = new BinaryExp(l, r, op);
        }
        else if (match(Token::AND))
        {
            BinaryOp op = AND_op;
            Exp *r = parseCE();
            if (l->type() == "BoolExp" and r->type() == "BoolExp")
            {
                l = new BoolExp(((BoolExp *)l)->value and ((BoolExp *)r)->value);
            }
            else
                l = new BinaryExp(l, r, op);
        }
    }
    return l;
}
Exp *Parser::parseCE()
{
    Exp *l = parseBE();

    if (match(Token::LE))
    {
        BinaryOp op = LE_OP;
        Exp *r = parseBE();
        if (l->type() == "NumberExp" and r->type() == "NumberExp")
            l = new BoolExp(int(((NumberExp *)l)->value < ((NumberExp *)r)->value));
        else
            l = new BinaryExp(l, r, op);
    }

    else if (match(Token::LEEQ))
    {
        BinaryOp op = LEEQ_OP;
        Exp *r = parseBE();
        if (l->type() == "NumberExp" and r->type() == "NumberExp")
            l = new BoolExp(int(((NumberExp *)l)->value <= ((NumberExp *)r)->value));
        else
            l = new BinaryExp(l, r, op);
    }

    else if (match(Token::GR))
    {
        BinaryOp op = GR_OP;
        Exp *r = parseBE();
        if (l->type() == "NumberExp" and r->type() == "NumberExp")
            l = new BoolExp(int(((NumberExp *)l)->value > ((NumberExp *)r)->value));
        else
            l = new BinaryExp(l, r, op);
    }

    else if (match(Token::GREQ))
    {
        BinaryOp op = GREQ_OP;
        Exp *r = parseBE();
        if (l->type() == "NumberExp" and r->type() == "NumberExp")
            l = new BoolExp(int(((NumberExp *)l)->value >= ((NumberExp *)r)->value));
        else
            l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp *Parser::parseBE()
{
    auto l = parseE();
    while (match(Token::PLUS) || match(Token::MINUS))
    {
        BinaryOp op;
        if (previous->type == Token::PLUS)
        {
            op = PLUS_OP;
        }
        else
        {
            op = MINUS_OP;
        }
        auto r = parseE();
        // cout<< l->type()<<"  ";
        if (l->type() == "NumberExp" and r->type() == "NumberExp")
        {
            // cout<<"optimizandoooooooo"<<endl;
            switch (op)
            {
            case (PLUS_OP):
                l = new NumberExp(((NumberExp *)l)->value + ((NumberExp *)r)->value);
                break;
            case (MINUS_OP):
                l = new NumberExp(((NumberExp *)l)->value - ((NumberExp *)r)->value);
                break;
            }
        }
        else
        {
            l = new BinaryExp(l, r, op);
        }
    }
    return l;
}

Exp *Parser::parseE()
{
    auto l = parseT();
    while (match(Token::MUL) || match(Token::DIV))
    {
        BinaryOp op;
        if (previous->type == Token::MUL)
        {
            op = MUL_OP;
        }
        else
        {
            op = DIV_OP;
        }
        auto r = parseT();
        if (l->type() == "NumberExp" and r->type() == "NumberExp")
        {
            switch (op)
            {
            case (MUL_OP):
                l = new NumberExp(((NumberExp *)l)->value * ((NumberExp *)r)->value);
                break;
            case (DIV_OP):
                l = new NumberExp(((NumberExp *)l)->value / ((NumberExp *)r)->value);
                break;
            }
        }
        else
        {
            l = new BinaryExp(l, r, op);
        }
    }
    return l;
}

Exp *Parser::parseT()
{
    Exp *l = parseF();
    if (match(Token::POW))
    {
        BinaryOp op = POW_OP;
        Exp *r = parseF();
        l = new BinaryExp(l, r, op);
    }
    return l;
}

Exp *Parser::parseF()
{
    string nom;
    if (match(Token::NUM))
    {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::VEC)) {// paresar el array
        arrExp* arr=new arrExp();
        match(Token::EXCLAM);
        match(Token::LBRACE);
        while(!match(Token::RBRACE)){
            arr->elements.push_back(parseDE());
            match(Token::COMA);
        }
        return arr;
    }
    else if (match(Token::STRING))
    {
        return new strExp(previous->text);
    }
    else if (match(Token::TRUE))
    {
        return new BoolExp(1);
    }
    else if (match(Token::FALSE))
    {
        return new BoolExp(0);
    }
    else if (match(Token::LPAREN))
    {
        auto e = parseDE();
        match(Token::RPAREN);
        return e;
    }
    else if (match(Token::ID))
    {    
        nom = previous->text;
        if (check(Token::LPAREN))
        {
            match(Token::LPAREN);
            FcallExp *fcall = new FcallExp();
            fcall->nombre = nom;
            fcall->argumentos.push_back(parseDE());
            while (match(Token::COMA))
            {
                fcall->argumentos.push_back(parseDE());
            }
            match(Token::RPAREN);
            return fcall;
        }
        if(match(Token::LBRACE)) {
          accesExp* acc=new accesExp();
          acc->variable=nom;
          acc->indexes.push_back(parseDE());
          match(Token::RBRACE);
          while(match(Token::LBRACE)){
              acc->indexes.push_back(parseDE());
              match(Token::RBRACE);
          }
          if(match(Token::DOT)){
               if(match(Token::LEN)) {
                  match(Token::LPAREN);
                  match((Token::RPAREN));
                  return new lenExp(acc);
              }else {
                   match(Token::CLONE);
                   match(Token::LPAREN);
                   match(Token::RPAREN);
                   match(Token::COMA);
               }

          }
          return acc;
        }

        else {
            match(Token::DOT);
            if(match(Token::LEN)){

                match(Token::LPAREN);
                match((Token::RPAREN));
                return new lenExp(new IdExp(nom));
            }else if(match(Token::CLONE)) {
                match(Token::LPAREN);
                match(Token::RPAREN);
                match(Token::COMA);
            }
            return new IdExp(nom);
        }
    }
    else {
        cout<<current->text;
        throw runtime_error("Error sintáctico");
    }
}