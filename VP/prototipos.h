#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

struct artista;
struct album;
struct musica;
struct arvore;

typedef enum {ARTISTA, ALBUM} TipoDado;
typedef enum {CANTOR = 1, DUPLA, BANDA, GRUPO} TipoArtista;
typedef enum {VERMELHO, PRETO} Cor;

typedef struct artista {
    TipoArtista TipArt;
    char nome[50], estiloMusical[70];
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

/* --- Inicialização e Utilitários --- */
Arvore *inicializar();
Musica *inicializarM();
void deixarMaiusculo(char *str);

/* --- Alocação e preenchimento --- */
Arvore *alocarTree(TipoDado tipo);
Musica *alocarMusica();
void preencherNo(Arvore *no);
void preencherMusica(Musica *musica);

/* --- Balanceamento --- */
void rotacionarEsquerda(Arvore **raiz);
void rotacionarDireita(Arvore **raiz);
void trocaCor(Arvore **raiz);
void balanceamento(Arvore **raiz);
void atualizaCorRaiz(Arvore **raiz);

/* --- Inserção --- */
int inserirArvore(Arvore **raiz, Arvore *novoNo);
int inserirMusica(Musica **lista, Musica *novaMusica);

/* --- Busca --- */
Arvore *buscarArvRubroNegra(Arvore *raiz, char *nome);
Musica *buscarMusica(Musica *lista, char *titulo);
void buscarAlbumDeArtista(Arvore *artista, char *tituloAlbum, Arvore **busca);
Musica *buscarMusicaDeAlbum(Arvore *album, char *tituloAlbum, char *tituloMusica);
Musica *buscarMusicaDeArtista(Arvore *raiz, char *nomeArtista, char *tituloMusica);
void buscarMusicaEmAlbuns(Arvore *albuns, Musica **busca, char *tituloMusica);
/* --- Remoção --- */
int removerArvRubroNegra(Arvore **raiz, char *nome);
int removeNoRN(Arvore **raiz, char *nome); 
void move2EsqRed(Arvore **raiz);
void move2DirRed(Arvore **raiz);
void removeMenor(Arvore **raix);
Arvore *procuraMenor(Arvore *raiz);
int removerMusica(Musica **lista, char *titulo);

/* --- Impressão e mostrar específicas --- */
void imprimirArvRubroNegra(Arvore *raiz);
void imprimirMusicas(Musica *lista);
void mostrarAlbunsDeArtista(Arvore *raiz, char *nomeArtista);
void mostrarMusicasDeAlbum(Arvore *raiz);
void mostrarArtistasPorEstilo(Arvore *raiz, char *estiloMusical);
void mostrarMusicasDeArtista(Arvore *raiz, char *nomeArtista);

/* --- Utilitários --- */
void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes);
void liberarMusicas(Musica *lista);
void liberarAlbuns(Arvore *albuns);
void liberarArvore(Arvore *raiz);

#endif