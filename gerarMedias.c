#include <stdio.h>
#include <stdlib.h>

typedef struct {
    long long n;
    double soma;
    int threads;
    int contador;
} Registro;

#define MAX_REGISTROS 360  

int main() {
    Registro registros[MAX_REGISTROS];
    int total_registros = 0;

    FILE *entrada = fopen("tempos.csv", "r");
    if (entrada == NULL) {
        perror("Erro ao abrir dados.csv");
        return 1;
    }

    int threads;
    long long n;
    double tempo;
    while (fscanf(entrada, "%d,%lld,%lf", &threads, &n, &tempo) == 3) {
        // Verifica se já existe esse grupo de (threads, n)
        int encontrado = 0;
        for (int i = 0; i < total_registros; i++) {
            if (registros[i].threads == threads && registros[i].n == n) {
                registros[i].soma += tempo;
                registros[i].contador++;
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            registros[total_registros].threads = threads;
            registros[total_registros].n = n;
            registros[total_registros].soma = tempo;
            registros[total_registros].contador = 1;
            total_registros++;
        }
    }

    fclose(entrada);

    FILE *saida = fopen("medias.csv", "w");
    if (saida == NULL) {
        perror("Erro ao criar medias.csv");
        return 1;
    }

    fprintf(saida, "Threads,N,MédiaTempo(s)\n");
    for (int i = 0; i < total_registros; i++) {
        double media = registros[i].soma / registros[i].contador;
        fprintf(saida, "%d,%lld,%.8f\n", registros[i].threads, registros[i].n, media);
    }

    fclose(saida);

    printf("Arquivo medias.csv gerado com sucesso!\n");

    return 0;
}
