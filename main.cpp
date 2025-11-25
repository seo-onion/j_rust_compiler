#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "scanner.h"
#include "parser.h"
#include "ast.h"
#include "visitor.h"
#include "ast_to_dot.h"

using namespace std;

int main(int argc, const char* argv[]) {
    // Verificar número de argumentos
    if (argc != 2) {
        cout << "Número incorrecto de argumentos.\n";
        cout << "Uso: " << argv[0] << " <archivo_de_entrada>" << endl;
        return 1;
    }

    // Abrir archivo de entrada
    ifstream infile(argv[1]);
    if (!infile.is_open()) {
        cout << "No se pudo abrir el archivo: " << argv[1] << endl;
        return 1;
    }

    // Leer contenido completo del archivo en un string
    string input, line;
    while (getline(infile, line)) {
        input += line + '\n';
    }
    infile.close();

    // Crear instancias de Scanner 
    Scanner scanner1(input.c_str());
    Scanner scanner2(input.c_str());

    ejecutar_scanner(&scanner2, argv[1]);


    // Crear instancias de Parser
    Parser parser(&scanner1);

    // Parsear y generar AST
    cout << "Iniciando parseo..." << endl;
    Program* program = parser.parseProgram();
    cout << "Parseo exitoso!" << endl;

    // Obtener nombre base del archivo
    string inputFile(argv[1]);
    size_t dotPos = inputFile.find_last_of('.');
    string baseName = (dotPos == string::npos) ? inputFile : inputFile.substr(0, dotPos);

    // 1. Generar archivo AST en formato DOT
    string dotFilename = baseName + ".dot";
    ofstream dotfile(dotFilename);
    if (dotfile.is_open()) {
        cout << "Generando AST en formato DOT: " << dotFilename << endl;
        ASTtoDOT dotGen(dotfile);
        dotGen.generate(program);
        dotfile.close();

        // Generar PNG desde DOT usando graphviz
        string pngFilename = baseName + ".png";
        string dotCommand = "dot -Tpng " + dotFilename + " -o " + pngFilename;
        int result = system(dotCommand.c_str());
        if (result == 0) {
            cout << "AST PNG generado: " << pngFilename << endl;
        } else {
            cerr << "Advertencia: No se pudo generar PNG (¿graphviz instalado?)" << endl;
        }
    } else {
        cerr << "Error al crear archivo DOT" << endl;
    }

    // 2. Generar archivo de ensamblador
    string outputFilename = baseName + ".s";
    ofstream outfile(outputFilename);
    if (!outfile.is_open()) {
        cerr << "Error al crear el archivo de salida: " << outputFilename << endl;
        return 1;
    }

    cout << "Generando codigo ensamblador en " << outputFilename << endl;
    Typechecker tc;

    tc.generar(program);
    cout<<endl;
    cout<<"ids: tipos"<<endl;
    for(auto [v,t] :tc.tipos){
        cout<<v<<": "<<t<<endl;
    }
    cout<<endl;
    cout<<"funciones: variables"<<endl;
    for(auto [v,t] :tc.bytes_per_funct){
        cout<<v<<": "<<t<<endl;
    }
    GenCodeVisitor codigo(outfile);
    codigo.tc=&tc;
    codigo.generar(program);
    outfile.close();

    cout << "Compilacion completada exitosamente!" << endl;
    return 0;
}
