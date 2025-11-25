#include "ast_to_dot.h"

string ASTtoDOT::visitProgram(Program* p) {
    string nodeId = newNodeId();
    addNode(nodeId, "Program");

    // Variables globales
    if (!p->vdlist.empty()) {
        string globalVarsId = newNodeId();
        addNode(globalVarsId, "Global Variables");
        addEdge(nodeId, globalVarsId);

        for (auto vd : p->vdlist) {
            string vdId = visitVarDec(vd);
            addEdge(globalVarsId, vdId);
        }
    }

    // Funciones
    if (!p->fdlist.empty()) {
        string functionsId = newNodeId();
        addNode(functionsId, "Functions");
        addEdge(nodeId, functionsId);

        for (auto fd : p->fdlist) {
            string fdId = visitFunDec(fd);
            addEdge(functionsId, fdId);
        }
    }

    return nodeId;
}

string ASTtoDOT::visitFunDec(FunDec* fd) {
    string nodeId = newNodeId();
    string label = "Function: " + fd->nombre + "\\nReturn: " + fd->tipo;
    addNode(nodeId, label);

    // Parámetros
    if (!fd->Pnombres.empty()) {
        string paramsId = newNodeId();
        addNode(paramsId, "Parameters");
        addEdge(nodeId, paramsId);

        for (size_t i = 0; i < fd->Pnombres.size(); i++) {
            string paramId = newNodeId();
            string paramLabel = fd->Pnombres[i] + ": " + fd->Ptipos[i];
            addNode(paramId, paramLabel);
            addEdge(paramsId, paramId);
        }
    }

    // Cuerpo
    if (fd->cuerpo) {
        string bodyId = visitBody(fd->cuerpo);
        addEdge(nodeId, bodyId, "body");
    }

    return nodeId;
}

string ASTtoDOT::visitBody(Body* body) {
    string nodeId = newNodeId();
    addNode(nodeId, "Body");

    // Declaraciones locales
    if (!body->declarations.empty()) {
        string declsId = newNodeId();
        addNode(declsId, "Local Declarations");
        addEdge(nodeId, declsId);

        for (auto vd : body->declarations) {
            string vdId = visitVarDec(vd);
            addEdge(declsId, vdId);
        }
    }

    // Statements
    if (!body->StmList.empty()) {
        string stmsId = newNodeId();
        addNode(stmsId, "Statements");
        addEdge(nodeId, stmsId);

        for (auto stm : body->StmList) {
            string stmId = visitStm(stm);
            addEdge(stmsId, stmId);
        }
    }

    return nodeId;
}

string ASTtoDOT::visitVarDec(VarDec* vd) {
    string nodeId = newNodeId();
    string label = "VarDec: " + vd->type;
    addNode(nodeId, label);

    for (auto& var : vd->vars) {
        string varId = newNodeId();
        addNode(varId, var);
        addEdge(nodeId, varId);
    }

    return nodeId;
}

