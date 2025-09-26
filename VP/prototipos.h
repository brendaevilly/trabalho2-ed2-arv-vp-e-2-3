#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct artista;
struct album;
struct musica;
struct arvore;

typedef enum {ARTISTA, ALBUM} TipoDado;
typedef enum {CANTOR, DUPLA, BANDA, GRUPO} TipoArtista;
typedef enum {VERMELHO, PRETO} Cor;

typedef struct artista {
    TipoArtista tipo;
    char nome[50], estiloMusical[50];
    int numeroAlbuns;
    struct arvore *albuns;
} Artista;

typedef struct album {
    char titulo[50];
    int anoLancamento, numeroMusicas;
    struct musica *musicas;
} Album;

typedef struct musica {
    char titulo[50];
    int minutos;
    struct musica *prox;
} Musica;

typedef union {
    struct artista ARTISTA;
    struct album ALBUM;
} DadoUnion;

typedef struct arvore {
    TipoDado tipo;
    DadoUnion dado;
    Cor cor;
    struct arvore *esq, *dir;
} Arvore;

// Inicialização
Arvore *inicializar();
Musica *inicializarM();

// Alocação e preenchimento
Arvore *alocarTree(TipoDado tipo);
Musica *alocarMusic();
void preencherNo(Arvore *no);

// Balanceamento
void rotacionarEsquerda(Arvore **raiz);
void rotacionarDireita(Arvore **raiz);
void trocaCor(Arvore **raiz);
void balanceamento(Arvore **raiz);

// Inserção
void inserirArvRubroNegra(Arvore **raiz, Arvore *novoNo);
void inserirMusica(Musica **lista, Musica novaMusica);

// Busca 
Arvore *buscarArvRubroNegra(Arvore *raiz, char *nome);
Musica *buscarMusica(Musica *lista, char *titulo);

// Remoção 
void removerArvRubroNegra(Arvore **raiz, char *nome);
void removerMusica(Musica **lista, char *titulo);

// Impressão
void imprimirArvRubroNegra(Arvore *raiz);
void imprimirMusicas(Musica *lista);

// Experimento
void experimentoBusca30(Arvore *raiz);

// Utilitários 
void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes);
void liberarMusicas(Musica *lista);
void liberarAlbuns(Arvore *albuns);
void liberarArtistas(Arvore *raiz);

#endif