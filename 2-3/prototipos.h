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
    TipoArtista tipo;
    char nome[50], estiloMusical[40];
    int numeroAlbuns;
    struct arvore *albuns;
}Artista;

typedef struct album{
    char nome[50];
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
    DadoUnion info1;
    DadoUnion info2;
    int nInfos;
    struct arvore *esq, *cen, *dir;
}Arvore;

Arvore *inicializar();
Musica *inicializarM();

Arvore *alocar(TipoDado tipo);
Musica *alocarM();
void preencheInfo(TipoDado tipo, DadoUnion *info);

Arvore *criaNo(DadoUnion info, Arvore *Fesq, Arvore *Fcen);
void adicionaInfo(Arvore **no, DadoUnion info, Arvore *filho);
Arvore *quebrarNo(Arvore **no, DadoUnion info, Arvore *filho, DadoUnion **sobe);


#endif