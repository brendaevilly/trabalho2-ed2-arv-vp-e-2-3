#include <stdio.h>
#include <stdlib.h>
#include "prototipos.h"


//funções básicas da arvore rubro-negra (aqui criamos os nós, rotacionamos etc)

Arvore* criarNo (TipoDado tipo, DadoUnion dado) {
    Arvore* novoNo = (Arvore*)malloc(sizeof(Arvore));
    if (!novoNo) {
        fprintf(stderr, "Erro ao alocar memoria para novo no.\n");
        exit(EXIT_FAILURE);
    }
    novoNo->tipo = tipo;
    novoNo->dado = dado;
    novoNo->cor = VERMELHO; 
    novoNo->esq = novoNo->dir = NULL;
    return novoNo;
}