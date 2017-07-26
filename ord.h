/*  Arquivo de definições do ord.c
    Data: 02/08/2017 */

typedef struct{
    int profundidade;
    int num;
    int *chaves;
}bucket_t;

typedef struct{
    int profundidade;
    bucket_t* buckets;
}diretorio_t;

void inicializaBucket(bucket_t* bucket, int profundidade, int num_chaves);
int makeAddress(int address, int profundidade);
