#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "prototipos.h"

// Inicialização, Alocação e Utilitários
Arvore *inicializar(){
    return NULL;
}

Musica *inicializarM(){
    return NULL;
}

void deixarMaiusculo(char *str){
    for(int i=0; str[i]!='\0'; i++) str[i] = toupper((unsigned char) str[i]);
}

Arvore *alocarTree(TipoDado tipo){
    Arvore *novo = (Arvore*) malloc(sizeof(Arvore));
    if(novo){
        if(!novo) return NULL;
        novo->tipo = tipo;
        novo->cor = VERMELHO;
        novo->esq = novo->dir = NULL;
    }
    return (novo);
}

int pegaCor(Arvore *R){
    int cor;
    if (R == NULL) cor = PRETO;
    else cor = R->cor;
    return cor;
}

void preencherNo(Arvore *no){
    if(no != NULL){
        if(no->tipo == ARTISTA) {
            printf("\n=== Cadastro de Artista ===\n");

            printf("Nome do artista: ");
            scanf(" %[^\n]", no->dado.ARTISTA.nome);
            deixarMaiusculo(no->dado.ARTISTA.nome);

            printf("Estilo musical: ");
            scanf(" %[^\n]", no->dado.ARTISTA.estiloMusical);
            deixarMaiusculo(no->dado.ARTISTA.estiloMusical);

            int op = 0;
            while (op < 1 || op > 4) {
                printf("Tipo (1-Cantor, 2-Dupla, 3-Banda, 4-Grupo): ");
                scanf("%d", &op); printf("\n");
            }
            no->dado.ARTISTA.TipArt = (TipoArtista) op;

            no->dado.ARTISTA.numeroAlbuns = 0;
            no->dado.ARTISTA.albuns = inicializar();  
        }
        else if(no->tipo == ALBUM) {
            printf("\n=== Cadastro de Álbum ===\n");

            printf("Título do álbum: ");
            scanf(" %[^\n]", no->dado.ALBUM.titulo);
            deixarMaiusculo(no->dado.ALBUM.titulo);

            printf("Ano de lançamento: ");
            scanf("%d", &no->dado.ALBUM.anoLancamento);

            no->dado.ALBUM.numeroMusicas = 0;
            no->dado.ALBUM.musicas = inicializarM();  
        }
    }else{
        printf("Erro!\n");
    }
}

void preencherMusica(Musica *musica) {
    if(musica != NULL){
        printf("\n=== Cadastro de Música ===\n");

        printf("Título da música: ");
        scanf(" %[^\n]", musica->titulo);
        deixarMaiusculo(musica->titulo);

        printf("Duração (em minutos): ");
        scanf("%d", &musica->minutos);
    }
}


Musica *alocarMusica(){
    Musica *no = (Musica*)malloc(sizeof(Musica));
    if(no) no->prox = NULL;

    return (no);
}

struct tm *tempoAtual(){
    time_t agora;
    time(&agora);

    struct tm *infoTempoLocal = malloc(sizeof(struct tm));  // aloca memória própria
    if(infoTempoLocal){
        struct tm *tmp = localtime(&agora);
        if(tmp) *infoTempoLocal = *tmp;
    }

    return infoTempoLocal;
}

// Balanceamento
void rotacionarEsquerda(Arvore **raiz){
    Arvore *aux = (*raiz)->dir;
    (*raiz)->dir = aux->esq;
    aux->esq = *raiz;
    aux->cor = pegaCor(*raiz);
    (*raiz)->cor = VERMELHO;
    *raiz = aux;  
}

void rotacionarDireita(Arvore **raiz){
    Arvore *aux = (*raiz)->esq;
    (*raiz)->esq = aux->dir;
    aux->dir = *raiz;
    aux->cor = pegaCor(*raiz);
    (*raiz)->cor = VERMELHO;
    *raiz = aux;
}


