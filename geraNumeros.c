#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 1000000
//#define TEXTO

int main(int argc, char *argv[]){
    //Seed de tempo e declaração de variáveis
    srand(time(NULL));
    long long int n, num, *lista;
    FILE *arquivo;
    size_t ret;

    if(argc<2){
        //fprintf(stderr, "Digite %s <dimensão> <nome do arquivo de saída>\n", argv[0]);
        fprintf(stderr, "Digite %s <quantidade de números>\n", argv[0]);
        return 3;
    }

    n = atoll(argv[1]);
    #ifdef TEXTO
    fprintf(stdout, "GERADOR DE Números\n");
    fprintf(stdout, "N = %lld\n\n", n);
    #endif

    //inicialização de vetores
    lista = (long long int*)malloc(sizeof(long long int)*n);
    if(!lista){
        fprintf(stderr, "Erro de alocao da memoria da lista\n");
        return 1;
    }

    //Preenchendo a lista
    for(long long int i=0; i<n; i++) {
         num = rand() % MAX;
         lista[i] = num;
    }

    //Mostrar valores atribuídos aos vetores e o produto interno
    #ifdef TEXTO   
    fprintf(stdout, "Valores da Lista\n");
    for(long long int i=0; i<n; i++) {
       fprintf(stdout, "%lld ",lista[i]);
    }
    fprintf(stdout, "\n\n");
    #endif

    //Escrevendo no arquivo binário
    //abre o arquivo para escrita binaria
    char nomeArquivo[20];
    sprintf(nomeArquivo, "lista%lld", n);
    arquivo = fopen(nomeArquivo, "wb");
    if(!arquivo) {
       fprintf(stderr, "Erro de abertura do arquivo\n");
       return 4;
    }

    //Escreve a quantidade de números na lista
    ret = fwrite(&n, sizeof(long long int), 1, arquivo);
    if(ret != 1) {
        fprintf(stderr, "Erro de escrita no  arquivo\n");
        return 4;
     }
    //Escreve os elementos da lista
    ret = fwrite(lista, sizeof(long long int), n, arquivo);
    if(ret < n) {
       fprintf(stderr, "Erro de escrita no  arquivo\n");
       return 4;
    }
    
    //finaliza o uso das variaveis
    fclose(arquivo);
    free(lista);
    return 0;
}