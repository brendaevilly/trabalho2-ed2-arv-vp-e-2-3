#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

        if(tipo == ARTISTA){
            no->info1.ARTISTA.numeroAlbuns = 0;
            no->info1.ARTISTA.albuns = inicializar();

            no->info2.ARTISTA.numeroAlbuns = 0;
            no->info2.ARTISTA.albuns = inicializar();
        } else if(tipo == ALBUM){
            no->info1.ALBUM.numeroMusicas = 0;
            no->info1.ALBUM.musicas = inicializarM();

            no->info2.ALBUM.numeroMusicas = 0;
            no->info2.ALBUM.musicas = inicializarM();
        }
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
        deixarMaiusculo(info->ARTISTA.nome);
        printf("Estilo musical: ");
        scanf(" %[^\n]", info->ARTISTA.estiloMusical);
        deixarMaiusculo(info->ARTISTA.estiloMusical);
        int op = 0;
        while (op < 1 || op > 4) {
            printf("Tipo (1-Cantor, 2-Dupla, 3-Banda, 4-Grupo): ");
            scanf("%d", &op); printf("\n");
        }
        info->ARTISTA.tipo = (TipoArtista) op;

        info->ARTISTA.numeroAlbuns = 0;
        info->ARTISTA.albuns = inicializar();
    }else if(tipo == ALBUM){
        printf("Título: ");
        scanf(" %[^\n]", info->ALBUM.nome);
        deixarMaiusculo(info->ALBUM.nome);
        printf("Ano de lançamento: ");
        scanf("%d", &info->ALBUM.anoLancamento);
        deixarMaiusculo(info->ALBUM.nome);
        while(info->ALBUM.anoLancamento > 2025){
            printf("[ERRO] Insira um ano válido.\n");
            printf("Ano de lançamento: ");
            scanf("%d", &info->ALBUM.anoLancamento);
        }
        info->ALBUM.numeroMusicas = 0;
        info->ALBUM.musicas = inicializarM();
    }else printf("[ERRO] Tipo inválido.\n");
}

Arvore *criaNo(DadoUnion info, Arvore *Fesq, Arvore *Fcen, TipoDado tipo){
    Arvore *no;
    no = (Arvore *)malloc(sizeof(Arvore));
    no->info1 = info;
    no->tipo = tipo;
    no->esq = Fesq;
    no->cen = Fcen;
    no->dir = NULL;
    no->nInfos = 1;

    return (no);
}

void adicionaInfo(Arvore **no, DadoUnion info, Arvore *filho, TipoDado tipo) {
    if (tipo == ARTISTA) {
        if (strcmp(info.ARTISTA.nome, (*no)->info1.ARTISTA.nome) > 0) {
            (*no)->info2 = info;
            (*no)->dir = filho;
        } else {
            (*no)->info2 = (*no)->info1;
            (*no)->dir = (*no)->cen;
            (*no)->info1 = info;
            (*no)->cen = filho;
        }
    } else {  // ALBUM
        if (strcmp(info.ALBUM.nome, (*no)->info1.ALBUM.nome) > 0) {
            (*no)->info2 = info;
            (*no)->dir = filho;
        } else {
            (*no)->info2 = (*no)->info1;
            (*no)->dir = (*no)->cen;
            (*no)->info1 = info;
            (*no)->cen = filho;
        }
    }
    (*no)->nInfos = 2;
}

Arvore *quebrarNo(Arvore **no, DadoUnion info, Arvore *filho, DadoUnion *sobe, TipoDado tipo) {
    Arvore *maior = NULL;
    int cmp1, cmp2;
    if (tipo == ARTISTA) {
        cmp1 = strcmp(info.ARTISTA.nome, (*no)->info1.ARTISTA.nome);
        cmp2 = strcmp(info.ARTISTA.nome, (*no)->info2.ARTISTA.nome);
    } else {  // ALBUM
        cmp1 = strcmp(info.ALBUM.nome, (*no)->info1.ALBUM.nome);
        cmp2 = strcmp(info.ALBUM.nome, (*no)->info2.ALBUM.nome);
    }
    if (cmp2 > 0) {
        *sobe = (*no)->info2;
        maior = criaNo(info, (*no)->dir, filho, tipo);
    } else if (cmp1 > 0) {
        *sobe = info;
        maior = criaNo((*no)->info2, filho, (*no)->dir, tipo);
    } else {
        *sobe = (*no)->info1;
        maior = criaNo((*no)->info2, (*no)->cen, (*no)->dir, tipo);
        (*no)->info1 = info;
        (*no)->cen = filho;
    }
    (*no)->dir = NULL;
    (*no)->nInfos = 1;
    return (maior);
}

