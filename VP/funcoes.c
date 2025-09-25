#include <stdio.h>
#include <stdlib.h>
#include "prototipos.h"

Arvore *inicializar(){
    return NULL;
}

Musica *inicializarM(){
    return NULL;
}

Arvore *alocar(TipoDado tipo){
    Arvore *no = (Arvore *)malloc(sizeof(Arvore));
    if(!no) return NULL;
    no->tipo = tipo;
    no->cor = VERMELHO;
    no->esq = no->dir = NULL;

    return (no);
}

Musica *alocarM(){
    Musica *no = (Musica *)malloc(sizeof(Musica));
    if(!no) return NULL;
    no->prox = NULL;

    return (no);
}