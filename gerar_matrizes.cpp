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

using namespace std;

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    
    Matriz* m1 = new Matriz();
    Matriz* m2 = new Matriz();


    m1->gerarMatrizArquivo(N, N, "data/input/arquivo01.txt");
    m2->gerarMatrizArquivo(N, N, "data/input/arquivo02.txt");

    
    return 0;
}