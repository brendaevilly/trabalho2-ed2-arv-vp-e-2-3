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
        novo->tipo = tipo;
        novo->cor = VERMELHO;
        novo->esq = novo->dir = NULL;

        if(tipo == ARTISTA){
            novo->dado.ARTISTA.numeroAlbuns = 0;
            novo->dado.ARTISTA.albuns = inicializar();
        } else if(tipo == ALBUM){
            novo->dado.ALBUM.numeroMusicas = 0;
            novo->dado.ALBUM.musicas = inicializarM();
        }
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
            printf("\n=== Cadastro de Album ===\n");

            printf("Titulo do album: ");
            scanf(" %[^\n]", no->dado.ALBUM.titulo);
            deixarMaiusculo(no->dado.ALBUM.titulo);

            printf("Ano de lancamento: ");
            scanf("%d", &no->dado.ALBUM.anoLancamento);

            while(no->dado.ALBUM.anoLancamento > 2025){
                printf("[ERRO] Insira um ano valido.\n");
                printf("Ano de lançamento: ");
                scanf("%d", &no->dado.ALBUM.anoLancamento);
            }

            no->dado.ALBUM.numeroMusicas = 0;
            no->dado.ALBUM.musicas = inicializarM();  
        }
    }else{
        printf("Erro!\n");
    }
}

void preencherMusica(Musica *musica) {
    if(musica != NULL){
        printf("\n=== Cadastro de Musica ===\n");

        printf("Titulo da musica: ");
        scanf(" %[^\n]", musica->titulo);
        deixarMaiusculo(musica->titulo);

        printf("Duracao (em minutos): ");
        scanf("%d", &musica->minutos);
    }
}


Musica *alocarMusica(){
    Musica *no = (Musica*)malloc(sizeof(Musica));
    if(no) no->prox = NULL;

    return (no);
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
    if (*raiz) {
        if (pegaCor((*raiz)->dir) == VERMELHO && pegaCor((*raiz)->esq) == PRETO)
            rotacionarEsquerda(raiz);

        if (pegaCor((*raiz)->esq) == VERMELHO && pegaCor((*raiz)->esq->esq) == VERMELHO)
            rotacionarDireita(raiz);
        
        if (pegaCor((*raiz)->esq) == VERMELHO && pegaCor((*raiz)->dir) == VERMELHO) 
            trocaCor(raiz);
    }
}

int inserirArvore(Arvore **raiz, Arvore *novoNo){
    int inseriu = 0;
    if (*raiz == NULL){
        *raiz = novoNo;
        inseriu = 1;
    }
    else {
        if(strcmp(novoNo->dado.ARTISTA.nome, (*raiz)->dado.ARTISTA.nome) < 0)
            inseriu = inserirArvore(&(*raiz)->esq, novoNo);
        else if(strcmp(novoNo->dado.ARTISTA.nome, (*raiz)->dado.ARTISTA.nome) > 0) 
            inseriu = inserirArvore(&(*raiz)->dir, novoNo);
    }

    if(inseriu) balanceamento(raiz);
    return inseriu;
}

void atualizaCorRaiz(Arvore **raiz){
    if(*raiz) (*raiz)->cor = PRETO;
}

int inserirMusica(Musica **lista, Musica *novaMusica){
    int inseriu = 1;
    if(*lista == NULL){
        *lista = novaMusica;
    } else {
        Musica *atual = *lista;
        Musica *ant = inicializarM();
        while(atual && strcmp(novaMusica->titulo, atual->titulo) >= 0){ 
            if(strcmp(atual->titulo, novaMusica->titulo) == 0) inseriu = 0;
            ant = atual;
            atual = atual->prox;
        }
        if(inseriu){
            if(!atual) ant->prox = novaMusica;
            else if(strcmp(atual->titulo, (*lista)->titulo) == 0){
                novaMusica->prox = *lista;
                *lista = novaMusica;
            }else{
                ant->prox = novaMusica;
                novaMusica->prox = atual;
            }
        }
        
    }
    return inseriu;
}

// Busca

Arvore *buscarArvRubroNegra(Arvore *raiz, char *nome){
    Arvore *busca = NULL;
    if(raiz){
        int cmp;
        if(raiz->tipo == ALBUM)
            cmp = strcmp(nome, raiz->dado.ALBUM.titulo);
        else cmp = strcmp(nome, raiz->dado.ARTISTA.nome);

        if(cmp == 0) busca = raiz;
        else if(cmp < 0)
            busca = buscarArvRubroNegra(raiz->esq, nome);
        else busca = buscarArvRubroNegra(raiz->dir, nome);
    }

    return busca;
}