void trocaCor(Arvore **raiz){
    (*raiz)->cor = !(*raiz)->cor;
    if((*raiz)->esq) (*raiz)->esq->cor = !(*raiz)->esq->cor;
    if((*raiz)->dir) (*raiz)->dir->cor = !(*raiz)->dir->cor;
}


void balanceamento(Arvore **raiz) {
    if (raiz && *raiz) {
        if (((*raiz)->dir && pegaCor((*raiz)->dir) == VERMELHO) && (!((*raiz)->esq)|| pegaCor((*raiz)->esq) == PRETO)) 
            rotacionarEsquerda(raiz);

        if (((*raiz)->esq && pegaCor((*raiz)->esq) == VERMELHO) && ((*raiz)->esq->esq && pegaCor((*raiz)->esq->esq) == VERMELHO)) 
            rotacionarDireita(raiz);
        
        if (((*raiz)->esq && (*raiz)->dir) && pegaCor((*raiz)->esq) == VERMELHO && pegaCor((*raiz)->dir) == VERMELHO) 
            trocaCor(raiz);
    }
}

int inserirArvore(Arvore **raiz, Arvore *novoNo){
    int inseriu = 0;
    if(raiz){
        if (*raiz == NULL){
            *raiz = novoNo;
            inseriu = 1;
        }
        else {
            if(strcmp(novoNo->dado.ARTISTA.nome, (*raiz)->dado.ARTISTA.nome) < 0)
                inseriu = inserirArvore(&(*raiz)->esq, novoNo);
            else inseriu = inserirArvore(&(*raiz)->dir, novoNo);
        } 
    }
    balanceamento(raiz);
    return inseriu;
}

void atualizaCorRaiz(Arvore **raiz){
    if(raiz != NULL && *raiz != NULL)
        (*raiz)->cor = PRETO;
}

int inserirMusica(Musica **lista, Musica *novaMusica){
    int inseriu = 1;
    if(*lista == NULL){
        *lista = novaMusica;
    } else {
        Musica *atual = *lista;
        Musica *ant = inicializarM();
        while(strcmp((novaMusica, atual) >= 0) && atual){ 
            if(strcmp(atual->titulo, novaMusica->titulo) == 0) inseriu = 0;
            ant = atual;
            atual = atual->prox;
        }
        if(inseriu){
            if(strcmp(atual->titulo, (*lista)->titulo) == 0){
                *novaMusica = *lista;
                *lista = novaMusica;
            }else{
                if(!atual) ant->prox = novaMusica;
                else{
                    ant->prox = novaMusica;
                    novaMusica->prox = atual;
                }
            }
        }
        
    }
    return inseriu;
}

// Busca

Arvore *buscarArvRubroNegra(Arvore *raiz, char *nome){
    if(raiz != NULL){
        if(strcmp(nome, raiz->dado.ARTISTA.nome) == 0) {
            return raiz;
        } else if(strcmp(nome, raiz->dado.ARTISTA.nome) < 0) {
            return buscarArvRubroNegra(raiz->esq, nome);
        } else {
            return buscarArvRubroNegra(raiz->dir, nome);
        }
    } else {
        printf("ERROR!");
        return NULL; 
    }
}

Musica *buscarMusica(Musica *lista, char *titulo){
    Musica *aux = lista;
    while(aux != NULL){
        if(strcmp(titulo, aux->titulo) == 0){
            return aux;
        }
        aux = aux->prox;
    }
    return NULL;
}

// função de mostrar o caminha da busca que foi pedido no trabalho
void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes){
    if(raiz == NULL){
        printf(" (NULL) ");
        return;
    }

    printf(" -> [%s]", raiz->dado.ARTISTA.nome);
    (*comparacoes)++;

    if(strcmp(nome, raiz->dado.ARTISTA.nome) == 0){
        printf("  [ENCONTRADO]\n");
    } 
    else if(strcmp(nome, raiz->dado.ARTISTA.nome) < 0){
        mostrarCaminhoBusca(raiz->esq, nome, comparacoes);
    } 
    else {
        mostrarCaminhoBusca(raiz->dir, nome, comparacoes);
    }
}

