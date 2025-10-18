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
                nInfoBusca = buscarNaArvore23((*R)->esq, nome, busca, R);
            else if((*R)->nInfos == 1 || (compInfo2 != 2 && compInfo2 < 0))
                nInfoBusca = buscarNaArvore23((*R)->cen, nome, busca, R);
            else nInfoBusca = buscarNaArvore23((*R)->dir, nome, busca, R);
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
                nInfoBusca = buscarNaArvore23((*R)->esq, nome, busca, R);
            else if((*R)->nInfos == 1 || (compInfo2 != 2 && compInfo2 < 0))
                nInfoBusca = buscarNaArvore23((*R)->cen, nome, busca, R);
            else nInfoBusca = buscarNaArvore23((*R)->dir, nome, busca, R);
        }
    }
}

Arvore *buscarAlbumDeArtista(DadoUnion *artista, char *tituloAlbum, int *nInfoBuscada) {
    Arvore *raizAlbuns = artista->ARTISTA.albuns;
    if(raizAlbuns) {
        Arvore *busca = inicializar();
        *nInfoBuscada = buscarNaArvore23(raizAlbuns, tituloAlbum, busca);
    } 
    return busca;
}

Musica *buscarMusicaDeAlbum(DadoUnion *album, char *tituloAlbum, char *tituloMusica){
    Musica *musica = NULL;
    if(album) 
        musica = buscarMusica(album->ALBUM.musicas, tituloMusica);
    return musica;
}

void buscarMusicaEmAlbuns(Arvore *albuns, Musica *busca, char *tituloMusica) {
    if(albuns && !busca){
        busca = buscarMusicaDeAlbum(albuns, albuns->info1.ALBUM.nome, tituloMusica);
        if(albuns->nInfos == 2 && !busca)
            busca = buscarMusicaDeAlbum(albuns, albuns->info2.ALBUM.nome, tituloMusica);
        buscarMusicaEmAlbuns(albuns->esq, busca, tituloMusica);
        buscarMusicaEmAlbuns(albuns->cen, busca, tituloMusica);
        if(albuns->nInfos == 2)
            buscarMusicaEmAlbuns(albuns->dir, busca, tituloMusica);
    }
}

Musica *buscarMusicaDeArtista(Arvore *artistas, char *nomeArtista, char *tituloMusica) {
    Musica *musica = inicializarM();
    Arvore *artista = inicializar();
    int nInfoBuscada = buscarNaArvore23(artistas, nomeArtista, artista);
    if (artista){
        if(nInfoBuscada == 1)
            musica = buscarMusicaEmAlbuns(artista->info1.ARTISTA.albuns, tituloMusica);
        else musica = buscarMusicaEmAlbuns(artista->info2.ARTISTA.albuns, tituloMusica);
    }
        
    return (musica);
}

void mostrarArtistasPorEstilo(Arvore *artistas, char *estilo) {
    if (artistas){
        int compInfo1 = 2, compInfo2 = 2;

        compInfo1 = strcmp(estilo, artistas->info1.ARTISTA.estiloMusical);
        if(raiz->nInfos == 2)
            compInfo2 = strcmp(estilo, artistas->info2.ARTISTA.estiloMusical);

        if(compInfo1 == 0){
            printf("%s (%s)\n", artistas->info1.ARTISTA.nome, artistas->info1.ARTISTA.estiloMusical);
        }else if(compInfo2 != 2 && compInfo2 == 0){
            printf("%s (%s)\n", artistas->info2.ARTISTA.nome, artistas->info2.ARTISTA.estiloMusical);
        }else{
            if(compInfo1 < 0)
                nInfoBusca = buscarNaArvore23((*R)->esq, nome, busca, R);
            else if((*R)->nInfos == 1 || (compInfo2 != 2 && compInfo2 < 0))
                nInfoBusca = buscarNaArvore23((*R)->cen, nome, busca, R);
            else nInfoBusca = buscarNaArvore23((*R)->dir, nome, busca, R);
        }
    }
}