Arvore *inserirNo(Arvore **R, DadoUnion info, Arvore *Pai, DadoUnion *sobe, int *inserido, TipoDado tipo) {
    Arvore *maior = NULL;
    int duplicado = 0;

    if (!(*R)) {
        *R = criaNo(info, NULL, NULL, tipo);
        *inserido = 1;
    } else {
        // Define ponteiros para os nomes baseados no tipo
        char *nome_info, *nome1, *nome2;
        if (tipo == ARTISTA) {
            nome_info = info.ARTISTA.nome;
            nome1 = (*R)->info1.ARTISTA.nome;
            nome2 = (*R)->info2.ARTISTA.nome;
        } else {  // ALBUM
            nome_info = info.ALBUM.nome;
            nome1 = (*R)->info1.ALBUM.nome;
            nome2 = (*R)->info2.ALBUM.nome;
        }

        if (ehFolha(*R)) {
            if ((*R)->nInfos == 1) {
                if (strcmp(nome_info, nome1) != 0) {
                    adicionaInfo(R, info, NULL, tipo);
                    *inserido = 1;
                }
            } else {
                if (strcmp(nome_info, nome1) != 0 && strcmp(nome_info, nome2) != 0) {
                    maior = quebrarNo(R, info, NULL, sobe, tipo);
                    *inserido = 1;
                    if (!Pai) {
                        *R = criaNo(*sobe, *R, maior, tipo);
                        maior = NULL;
                    }
                }
            }
        } else {

            if (strcmp(nome_info, nome1) == 0 || ((*R)->nInfos == 2 && strcmp(nome_info, nome2) == 0)) {
                duplicado = 1;
                *inserido = 0;
            } else {
                if (strcmp(nome_info, nome1) < 0)
                    maior = inserirNo(&((*R)->esq), info, *R, sobe, inserido, tipo);
                else if ((*R)->nInfos == 1 || strcmp(nome_info, nome2) < 0)
                    maior = inserirNo(&((*R)->cen), info, *R, sobe, inserido, tipo);
                else
                    maior = inserirNo(&((*R)->dir), info, *R, sobe, inserido, tipo);
            }

            if (maior) {
                if ((*R)->nInfos == 1) {
                    int cmp = strcmp(nome_info, nome1);
                    if (cmp != 0) {
                        adicionaInfo(R, *sobe, maior, tipo);
                    }
                    maior = NULL;
                } else {
                    int cmp1 = strcmp(nome_info, nome1);
                    int cmp2 = strcmp(nome_info, nome2);
                    if (cmp1 != 0 && cmp2 != 0) {
                        maior = quebrarNo(R, *sobe, maior, sobe, tipo);
                        if (!Pai) {
                            *R = criaNo(*sobe, *R, maior, tipo);
                            maior = NULL;
                        }
                    }
                }
            }
        }
    }

    return maior;
}


