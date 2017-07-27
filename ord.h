/*  Arquivo de definições do ord.c
    Data: 02/08/2017 */

typedef struct{
    int id;
    int profundidade;
    int num;
    int *chaves;
}bucket_t;

typedef struct{
    bucket_t* bucket;
}dir_cell_t;

typedef struct{
    int profundidade;
    int num_buckets;
    dir_cell_t* cell;
}diretorio_t;

void inicializaBucket(bucket_t* bucket, int profundidade, int id);
int makeAddress(int address, int profundidade);
int findBucket(int chave, bucket_t** bucket);
int opAdd(int chave);
void addBucketKey(bucket_t** bucket, int chave);
void bucketSplit(bucket_t** bucket);
void dirDouble();
void findNewRange(bucket_t *bucket, int* new_start, int* new_end);
void insBucket(bucket_t** bucket, int start, int end);
void printDiretorio();
void printBucket(bucket_t* bucket);
