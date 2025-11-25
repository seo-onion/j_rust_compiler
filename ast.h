#ifndef AST_H
#define AST_H

#include <string>
#include <list>
#include <ostream>
#include <vector>
using namespace std;

class Visitor;
class VarDec;

// Operadores binarios soportados
enum BinaryOp
{
    PLUS_OP,
    MINUS_OP,
    MUL_OP,
    DIV_OP,
    POW_OP,
    LE_OP,
    GR_OP,
    GREQ_OP,
    LEEQ_OP,
    AND_op,
    ORR_op
};

// Clase abstracta Exp
class Exp
{
public:
    virtual string type() = 0;
    virtual string accept(Visitor *visitor) = 0;
    virtual ~Exp() = 0;                     // Destructor puro → clase abstracta
    static string binopToChar(BinaryOp op); // Conversión operador → string
};

class Stm
{
public:
    virtual string accept(Visitor *visitor) = 0;
    virtual ~Stm() = 0;
};

class Body
{
public:
    list<Stm*> StmList;
    list<VarDec*> declarations;
    string accept(Visitor *visitor);
    Body();
    ~Body();
};

class VarDec
{
public:
    string type;
    list<string> vars;
    VarDec();
    string accept(Visitor *visitor);
    ~VarDec();
};


class FunDec
{
public:
    string nombre;
    string tipo;
    Body *cuerpo;
    bool isGeneric() { return !typeParams.empty(); }
    vector<string> Ptipos;
    vector<string> Pnombres;
    vector<string> typeParams;
    string accept(Visitor *visitor);
    
    FunDec() {};
    ~FunDec() {};
};

class Program
{
public:
    list<VarDec*> vdlist;
    list<FunDec*> fdlist;
    Program() {};
    ~Program() {};
    string accept(Visitor *visitor);
};


class FcallStm : public Stm
{
public:
    string nombre;
    vector<Exp*> argumentos;
    string accept(Visitor *visitor);
    FcallStm() {};
    ~FcallStm() {};
};

class FcallExp : public Exp
{
public:
    string exp_type = "FcallExp";
    string nombre;
    vector<Exp*> argumentos;
    string accept(Visitor *visitor);
    string accept(Visitor *visitor, vector<string> typeArgs);
    FcallExp() {};
    ~FcallExp() {};
    string type() { return exp_type; }
};



class IfStm : public Stm
{
public:
    Exp* condition;
    Body* then;
    Body* els;
    IfStm(Exp *condition, Body *then, Body *els);
    string accept(Visitor *visitor);
    ~IfStm() {};
};

class WhileStm : public Stm
{
public:
    Exp *condition;
    Body *b;
    WhileStm(Exp *condition, Body *b);
    string accept(Visitor *visitor);
    ~WhileStm() {};
};

// Expresión binaria
class BinaryExp : public Exp
{

public:
    string exp_type = "BinaryExp";
    Exp *left;
    Exp *right;
    BinaryOp op;
    string type() { return exp_type; }
    string accept(Visitor *visitor);
    BinaryExp(Exp *l, Exp *r, BinaryOp op);
    ~BinaryExp();
};
class strExp : public Exp
{
public:
    string exp_type = "strExp";
    string value;
    strExp(const string &v);
    ~strExp();
    string accept(Visitor *visitor);
    string type() { return exp_type; }
};
class lenExp : public Exp
{
public:
    string exp_type = "lenExp";
    Exp *container;
    lenExp(Exp *c) : container(c) {};
    ~lenExp() {};
    string accept(Visitor *visitor);
    string type() { return exp_type; }
};

class arrExp : public Exp
{
public:
    string exp_type = "arrExp";
    string tipo;
    list<Exp*> elements;
    arrExp() {};
    ~arrExp() {};
    string accept(Visitor *visitor);
    string type() { return exp_type; }
};

class accesExp : public Exp
{
public:
    string exp_type = "accesExp";
    string variable;
    list<Exp*> indexes;
    accesExp() {};
    ~accesExp() {};
    string accept(Visitor *visitor);
    string type() { return exp_type; }
};
// Expresión numérica
class NumberExp : public Exp
{
public:
    string exp_type = "NumberExp";
    int value;
    string accept(Visitor *visitor);
    NumberExp(int v);
    ~NumberExp();
    string type() { return exp_type; }
};
class BoolExp : public Exp
{
public:
    string exp_type = "BoolExp";
    int value;
    string accept(Visitor *visitor);
    BoolExp(int v) { value = bool(v); }
    ~BoolExp() {};
    string type() { return exp_type; }
};

// Expresión numérica
class IdExp : public Exp
{
public:
    string exp_type = "IdExp";

    string value;
    string accept(Visitor *visitor);
    IdExp(string v);
    ~IdExp();
    string type() { return exp_type; }
};

class AssignStm : public Stm
{
public:
    string id;
    Exp *e;
    AssignStm(string, Exp *);
    ~AssignStm();
    string accept(Visitor *visitor);
};

class AssignPStm : public Stm
{
public:
    accesExp *arr;
    Exp *e;
    AssignPStm() {};
    ~AssignPStm() {};
    string accept(Visitor *visitor);
};

class PrintStm : public Stm
{
public:
    Exp *e;
    PrintStm(Exp *);
    ~PrintStm();
    string accept(Visitor *visitor);
};

class pushStm : public Stm
{
public:
    Exp *p;
    Exp *vector;
    pushStm() {};
    ~pushStm() {};
    string accept(Visitor *visitor);
};

class ReturnStm : public Stm
{
public:
    Exp *e;
    ReturnStm() {};
    ~ReturnStm() {};
    string accept(Visitor *visitor);
};


#endif // AST_H
