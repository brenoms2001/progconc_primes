#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

long long int *lista;
int primos = 0;
pthread_mutex_t mutex;
typedef struct{
    int id; //identificador do elemento que a thread ira processar
    long long int dim; //dimensao das estruturas de entrada (matriz quadrada)
    int nthreads; //numero de threads
 } tArgs;

//Funções
int ehPrimo(long long int n);
void *tarefa(void *arg);

int main(int argc, char* argv[]){
    int nThreads; //numero de threads
    long long int n;
    size_t ret; 
    pthread_t *tid; //identificadores das threads no sistema
    tArgs *args; //identificadores locais das threads e dimensao
    double inicio, fim, delta, total=0;
    FILE *arquivo, *registro; //lista de entrada
   
    if(argc < 3){ 
        printf("Use: %s <nome do arquivo> <numero de threads> \n", argv[0]); 
        return 1; 
    }

    nThreads = atoi(argv[2]);
    
    //Abertura de arquivo
    arquivo = fopen(argv[1], "rb");
    if(arquivo==NULL){
        printf("Falha na abertura do arquivo %s\n", argv[1]); 
        return 2; 
    }
    
    //Obtendo n
    ret = fread(&n, sizeof(long long int), 1, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura da quantidade de elementos da lista\n");
        return 3;
    }

    //Carregando lista
    lista = (long long int *)malloc(sizeof(long long int)*n);
    ret = fread(lista, sizeof(long long int), n, arquivo);
    if(!ret) {
        fprintf(stderr, "Erro de leitura dos elementos da lista\n");
        return 3;
    }

    //Criando array de threads
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nThreads);
    if(tid==NULL) {
        puts("ERRO--malloc"); 
        return 4;
    }

    //Criando array de structs
    args = (tArgs*) malloc(sizeof(tArgs)*nThreads);
    if(args==NULL) {
        puts("ERRO--malloc");
        return 4;
    }

    GET_TIME(inicio);
    //criacao das threads
    for(int i=0; i<nThreads; i++) {
       (args+i)->id = i;
       (args+i)->dim = n;
       (args+i)->nthreads = nThreads;
       if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
          puts("ERRO--pthread_create"); return 3;
       }
    } 

    //espera pelo termino da threads
    for(int i=0; i<nThreads; i++) {
       pthread_join(*(tid+i), NULL);
    }
    GET_TIME(fim);
    delta = fim - inicio;

    //Escrevendo o tempo final em um arquivo com outros deltas para o mesmo N e para 
    //a mesma quantidade de threads
    char nome_arquivo[100];
    sprintf(nome_arquivo, "tempos.csv");  
    registro = fopen(nome_arquivo, "a");
    if (registro == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    
    fprintf(registro, "%d,%lld,%.8f\n", nThreads, n, delta);

    fclose(registro);
    fclose(arquivo);
    free(tid);
    free(args);
}


int ehPrimo(long long int n) {
    int i;
    if (n<=1) return 0;
    if (n==2) return 1;
    if (n%2==0) return 0;
    for (i=3; i<sqrt(n)+1; i+=2)
        if(n%i==0) return 0;
    return 1;
}

void *tarefa(void *arg) {
    tArgs *args = (tArgs*) arg;
    for(int i = args->id; i < args->dim; i += args->nthreads){
        if(ehPrimo(lista[i])) {
            pthread_mutex_lock(&mutex);
            #ifdef TEXTO
            fprintf(stdout, "%lld é primo\n", lista[i]);
            #endif
            primos++;
            pthread_mutex_unlock(&mutex);           
        }
    }   
    pthread_exit(NULL);
}