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

// operações básicas da arvore rubro-negra

// Criação e manipulação de nós
Arvore* criarNo(TipoDado tipo, DadoUnion dado);
void liberarArvore(Arvore* raiz);
// rotações
Arvore* rotacaoEsquerda(Arvore** h);
Arvore* rotacaoDireita(Arvore **h);
// Operações de cor
void trocarCores(Arvore **h);
int ehVermelho(Arvore* no);
// Inserção e remoção
Arvore* inserirRB(Arvore **h, TipoDado tipo, DadoUnion dado, char* nome);
Arvore* removerRB(Arvore **h, char* nome, TipoDado tipo);


//operações de busca

// Busca de artistas
Arvore* buscarArtista(Arvore* raiz, char* nome);
void buscarEExibirArtista(Arvore* raiz);
// Busca de álbuns
Arvore* buscarAlbum(Arvore* raizAlbuns, char* titulo);
void buscarEExibirAlbum(Arvore* raiz);
// Busca de músicas
Musica* buscarMusica(Musica* head, char* titulo);
void buscarMusicaGlobal(Arvore* raiz, char* tituloMusica);
void buscarMusicaEmAlbuns(Arvore* albumRaiz, char* tituloMusica, char* nomeArtista);
//operações de inserção
// Inserção de artistas
Arvore* inserirArtista(Arvore* raiz);
Arvore* inserirArtistaManual(Arvore* raiz, char* nome, TipoArtista tipo, char* estilo);
// Inserção de álbuns
void inserirAlbum(Arvore* raiz);
int inserirAlbumManual(Arvore* raiz, char* nomeArtista, char* tituloAlbum, int ano);
// Inserção de músicas
void inserirMusicaNoAlbum(Arvore* raiz);
int inserirMusicaManual(Arvore* raiz, char* nomeArtista, char* tituloAlbum, char* tituloMusica, int minutos);
Musica* inserirMusica(Musica* head, char* titulo, int minutos);


//operações de remoção

Arvore* removerArtista(Arvore* raiz);
void liberarArtista(Artista* artista);
// Remoção de álbuns
void removerAlbum(Arvore* raiz);
void liberarAlbum(Album* album);
// Remoção de músicas
void removerMusica(Arvore* raiz);
Musica* removerMusicaDaLista(Musica* head, char* titulo);
void liberarListaMusicas(Musica* head);


//operações de exibição

void listarArtistas(Arvore* raiz);
void listarArtistasDetalhado(Arvore* raiz);
void exibirArtista(Artista* artista);
// Listagem de álbuns
void listarAlbunsArtista(Arvore* albumRaiz);
void listarAlbunsDetalhado(Arvore* albumRaiz);
void exibirAlbum(Album* album);
// Listagem de músicas
void listarMusicas(Musica* head);
void listarMusicasDetalhado(Musica* head);
void exibirMusica(Musica* musica);

#endif