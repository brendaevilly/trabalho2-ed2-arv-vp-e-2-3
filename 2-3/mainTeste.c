#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototipos.h"

#include <time.h>  // Para clock()

void mostrarCaminhoBusca23(Arvore *raiz, char *nome, int *comparacoes, int *nosPercorridos) {

    if (raiz) {
        char *nome1 = raiz->info1.ARTISTA.nome;
        char *nome2 = (raiz->nInfos == 2) ? raiz->info2.ARTISTA.nome : NULL;

        printf(" -> [%s]", nome1);
        (*comparacoes)++;
        (*nosPercorridos)++;
        if (strcmp(nome, nome1) == 0) {
            printf(" [ENCONTRADO]");
            return;
        }

        if (nome2) {
            printf(" -> [%s]", nome2);
            (*comparacoes)++;
            if (strcmp(nome, nome2) == 0) {
                printf(" [ENCONTRADO]");
                return;
            }
        }

        if (strcmp(nome, nome1) < 0) {
            mostrarCaminhoBusca23(raiz->esq, nome, comparacoes, nosPercorridos);
        } else if (raiz->nInfos == 1 || strcmp(nome, nome2) < 0) {
            mostrarCaminhoBusca23(raiz->cen, nome, comparacoes, nosPercorridos);
        } else {
            mostrarCaminhoBusca23(raiz->dir, nome, comparacoes, nosPercorridos);
        }
    } else {
        printf(" (NAO ESTA NA ARVORE)");
    }
// Tempo total
}


int main(){
    Arvore *biblioteca = inicializar();
    DadoUnion sobe;
    char *nomes[30] = {"ARTISTA1", "ARTISTA2", "ARTISTA3", "ARTISTA4", "ARTISTA5", "ARTISTA6", "ARTISTA7", "ARTISTA8", "ARTISTA9", "ARTISTA10", "ARTISTA11", "ARTISTA12", "ARTISTA13", "ARTISTA14", "ARTISTA15", "ARTISTA16", "ARTISTA17", "ARTISTA18", "ARTISTA19", "ARTISTA20", "ARTISTA21", "ARTISTA22", "ARTISTA23", "ARTISTA24", "ARTISTA25", "ARTISTA26", "ARTISTA27", "ARTISTA28", "ARTISTA29", "ARTISTA30"}; 
    double tempoTotal = 0.0;
    double tempoMicroTotal = 0.0;
    int comparacoesTotal = 0, nosPercorridosTotal = 0;

    for (int i = 0; i < 30; i++) {
        DadoUnion info;
        int inserido = 0;
        strcpy(info.ARTISTA.nome, nomes[i]);
        inserirNo(&biblioteca, info, NULL, &sobe,  &inserido, ARTISTA);
    }

    double tempoFinal = 0.0;
    double tempoMicroFinal = 0.0;
    double compFinal = 0.0;
    double nosPercFinal = 0.0;

    for (int j=0; j<30; j++){
        printf("\n--- Busca %d de buscas de 30 numeros ---\n", j + 1);
        for (int i = 0; i < 30; i++) {
            int comparacoes = 0;
            double tempo = 0.0;
            double tempoMicro = 0.0;
            int nosPercorridos = 0;

            printf("\nBusca %d: %s\nCaminho: ", i+1, nomes[i]);
            clock_t inicio = clock();
            mostrarCaminhoBusca23(biblioteca, nomes[i], &comparacoes, &nosPercorridos);
            clock_t fim = clock();

            tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
            tempoMicro = tempo * 1000000.0;

            printf("\nComparacoes: %d, Tempo: %.6f segundos\n", comparacoes, tempo);

            comparacoesTotal += comparacoes;
            tempoTotal += tempo;
            tempoMicroTotal += tempoMicro;
            nosPercorridosTotal += nosPercorridos;
        }

        compFinal += (double)comparacoesTotal / 30;
        tempoFinal += tempoTotal / 30;
        tempoMicroFinal += tempoMicroTotal / 30;
        nosPercFinal += (double)nosPercorridosTotal / 30;

        comparacoesTotal = 0;
        tempoTotal = 0.0;
        tempoMicroTotal = 0.0;
        nosPercorridosTotal = 0;
    }
    

    printf("\nMedia de comparacoes: %.2f\nMedia de tempo: %.6f segundos\nMedia de tempo: %.3f microssegundos\nMedia de nos percorridos: %.2f\n", (double)compFinal / 30, tempoFinal / 30, tempoMicroFinal / 30, (double)nosPercFinal / 30);

    return 0;
}

/*

Media de comparacoes: 4.83
Media de tempo: 0.000001 segundos
Media de tempo: 1.434 microssegundos
Media de nos percorridos: 3.27

*/