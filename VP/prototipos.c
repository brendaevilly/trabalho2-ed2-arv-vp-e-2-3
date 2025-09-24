#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H

struct artista;
struct album;
struct musica;
struct arvore;

typedef enum {ARTISTA, ALBUM} TipoDado;
typedef enum {CANTOR, DUPLA, BANDA, GRUPO} TipoArtista;
typedef enum {VERMELHO, PRETO} Cor;

typedef struct artista{
    char nome[50], estiloMusical[50];
    int numeroAlbuns;
    struct arvore *albuns;
}Artista;

typedef struct album{
    char titulo[50];
    int anoLancamento, numeroMusicas;
    struct musica *musicas;
}Album;

typedef struct musica{
    char titulo[50];
    int minutos;
    struct musica *prox;
}Musica;

typedef union{
    struct artista ARTISTA;
    struct album ALBUM;
}DadoUnion;

typedef struct arvore{
    TipoDado tipo;
    DadoUnion dado;
    Cor cor;
    struct arvore *esq, *dir;
}Arvore;

#endif