/*Arvore *buscarAlbumDeArtista(Arvore *raiz, char *nomeArtista, char *tituloAlbum){
    Album *album = NULL;
    Arvore *artista = buscarArvRubroNegra(raiz, nomeArtista);
    if(artista != NULL && artista->tipo == ARTISTA){
        //Arvore *albumNo = artista->dado.ARTISTA.albuns;
        Arvore *albumNo = buscarArvRubroNegra(artista->dado.ARTISTA.albuns, tituloAlbum);
        if(albumNo != NULL && albumNo->tipo == ALBUM){
            album = &albumNo->dado.ALBUM;
                }
            } 
    return (album);
}*/

Arvore *buscarAlbumDeArtista(Arvore *artista, char *tituloAlbum) {
    if (artista == NULL || artista->tipo != ARTISTA) {
        printf("Artista inválido ou não encontrado.\n");
        return NULL;
    }

    Arvore *raizAlbuns = artista->dado.ARTISTA.albuns;
    while (raizAlbuns != NULL) {
        int comparacao = strcmp(tituloAlbum, raizAlbuns->dado.ALBUM.titulo);
        if (comparacao == 0) {
            return raizAlbuns;
        } else if (comparacao < 0) {
            raizAlbuns = raizAlbuns->esq;
        } else {
            raizAlbuns = raizAlbuns->dir;
        }
    }

    return NULL; // álbum não encontrado
}


Musica *buscarMusicaEmAlbuns(Arvore *albuns, char *titulo) {
    if(albuns == NULL) return NULL;
    Musica *m = buscarMusicaEmAlbuns(albuns->esq, titulo);
    if(m) return m;
    if(albuns->tipo == ALBUM) {
        Musica *res = buscarMusica(albuns->dado.ALBUM.musicas, titulo);
        if(res) return res;
    }
    return buscarMusicaEmAlbuns(albuns->dir, titulo);
}


Musica *buscarMusicaDeAlbum(Arvore *raiz, char *nomeArtista, char *tituloAlbum, char *tituloMusica){
    Musica *musica = NULL;
    Arvore *album = buscarAlbumDeArtista(raiz, tituloAlbum);
    if(album != NULL){
        musica = buscarMusica(album->dado.ALBUM.musicas, tituloMusica);
    }
    return musica;
}

Musica *buscarMusicaDeArtista(Arvore *raiz, char *nomeArtista, char *tituloMusica) {
    Musica *musica = NULL;
    Arvore *artista = buscarArvRubroNegra(raiz, nomeArtista);
    if (artista != NULL && artista->tipo == ARTISTA) {
        musica = buscarMusicaEmAlbuns(artista->dado.ARTISTA.albuns, tituloMusica);
    }
    return musica;
}

void buscarArtistasPorEstilo(Arvore *raiz, char *estilo) {
    if (raiz != NULL){
        deixarMaiusculo(estilo);
        buscarArtistasPorEstilo(raiz->esq, estilo);
        if (raiz->tipo == ARTISTA && strcmp(raiz->dado.ARTISTA.estiloMusical, estilo) == 0) {
            printf("🎵 Artista encontrado: %s (%s)\n", 
                raiz->dado.ARTISTA.nome, 
                raiz->dado.ARTISTA.estiloMusical);
        }
        buscarArtistasPorEstilo(raiz->dir, estilo);
    } else {
        printf("Não existem artistas com esse estilo musical cadastrados.\n");
        exit(1);
    }
    return;
}

// Remoção int removerArvRubroNegra(Arvore **raiz, char *nome); // aqui vem a remover da rubro negra
void move2EsqRed(Arvore **R){
    trocaCor(R);

    if ((*R)->dir && ((*R)->dir->esq) == VERMELHO){
        rotacionarDireita(&(*R)->dir);
        rotacionarEsquerda(R);
        trocaCor(R);
    }

}

