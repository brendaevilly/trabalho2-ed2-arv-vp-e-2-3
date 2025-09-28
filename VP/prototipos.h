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

// Inicialização e Utilitários
Arvore *inicializar();
Musica *inicializarM();
void deixaMaiuscula(char *str);
struct tm *tempoAtual();

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
int inserirArvRubroNegra(Arvore **raiz, Arvore *novoNo);
void inserirMusica(Musica **lista, Musica novaMusica);

// Busca 
Arvore *buscarArvRubroNegra(Arvore *raiz, char *nome);
Musica *buscarMusic(Musica *lista, char *titulo);
Album *buscarAlbumDeArtista(Arvore *raiz, char *nomeArtista, char *tituloAlbum);
void buscarMusicaDeAlbum(Arvore *raiz, char *nomeArtista, char *tituloAlbum, char *tituloMusica);
void buscarMusicaDeArtista(Arvore *raiz, char *nomeArtista, char *tituloMusica);
void buscarArtistasPorEstilo(Arvore *raiz, char *estilo);

// Remoção 
int removerArvRubroNegra(Arvore **raiz, char *nome);
int removerMusic(Musica **lista, char *titulo);

// Impressão
void imprimirArvRubroNegra(Arvore *raiz);
void imprimirMusics(Musica *lista);
    //funções de mostrar especificas
    void mostrarAlbunsDeArtista(Arvore *raiz, char *nomeArtista);
    void mostrarMusicasDeAlbum(Arvore *raiz, char *nomeArtista, char *tituloAlbum);
    void mostrarArtistasPorEstilo(Arvore *raiz, char *estiloMusical);
    void mostrarMusicasDeArtista(Arvore *raiz, char *nomeArtista);
    void mostrarMusicaDeArtista(Arvore *raiz, char *nomeArtista, char *tituloMusica);

// Experimento
void experimentoBusca30(Arvore *raiz);

// Utilitários 
void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes);
void liberarMusicas(Musica *lista);
void liberarAlbuns(Arvore *albuns);
void liberarArtistas(Arvore *raiz);

#endif