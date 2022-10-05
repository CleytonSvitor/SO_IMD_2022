#ifndef MATRIZ
#define MATRIZ

#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

class Matriz {

    public:
        int linhas;
        int colunas;
        vector<vector<int>> matriz;

        Matriz() {
            
        }

        Matriz(string nome) {
            lerMatriz(nome);
        }

        Matriz(int linhas, int colunas) {
            this->linhas = linhas;
            this->colunas = colunas;

            for (int i = 0; i < this->linhas; i++) {
                matriz.push_back(vector<int>());
                for (int j = 0; j < this->colunas; j++){
                    matriz[i].push_back(0);
                }                
            }
        }

        Matriz(int linhas, int colunas, vector<vector<int>> matriz) {
            this->linhas = linhas;
            this->colunas = colunas;
            this->matriz = matriz;
        }
        
        void multiplicar (Matriz* m1, Matriz* m2, Matriz* result) {
            vector<vector<int>> res;
            steady_clock::time_point begin = steady_clock::now();
            for (int i = 0; i < m1->linhas; i++) {
                res.push_back(vector<int>());
                for (int j = 0; j < m2->colunas; j++) {
                    res[i].push_back(0);
                    for (int k = 0; k <  m1->linhas; k++) {
                        res[i][j] += m1->matriz[i][k] * m2->matriz[k][j];
                    }
                }
            }
            steady_clock::time_point end = steady_clock::now();
            result->matriz = res;

            cout << "\n \n" << "Tempo: " << duration_cast<milliseconds>(end - begin).count() << " [ms]" << endl;
        }

        void lerLinhasColunas(string nome) {
            string filename(nome);
            string line;
            ifstream input_file(filename);

            if (!input_file.is_open()) {
                cout << "(lerLinhasColunas): Erro ao abrir aquivo " << nome << endl;
            }

            
            while (getline(input_file, line)){
                vector<string> linhaVector = this->split(line, " ");
                this->linhas = stoi(linhaVector[0]);
                this->colunas = stoi(linhaVector[1]);

                break;
            }

            for (int i = 0; i < this->linhas; i++) {
                this->matriz.push_back(vector<int>());
                this->matriz[i].reserve(this->colunas * sizeof(int));
            }
            
            for (int j = 0; j < this->colunas; j++) {
                this->matriz[j].push_back(0);
            } 
        }

        void lerMatriz(string nome) {
            this->lerLinhasColunas(nome);

            string filename(nome);
            string line;
            ifstream input_file(filename);

            if (!input_file.is_open()) {
                cout << "Erro ao abrir aquivo " << nome << endl;
            }

            int count = 0;
            while (getline(input_file, line)){
                vector<string> linhaVector = this->split(line, " ");
                if(count == 0) {
                    count++;
                    continue;
                }

                vector<string> indicesVector = this->split(linhaVector[0], "_");
                int i = stoi(indicesVector[1]);
                int j = stoi(indicesVector[2]);       
                matriz[i][j] = stoi(linhaVector[1]);
            }
        }

        void printMatriz() {
            for (int i = 0; i < this->colunas; i++) {
                for (int j = 0; j < this->linhas; j++) {
                    cout << this->matriz[i][j] << " ";
                }
                cout << endl;
            }   
        }

        void printMatriz(vector<vector<int>> matriz) {
            for (int i = 0; i < this->colunas; i++) {
                for (int j = 0; j < this->linhas; j++) {
                    cout << matriz[i][j] << " ";
                }
                cout << endl;
            }   
        }

        vector<string> split (string s, string delimiter) {
            size_t pos_start = 0, pos_end, delim_len = delimiter.length();
            string token;
            vector<string> res;

            while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
                token = s.substr (pos_start, pos_end - pos_start);
                pos_start = pos_end + delim_len;
                res.push_back (token);
            }

            res.push_back (s.substr (pos_start));
            return res;
        }

        void gerarMatrizArquivo(int colunas, int linhas, string nome) {
            Matriz matriz = Matriz(linhas, colunas);

            for (int i = 0; i < matriz.linhas; i++) {
                for (int j = 0; j < matriz.colunas; j++) {
                    int elemento = (rand() % 10) + 1;
                    matriz.matriz[i][j] = elemento;
                }
            }
            this->escreverNoArquivo(nome, &matriz);  
        }

        void escreverNoArquivo(string nome, Matriz* matriz) {
            ofstream out(nome);
            out << matriz->linhas << " " << matriz->colunas <<  "\n";
            for (int i = 0; i < matriz->colunas; i++) {
                for (int j = 0; j < matriz->linhas; j++) {
                    out << "C_" << i  << "_" << j << " " <<  matriz->matriz[i][j] <<  "\n"; 
                }
            }
            out.close(); 
        }

};

#endif