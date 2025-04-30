#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHAS 1000
#define NUM_THREADS 3
#define NUM_NS 6

int main() {
    int threads[NUM_THREADS] = {1, 2, 4};
    long long Ns[NUM_NS] = {10, 100, 1000, 10000, 100000, 1000000};

    FILE *entrada = fopen("tempos.csv", "r");
    if (!entrada) {
        perror("Erro ao abrir o arquivo 'tempos.csv'");
        return 1;
    }

    double soma[NUM_THREADS][NUM_NS] = {0};
    int contagem[NUM_THREADS][NUM_NS] = {0};

    int thread;
    long long N;
    double tempo;

    char linha[256];
    // Ignora cabeçalho se houver
    fgets(linha, sizeof(linha), entrada);

    while (fgets(linha, sizeof(linha), entrada)) {
        if (sscanf(linha, "%d,%lld,%lf", &thread, &N, &tempo) == 3) {
            // Descobrir o índice do thread e do N
            int t_index = -1, n_index = -1;
            for (int i = 0; i < NUM_THREADS; i++) {
                if (threads[i] == thread) {
                    t_index = i;
                    break;
                }
            }
            for (int i = 0; i < NUM_NS; i++) {
                if (Ns[i] == N) {
                    n_index = i;
                    break;
                }
            }

            if (t_index != -1 && n_index != -1) {
                soma[t_index][n_index] += tempo;
                contagem[t_index][n_index]++;
            }
        }
    }

    fclose(entrada);

    // Gerar CSV de saída
    FILE *saida = fopen("medias.csv", "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        return 1;
    }

    // Cabeçalho
    fprintf(saida, "Threads");
    for (int i = 0; i < NUM_NS; i++) {
        fprintf(saida, ",N=%lld", Ns[i]);
    }
    fprintf(saida, "\n");

    for (int t = 0; t < NUM_THREADS; t++) {
        fprintf(saida, "%d", threads[t]);
        for (int n = 0; n < NUM_NS; n++) {
            if (contagem[t][n] > 0) {
                fprintf(saida, ",%.8f", soma[t][n] / contagem[t][n]);
            } else {
                fprintf(saida, ",");  // Valor ausente
            }
        }
        fprintf(saida, "\n");
    }

    fclose(saida);
    printf("Arquivo 'tempos_formatado.csv' gerado com sucesso.\n");
    return 0;
}
