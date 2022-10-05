#include <thread>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include <unistd.h>

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

void * calcularThread(int i, int P, int qtdElementos, string pathParticoes, Matriz * m1, Matriz * m2) {
    int inicio = i;
    int fim = P - 1;
    if (inicio > 0) {
        inicio = i * P;
        fim = ((i+1) * P) - 1;
    }

    cout << "entrou na thead" << endl;

    // criando arquivo de saida   
    string nomeArquivo = pathParticoes + to_string(m1->linhas) + "_" +  to_string(m2->colunas) + "_" + to_string(i) + "_arquivo"+ ".txt";
    ofstream out(nomeArquivo);

    // capturando tempo inicial
    auto timeStart = steady_clock::now();

    // calculo da multiplicacao da particao
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

    // calculo do tempo percorrido
    auto timeEnd = steady_clock::now();
    auto elapsed = to_string(duration_cast<seconds>(timeEnd - timeStart).count());

    cout << "TEMPO " << elapsed << endl;
    
    // escrevendo arquivo
    out << "TEMPO " << elapsed << endl;
    out.close();

    // finalizando a thread
    pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
    // carreganeto das variaveis de argumento
    string arquivo_matriz_01 = argv[1];
    string arquivo_matriz_02 = argv[2];
    int P = atoi(argv[3]);

    // diretorios dos arquivos
    string pathBase = "data/";
    string pathParticoes = pathBase + "particoes/";
    arquivo_matriz_01 = pathBase + "input/" + arquivo_matriz_01;
    arquivo_matriz_02 = pathBase + "input/" + arquivo_matriz_02;

    cout << "lendo arquivos" << endl;

    // criacacao das matrizes
    Matriz* m1 = new Matriz(arquivo_matriz_01);
    Matriz* m2 = new Matriz(arquivo_matriz_02);

    // calculando variaveis para o calculo
    int qtdElementos = m1->linhas * m2->colunas;
    int qtdThreads = qtdElementos / P;
    int resto = qtdElementos % P;

    // cubrindo a todos os casos para que nao falte threads
    if(resto > 0) qtdThreads++;
    
    // criando as threads
    thread *threads = new thread[qtdThreads];

    cout << "criando threads" << endl;

    // startando as threads
    for(int i = 0; i < qtdThreads; i++) {
        threads[i] = thread(calcularThread, i, P, qtdElementos, pathParticoes, m1, m2);
    }

    // esperando pela finalizacao do processo das threads
    for(int i = 0; i < qtdThreads; i++) {
        threads[i].join();
    }

    cout << "threads finalizadas" << endl;

    // criando arquivo de resultado
    string arquivoResult = pathBase + "matriz_result_thread.txt";
    ofstream out(arquivoResult);

    double tempoTotal = 0.0;
    for (size_t i = 0; i < qtdThreads; i++) {
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
    tempoTotal = tempoTotal / pow(10, 6);
    out << "TEMPO " << tempoTotal << endl;
    //cout <<  "TEMPO [" << z << "]: " << tempoTotal << endl;
    out.close();

    // liberando memoria
    delete [] threads;
    delete m1;
    delete m2;

    return 0;
}