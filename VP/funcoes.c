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

void preencherNo(Arvore *no) {
    if(no != NULL){
        if(no->tipo == ARTISTA) {
            printf("\n=== Cadastro de Artista ===\n");

            printf("Nome do artista: ");
            scanf(" %[^\n]", no->dado.ARTISTA.nome);

            printf("Estilo musical: ");
            scanf(" %[^\n]", no->dado.ARTISTA.estiloMusical);

            printf("Tipo (0-Cantor, 1-Dupla, 2-Banda, 3-Grupo): ");
            int t;
            scanf("%d", &t);
            no->dado.ARTISTA.tipo = (TipoArtista)t;

            no->dado.ARTISTA.numeroAlbuns = 0;
            no->dado.ARTISTA.albuns = inicializar();  
        }
        else if(no->tipo == ALBUM) {
            printf("\n=== Cadastro de Álbum ===\n");

            printf("Título do álbum: ");
            scanf(" %[^\n]", no->dado.ALBUM.titulo);

            printf("Ano de lançamento: ");
            scanf("%d", &no->dado.ALBUM.anoLancamento);

            no->dado.ALBUM.numeroMusicas = 0;
            no->dado.ALBUM.musicas = inicializarM();  
        }
    }
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

void inserirArvRubroNegra(Arvore **raiz, Arvore *novoNo){
    if (raiz == NULL){
        raiz = &novoNo;
    }
    else {
        if(strcmp(novoNo->dado.ARTISTA.nome, (*raiz)->dado.ARTISTA.nome) < 0) {
            inserirArvRubroNegra(&(*raiz)->esq, novoNo);
        } else {
            inserirArvRubroNegra(&(*raiz)->dir, novoNo);
        }
    } 

    balanceamento(raiz);
}
