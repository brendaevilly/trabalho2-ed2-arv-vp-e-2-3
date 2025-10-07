#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototipos.h"

Arvore *inicializar(){
    return NULL;
}

Musica *inicializarM(){
    return NULL;
}

void deixarMaiusculo(char *str){
    for(int i=0; str[i]!='\0'; i++) str[i] = toupper((unsigned char) str[i]);
}

Arvore *alocar(TipoDado tipo){
    Arvore *no = (Arvore *)malloc(sizeof(Arvore));
    if(no){
        no->tipo = tipo;
        no->esq = no->cen = no->dir = NULL; 
    }
    return (no);
}

Musica *alocarM(){
    Musica *no = (Musica *)malloc(sizeof(Musica));
    if(no) no->prox = NULL;
    return (no);
}

int ehFolha(Arvore *R){
    int i = 0;
    if(!R->esq) i = 1;
    return (i);
}

void preencheInfo(TipoDado tipo, DadoUnion *info){
    if(tipo == ARTISTA){
        printf("Nome: ");
        scanf(" %[^\n]", info->ARTISTA.nome);
        printf("Estilo musical: ");
        scanf(" %[^\n]", &info->ARTISTA.estiloMusical);
        int op = 0;
        while (op < 1 || op > 4) {
            printf("Tipo (1-Cantor, 2-Dupla, 3-Banda, 4-Grupo): ");
            scanf("%d", &op); printf("\n");
        }
        no->dado.ARTISTA.TipArt = (TipoArtista) op;

        no->dado.ARTISTA.numeroAlbuns = 0;
        no->dado.ARTISTA.albuns = inicializar(); 
    }else if(tipo == ALBUM){
        printf("Título: ");
        scanf(" %[^\n]", info->ALBUM.nome);
        printf("Ano de lançamento: ");
        scanf("%d", &info->ALBUM.anoLancamento);
        while(info->ALBUM.anoLancamento > 2025){
            printf("[ERRO] Insira um ano válido.\n");
            printf("Ano de lançamento: ");
            scanf("%d", &info->ALBUM.anoLancamento);
        }
        no->dado.ALBUM.numeroMusicas = 0;
        no->dado.ALBUM.musicas = inicializarM();
    }else printf("[ERRO] Tipo inválido.\n");
}

Arvore *criaNo(DadoUnion info, Arvore *Fesq, Arvore *Fcen){
    Arvore *no;
    no = (Arvore *)malloc(sizeof(Arvore));
    no->info1 = info;
    no->esq = Fesq;
    no->cen = Fcen;
    no->dir = NULL;
    no->nInfos = 1;

    return (no);
}

void adicionaInfo(Arvore **no, DadoUnion info, Arvore *filho){
    if(strcmp(info->nome, (*no)->info1.ARTISTA.nome) > 0){
        (*no)->info2 = info;
        (*no)->dir = filho;
    }else{
        (*no)->info2 = (*no)->info1;
        (*no)->dir = (*no)->cen;
        (*no)->info1 = info;
        (*no)->cen = filho;
    }
    (*no)->nInfos = 2;
}

Arvore *quebrarNo(Arvore **no, DadoUnion info, Arvore *filho, DadoUnion *sobe){
    Arvore *maior;
    if(strcmp(info->nome, (*no)->info2.ARTISTA.nome) > 0){
        *sobe = (*no)->info2;
        (*no)->nInfos = 1;
        maior = criaNo(info, (*no)->dir, filho);
        (*no)->dir = NULL;
    }else if(strcmp(info->nome, (*no)->info1.ARTISTA.nome) > 0){
        *sobe = info;
        maior = criaNo((*no)->info2, filho, (*no)->dir);
        (*no)->dir = NULL;
        (*no)->nInfos = 1;
    }else{
        *sobe = (*no)->info1;
        (*no)->info1 = info;
        (*no)->nInfos = 1;
        maior = criaNo((*no)->info2, (*no)->cen, (*no)->dir);
        (*no)->cen = filho;
        (*no)->dir = NULL;
    }

    return (maior);
}