void imprimirEstruturaArvore23(Arvore *raiz, int nivel, char *posicao) {
    if (raiz) {
        // Indentação para o nível
        for (int i = 0; i < nivel; i++) {
            printf("  ");
        }
        // Imprime o nível, posição, tipo e nInfos
        printf("Nivel %d - Posicao: %s - Tipo: %s, nInfos: %d\n", nivel, posicao, (raiz->tipo == ARTISTA ? "ARTISTA" : "ALBUM"), raiz->nInfos);

        // Indentação extra para as infos
        for (int i = 0; i < nivel + 1; i++) {
            printf("  ");
        }

        // Imprime info1
        if (raiz->tipo == ARTISTA) {
            printf("Info1: %s (Estilo: %s)\n", raiz->info1.ARTISTA.nome, raiz->info1.ARTISTA.estiloMusical);
        } else {
            printf("Info1: %s (Ano: %d)\n", raiz->info1.ALBUM.nome, raiz->info1.ALBUM.anoLancamento);
        }

        // Imprime info2 se existir
        if (raiz->nInfos == 2) {
            for (int i = 0; i < nivel + 1; i++) {
                printf("  ");
            }
            if (raiz->tipo == ARTISTA) {
                printf("Info2: %s (Estilo: %s)\n", raiz->info2.ARTISTA.nome, raiz->info2.ARTISTA.estiloMusical);
            } else {
                printf("Info2: %s (Ano: %d)\n", raiz->info2.ALBUM.nome, raiz->info2.ALBUM.anoLancamento);
            }
        }

        // Recursão para filhos com posições
        imprimirEstruturaArvore23(raiz->esq, nivel + 1, "ESQUERDA");
        imprimirEstruturaArvore23(raiz->cen, nivel + 1, "CENTRO");
        if (raiz->nInfos == 2) {
            imprimirEstruturaArvore23(raiz->dir, nivel + 1, "DIREITA");
        }
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


int buscarNaArvore23(Arvore *raiz, char *nome, Arvore **busca){
    int nInfoBusca = 0;
    if(raiz){
        int compInfo1, compInfo2;
       
        if (raiz->tipo == ARTISTA) {
            compInfo1 = strcmp(nome, raiz->info1.ARTISTA.nome);
            if (raiz->nInfos == 2)
                compInfo2 = strcmp(nome, raiz->info2.ARTISTA.nome);
        } else{
            compInfo1 = strcmp(nome, raiz->info1.ALBUM.nome);
            if (raiz->nInfos == 2)
                compInfo2 = strcmp(nome, raiz->info2.ALBUM.nome);
        }
            
        if(compInfo1 == 0){
            *busca = raiz;
            nInfoBusca = 1;
        }else if(compInfo2 == 0){
                *busca = raiz;
                nInfoBusca = 2;
        }else{
            if(compInfo1 < 0){
                nInfoBusca = buscarNaArvore23(raiz->esq, nome, busca);
            }
            else{
                if(raiz->nInfos == 1){
                    nInfoBusca = buscarNaArvore23(raiz->cen, nome, busca);
                }                    
                else if(compInfo2 < 0)
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
        printf(" -> [%s]", raiz->info1.ARTISTA.nome);
        (*comparacoes)++;

        if(raiz->nInfos == 2){
            compInfo2 = strcmp(nome, raiz->info2.ARTISTA.nome);
            printf(" -> [%s]", raiz->info2.ALBUM.nome);
            (*comparacoes)++;
        }
            
        if(compInfo1 == 0){
            printf("  [ENCONTRADO]\n");
        }else if(compInfo2 == 0){
            printf("  [ENCONTRADO]\n");
        }else{
            if(compInfo1 < 0)
                mostrarCaminhoBusca(raiz->esq, nome, comparacoes);
            else if(raiz->nInfos == 1 || (compInfo2 != 2 && compInfo2 < 0))
                mostrarCaminhoBusca(raiz->cen, nome, comparacoes);
            else mostrarCaminhoBusca(raiz->dir, nome, comparacoes);
        }
    }else printf("[NAO ENCONTRADO]\n");
}

Arvore *buscarAlbumDeArtista(DadoUnion *artista, char *tituloAlbum, int *nInfoBuscada) {
    Arvore *raizAlbuns = artista->ARTISTA.albuns;
    Arvore *busca = inicializar();
    if(raizAlbuns) {
        *nInfoBuscada = buscarNaArvore23(raizAlbuns, tituloAlbum, &busca);
    } 
    return busca;
}

Musica *buscarMusicaDeAlbum(DadoUnion *album, char *tituloAlbum, char *tituloMusica){
    Musica *musica = NULL;
    if(album) 
        musica = buscarMusica(album->ALBUM.musicas, tituloMusica);
    return musica;
}

void buscarMusicaEmAlbuns(Arvore *albuns, Musica **busca, char *tituloMusica) {
    if(albuns && !(*busca)){
        *busca = buscarMusicaDeAlbum(&(albuns->info1), albuns->info1.ALBUM.nome, tituloMusica);
        if(albuns->nInfos == 2 && !(*busca))
            *busca = buscarMusicaDeAlbum(&(albuns->info2), albuns->info2.ALBUM.nome, tituloMusica);
        buscarMusicaEmAlbuns(albuns->esq, busca, tituloMusica);
        buscarMusicaEmAlbuns(albuns->cen, busca, tituloMusica);
        if(albuns->nInfos == 2)
            buscarMusicaEmAlbuns(albuns->dir, busca, tituloMusica);
    }
}

Musica *buscarMusicaDeArtista(Arvore *artistas, char *nomeArtista, char *tituloMusica) {
    Musica *musica = inicializarM();
    Arvore *artista = inicializar();
    int nInfoBuscada = buscarNaArvore23(artistas, nomeArtista, &artista);
    if (artista){
        if(nInfoBuscada == 1)
            buscarMusicaEmAlbuns(artista->info1.ARTISTA.albuns, &musica, tituloMusica);
        else buscarMusicaEmAlbuns(artista->info2.ARTISTA.albuns, &musica, tituloMusica);
    }
        
    return (musica);
}

void mostrarArtistasPorEstilo(Arvore *artistas, char *estilo) {
    if (artistas){
    if (strcmp(estilo, artistas->info1.ARTISTA.estiloMusical) == 0) 
        printf("%s (%s)\n", artistas->info1.ARTISTA.nome, artistas->info1.ARTISTA.estiloMusical);

    if (artistas->nInfos == 2 && strcmp(estilo, artistas->info2.ARTISTA.estiloMusical) == 0) 
        printf("%s (%s)\n", artistas->info2.ARTISTA.nome, artistas->info2.ARTISTA.estiloMusical);

    mostrarArtistasPorEstilo(artistas->esq, estilo);
    mostrarArtistasPorEstilo(artistas->cen, estilo);
    mostrarArtistasPorEstilo(artistas->dir, estilo);
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
        printf("Nome: %s\n", raiz->info2.ARTISTA.nome);  // Corrigido: era info1
        printf("Estilo musical: %s\n", raiz->info2.ARTISTA.estiloMusical);  // Corrigido
        printf("Tipo: ");
        switch(raiz->info2.ARTISTA.tipo) {  // Corrigido
            case CANTOR: printf("Cantor\n"); break;
            case DUPLA: printf("Dupla\n"); break;
            case BANDA: printf("Banda\n"); break;
            case GRUPO: printf("Grupo\n"); break;
            default: printf("Desconhecido\n"); break;
        }
        printf("Número de álbuns: %d\n", raiz->info2.ARTISTA.numeroAlbuns);  // Corrigido
    }
}

void mostraAlbum(Arvore *raiz){
    printf("\n=== Album ===\n");
    printf("Título: %s\n", raiz->info1.ALBUM.nome);
    printf("Ano de lançamento: %d\n", raiz->info1.ALBUM.anoLancamento);
    printf("Número de músicas: %d\n", raiz->info1.ALBUM.numeroMusicas);
    if(raiz->nInfos == 2){
        printf("\n=== Álbum ===\n");
        printf("Título: %s\n", raiz->info2.ALBUM.nome);  // Corrigido: era info1
        printf("Ano de lançamento: %d\n", raiz->info2.ALBUM.anoLancamento);  // Corrigido
        printf("Número de músicas: %d\n", raiz->info2.ALBUM.numeroMusicas);  // Corrigido
    }
}

void imprimirArvore23(Arvore *raiz){
    if(raiz){
        imprimirArvore23(raiz->esq);
        imprimirArvore23(raiz->cen);
        if(raiz->nInfos == 2)
            imprimirArvore23(raiz->dir);
        if(raiz->tipo == ARTISTA)
            mostraArtista(raiz);
        else if(raiz->tipo == ALBUM)
            mostraAlbum(raiz);
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
    int nInfoBuscada = buscarNaArvore23(artistas, nomeArtista, &artista);
    if(artista){
        if(nInfoBuscada == 1){
            printf("\n=== Albuns de %s ===\n", artista->info1.ARTISTA.nome);
            imprimirArvore23(artista->info1.ARTISTA.albuns);
        }else{
            printf("\n=== Albuns de %s ===\n", artista->info2.ARTISTA.nome);
            imprimirArvore23(artista->info2.ARTISTA.albuns);
        }
        
    }else printf("Artista nao encontrado.\n");
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
        imprimirAlbunsRec(n->cen);

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
    int nInfoBuscada = buscarNaArvore23(artistas, nomeArtista, &artista);
    if(artista){
        Arvore *aux = inicializar();
        if(nInfoBuscada == 1)
            aux = artista->info1.ARTISTA.albuns;
        else aux = artista->info2.ARTISTA.albuns;

        if(!aux) printf("Nenhum album cadastrado para este artista.\n");
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
        // libera filhos primeiro
        liberarArvore(raiz->esq);
        liberarArvore(raiz->cen);
        liberarArvore(raiz->dir);

        // libera albuns e músicas se for artista
        if(raiz->tipo == ARTISTA){
            if(raiz->info1.ARTISTA.numeroAlbuns > 0)
                liberarArvore(raiz->info1.ARTISTA.albuns);
            if(raiz->info2.ARTISTA.numeroAlbuns > 0)
                liberarArvore(raiz->info2.ARTISTA.albuns);
        }
        // libera músicas se for album
        if(raiz->tipo == ALBUM){
            liberarMusicas(raiz->info1.ALBUM.musicas);
            liberarMusicas(raiz->info2.ALBUM.musicas);
        }

        // finalmente libera o próprio nó
        free(raiz);
    }
}

// ================= REMOVER ======================

int ehVazio(Arvore *no){
    int vazio = 0;
    if(no->nInfos == 0) vazio = 1;
    return (vazio);
}

void deQuemSouFilho(Arvore *R, Arvore *pai, char *posicao) {
    if (R == pai->esq) strcpy(posicao, "ESQUERDA");
    else if (R == pai->cen) strcpy(posicao, "CENTRO");
    else if (R == pai->dir) strcpy(posicao, "DIREITA");
}

void onda_CentroParaDireita_CentroDuasInfos(Arvore **R, Arvore **pai){
    // info 2 do pai desce para o nó vazio
    (*R)->info1 = (*pai)->info2;
    (*R)->nInfos = 1;
    // info 2 do meio sobe para a info 2 do pai
    (*pai)->info2 = (*pai)->cen->info2;
    (*pai)->cen->nInfos = 1;
    // ajusta os filhos
    (*R)->esq = (*pai)->cen->dir;
    (*pai)->cen->dir = NULL;
}

void onda_CentroParaDireita_CentroUmaInfo(Arvore **R, Arvore **pai){
    // info 2 do pai desce para o nó vazio e ajusta os filhos
    (*R)->info2 = (*pai)->info2;
    (*R)->dir = (*R)->cen;

    // info 1 do centro do pai vai para a info um do nó vazio e nInfos são atualizados
    (*R)->info1 = (*pai)->cen->info1;
    (*R)->nInfos = 2;
    (*pai)->nInfos = 1;

    // centro e esquerda do pai vão para meio e esquerda do nó vazio
    (*R)->cen = (*pai)->cen->cen;
    (*R)->esq = (*pai)->cen->esq;

    // centro do pai é liberado
    free((*pai)->cen);

    // centro do pai recebe o nó vazio e direita do pai é nula
    (*pai)->cen = *R;
    (*pai)->dir = NULL;
}

void onda_EsquerdaParaCentro_EsquerdaDuasInfos(Arvore **R, Arvore **pai){
    // info 1 do pai desce para o nó vazio
    (*R)->info1 = (*pai)->info1;
    (*R)->nInfos = 1;

    // info 2 da esquerda sobe para a info 1 do pai
    (*pai)->info1 = (*pai)->esq->info2;
    (*pai)->esq->nInfos = 1;
    
    // ajusta os filhos
    (*R)->esq = (*pai)->esq->dir;
    (*pai)->esq->dir = NULL;
}

void onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiDuasInfos(Arvore **R, Arvore **pai){
    // info1 do pai desce para a raiz
    (*R)->info2 = (*pai)->info1;
    // info1 da esquerda passou para a raiz
    (*R)->info1 = (*pai)->esq->info1;
    (*R)->nInfos = 2;

    // ajusta os filhos do nó "vazio"
    (*R)->dir = (*R)->cen;
    (*R)->cen = (*pai)->esq->cen;
    (*R)->esq = (*pai)->esq->esq;

    // ajusta o pai  
    (*pai)->info1 = (*pai)->info2;
    (*pai)->nInfos = 1;

    // ajusta os filhos do pai
    free((*pai)->esq);
    (*pai)->esq = *R;
    (*pai)->cen = (*pai)->dir;
    (*pai)->dir = NULL;
}

void onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiUmaInfo(Arvore **R, Arvore **pai){
    // info2 do nó vazio recebe info1 do pai
    (*R)->info2 = (*pai)->info1;
    // info1 do nó vazio recebe info1 da esquerda do pai
    (*R)->info1 = (*pai)->esq->info1;
    (*R)->nInfos = 2;

    // deixa o pai vazio (resolvido na próxima chamada)
    (*pai)->nInfos = 0;

    // ajusta os filhos do nó vazio
    (*R)->dir = (*R)->cen;
    (*R)->cen = (*pai)->esq->cen;
    (*R)->esq = (*pai)->esq->esq;

    // libera a esquerda do pai
    free((*pai)->esq);
    (*pai)->esq = NULL;
}

void onda_CentroParaEsquerda_CentroDuasInfos(Arvore **R, Arvore **pai){
    // info1 do pai desce para o nó vazio
    (*R)->info1 = (*pai)->info1;
    (*R)->nInfos = 1;

    // info1 do centro sobe para a info1 do pai
    (*pai)->info1 = (*pai)->cen->info1;
    (*pai)->cen->info1 = (*pai)->cen->info2;
    (*pai)->cen->nInfos = 1;

    // ajusta os filhos no nó "vazio"
    (*R)->esq = (*R)->cen;
    (*R)->cen = (*pai)->cen->esq;

    // ajusta os filhos do pai
    (*pai)->cen->esq = (*pai)->cen->cen;
    (*pai)->cen->cen = (*pai)->cen->dir;
    (*pai)->cen->dir = NULL;
}

void onda_CentroParaEsquerda_CentroUmaInfo_PaiDuasInfos(Arvore **R, Arvore **pai){
    // info1 do pai desce para o nó vazio
    (*R)->info1 = (*pai)->info1;
    // info1 do centro do pai vai para o nó vazio
    (*R)->info2 = (*pai)->cen->info1;
    (*R)->nInfos = 2;

    // ajusta os filhos do nó "vazio"
    (*R)->esq = (*R)->cen;
    (*R)->cen = (*pai)->cen->esq;
    (*R)->dir = (*pai)->cen->cen;

    // ajusta o pai
    (*pai)->info1 = (*pai)->info2;
    (*pai)->nInfos = 1;

    // ajusta os filhos do pai
    free((*pai)->cen);
    (*pai)->cen = (*pai)->dir;
    (*pai)->dir = NULL;
}

void onda_CentroParaEsquerda_CentroUmaInfo_PaiUmaInfo(Arvore **R, Arvore **pai){
    // info1 do pai desce para o nó vazio
    (*R)->info1 = (*pai)->info1;
    // info1 do centro do pai vai para o nó vazio
    (*R)->info2 = (*pai)->cen->info1;
    (*R)->nInfos = 2;

    // ajusta os filhos do nó "vazio"
    (*R)->esq = (*R)->cen;
    (*R)->cen = (*pai)->cen->esq;
    (*R)->dir = (*pai)->cen->cen;

    // deixa o pai vazio (resolvido na próxima chamada)
    (*pai)->nInfos = 0;

    // ajusta os filhos do pai
    free((*pai)->cen);
    (*pai)->cen = (*R);
    (*pai)->esq = NULL;
}

void balanceia(Arvore **R, Arvore **pai){
    if(*R){
        if(ehVazio(*R)){
            if(*pai){

                char posicao[20];
                deQuemSouFilho(*R, *pai, posicao);

                if(strcmp(posicao, "DIREITA") == 0){
                    if((*pai)->cen->nInfos == 2)
                        onda_CentroParaDireita_CentroDuasInfos(R, pai);
                    else if((*pai)->cen->nInfos == 1)
                        onda_CentroParaDireita_CentroUmaInfo(R, pai);
                }
                else if(strcmp(posicao, "CENTRO") == 0){
                    if((*pai)->esq->nInfos == 2)
                        onda_EsquerdaParaCentro_EsquerdaDuasInfos(R, pai);
                    else if((*pai)->esq->nInfos == 1){
                        if((*pai)->nInfos == 2)
                            onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiDuasInfos(R, pai);
                        else if((*pai)->nInfos == 1)
                            onda_EsquerdaParaCentro_EsquerdaEmaInfo_PaiUmaInfo(R, pai);
                    }
                }
                else if(strcmp(posicao, "ESQUERDA") == 0){
                    if((*pai)->cen->nInfos == 2)
                        onda_CentroParaEsquerda_CentroDuasInfos(R, pai);
                    else if((*pai)->cen->nInfos == 1){
                        if((*pai)->nInfos == 2)
                            onda_CentroParaEsquerda_CentroUmaInfo_PaiDuasInfos(R, pai);
                        else if((*pai)->nInfos == 1)
                            onda_CentroParaEsquerda_CentroUmaInfo_PaiUmaInfo(R, pai);
                    }
                }
            }
            else *R = (*R)->cen;
        }
    }
}

// encontra o maior filho da subárvore esquerda (ou meio dependendo da pos)
void maiorFilhoEsq(Arvore **R, Arvore **maiorFilho, Arvore **paiM, int posicaoInfoQueVaiSerRemovida){
    if(*maiorFilho){
        if(ehFolha(*maiorFilho)){
            DadoUnion infoQueVaiSerRemovida;
            if(posicaoInfoQueVaiSerRemovida == 1){
                infoQueVaiSerRemovida = (*R)->info1;
                if((*maiorFilho)->nInfos == 2){
                    (*R)->info1 = (*maiorFilho)->info2;
                    (*maiorFilho)->info2 = infoQueVaiSerRemovida;
                }else{ 
                    (*R)->info1 = (*maiorFilho)->info1;
                    (*maiorFilho)->info1 = infoQueVaiSerRemovida;
                }
            }else if(posicaoInfoQueVaiSerRemovida == 2){ 
                infoQueVaiSerRemovida = (*R)->info2;
                if((*maiorFilho)->nInfos == 2){
                    (*R)->info2 = (*maiorFilho)->info2;
                    (*maiorFilho)->info2 = infoQueVaiSerRemovida;
                }else{ 
                    (*R)->info2 = (*maiorFilho)->info1;
                    (*maiorFilho)->info1 = infoQueVaiSerRemovida;
                }
            }
            removerArvore23(maiorFilho, paiM, &infoQueVaiSerRemovida);
        }else{
            if((*maiorFilho)->nInfos == 2){
                maiorFilhoEsq(R, &(*maiorFilho)->dir, paiM, posicaoInfoQueVaiSerRemovida);
            }else if((*maiorFilho)->nInfos == 1){
                maiorFilhoEsq(R, &(*maiorFilho)->cen, paiM, posicaoInfoQueVaiSerRemovida);
            }
        }
    }
    balanceia(maiorFilho, paiM);
}

void removerArvore23(Arvore **R, Arvore **pai, DadoUnion *info){
    int compInfo1 = 2, compInfo2 = 2;
    if(*R){
        if(strcmp(info->ALBUM.nome, (*R)->info1.ALBUM.nome) == 0){
            if(ehFolha(*R)){
                if((*R)->nInfos == 2){
                    (*R)->info1 = (*R)->info2;
                    (*R)->nInfos = 1;
                }else (*R)->nInfos = 0;
            }else{
                Arvore **maiorFilho = &((*R)->esq);
                Arvore **paiM = R;
                maiorFilhoEsq(R, maiorFilho, paiM, 1);
            }
        }else if((*R)->nInfos == 2 && (strcmp(info->ALBUM.nome, (*R)->info2.ALBUM.nome)) == 0){
            if(ehFolha(*R)) (*R)->nInfos = 1;
            else{
                Arvore **maiorFilho = &((*R)->cen);
                Arvore **paiM = R;
                maiorFilhoEsq(R, maiorFilho, paiM, 2);
            }
        }else{
            compInfo1 = strcmp(info->ARTISTA.nome, (*R)->info1.ARTISTA.nome);
            if((*R)->nInfos == 2)
                compInfo2 = strcmp(info->ARTISTA.nome, (*R)->info2.ARTISTA.nome);

            if(compInfo1 < 0)
                removerArvore23(&((*R)->esq), R, info);
            else if((*R)->nInfos == 1 || (compInfo2 != 2 && compInfo2 < 0))
                removerArvore23(&((*R)->cen), R, info);
            else removerArvore23(&((*R)->dir), R, info);
        }    
    }

    balanceia(R, pai);
}