void move2DirRed(Arvore **R){
    trocaCor(R);
    if ((*R)->esq && ((*R)->esq->esq) == VERMELHO){
        rotacionarDireita(R);
        trocaCor(R);

    }
}

void removeMenor(Arvore **R){
    if (!(*R)->esq){
        free(*R);
        *R = NULL;
    } else {
        if (pegaCor((*R)->esq) == PRETO && pegaCor((*R)->esq->esq) == PRETO){
            move2EsqRed(R);
        }
        
        removeMenor(&(*R)->esq);
        balanceamento(R);
    }
}

Arvore *procuraMenor(Arvore *R){
    Arvore *menor = R;

    if(R){
        if(R->esq){
            menor = procuraMenor(R->esq);
        }
    }

    return(menor);
}

int removeNoRN(Arvore **R, char *nome){
    int existe = 0;
    if(*R){
        if (strcmp(nome, (*R)->dado.ARTISTA.nome) < 0){
            if((*R)->esq && pegaCor((*R)->esq) == PRETO && pegaCor((*R)->esq->esq) == PRETO){
                move2EsqRed(R);
            } existe = removeNoRN(&(*R)->esq, nome);
        } else {
            
            if(pegaCor((*R)->esq) == VERMELHO){
                rotacionarDireita(R);
            }

            if (strcmp(nome, (*R)->dado.ARTISTA.nome) == 0 && (*R)->dir == NULL)
            {
                free(*R);
                *R = NULL;
                existe = 1;
            } else {

            if((*R)->dir && pegaCor((*R)->dir) == PRETO && pegaCor((*R)->dir->esq) == PRETO){
                    move2DirRed(R);
            }
            if(strcmp(nome, (*R)->dado.ARTISTA.nome) == 0){
                Arvore *aux = procuraMenor((*R)->dir);
                (*R)->dado = aux->dado;
                removeMenor(&(*R)->dir);
                existe = 1;
            }
            else {
                existe = removeNoRN(&(*R)->dir, nome);
                }
            }
        }
    }
    balanceamento(R);
    return(existe);
}

int removerArvRubroNegra(Arvore **R, char *nome){
    int removeu = removeNoRN(R, nome);
    if(removeu){
        (*R)->cor = PRETO;
    }
    return(removeu);
}

int removerMusica(Musica **lista, char titulo[]){
    int removeu = 0;
    if(*lista != NULL){
        Musica *aux = *lista;
        Musica *ant = NULL;

        while(aux != NULL && strcmp(aux->titulo, titulo) != 0){
            ant = aux;
            aux = aux->prox;
        }

        if(aux != NULL){
            if(ant == NULL){
                *lista = aux->prox;
            } else {
                ant->prox = aux->prox;
            }
            free(aux);
            removeu = 1;
        }
    }
    return (removeu);
}


// Impresão e funções de mostrar 

void imprimirArvRubroNegra(Arvore *no){
    if(no != NULL){
        imprimirArvRubroNegra(no->esq);
        if(no->tipo == ARTISTA) {
            printf("\n=== Artista ===\n");
            printf("Nome: %s\n", no->dado.ARTISTA.nome);
            printf("Estilo musical: %s\n", no->dado.ARTISTA.estiloMusical);
            printf("Tipo: ");
            switch(no->dado.ARTISTA.TipArt) {
                case CANTOR: printf("Cantor\n"); break;
                case DUPLA: printf("Dupla\n"); break;
                case BANDA: printf("Banda\n"); break;
                case GRUPO: printf("Grupo\n"); break;
                default: printf("Desconhecido\n"); break;
            }
            printf("Número de álbuns: %d\n", no->dado.ARTISTA.numeroAlbuns);
        }
        else if(no->tipo == ALBUM) {
            printf("\n=== Álbum ===\n");
            printf("Título: %s\n", no->dado.ALBUM.titulo);
            printf("Ano de lançamento: %d\n", no->dado.ALBUM.anoLancamento);
            printf("Número de músicas: %d\n", no->dado.ALBUM.numeroMusicas);
        }
        imprimirArvRubroNegra(no->dir);
    }
}

