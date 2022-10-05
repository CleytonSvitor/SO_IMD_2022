#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <sys/wait.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "Matriz.cpp"
#include <math.h> 
using namespace std;

int calculaElemento(Matriz* m1, Matriz* m2, int linha, int coluna) {
    int elemento = 0;
    for (int i = 0; i < m1->colunas; i++) {
        elemento += m1->matriz[linha][i] * m2->matriz[i][coluna];
    }
    return elemento;
}


int main(int argc, char *argv[]) {
    double soma = 0;
    
    string arquivo_matriz_01 = argv[1];
    string arquivo_matriz_02 = argv[2];
    int P = atoi(argv[3]);

    string pathBase = "data/";
    string pathParticoes = pathBase + "particoes/";

    arquivo_matriz_01 = pathBase + "input/" + arquivo_matriz_01;
    arquivo_matriz_02 = pathBase + "input/" + arquivo_matriz_02;

    cout << "lendo arquivos" << endl;

    Matriz* m1 = new Matriz(arquivo_matriz_01);
    Matriz* m2 = new Matriz(arquivo_matriz_02);

    int qtdElementos = m1->linhas * m2->colunas;
    int qtdProcessos = qtdElementos  / P;
    int resto = qtdElementos % P;

    if(resto > 0) {
        qtdProcessos++;
    }

    pid_t processo_filho[qtdProcessos];

    for(int i = 0; i < qtdProcessos; i++) {
        cout << "criando processo " << i << endl;

        processo_filho[i] = fork();

        if(processo_filho[i] == (pid_t) 0) {
            int inicio = i;
            int fim = P - 1;
            if (inicio > 0) {
                inicio = i * P;
                fim = ((i+1) * P) - 1;
            }                

            string nomeArquivo = pathParticoes 
                + to_string(m1->linhas) + "_" + to_string(m2->colunas) 
                + "_" + to_string(i) + "_arquivo"+ ".txt";
            ofstream out(nomeArquivo);


            auto timeStart = steady_clock::now();

            for(int j = inicio; j <= fim; j++) {
                int linha = j / m1->linhas;
                int coluna = j % m2->colunas; 
            
                int elemento = calculaElemento(m1, m2, linha, coluna);
                out <<  
                    to_string(linha) + "_" + to_string(coluna) +
                    " " + to_string(elemento)  + "\n";
                int indicePosterior = j + 1;
                if (indicePosterior >= qtdElementos) {
                    break;
                }
            } 
            
            auto timeEnd = steady_clock::now();
            auto elapsed = to_string(duration_cast<seconds>(timeEnd - timeStart).count());

            cout << "finalizado " << getpid() << " tempo " << elapsed << endl;

            out << "TEMPO " << elapsed << endl;
            out.close();

            exit(0);
        }
    }  

    for(int i = 0; i < qtdProcessos; i++) {
        wait(NULL);
    }

    // concatena os arquivos 
    string nomeArquivoCompleto = pathBase + "matriz_result_processos.txt";
    ofstream out(nomeArquivoCompleto);
    double tempoTotal = 0.0;
    for (size_t i = 0; i < qtdProcessos; i++) {
        string nomeArquivo = pathParticoes + to_string(m1->linhas) + "_" + to_string(m2->colunas) + "_" + to_string(i) + "_arquivo"+ ".txt";
        string filename(nomeArquivo);
        string line;
        ifstream input_file(filename);

        int count = 0;
        while (getline(input_file, line)){

            vector<string> vectorLine = m1->split(line, " ");
            if(!vectorLine[0].compare("TEMPO")) {
                float tempo = stof(vectorLine[1]);
                tempoTotal += tempo;
                continue;
            }
            out << line << endl;
        }
    }

    cout << "total " << tempoTotal / qtdProcessos << endl;

    out << "TEMPO " << tempoTotal  << endl;
    out.close();

    return 0;
}