Musica *buscarMusica(Musica *lista, char *titulo){
    Musica *aux = lista, *busca = NULL;
    while(aux){
        if(strcmp(titulo, aux->titulo) == 0) busca = aux;
        aux = aux->prox;
    }
    return busca;
}

// função de mostrar o caminha da busca que foi pedido no trabalho
void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes){
    if(raiz){
        printf(" -> [%s]", raiz->dado.ARTISTA.nome);
        (*comparacoes)++;

        if(strcmp(nome, raiz->dado.ARTISTA.nome) == 0) printf("  [ENCONTRADO]\n");
        else if(strcmp(nome, raiz->dado.ARTISTA.nome) < 0) mostrarCaminhoBusca(raiz->esq, nome, comparacoes); 
        else mostrarCaminhoBusca(raiz->dir, nome, comparacoes);
    }else printf(" (NAO ESTA NA ARVORE) ");
}

void buscarAlbumDeArtista(Arvore *artista, char *tituloAlbum, Arvore **busca){
    Arvore *raizAlbuns = artista->dado.ARTISTA.albuns;
    if(raizAlbuns) {
        *busca = buscarArvRubroNegra(raizAlbuns, tituloAlbum);
    } 
}

Musica *buscarMusicaDeAlbum(Arvore *album, char *tituloAlbum, char *tituloMusica){
    Musica *musica = NULL;
    if(album) 
        musica = buscarMusica(album->dado.ALBUM.musicas, tituloMusica);
    return musica;
}

void buscarMusicaEmAlbuns(Arvore *albuns, Musica **busca, char *tituloMusica) {
    if(albuns && !(*busca)){
        *busca = buscarMusicaDeAlbum(albuns, albuns->dado.ALBUM.titulo, tituloMusica);
        buscarMusicaEmAlbuns(albuns->esq, busca, tituloMusica);
        buscarMusicaEmAlbuns(albuns->dir, busca, tituloMusica);
    }
}

Musica *buscarMusicaDeArtista(Arvore *artistas, char *nomeArtista, char *tituloMusica) {
    Musica *musica = NULL;
    Arvore *artista = buscarArvRubroNegra(artistas, nomeArtista);
    if (artista)
        buscarMusicaEmAlbuns(artista->dado.ARTISTA.albuns, &musica, tituloMusica);
    return musica;
}

void move2EsqRed(Arvore **R){
    trocaCor(R);
    if ((*R)->dir){
        if(pegaCor((*R)->dir->esq) == VERMELHO){
            rotacionarDireita(&(*R)->dir);
            rotacionarEsquerda(R);
            trocaCor(R); 
        }
    }
}

void move2DirRed(Arvore **R){
    trocaCor(R);
    if ((*R)->esq){
        if(pegaCor((*R)->esq->esq) == VERMELHO){
            rotacionarDireita(R);
            trocaCor(R);
        }
        
    }
}

void removeMenor(Arvore **R){
    if (!(*R)->esq){
        Arvore *aux = *R;
        *R = (*R)->dir;
        free(aux);
    } else {
        if ((*R)->esq && pegaCor((*R)->esq) == PRETO && pegaCor((*R)->esq->esq) == PRETO)
            move2EsqRed(R);
        removeMenor(&(*R)->esq);
        balanceamento(R);
    }
}

Arvore *procuraMenor(Arvore *R) {
    Arvore *menor = R;
    if (R && R->esq)
        menor = procuraMenor(R->esq);
    return menor;
}


int removeNoRN(Arvore **R, char *nome) {
    int existe = 0;
    if (*R) {
        int cmp;
        if ((*R)->tipo == ALBUM)
            cmp = strcmp(nome, (*R)->dado.ALBUM.titulo);
        else
            cmp = strcmp(nome, (*R)->dado.ARTISTA.nome);

        if (cmp < 0) {
            if (pegaCor((*R)->esq) == PRETO) {
                if ((*R)->esq && pegaCor((*R)->esq->esq) == PRETO)
                    move2EsqRed(R);
            }
            existe = removeNoRN(&(*R)->esq, nome);
        } else {
            if ((*R)->esq) {
                if (pegaCor((*R)->esq) == VERMELHO)
                    rotacionarDireita(R);
            }

            if (cmp == 0 && (*R)->dir == NULL) {
                free(*R);
                *R = NULL;
                existe = 1;
            } else {
                if ((*R)->dir && pegaCor((*R)->dir) == PRETO && pegaCor((*R)->dir->esq) == PRETO)
                    move2DirRed(R);

                if (cmp == 0) {
                    Arvore *aux = procuraMenor((*R)->dir);
                    (*R)->dado = aux->dado;
                    removeMenor(&(*R)->dir);
                    existe = 1;
                } else {
                    existe = removeNoRN(&(*R)->dir, nome);
                }
            }
        }
    }

    balanceamento(R);
    return existe;
}


