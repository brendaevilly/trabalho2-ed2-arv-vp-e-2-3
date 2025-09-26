#include <stdio.h>
#include <stdlib.h>
#include "prototipos.h"


// Inicialização e Alocação
Arvore *inicializar(){
    return NULL;
}

Musica *inicializarM(){
    return NULL;
}

Arvore *alocarTree(TipoDado tipo){
    Arvore *no = (Arvore *)malloc(sizeof(Arvore));
    if(!no) return NULL;
    no->tipo = tipo;
    no->cor = VERMELHO;
    no->esq = no->dir = NULL;

    return (no);
}

Musica *alocarMusic(){
    Musica *no = (Musica*)malloc(sizeof(Musica));
    if(!no) return NULL;
    no->prox = NULL;

    return (no);
}

// Balanceamento
void rotacionarEsquerda(Arvore **raiz){
    Arvore *aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    *raiz = aux;
}

void rotacionarDireita(Arvore **raiz){
    Arvore *aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    *raiz = aux;
}

void trocaCor(Arvore **raiz){
        if((*raiz)->esq && (*raiz)->esq->cor == VERMELHO){
            (*raiz)->cor = VERMELHO;
            (*raiz)->esq->cor = PRETO;
            (*raiz)->dir->cor = PRETO;
        }
}

void balanceamento(Arvore **raiz){

    if (((*raiz)->dir->cor == VERMELHO) && ((*raiz)->esq->cor == PRETO)){
        rotacionarEsquerda(raiz);
    }
    if(((*raiz)->esq->cor == VERMELHO) && ((*raiz)->esq->esq->cor == VERMELHO)){
        rotacionarDireita(raiz);
    }
    if(((*raiz)->esq->cor == VERMELHO) && ((*raiz)->dir->cor == VERMELHO)){
        trocaCor(raiz);
    }
}

void inser