#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include "environment.h"
using namespace std;

class BinaryExp;
class NumberExp;
class Program;
class PrintStm;
class WhileStm;
class IfStm;
class AssignStm;
class Body;
class Vardec;
class FcallExp;
class ReturnStm;
class FunDec;


class Visitor {
public:
    // 
    virtual string visit(BinaryExp* exp) = 0;
    virtual string visit(NumberExp* exp) = 0;
    virtual string visit(IdExp* exp) = 0;
    virtual string visit(Program* p) = 0;
    virtual string visit(PrintStm* stm) = 0;
    virtual string visit(WhileStm* stm) = 0;
    virtual string visit(IfStm* stm) = 0;
    virtual string visit(AssignStm* stm) = 0;
    virtual string visit(Body* body) = 0;
    virtual string visit(VarDec* vd) = 0;
    virtual string visit(FcallExp* stm) = 0;
    virtual string visit(ReturnStm* r) = 0;
    virtual string visit(FunDec* fd) = 0;
    virtual string visit(strExp* str) = 0;
    virtual string visit(FcallStm* stm) =0;
    virtual string visit(arrExp* arr) =0;
    virtual string visit(accesExp* exp) = 0;
    virtual string visit(AssignPStm* stm) =0;
    virtual string visit(BoolExp* stm) =0;

};
class Typechecker : public Visitor {
public:
    Program* p;
    int generar(Program* program);
    unordered_map<string, string> tipos;
    unordered_map<string, int> vars_per_funct;
    int locales;
    string nombreFuncion;
    string visit(BinaryExp* exp) override;
    string visit(NumberExp* exp) override;
    string visit(IdExp* exp) override;
    string visit(Program* p) override ;
    string visit(PrintStm* stm) override;
    string visit(AssignStm* stm) override;
    string visit(WhileStm* stm) override;
    string visit(FcallStm* stm) override;
    string visit(BoolExp* stm) override;

    string visit(IfStm* stm) override;
    string visit(Body* body) override;
    string visit(VarDec* vd) override;
    string visit(FcallExp* fcall) override;
    string visit(ReturnStm* r) override;
    string visit(FunDec* fd) override;
    string visit(strExp* str) override;
    string visit(arrExp* str) override;
    string visit(accesExp* exp) override;
    string visit(AssignPStm* stm) override;



};

class GenCodeVisitor : public Visitor {
private:
    std::ostream& out;
public:
    GenCodeVisitor(std::ostream& out) : out(out) {}
    int generar(Program* program);
    Environment<int> memoria;
    unordered_map<string, bool> memoriaGlobal;
    Typechecker* tc;

    int offset = -8;
    int labelcont = 0;
    bool entornoFuncion = false;
    string nombreFuncion;
    string visit(BoolExp* stm) override;
    string visit(BinaryExp* exp) override;
    string visit(NumberExp* exp) override;
    string visit(IdExp* exp) override;
    string visit(Program* p) override ;
    string visit(PrintStm* stm) override;
    string visit(AssignStm* stm) override;
    string visit(WhileStm* stm) override;
    string visit(IfStm* stm) override;
    string visit(Body* body) override;
    string visit(VarDec* vd) override;
    string visit(FcallExp* fcall) override;
    string visit(ReturnStm* r) override;
    string visit(FunDec* fd) override;
    string visit(strExp* str) override;
    string visit(FcallStm* stm) override;
    string visit(arrExp* arr) override;
    string visit(accesExp* exp) override;
    string visit(AssignPStm* stm) override;


};

#endif // VISITOR_H