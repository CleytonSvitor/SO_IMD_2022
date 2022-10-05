# Processos, Memória compartilhada e Sincronização

Projeto desenvolvido para a disciplina de Sistemas Operacionais em que realizamos a multiplicação de matrizes, programação paralela, com o objetivo de aplicar os conceitos vistos em aula, verificarmos o tempo de execução sequencial, paralelo usando threads e paralelo usando processos e, ao final, discutir as vantagens e desvantagens das soluções.

## Começando

### Inicialmente, entre no diretório /src
```shell
$ cd src
```

### Gerando as matrizes
```shell
$ g++ gerar_matrizes.cpp -o gerar_matrizes && ./gerar_matrizes 50
```

### Compilando e executando o código Sequencial
```shell
$ g++ sequencial.cpp -o sequencial && ./sequencial arquivo01.txt arquivo02.txt
```

### Compilando e executando o código com Processos
```shell
$ g++ processos.cpp -o processos && ./processos arquivo01.txt arquivo02.txt 3
```

### Compilando e executando o código com Threads
```shell
$ g++ thread.cpp -o thread -pthread && ./thread arquivo01.txt arquivo02.txt 3
```
