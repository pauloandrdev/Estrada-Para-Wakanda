#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "cidades.h"

int main(int argc, char *argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    if (argc != 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    const char *nomeArquivo = argv[1];

    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) {
        printf("Erro ao ler o arquivo ou dados invalidos.\n");
        return 1;
    }

    printf("Comprimento da estrada: %d\n", estrada->T);
    printf("Numero de cidades: %d\n", estrada->N);
    printf("Cidades (ordenadas por posicao):\n");
    for (int i = 0; i < estrada->N; i++) {
        printf("  %s: posicao %d\n", estrada->C[i].Nome, estrada->C[i].Posicao);
    }

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    if (menorVizinhanca >= 0) {
        printf("\nMenor vizinhanca: %.2f\n", menorVizinhanca);
    } else {
        printf("\nErro ao calcular menor vizinhanca.\n");
    }

    char *cidade = cidadeMenorVizinhanca(nomeArquivo);
    if (cidade) {
        printf("Cidade com menor vizinhanca: %s\n", cidade);
        free(cidade);
    } else {
        printf("Erro ao determinar cidade com menor vizinhanca.\n");
    }

    free(estrada->C);
    free(estrada);

    return 0;
} 