void mostraArtista(Arvore *raiz){
    printf("\n=== Artista ===\n");
    printf("Nome: %s\n", raiz->info1.ARTISTA.nome);
    printf("Estilo musical: %s\n", raiz->info1.ARTISTA.estiloMusical);
    printf("Tipo: ");
    switch(raiz->info1.ARTISTA.tipo) {
        case CANTOR: printf("Cantor\n"); break;
        case DUPLA: printf("Dupla\n"); break;
        case BANDA: printf("Banda\n"); break;
        case GRUPO: printf("Grupo\n"); break;
        default: printf("Desconhecido\n"); break;
    }
    printf("Número de álbuns: %d\n", raiz->info1.ARTISTA.numeroAlbuns);

    if(raiz->nInfos == 2){
        printf("\n=== Artista ===\n");
        printf("Nome: %s\n", raiz->info1.ARTISTA.nome);
        printf("Estilo musical: %s\n", raiz->info1.ARTISTA.estiloMusical);
        printf("Tipo: ");
        switch(raiz->info1.ARTISTA.tipo) {
            case CANTOR: printf("Cantor\n"); break;
            case DUPLA: printf("Dupla\n"); break;
            case BANDA: printf("Banda\n"); break;
            case GRUPO: printf("Grupo\n"); break;
            default: printf("Desconhecido\n"); break;
        }
        printf("Número de álbuns: %d\n", raiz->info1.ARTISTA.numeroAlbuns);
    }
}

void mostraAlbum(Arvore *raiz){
    printf("\n=== Álbum ===\n");
    printf("Título: %s\n", raiz->info1.ALBUM.nome);
    printf("Ano de lançamento: %d\n", raiz->info1.ALBUM.anoLancamento);
    printf("Número de músicas: %d\n", raiz->info1.ALBUM.numeroMusicas);
    if(raiz->nInfos == 2){
        printf("\n=== Álbum ===\n");
        printf("Título: %s\n", raiz->info2.ALBUM.nome);
        printf("Ano de lançamento: %d\n", raiz->info2.ALBUM.anoLancamento);
        printf("Número de músicas: %d\n", raiz->info2.ALBUM.numeroMusicas);
    }
}

void imprimirArvore23(Arvore *raiz){
    if(raiz){
        imprimirArvore23(raiz->esq);
        imprimirArvore23(raiz->cen);

        if(raiz->tipo == ARTISTA)
            mostraArtista(raiz);
        else if(raiz->tipo == ALBUM)
            mostraAlbum(raiz);

        if(raiz->nInfos == 2)
            imprimirArvore23(raiz->dir);
    }
}

void imprimirMusicas(Musica *lista){
    if(lista){
        Musica *aux = lista;
        printf("\n=== Músicas ===\n");
        while(aux){
            printf("Título: %s, Duração: %d minutos\n", aux->titulo, aux->minutos);
            aux = aux->prox;
        }
    }else printf("Nenhuma música cadastrada.\n");
}

void mostrarAlbunsDeArtista(Arvore *artistas, char *nomeArtista){
    Arvore *artista;
    int nInfoBuscada = buscarNaArvore23(artistas, nome, artista);
    if(artista){
        if(nInfoBuscada == 1){
            printf("\n=== Álbuns de %s ===\n", artista->info1.ARTISTA.nome);
            imprimirArvore23(artista->info1.ARTISTA.albuns);
        }else{
            printf("\n=== Álbuns de %s ===\n", artista->info2.ARTISTA.nome);
            imprimirArvore23(artista->info2.ARTISTA.albuns);
        }
        
    }else printf("Artista não encontrado.\n");
}

void mostrarMusicasDeAlbum(DadoUnion *album){
    if(album){
        printf("\n=== Músicas do Álbum %s ===\n", album->ALBUM.nome);
        imprimirMusicas(album->ALBUM.musicas);
    }
}