string ASTtoDOT::visitStm(Stm* stm) {
    // AssignStm
    if (AssignStm* assign = dynamic_cast<AssignStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Assign: " + assign->id);
        string expId = visitExp(assign->e);
        addEdge(nodeId, expId);
        return nodeId;
    }

    // PrintStm
    if (PrintStm* print = dynamic_cast<PrintStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Print");
        string expId = visitExp(print->e);
        addEdge(nodeId, expId);
        return nodeId;
    }

    // IfStm
    if (IfStm* ifstm = dynamic_cast<IfStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "If");

        string condId = visitExp(ifstm->condition);
        addEdge(nodeId, condId, "condition");

        string thenId = visitBody(ifstm->then);
        addEdge(nodeId, thenId, "then");

        if (ifstm->els) {
            string elseId = visitBody(ifstm->els);
            addEdge(nodeId, elseId, "else");
        }
        return nodeId;
    }

    // WhileStm
    if (WhileStm* whilestm = dynamic_cast<WhileStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "While");

        string condId = visitExp(whilestm->condition);
        addEdge(nodeId, condId, "condition");

        string bodyId = visitBody(whilestm->b);
        addEdge(nodeId, bodyId, "body");
        return nodeId;
    }

    // ReturnStm
    if (ReturnStm* retstm = dynamic_cast<ReturnStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Return");
        if (retstm->e) {
            string expId = visitExp(retstm->e);
            addEdge(nodeId, expId);
        }
        return nodeId;
    }

    // FcallStm
    if (FcallStm* fcall = dynamic_cast<FcallStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "FunctionCall: " + fcall->nombre);

        for (auto arg : fcall->argumentos) {
            string argId = visitExp(arg);
            addEdge(nodeId, argId);
        }
        return nodeId;
    }

    // AssignPStm
    if (AssignPStm* assignp = dynamic_cast<AssignPStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "ArrayAssign");

        string arrId = visitExp(assignp->arr);
        addEdge(nodeId, arrId, "array");

        string expId = visitExp(assignp->e);
        addEdge(nodeId, expId, "value");
        return nodeId;
    }

    // pushStm
    if (pushStm* push = dynamic_cast<pushStm*>(stm)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Push");

        string vecId = visitExp(push->vector);
        addEdge(nodeId, vecId, "vector");

        string valId = visitExp(push->p);
        addEdge(nodeId, valId, "value");
        return nodeId;
    }

    return newNodeId();
}

string ASTtoDOT::visitExp(Exp* exp) {
    // BinaryExp
    if (BinaryExp* binexp = dynamic_cast<BinaryExp*>(exp)) {
        string nodeId = newNodeId();
        string op = Exp::binopToChar(binexp->op);
        addNode(nodeId, "BinaryOp: " + op);

        string leftId = visitExp(binexp->left);
        addEdge(nodeId, leftId, "left");

        string rightId = visitExp(binexp->right);
        addEdge(nodeId, rightId, "right");
        return nodeId;
    }

    // NumberExp
    if (NumberExp* numexp = dynamic_cast<NumberExp*>(exp)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Number: " + to_string(numexp->value));
        return nodeId;
    }

    // IdExp
    if (IdExp* idexp = dynamic_cast<IdExp*>(exp)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Id: " + idexp->value);
        return nodeId;
    }

    // BoolExp
    if (BoolExp* boolexp = dynamic_cast<BoolExp*>(exp)) {
        string nodeId = newNodeId();
        string val = boolexp->value ? "true" : "false";
        addNode(nodeId, "Bool: " + val);
        return nodeId;
    }

    // strExp
    if (strExp* str = dynamic_cast<strExp*>(exp)) {
        string nodeId = newNodeId();
        addNode(nodeId, "String: \\\"" + str->value + "\\\"");
        return nodeId;
    }

    // arrExp
    if (arrExp* arr = dynamic_cast<arrExp*>(exp)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Array: " + arr->tipo);

        for (auto elem : arr->elements) {
            string elemId = visitExp(elem);
            addEdge(nodeId, elemId);
        }
        return nodeId;
    }

    // accesExp
    if (accesExp* acc = dynamic_cast<accesExp*>(exp)) {
        string nodeId = newNodeId();
        addNode(nodeId, "ArrayAccess: " + acc->variable);

        for (auto idx : acc->indexes) {
            string idxId = visitExp(idx);
            addEdge(nodeId, idxId, "index");
        }
        return nodeId;
    }

    // lenExp
    if (lenExp* len = dynamic_cast<lenExp*>(exp)) {
        string nodeId = newNodeId();
        addNode(nodeId, "Length");

        string contId = visitExp(len->container);
        addEdge(nodeId, contId);
        return nodeId;
    }

    // FcallExp
    if (FcallExp* fcall = dynamic_cast<FcallExp*>(exp)) {
        string nodeId = newNodeId();
        addNode(nodeId, "FunctionCall: " + fcall->nombre);

        for (auto arg : fcall->argumentos) {
            string argId = visitExp(arg);
            addEdge(nodeId, argId);
        }
        return nodeId;
    }

    return newNodeId();
}
