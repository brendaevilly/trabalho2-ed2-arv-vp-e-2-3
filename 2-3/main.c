#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prototipos.h"

void menu(){
    printf("\n========== MENU PRINCIPAL ==========\n");
    printf("1  - Cadastrar artista\n");
    printf("2  - Cadastrar album\n");
    printf("3  - Cadastrar musica\n");

    printf("---------------------------------------\n");

    printf("4  - Listar todos os artistas\n");
    printf("5  - Mostrar albuns de um artista\n");
    printf("6  - Mostrar musicas de um album\n");
    printf("7  - Mostrar musicas de um artista\n");

    printf("---------------------------------------\n");

    printf("8  - Buscar artista (mostrar caminho)\n");
    printf("9  - Buscar musica de artista\n");
    printf("10 - Buscar musica em um album\n");

    printf("---------------------------------------\n");

    printf("11 - Mostrar artistas por estilo musical\n");
    printf("12 - Remover musica de um album\n");
    printf("13 - Remover album de um artista\n");
    printf("14 - Remover artista\n");
    printf("-----------------------------------------\n");
    printf("0  - Sair\n");
    printf("====================================\n");
    printf("Escolha uma opcao: ");
}

int main(){
    Arvore *biblioteca = inicializar();
    int opcao;

    DadoUnion sobe;

    do {
        DadoUnion sobe;
        menu();
        scanf("%d", &opcao);

        switch(opcao){
            case 1: {
                int inserido = 0;
                DadoUnion info;
                preencheInfo(ARTISTA, &info);

                Arvore *busca = inicializar();
                int nInfoBuscada = buscarNaArvore23(biblioteca, info.ARTISTA.nome, &busca);
                if(!busca){
                    inserirNo(&biblioteca, info, NULL, &sobe, &inserido, ARTISTA);
                    if(inserido) printf("\n Artista cadastrado com sucesso!\n");
                    else printf("\n Erro ao cadastrar artista.\n");
                } else{
                    printf("\n Artista ja cadastrado.\n");
                    printf("%s\n", busca->info1.ARTISTA.nome);
                } 

                break;
            }
            case 2: {
                char nomeArtista[50];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *busca = NULL;
                int nInfoBuscada = buscarNaArvore23(biblioteca, nomeArtista, &busca);
                if(busca){
                    DadoUnion info;
                    preencheInfo(ALBUM, &info);

                    Arvore *album = NULL;
                    
                    if(nInfoBuscada == 1)
                        buscarNaArvore23(busca->info1.ARTISTA.albuns, info.ALBUM.nome, &album);
                    else
                        buscarNaArvore23(busca->info2.ARTISTA.albuns, info.ALBUM.nome, &album);

                    if(!album){
                        int inserido = 0;
                        if(nInfoBuscada == 1){
                            inserirNo(&(busca->info1.ARTISTA.albuns), info, NULL, &sobe, &inserido, ALBUM);
                            if(inserido){
                                busca->info1.ARTISTA.numeroAlbuns++;
                                printf("\n Album cadastrado com sucesso!\n");
                            } 
                            else printf("\n Erro ao cadastrar album.\n");
                            
                        } 
                        else{
                            inserirNo(&(busca->info2.ARTISTA.albuns), info, NULL, &sobe, &inserido, ALBUM);
                            if(inserido){
                                busca->info2.ARTISTA.numeroAlbuns++;
                                printf("\n Album cadastrado com sucesso!\n");
                            } 
                            else printf("\n Erro ao cadastrar album.\n");
                        }
                    } else printf("\n Album ja cadastrado para este artista.\n");
                } else printf("\n Artista nao encontrado!\n");
                break;
            } 
            case 3: { // CADASTRAR MÚSICA
                char nomeArtista[50], tituloAlbum[50];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                int nInfoBuscada;
                Arvore *artista = inicializar();
                nInfoBuscada = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if (artista) {
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    printf("%s", tituloAlbum);

                    DadoUnion art;
                    if(nInfoBuscada == 1)art = artista->info1;
                    else art = artista->info2;

                    Arvore *album = NULL;
                    int nInfoAlbum = buscarNaArvore23(art.ARTISTA.albuns, tituloAlbum, &album);

                    if (album) {
                        Musica *novaMusica = alocarM();
                        preencherMusica(novaMusica);    

                        if(nInfoAlbum == 1){
                            int inserido = inserirMusica(&(album->info1.ALBUM.musicas), novaMusica);
                            if(inserido){
                                album->info1.ALBUM.numeroMusicas++;
                                printf("\n Musica cadastrada com sucesso!\n");
                            } else {
                                printf("\n Erro ao cadastrar musica.\n");
                                free(novaMusica);
                            }
                        } else {
                            int inserido = inserirMusica(&(album->info2.ALBUM.musicas), novaMusica);
                            if(inserido){
                                album->info2.ALBUM.numeroMusicas++;
                                printf("\n Musica cadastrada com sucesso!\n");
                            } else {
                                printf("\n Erro ao cadastrar musica.\n");
                                free(novaMusica);
                            }
                        }
                    } else printf("\n Album nao encontrado.\n");
                } else printf("\n Artista nao encontrado.\n");
                break;

            } 
            case 4: { // LISTAR ARTISTAS
                imprimirArvore23(biblioteca);
                break;
            }
            case 5: { // MOSTRAR ÁLBUNS DE ARTISTA
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);
                mostrarAlbunsDeArtista(biblioteca, nome);
                break;
            }
            case 6: { // MOSTRAR MÚSICAS DE UM ÁLBUM
                char nome[100], nalbum[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);

                Arvore *artista = inicializar();
                int nInfoArt = buscarNaArvore23(biblioteca, nome, &artista);

                if (artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", nalbum);
                    deixarMaiusculo(nalbum);
                    Arvore *album;
                    int nInfoAlbum;

                    if(nInfoArt == 1)
                        album = buscarAlbumDeArtista(&artista->info1, nalbum, &nInfoAlbum);
                    else 
                        album = buscarAlbumDeArtista(&artista->info2, nalbum, &nInfoAlbum);
        
                    if(album)
                        if(nInfoAlbum == 1)
                            mostrarMusicasDeAlbum(&album->info1);
                        else mostrarMusicasDeAlbum(&album->info2);
                    else printf("Album nao encontrado");
                } else printf("Artista nao encontnrado");
                break;
            }
            case 7: { // MOSTRAR MÚSICAS DE UM ARTISTA
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);
                mostrarMusicasDeArtista(biblioteca, nome);
                break;
            }
            case 8: { // BUSCAR ARTISTA (CAMINHO)
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);

                int comparacoes = 0;
                printf("\n Caminho da busca:\n");
                mostrarCaminhoBusca(biblioteca, nome, &comparacoes);
                printf(" Comparacoes realizadas: %d\n", comparacoes);
                break;
            }
            case 9: { // BUSCAR MÚSICA DE ARTISTA
                char nomeArtista[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista;
                int nInfoBuscada = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if(artista){
                    printf("Digite o titulo da musica: ");
                    scanf(" %[^\n]", tituloMusica); 
                    deixarMaiusculo(tituloMusica);
                    
                    Musica *musica = inicializarM(); 
                    musica = buscarMusicaDeArtista(biblioteca, nomeArtista, tituloMusica);

                    if(musica) printf("\n Musica encontrada: %s (%d min)\n", musica->titulo, musica->minutos);
                    else printf("\n Musica nao encontrada.\n");
                } else printf("\n Artista nao encontrado.\n");                
                break;
            }
            case 10: { // BUSCAR MÚSICA EM UM ÁLBUM
                char nomeArtista[100], tituloAlbum[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista;
                int nInfoBuscada = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if(artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    Arvore *album;
                    int nInfoA;
                    if(nInfoBuscada == 1) album = buscarAlbumDeArtista(&artista->info1, tituloAlbum, &nInfoA);
                    else album = buscarAlbumDeArtista(&artista->info2, tituloAlbum, &nInfoA);

                    if(album){
                        printf("Digite o titulo da musica: ");
                        scanf(" %[^\n]", tituloMusica);
                        deixarMaiusculo(tituloMusica);

                        Musica *musica = NULL;
                        if(nInfoA == 1)
                            musica = buscarMusicaDeAlbum(&album->info1, tituloAlbum, tituloMusica);
                        else musica = buscarMusicaDeAlbum(&album->info2, tituloAlbum, tituloMusica);

                        if(musica) printf("\n Musica encontrada: %s (%d min)\n", musica->titulo, musica->minutos);
                        else printf("\n Musica nao encontrada.\n");
                    } else printf("\n Album nao encontrado.\n");
                } else printf("\n Artista nao encontrado.\n");
                break;
            }
            case 11: { // MOSTRAR ARTISTAS POR ESTILO
                char estilo[50];
                printf("Digite o estilo musical: ");
                scanf(" %[^\n]", estilo);
                deixarMaiusculo(estilo);
                mostrarArtistasPorEstilo(biblioteca, estilo);
                break;
            }
            case 12: { // REMOVER MÚSICA
                char nomeArtista[100], tituloAlbum[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista;
                int nInfoBuscada = buscarNaArvore23(biblioteca, nomeArtista, &artista);
                if(artista){
                    printf("Digite o titulo do album: ");
                    scanf(" %[^\n]", tituloAlbum);
                    deixarMaiusculo(tituloAlbum);

                    Arvore *album;
                    int nInfoA;
                    if(nInfoBuscada == 1) album = buscarAlbumDeArtista(&artista->info1, tituloAlbum, &nInfoA);
                    else album = buscarAlbumDeArtista(&artista->info2, tituloAlbum, &nInfoA);

                    if(album){
                        printf("Digite o titulo da musica a remover: ");
                        scanf(" %[^\n]", tituloMusica);
                        deixarMaiusculo(tituloMusica);

                        int removeu;
                        if(nInfoA == 1){
                            removeu = removerMusica(&album->info1.ALBUM.musicas, tituloMusica);
                            album->info1.ALBUM.numeroMusicas--;
                        }else{
                            removeu = removerMusica(&album->info2.ALBUM.musicas, tituloMusica);
                            album->info2.ALBUM.numeroMusicas--;
                        } 

                        if(removeu){
                            printf("\n Musica removida com sucesso!\n");
                        }else printf("\n Musica nao encontrada.\n");
                    } else printf("\n Album nao encontrado.\n");   
                } else printf("\n Artista nao encontrado.\n");

                break;
            }/*
            case 13: {
                char nomeAlbum[100], nomeArtista[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista){
                    printf("Digite o nome do album a ser removido: ");
                    scanf(" %[^\n]", nomeAlbum);

                    deixarMaiusculo(nomeAlbum);
                    if(removerArvRubroNegra(&(artista->dado.ARTISTA.albuns), nomeAlbum))
                        printf("Album %s removido com sucesso!\n", nomeAlbum);
                    else printf("Album nao encontrado.\n");
                } else printf("Artista nao encontrado.\n");
                
                break;
            }
            case 14:{
                char nomeArtista[100];
                printf("Digite o nome do Artista que deseja apagar: ");
                scanf(" %[^\n]", nomeArtista);
    
                deixarMaiusculo(nomeArtista);
                if(removerArvRubroNegra(&biblioteca, nomeArtista))
                    printf("Artista %s removido com sucesso!\n", nomeArtista);
                else printf("Artista nao encontrado.\n");
            }
*/

            
            case 0:
                printf("\n Encerrando o programa...\n");
                break;
            default:
                printf("\n Opcao invalida!\n");
        }

    } while(opcao != 0);

    liberarArvore(biblioteca);
    return 0;
}