void imprimirAlbunsRec(Arvore *n) {
    if(n){
        imprimirAlbunsRec(n->esq);
        imprimirAlbunsRec(n->cen)

        printf("\n--- Álbum: %s ---\n", n->info1.ALBUM.nome);
        imprimirMusicas(n->info1.ALBUM.musicas);

        if(n->nInfos == 2){
            imprimirMusicas(n->info2.ALBUM.musicas);
            imprimirAlbunsRec(n->dir);
        }
    }
}

void mostrarMusicasDeArtista(Arvore *artistas, char *nomeArtista){
    Arvore *artista;
    int nInfoBuscada = buscarNaArvore23(artistas, nome, artista);
    if(artista){
        Arvore *aux = inicializar();
        if(nInfoBusca == 1)
            aux = artista->info1.ARTISTA.albuns;
        else aux = artista->info2.ARTISTA.albuns;

        if(!aux) printf("Nenhum álbum cadastrado para este artista.\n");
        else imprimirAlbunsRec(aux);
    }else printf("Artista não encontrado.\n");
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
        liberarArvore(raiz->cen);
        free(raiz);
    }
}

// ================= REMOVER ======================

void maiorFilhoEsq(Arvore **R, Arvore **maiorFilho, Arvore **paiM, int pos){
    if(*maiorFilho){
        if((*maiorFilho)->esq == NULL){ // significa que é uma folha
            DadoUnion aux;
            if(pos == 1){ // a posição da info em relação ao nó, ela é uma info 1
                aux = (*maiorFilho)->info1;
                // remover a info do maior filho
                if((*maiorFilho)->nInfos == 2){
                    (*R)->info1 = (*maiorFilho)->info2;
                    (*maiorFilho)->info2 = aux;
                }else{
                    (*R)->info1 = (*maiorFilho)->info1;
                    (*maiorFilho)->info1 = aux;
                }
            }else if(pos == 2){ // a posição da info em relação ao nó, ela é uma info 2
            }
        }else{
            if(pos == 1){
                maiorFilhoEsq(raiz, &((*maiorFilho)->dir), paiM, pos);
            }else if(pos == 2){
                maiorFilhoEsq(raiz, &((*maiorFilho)->cen), paiM, pos);
            }
        }
    }
}

void removerArvore23(Arvore **R, Arvore **pai, DadoUnion *info){
    int compInfo1 = 2, compInfo2 = 2;
    if(*R){
        //se for remover a info1
        if(strcmp(info->ALBUM.nome, (*R)->info1.ALBUM.nome) == 0){
            //casos folha
            if(ehFolha(*R)){
                if((*R)->nInfos == 2){
                    (*R)->info1 = (*R)->info2;
                    (*R)->nInfos = 1;
                }else (*R)->nInfos = 0;
            //casos não-folha
            }else{
                Arvore **maiorFilho = &((*R)->esq);
                Arvore **paiM = R;
                maiorFilhoEsq(R, maiorFilho, paiM, 1); // Tem que fazer ainda
            }
        // se for remover a info2
        }else if((*R)->nInfos == 2 && (strcmp(info->ALBUM.nome, (*R)->info2.ALBUM.nome)) == 0){
            if(ehFolha(*R)) (*R)->nInfos = 1;
            else{
                Arvore **maiorFilho = &((*R)->cen);
                Arvore **paiM = R;
                maiorFilhoEsq(R, paiM, maiorFilho, 2);
                //sempre resolve com os filhos
                //filho dir
                //filho cen
                //juntar
            }
        }else{
            //recursão
            compInfo1 = strcmp(nome, (*R)->info1.ARTISTA.nome);
            if((*R)->nInfos == 2)
                compInfo2 = strcmp(nome, (*R)->info2.ARTISTA.nome);

            if(compInfo1 < 0)
                nInfoBusca = buscarNaArvore23((*R)->esq, nome, busca, R);
            else if((*R)->nInfos == 1 || (compInfo2 != 2 && compInfo2 < 0))
                nInfoBusca = buscarNaArvore23((*R)->cen, nome, busca, R);
            else nInfoBusca = buscarNaArvore23((*R)->dir, nome, busca, R);
        }    
    }
    //código de resolver pai/avô
    balanceia(R, pai);
}