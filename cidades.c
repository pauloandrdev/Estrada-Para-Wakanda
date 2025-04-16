#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

int compararCidades(const void *a, const void *b) {
    const Cidade *cidadeA = (const Cidade *)a;
    const Cidade *cidadeB = (const Cidade *)b;
    return cidadeA->Posicao - cidadeB->Posicao;
}

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) return NULL;

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        fclose(arquivo);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &estrada->T) != 1) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    if (estrada->T < 3 || estrada->T > 1000000) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    if (fscanf(arquivo, "%d", &estrada->N) != 1) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    if (estrada->N < 2 || estrada->N > 10000) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        free(estrada);
        fclose(arquivo);
        return NULL;
    }

    for (int i = 0; i < estrada->N; i++) {
        if (fscanf(arquivo, "%d", &estrada->C[i].Posicao) != 1) {
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }

        char linha[256];
        if (fgets(linha, sizeof(linha), arquivo) == NULL) {
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }

        char *nome = linha;
        while (*nome == ' ' || *nome == '\t') nome++;
        char *fim = nome + strlen(nome) - 1;
        while (fim > nome && (*fim == ' ' || *fim == '\t' || *fim == '\n')) fim--;
        *(fim + 1) = '\0';

        strncpy(estrada->C[i].Nome, nome, 255);
        estrada->C[i].Nome[255] = '\0';

        if (estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T) {
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }
    }

    qsort(estrada->C, estrada->N, sizeof(Cidade), compararCidades);

    for (int i = 1; i < estrada->N; i++) {
        if (estrada->C[i].Posicao == estrada->C[i-1].Posicao) {
            free(estrada->C);
            free(estrada);
            fclose(arquivo);
            return NULL;
        }
    }

    fclose(arquivo);
    return estrada;
}

double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1.0;

    double menorVizinhanca = estrada->T;

    for (int i = 0; i < estrada->N; i++) {
        double vizinhanca = 0.0;
        
        if (i == 0) {
            vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        } else if (i == estrada->N - 1) {
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
        } else {
            double vizinhancaEsquerda = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
            double vizinhancaDireita = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
            vizinhanca = vizinhancaEsquerda + vizinhancaDireita;
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    double menorVizinhanca = estrada->T;
    int indiceMenorVizinhanca = 0;

    for (int i = 0; i < estrada->N; i++) {
        double vizinhanca = 0.0;
        
        if (i == 0) {
            vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        } else if (i == estrada->N - 1) {
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
        } else {
            double vizinhancaEsquerda = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0;
            double vizinhancaDireita = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
            vizinhanca = vizinhancaEsquerda + vizinhancaDireita;
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            indiceMenorVizinhanca = i;
        }
    }

    char *nomeCidade = (char *)malloc(256 * sizeof(char));
    if (nomeCidade) {
        strncpy(nomeCidade, estrada->C[indiceMenorVizinhanca].Nome, 255);
        nomeCidade[255] = '\0';
    }

    free(estrada->C);
    free(estrada);
    return nomeCidade;
} 