int removerArvRubroNegra(Arvore **R, char *nome){
    int removeu = removeNoRN(R, nome);
    if(removeu && *R){
        (*R)->cor = PRETO;
    }
    return(removeu);
}


int removerMusica(Musica **lista, char *titulo){
    int removeu = 0;
    if(*lista){
        Musica *aux = *lista;
        Musica *ant = NULL;

        while(aux && strcmp(aux->titulo, titulo) != 0){
            ant = aux;
            aux = aux->prox;
        }

        if(aux){
            Musica *l = inicializarM();
            if(ant == NULL){
                l = *lista;
                free(l);
                *lista = aux->prox;
            }else{
                l = aux;
                ant->prox = aux->prox;
                free(l);
            }
            removeu = 1;
        }
    }
    return (removeu);
}


// Impresão e funções de mostrar 

void imprimirArvRubroNegra(Arvore *no){
    if(no){
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
            printf("Numero de albuns: %d\n", no->dado.ARTISTA.numeroAlbuns);
        }
        else if(no->tipo == ALBUM) {
            printf("\n=== Album ===\n");
            printf("Titulo: %s\n", no->dado.ALBUM.titulo);
            printf("Ano de lancamento: %d\n", no->dado.ALBUM.anoLancamento);
            printf("Numero de musicas: %d\n", no->dado.ALBUM.numeroMusicas);
        }
        imprimirArvRubroNegra(no->dir);
    }
}

void imprimirMusicas(Musica *lista){
    if(lista){
        Musica *aux = lista;
        printf("\n=== Musicas ===\n");
        while(aux){
            printf("Titulo: %s, Duracao: %d minutos\n", aux->titulo, aux->minutos);
            aux = aux->prox;
        }
    }else printf("Nenhuma musica cadastrada.\n");
}

void mostrarArtistasPorEstilo(Arvore *raiz, char *estiloMusical){
    if(raiz){
        mostrarArtistasPorEstilo(raiz->esq, estiloMusical);
        if(raiz->tipo == ARTISTA && strcmp(raiz->dado.ARTISTA.estiloMusical, estiloMusical) == 0)
            printf("Nome: %s\n", raiz->dado.ARTISTA.nome);
        mostrarArtistasPorEstilo(raiz->dir, estiloMusical);
    }
}

void mostrarAlbunsDeArtista(Arvore *artistas, char *nomeArtista){
    Arvore *artista = buscarArvRubroNegra(artistas, nomeArtista);
    if(artista){
        printf("\n=== Albuns de %s ===\n", artista->dado.ARTISTA.nome);
        imprimirArvRubroNegra(artista->dado.ARTISTA.albuns);
    }else printf("Artista nao encontrado.\n");
}

void mostrarMusicasDeAlbum(Arvore *album){
    if(album){
        printf("\n=== Musicas do Album %s ===\n", album->dado.ALBUM.titulo);
        imprimirMusicas(album->dado.ALBUM.musicas);
    }else printf("Album nao encontrado.\n");
}

//travessia in-order dos álbuns 
void imprimirAlbunsRec(Arvore *n) {
    if(n){
        imprimirAlbunsRec(n->esq);
        if(n->tipo == ALBUM) {
            printf("\n--- Album: %s ---\n", n->dado.ALBUM.titulo);
            imprimirMusicas(n->dado.ALBUM.musicas);
        }
        imprimirAlbunsRec(n->dir);
    }
}

void mostrarMusicasDeArtista(Arvore *artistas, char *nomeArtista){
    Arvore *artista = buscarArvRubroNegra(artistas, nomeArtista);
    if(artista){
        printf("\n=== Musicas de %s ===\n", artista->dado.ARTISTA.nome);
        Arvore *aux = artista->dado.ARTISTA.albuns;
        if(aux == NULL)
            printf("Nenhum album cadastrado para este artista.\n");
        else imprimirAlbunsRec(aux);
    }else printf("Artista nao encontrado.\n");
}

void liberarMusicas(Musica *lista){
    Musica *aux;
    while(lista){
        aux = lista;
        lista = lista->prox;
        free(aux);
    }
}

void liberarArvore(Arvore *raiz){
    if(raiz){
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);

        if(raiz->tipo == ARTISTA) liberarArvore(raiz->dado.ARTISTA.albuns);

        if(raiz->tipo == ALBUM) liberarMusicas(raiz->dado.ALBUM.musicas);

        free(raiz);
    }
}