Arvore *inserirNo(Arvore **R, DadoUnion info, Arvore *Pai, DadoUnion *sobe){
    Arvore *maior = inicializar();

    if(!(*R)) 
        *R = criaNo(info, NULL, NULL);
    else{
        if(ehFolha(*R)){
            if((*R)->info == 1) adicionaInfo(R, info, NULL);
            else{
                maior = quebrarNo(R, info, NULL, sobe);
                if(!Pai){
                    *R = criaNo(sobe, *R, maior);
                    maior = NULL;
                }
            }
        }else{
            if(strcmp(info.ALBUM.nome, (*R)->info1.ALBUM.nome) < 0)
                maior = inserirNo(&((*R)->esq), info, *R, sobe);
            else if((*R)->nInfos == 1 || strcmp(info.ALBUM.nome, (*R)->info2.ALBUM.nome) < 0)
                maior = inserirNo(&((*R)->cen), info, *R, sobe);
            else maior = inserirNo(&((*R)->dir), info, *R, sobe);
        }

        if(maior){
            if((*R)->nInfos == 1){
                adicionaInfo(R, *sobe, maior);
                maior = NULL;
            }else{
                maior = quebrarNo(R, *sobe, maior, sobe);
                if(!Pai){
                    *R = criaNo(*sobe, *R, maior);
                    maior = NULL;
                }
            }
        }
    }

    return (maior);
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

int buscarNaArvore23(Arvore *raiz, char *nome, Arvore *busca){
    int nInfoBusca = 0;
    if(raiz){
        int compInfo1 = 2, compInfo2 = 2;

        compInfo1 = strcmp(nome, raiz->info1.ARTISTA.nome);
        if(raiz->nInfos == 2)
            compInfo2 = strcmp(nome, raiz->info2.ARTISTA.nome);

        if(compInfo1 == 0){
            busca = raiz;
            nInfoBusca = 1;
        }else if(compInfo2 != 2 && compInfo2 == 0){
                busca = raiz;
                nInfoBusca = 2;
        }else{
            if(compInfo1 < 0)
                nInfoBusca = buscarNaArvore23(raiz->esq, nome, busca);
            else if(compInfo2 != 2){
                if(compInfo2 < 0)
                    nInfoBusca = buscarNaArvore23(raiz->cen, nome, busca);
                else nInfoBusca = buscarNaArvore23(raiz->dir, nome, busca);
            }
        }
    }

    return nInfoBusca;
}

Musica *buscarMusica(Musica *lista, char *titulo){
    Musica *aux = lista, *busca = NULL;
    while(aux){
        if(strcmp(titulo, aux->titulo) == 0) busca = aux;
        aux = aux->prox;
    }
    return busca;
}

void mostrarCaminhoBusca(Arvore *raiz, char *nome, int *comparacoes){
    if(raiz){
        int compInfo1 = 2, compInfo2 = 2;

        compInfo1 = strcmp(nome, raiz->info1.ARTISTA.nome);
        printf(" -> [%s]", raiz->info1.ALBUM.nome);
        (*comparacoes)++;

        if(raiz->nInfos == 2)
            compInfo2 = strcmp(nome, raiz->info2.ARTISTA.nome);
            printf(" -> [%s]", raiz->info2.ALBUM.nome);
            (*comparacoes)++;

        if(compInfo1 == 0){
            printf("  [ENCONTRADO]\n");
        }else if(compInfo2 != 2 && compInfo2 == 0){
            printf("  [ENCONTRADO]\n");
        }else{
            if(compInfo1 < 0)
                buscarNaArvore23(raiz->esq, nome, comparacoes);
            else if(compInfo2 != 2){
                if(compInfo2 < 0)
                    buscarNaArvore23(raiz->cen, nome, comparacoes);
                else buscarNaArvore23(raiz->dir, nome, comparacoes);
            }
        }
    }
}