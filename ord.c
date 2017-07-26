/*  Trabalho de Organização e Recuperação de Dados.
    Implementa um hashing extensível
    Autores: Bruno César e Cristofer Oswald
    Data: 26/08/2017 */

#include <stdio.h>
#include <stdlib.h>

#include "ord.h"

int main(int argc, char** argv){
    int num_chaves, chave;
    char* arquivo;
    char linha[8];
    FILE* chaves;

    diretorio_t diretorio;

    if(argc != 3){
        printf("Argumentos inválidos, utilze: ./ord arquivo_de_entrada numero_de_buckets\n");
        return 0;
    }

    arquivo = argv[1];
    num_chaves = atoi(argv[2]);

    chaves = fopen(arquivo, "r");
    if(chaves == NULL){
        printf("Erro ao ler arquivo\n");
        return 0;
    }

    diretorio.profundidade = 0;
    diretorio.buckets = malloc(sizeof(bucket));
    diretorio.buckets[0].profundidade = 0;
    diretorio.buckets[0].num = 1;
    diretorio.buckets[0].chaves = malloc(num_chaves * sizeof(int));

    while(!feof(chaves)){
        fgets(linha, 8, chaves);
        chave = atoi(linha);
    }

    return 0;
}

void inicializaBucket(bucket_t* bucket, int profundidade, int num_chaves){
    buckets->profundidade = profundidade;
    buckets->num = 1;
    buckets->chaves = malloc(num_chaves * sizeof(int));
}

/* Inverte os bits de um endereço, dada a profundidade */
int makeAddress(int address, int profundidade){
    int retval = 0;
    int mask = 1;
    int lowbit;
    int i;

    for(i = 0; i < profundidade; i++){
        retval = retval << 1;
        lowbit = address & mask;
        retval = retval | lowbit;
        address = address >> 1;
    }

    return retval;
}