void imprimirMusicas(Musica *lista){
    if(lista != NULL){
        Musica *aux = lista;
        printf("\n=== Músicas ===\n");
        while(aux != NULL){
            printf("Título: %s, Duração: %d minutos\n", aux->titulo, aux->minutos);
            aux = aux->prox;
        }
    } else {
        printf("Nenhuma música cadastrada.\n");
    }
}

void mostrarArtistas(Arvore *no, char *estilo){
    if(no != NULL){
        mostrarArtistas(no->esq, estilo);
        if(no->tipo == ARTISTA && strcmp(no->dado.ARTISTA.estiloMusical, estilo) == 0)
            printf("Nome: %s\n", no->dado.ARTISTA.nome);
        mostrarArtistas(no->dir, estilo);
    }
}

void listarMusicas(Arvore *no) {
    if (no == NULL) return;
    listarMusicas(no->esq);
    if (no->tipo == ALBUM)
        imprimirMusicas(no->dado.ALBUM.musicas);
    listarMusicas(no->dir);
}

void mostrarAlbunsDeArtista(Arvore *raiz, char *nomeArtista){
    Arvore *artista = buscarArvRubroNegra(raiz, nomeArtista);
    if(artista != NULL && artista->tipo == ARTISTA){
        printf("\n=== Álbuns de %s ===\n", artista->dado.ARTISTA.nome);
        imprimirArvRubroNegra(artista->dado.ARTISTA.albuns);
    } else {
        printf("Artista não encontrado.\n");
    }
}

void mostrarMusicasDeAlbum(Arvore *album){
    if(album != NULL){
        printf("\n=== Músicas do Álbum %s ===\n", album->dado.ALBUM.titulo);
        imprimirMusicas(album->dado.ALBUM.musicas);
    } else {
        printf("Álbum ou artista não encontrado.\n");
    }
}

        //travessia in-order dos álbuns 
void imprimirAlbunsRec(Arvore *n) {
    if(!n) return;
    imprimirAlbunsRec(n->esq);
    if(n->tipo == ALBUM) {
        printf("\n--- Álbum: %s ---\n", n->dado.ALBUM.titulo);
        imprimirMusicas(n->dado.ALBUM.musicas);
    }
    imprimirAlbunsRec(n->dir);
}

void mostrarMusicasDeArtista(Arvore *raiz, char *nomeArtista){
    Arvore *artista = buscarArvRubroNegra(raiz, nomeArtista);
    if(artista != NULL && artista->tipo == ARTISTA){
        printf("\n=== Músicas de %s ===\n", artista->dado.ARTISTA.nome);
        Arvore *aux = artista->dado.ARTISTA.albuns;
        if(aux == NULL) {
            printf("Nenhum álbum cadastrado para este artista.\n");
        } else {
            imprimirAlbunsRec(aux);
        }
    } else {
        printf("Artista não encontrado.\n");
    }
}

void mostrarArtistasPorEstilo(Arvore *raiz, char *estilo){
    printf("\n=== Artistas do estilo %s ===\n", estilo);
    mostrarArtistas(raiz, estilo);
}

void liberarMusicas(Musica *lista){
    Musica *aux;
    while(lista != NULL){
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
}

void liberarArvore(Arvore *raiz){
    if(raiz != NULL){
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        if(raiz->tipo == ARTISTA){
            liberarArvore(raiz->dado.ARTISTA.albuns);
        } else if(raiz->tipo == ALBUM){
            liberarMusicas(raiz->dado.ALBUM.musicas);
        }
        free(raiz);
    }
}

void liberarTempo(struct tm *tempo){
    if(tempo) free(tempo);
}

void liberarString(char *str){
    if(str) free(str);
}