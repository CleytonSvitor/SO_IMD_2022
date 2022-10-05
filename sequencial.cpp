#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include "Matriz.cpp"
#include <math.h> 
using namespace std;
using namespace std::chrono;

vector<vector<int>> n1;

#define MICRO 1000000


int main(int argc, char *argv[]) {
    double soma = 0;

    string arquivo_matriz_01 = argv[1];
    string arquivo_matriz_02 = argv[2];

    string pathBase = "data/";

    arquivo_matriz_01 = pathBase + "input/" + arquivo_matriz_01;
    arquivo_matriz_02 = pathBase + "input/" + arquivo_matriz_02;


    Matriz* m1 = new Matriz(arquivo_matriz_01);
    Matriz* m2 = new Matriz(arquivo_matriz_02);

    vector<vector<int>> res;
    steady_clock::time_point begin = steady_clock::now();
    for (int i = 0; i < m1->linhas; i++) {
        res.push_back(vector<int>());
        for (int j = 0; j < m2->colunas; j++) {
            res[i].push_back(0);
            for (int k = 0; k < m1->linhas; k++) {
                res[i][j] += m1->matriz[i][k] * m2->matriz[k][j];
            }
        }
    }
    steady_clock::time_point end = steady_clock::now();
    string secs = to_string(duration_cast<seconds>(end - begin).count());
    string tempo = "Tempo " + secs + "\n";
    string nomeArquivoCompleto = pathBase + "matriz_result_sequencial.txt";
    ofstream out(nomeArquivoCompleto);
    out << m1->linhas  << " " <<  m2->colunas <<  "\n"; 
    for (int i = 0; i < m1->linhas; i++) {
        for (int j = 0; j < m2->colunas; j++) {
            out << "C_" << i  << "_" << j << " " <<  res[i][j] <<  "\n"; 
        }
    }

    float tempoFloat = stof(secs);
    cout << secs << endl;
    out << tempoFloat;
    out.close();
    
    return 0;
}