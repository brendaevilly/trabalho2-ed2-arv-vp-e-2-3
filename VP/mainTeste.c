#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototipos.h"

#include <time.h>  // Para clock()
void mostrarCaminhoBuscaRN(Arvore *raiz, char *nome, int *comparacoes) {

    if (raiz) {
        printf(" -> [%s]", raiz->dado.ARTISTA.nome);
        (*comparacoes)++;
        if (strcmp(nome, raiz->dado.ARTISTA.nome) == 0) {
            printf(" [ENCONTRADO]");
            return;
        } else if (strcmp(nome, raiz->dado.ARTISTA.nome) < 0) {
            mostrarCaminhoBuscaRN(raiz->esq, nome, comparacoes);
        } else {
            mostrarCaminhoBuscaRN(raiz->dir, nome, comparacoes);
        }
    } else {
        printf(" (NAO ESTA NA ARVORE)");
    }
}



int main(){
    Arvore *biblioteca = inicializar();
    char *nomes[30] = {"ARTISTA1", "ARTISTA2", "ARTISTA3", "ARTISTA4", "ARTISTA5", "ARTISTA6", "ARTISTA7", "ARTISTA8", "ARTISTA9", "ARTISTA10", "ARTISTA11", "ARTISTA12", "ARTISTA13", "ARTISTA14", "ARTISTA15", "ARTISTA16", "ARTISTA17", "ARTISTA18", "ARTISTA19", "ARTISTA20", "ARTISTA21", "ARTISTA22", "ARTISTA23", "ARTISTA24", "ARTISTA25", "ARTISTA26", "ARTISTA27", "ARTISTA28", "ARTISTA29", "ARTISTA30"}; 
    double tempoTotal = 0.0;
    double tempoMicroTotal = 0.0;
    int comparacoesTotal = 0;

    for (int i = 0; i < 30; i++) {
        Arvore *info = alocarTree(ARTISTA);
        strcpy(info->dado.ARTISTA.nome, nomes[i]);

        inserirArvore(&biblioteca, info);
    }

    double tempoFinal = 0.0;
    double tempoMicroFinal = 0.0;
    double compFinal = 0.0;

    for (int j=0; j<30; j++){
        printf("\n--- Busca %d de buscas de 30 numeros ---\n", j + 1);
        for (int i = 0; i < 30; i++) {
            int comparacoes = 0;
            double tempo = 0.0;
            double tempoMicro = 0.0;

            printf("\nBusca %d: %s\nCaminho: ", i+1, nomes[i]);
            clock_t inicio = clock();
            mostrarCaminhoBuscaRN(biblioteca, nomes[i], &comparacoes);
            clock_t fim = clock();

            tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            tempoMicro = tempo * 1000000.0;

            printf("\nComparacoes: %d, Tempo: %.6f segundos\n", comparacoes, tempo);

            comparacoesTotal += comparacoes;
            tempoTotal += tempo;
            tempoMicroTotal += tempoMicro;
        }

        compFinal += (double)comparacoesTotal / 30;
        tempoFinal += tempoTotal / 30;
        tempoMicroFinal += tempoMicroTotal / 30;

        comparacoesTotal = 0;
        tempoTotal = 0.0;
        tempoMicroTotal = 0.0;
    }
    

    printf("\nMedia de comparacoes: %.2f\nMedia de tempo: %.6f segundos,\nMedia de tempo em microssegundos: %.3f segundos,\n Media de nos percorridos: %.2f\n", (double)compFinal / 30, tempoFinal / 30, tempoMicroFinal / 30, (double)compFinal/30);

    return 0;
}

/*

Media de comparacoes: 4.17
Media de tempo: 0.000002 segundos,
Media de tempo em microssegundos: 1.968 segundos,
Media de nos percorridos: 4.17

*/