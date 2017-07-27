/*  Trabalho de Organização e Recuperação de Dados.
    Implementa um hashing extensível
    Autores: Bruno César e Cristofer Oswald
    Data: 26/08/2017 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ord.h"

diretorio_t* diretorio;
int num_chaves;

int main(int argc, char** argv){
    int chave, address, profundidade = 0;
    char* arquivo;
    char linha[8];
    FILE* chaves;

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

    diretorio = malloc(sizeof(diretorio_t));

    diretorio->num_buckets = 1;
    diretorio->profundidade = 0;
    diretorio->cell = malloc(sizeof(dir_cell_t));
    diretorio->cell[0].bucket =  malloc(sizeof(bucket_t));
    inicializaBucket(diretorio->cell[0].bucket, 0, 0);

    while(!feof(chaves)){
        fgets(linha, 8, chaves);
        chave = atoi(linha);

        opAdd(chave);
    }
    printDiretorio();
    return 0;
}

void inicializaBucket(bucket_t* bucket, int profundidade, int id){
    bucket->id = id;
    bucket->profundidade = profundidade;
    bucket->num = 0;
    bucket->chaves = malloc(num_chaves * sizeof(int));
}

int opAdd(int chave){
    char c = 'n';
    bucket_t* bucket;

    /*while(c != 's'){
        printf("Continuar?\n");
        scanf(" %c\n", &c);
    }
    printDiretorio();*/

    if(!findBucket(chave, &bucket)){
        addBucketKey(&bucket, chave);
        return 1;
    }
    return 0;
}

int findBucket(int chave, bucket_t** bucket){
    int i;
    int address = makeAddress(chave, diretorio->profundidade);
    *bucket = diretorio->cell[address].bucket;

    for(i = 0; i < (*bucket)->num; i++){
        if((*bucket)->chaves[i] == chave){
            return 1;
        }
    }

    return 0;
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

void addBucketKey(bucket_t** bucket, int chave){
    printf("Adicionando %d\n", chave);
    int num = (*bucket)->num;
    if(num < num_chaves){
        printf("Coube\n");
        (*bucket)->chaves[num] = chave;
        (*bucket)->num = num + 1;
    }
    else{
        printf("Nao coube\n");
        printDiretorio();
        bucketSplit(bucket);
        opAdd(chave);
        printDiretorio();

    }
}

void bucketSplit(bucket_t** bucket){
    int i, new_start, new_end, new_add;
    int* b_chaves;
    bucket_t* new_bucket;

    if((*bucket)->profundidade == diretorio->profundidade){
        dirDouble();
        printDiretorio();
    }
    printf("Criou bucket\n");

    diretorio->num_buckets = diretorio->num_buckets + 1;
    new_bucket = malloc(sizeof(bucket_t));
    inicializaBucket(new_bucket, (*bucket)->profundidade, (*bucket)->id+1);

    findNewRange(*bucket, &new_start, &new_end);
    printf("start: %d, end: %d\n", new_start, new_end);
    insBucket(&new_bucket, new_start, new_end);

    (*bucket)->profundidade = (*bucket)->profundidade + 1;
    new_bucket->profundidade = (*bucket)->profundidade;

    b_chaves = (*bucket)->chaves;
    inicializaBucket(*bucket, (*bucket)->profundidade, (*bucket)->id);
    for(i = 0; i < (*bucket)->num; i++){
        new_add = makeAddress(b_chaves[i], diretorio->profundidade);
        if((new_add >= new_start) &&
            (new_add <= new_end)){
            new_bucket->chaves[new_bucket->num] = b_chaves[i];
            new_bucket->num = new_bucket->num + 1;
        }
        else{
            (*bucket)->chaves[(*bucket)->num] = b_chaves[i];
            (*bucket)->num = (*bucket)->num + 1;
        }
    }
}

void dirDouble(){
    int i;
    int tam = pow(2, diretorio->profundidade);
    int new_tam = tam * 2;
    diretorio_t *new_diretorio;
    printf("Doubrou dir para %d\n", new_tam);

    new_diretorio = malloc(sizeof(diretorio_t));
    new_diretorio->cell = malloc(sizeof(dir_cell_t) * new_tam);

    for(i = 0; i < tam; i++){
        new_diretorio->cell[i*2].bucket = diretorio->cell[i].bucket;
        new_diretorio->cell[(i*2) + 1].bucket = diretorio->cell[i].bucket;
    }

    new_diretorio->profundidade = diretorio->profundidade;
    new_diretorio->num_buckets = diretorio->num_buckets;
    free(diretorio->cell);
    free(diretorio);
    diretorio = new_diretorio;
    diretorio->profundidade = diretorio->profundidade + 1;

}

void findNewRange(bucket_t *bucket, int* new_start, int* new_end){
    int i;
    int mask = 1;
    int shared_add = makeAddress(bucket->chaves[0], bucket->profundidade);
    int new_shared = shared_add << 1;
    int bits_to_fill = diretorio->profundidade - (bucket->profundidade + 1);

    new_shared = new_shared | mask;
    *new_start = new_shared;
    *new_end = new_shared;

    for(i = 0; i < bits_to_fill; i++){
        *new_start = *new_start << 1;
        *new_end = *new_end << 1;
        *new_end = *new_end | mask;
    }
}

void insBucket(bucket_t** bucket, int start, int end){
    int i;
    for(i = start; i <= end; i++){
        diretorio->cell[i].bucket = *bucket;
    }
}

void printDiretorio(){
    int i, j, achou, atual = 0;
    int tam = pow(2, diretorio->profundidade);
    int buckets[diretorio->num_buckets];

    for(i = 0; i < diretorio->num_buckets; i++){
        buckets[i] = -1;
    }

    printf("\nPrintando diretório:\n");

    printf("Tamanho: %d\n", tam);
    printf("Número de buckets: %d\n", diretorio->num_buckets);
    printf("\nBuckets:\n");
    for(i = 0; i < tam; i++){
        printf("\tdir[%d] =  bucket #%d\n", i, diretorio->cell[i].bucket->id);
    }

    for(i = 0; i < tam; i++){
        achou = 0;
        for(j = 0; j < diretorio->num_buckets; j++){
            if(buckets[j] == diretorio->cell[i].bucket->id){
                achou = 1;
                break;
            }
        }
        if(!achou){
            printBucket(diretorio->cell[i].bucket);
            buckets[atual] = diretorio->cell[i].bucket->id;
            atual = atual + 1;
        }
    }
}

void printBucket(bucket_t* bucket){
    int i;
    printf("== Bucket #%d ==\n", bucket->id);
    printf("\tId: %d\n", bucket->id);
    printf("\tProfundidade: %d\n", bucket->profundidade);

    for(i = 0; i < bucket->num; i++){
        printf("\tchave[%d] = %d\n", i, bucket->chaves[i]);

    }

}
