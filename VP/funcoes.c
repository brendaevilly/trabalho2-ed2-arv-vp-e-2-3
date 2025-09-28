#include <stdio.h>
#include <stdlib.h>
#include "prototipos.h"


// Inicialização, Alocação e Utilitários
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

void deixaMaiuscula(char *str){
    for(int i=0; str[i]!='\0'; i++) str[i] = toupper((unsigned char) str[i]);
}

struct tm *tempoAtual(){
    time_t agora;
    time(&agora);

    struct tm *infoTempoLocal = malloc(sizeof(struct tm));  // aloca memória própria
    if(!infoTempoLocal) return NULL;

    struct tm *tmp = localtime(&agora);
    if(tmp) *infoTempoLocal = *tmp;  // copia o conteúdo
    else {
        free(infoTempoLocal);
        return NULL;
    }

    return infoTempoLocal;
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
    if(raiz != NULL || *raiz != NULL){
        if((*raiz)->esq && (*raiz)->dir &&
        (*raiz)->esq->cor == VERMELHO && (*raiz)->dir->cor == VERMELHO){
            (*raiz)->cor = VERMELHO;
            (*raiz)->esq->cor = PRETO;
            (*raiz)->dir->cor = PRETO;
        }
    }
}


void balanceamento(Arvore **raiz){
    if(raiz != NULL || *raiz != NULL){

        if((*raiz)->dir && (*raiz)->dir->cor == VERMELHO &&
        (!(*raiz)->esq || (*raiz)->esq->cor == PRETO)) {
            rotacionarEsquerda(raiz);
        }
        if(((*raiz)->esq->cor == VERMELHO) && ((*raiz)->esq->esq->cor == VERMELHO)){
            rotacionarDireita(raiz);
        }
        if(((*raiz)->esq->cor == VERMELHO) && ((*raiz)->dir->cor == VERMELHO)){
            trocaCor(raiz);
        }
    }
}

int inserirArvRubroNegra(Arvore **raiz, Arvore *novoNo){
    int inseriu = 0;
    if (*raiz == NULL){
        *raiz = novoNo;
        inseriu = 1;
    }
    else {
        if(strcmp(novoNo->dado.ARTISTA.nome, (*raiz)->dado.ARTISTA.nome) < 0) {
            inseriu = inserirArvRubroNegra(&(*raiz)->esq, novoNo);
        } else {
            inseriu = inserirArvRubroNegra(&(*raiz)->dir, novoNo);
        }
    } 

    balanceamento(raiz);
    return (inseriu);
}

int inserirMusic(Musica **lista, Musica novaMusica){
    Musica *novoNo = alocarMusic();
    if(!novoNo) return 0;
    *novoNo = novaMusica;

    if(*lista == NULL){
        *lista = novoNo;
    } else {
        Musica *aux = *lista;
        while(aux->prox != NULL){
            aux = aux->prox;
        }
        aux->prox = novoNo;
    }
    return 1;
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

Album *buscarAlbumDeArtista(Arvore *raiz, char *nomeArtista, char *tituloAlbum){
    Arvore *artista = buscarArvRubroNegra(raiz, nomeArtista);
    if(artista != NULL && artista->tipo == ARTISTA){
        Arvore *album = buscarArvRubroNegra(artista->dado.ARTISTA.albuns, tituloAlbum);
        if(album != NULL && album->tipo == ALBUM){
            printf("Álbum '%s' encontrado para o artista '%s'.\n", tituloAlbum, nomeArtista);
            return &album->dado.ALBUM;
        } else {
            printf("Álbum '%s' não encontrado para o artista '%s'.\n", tituloAlbum, nomeArtista);
        }
    } else {
        printf("Artista '%s' não encontrado.\n", nomeArtista);
    }
}





// Remoção int removerArvRubroNegra(Arvore **raiz, char *nome); // aqui vem a remover da rubro negra
int removerMusic(Musica **lista, char titulo[]){
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
            switch(no->dado.ARTISTA.tipo) {
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

void imprimirMusics(Musica *lista){
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