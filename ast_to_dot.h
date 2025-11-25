#ifndef AST_TO_DOT_H
#define AST_TO_DOT_H

#include "ast.h"
#include <fstream>
#include <string>

using namespace std;

class ASTtoDOT {
private:
    ofstream &out;
    int nodeCounter;

    string newNodeId() {
        return "n" + to_string(nodeCounter++);
    }

    void addNode(const string& id, const string& label) {
        out << "  " << id << " [label=\"" << label << "\"];" << endl;
    }

    void addEdge(const string& from, const string& to, const string& label = "") {
        out << "  " << from << " -> " << to;
        if (!label.empty()) {
            out << " [label=\"" << label << "\"]";
        }
        out << ";" << endl;
    }

    string visitProgram(Program* p);
    string visitFunDec(FunDec* fd);
    string visitBody(Body* body);
    string visitVarDec(VarDec* vd);
    string visitStm(Stm* stm);
    string visitExp(Exp* exp);

public:
    ASTtoDOT(ofstream &out) : out(out), nodeCounter(0) {}

    void generate(Program* program) {
        out << "digraph AST {" << endl;
        out << "  node [shape=box, style=\"filled,rounded\", fillcolor=lightblue, fontname=\"Arial\"];" << endl;
        out << "  edge [fontname=\"Arial\", fontsize=10];" << endl;
        visitProgram(program);
        out << "}" << endl;
    }
};

#endif // AST_TO_DOT_H
