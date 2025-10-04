#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include "prototipos.h"

void menu(){
    printf("\n========== MENU PRINCIPAL ==========\n");
    printf("1  - Cadastrar artista\n");
    printf("2  - Cadastrar álbum\n");
    printf("3  - Cadastrar música\n");

    printf("---------------------------------------\n");

    printf("4  - Listar todos os artistas\n");
    printf("5  - Mostrar álbuns de um artista\n");
    printf("6  - Mostrar músicas de um álbum\n");
    printf("7  - Mostrar músicas de um artista\n");

    printf("---------------------------------------\n");

    printf("8  - Buscar artista (mostrar caminho)\n");
    printf("9  - Buscar música de artista\n");
    printf("10 - Buscar música em um álbum\n");

    printf("---------------------------------------\n");

    printf("11 - Mostrar artistas por estilo musical\n");
    printf("12 - Remover música de um álbum\n");
    printf("13 - Remover álbum de um artista\n");
    printf("14 - Remover artista\n");
    printf("15 - Remover música de um artista\n");
    printf("-----------------------------------------\n");
    printf("0  - Sair\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
}

int main(){
    setlocale(LC_ALL, "Portuguese");

    Arvore *biblioteca = inicializar(); // árvore principal (artistas)
    int opcao;

    do {
        menu();
        scanf("%d", &opcao);
        getchar(); // limpar buffer

        switch(opcao){
            case 1: { // CADASTRAR ARTISTA
                Arvore *novoArtista = alocarTree(ARTISTA);
                if(novoArtista != NULL){
                    preencherNo(novoArtista);
                    if(inserirArvore(&biblioteca, novoArtista))
                        atualizaCorRaiz(&biblioteca);
                        printf("\n Artista cadastrado com sucesso!\n");
                    else
                        printf("\n Erro ao cadastrar artista.\n");
                }
                break;
            }

            case 2: { // CADASTRAR ÁLBUM
                char nomeArtista[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                getchar();
                deixarMaiusculo(nomeArtista);

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista && artista->tipo == ARTISTA){
                    Arvore *novoAlbum = alocarTree(ALBUM);
                    if(novoAlbum != NULL){
                        preencherNo(novoAlbum);
                        if(inserirArvore(&artista->dado.ARTISTA.albuns, novoAlbum)){
                            artista->dado.ARTISTA.numeroAlbuns++;
                            atualizaCorRaiz(&artista->dado.ARTISTA.albuns);
                            printf("\n Álbum cadastrado com sucesso!\n");
                        } else {
                            printf("\n Erro ao cadastrar álbum.\n");
                        }
                    }
                } else {
                    printf("\n Artista não encontrado!\n");
                }
                break;
            }

            case 3: { // CADASTRAR MÚSICA
                char nomeArtista[100], tituloAlbum[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                getchar();
                deixarMaiusculo(nomeArtista);

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if (artista && artista->tipo == ARTISTA) {
                    printf("Digite o título do álbum: ");
                    scanf(" %[^\n]", tituloAlbum);
                    getchar();
                    deixarMaiusculo(tituloAlbum);

                    Arvore *album = buscarAlbumDeArtista(artista, tituloAlbum);
                    if (album && album->tipo == ALBUM) {
                        Musica *novaMusica = alocarMusica();
                        if (novaMusica != NULL) {
                            preencherMusica(novaMusica);
                            if (inserirMusica(&album->dado.ALBUM.musicas, *novaMusica)) {
                                album->dado.ALBUM.numeroMusicas++;
                                printf("\n Música cadastrada com sucesso!\n");
                            } else {
                                printf("\n Erro ao cadastrar música.\n");
                            }
                        }
                    } else {
                        printf("\n Álbum não encontrado.\n");
                    }
                } else {
                    printf("\n Artista não encontrado.\n");
                }
                break;

            }

            case 4: { // LISTAR ARTISTAS
                imprimirArvRubroNegra(biblioteca);
                break;
            }

            case 5: { // MOSTRAR ÁLBUNS DE ARTISTA
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                getchar();
                deixarMaiusculo(nome);
                mostrarAlbunsDeArtista(biblioteca, nome);
                break;
            }

            case 6: { // MOSTRAR MÚSICAS DE UM ÁLBUM
                char nome[100], nalbum[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                getchar();
                deixarMaiusculo(nome);
                Arvore *artista = buscarArvRubroNegra(biblioteca, nome);
                if (artista){
                    if (artista && artista->tipo == ARTISTA){
                        printf("Digite o título do álbum: ");
                        scanf(" %[^\n]", nalbum);
                        getchar();
                        deixarMaiusculo(nalbum);
                        Arvore *album = buscarAlbumDeArtista(artista, nalbum);
                        mostrarMusicasDeAlbum(album);
                } else {printf("Artista não encontnrado");}
                }
                break;
            }

            case 7: { // MOSTRAR MÚSICAS DE UM ARTISTA
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                getchar();
                deixarMaiusculo(nome);
                mostrarMusicasDeArtista(biblioteca, nome);
                break;
            }

            case 8: { // BUSCAR ARTISTA (CAMINHO)
                char nome[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                getchar();
                deixarMaiusculo(nome);
                int comparacoes = 0;
                printf("\n Caminho da busca:\n");
                mostrarCaminhoBusca(biblioteca, nome, &comparacoes);
                printf(" Comparações realizadas: %d\n", comparacoes);
                break;
            }

            case 9: { // BUSCAR MÚSICA DE ARTISTA
                char nomeArtista[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                getchar();
                deixarMaiusculo(nomeArtista);
                printf("Digite o título da música: ");
                scanf(" %[^\n]", tituloMusica); 
                deixarMaiusculo(tituloMusica);
                
                Musica *musica = buscarMusicaDeArtista(biblioteca, nomeArtista, tituloMusica);

                if(musica){
                    printf("\n Música encontrada: %s (%d min)\n", musica->titulo, musica->minutos);
                } else {
                    printf("\n Música não encontrada.\n");
                }
                break;
            }

            case 10: { // BUSCAR MÚSICA EM UM ÁLBUM
                char nomeArtista[100], tituloAlbum[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                getchar();
                deixarMaiusculo(nomeArtista);
                printf("Digite o título do álbum: ");
                scanf(" %[^\n]", tituloAlbum);
                deixarMaiusculo(tituloAlbum);
                printf("Digite o título da música: ");
                scanf(" %[^\n]", tituloMusica);
                deixarMaiusculo(tituloMusica);

                Musica *musica = buscarMusicaDeAlbum(biblioteca, nomeArtista, tituloAlbum, tituloMusica);
                if(musica){
                    printf("\n Música encontrada: %s (%d min)\n", musica->titulo, musica->minutos);
                } else {
                    printf("\n Música não encontrada.\n");
                }
                break;
            }


            case 11: { // MOSTRAR ARTISTAS POR ESTILO
                char estilo[50];
                printf("Digite o estilo musical: ");
                scanf(" %[^\n]", estilo);
                getchar();
                deixarMaiusculo(estilo);
                mostrarArtistasPorEstilo(biblioteca, estilo);
                break;
            }

            case 12: { // REMOVER MÚSICA
                char nomeArtista[100], tituloAlbum[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                getchar();
                deixarMaiusculo(nomeArtista);
                printf("Digite o título do álbum: ");
                scanf(" %[^\n]", tituloAlbum);
                getchar();
                deixarMaiusculo(tituloAlbum);
                printf("Digite o título da música a remover: ");
                scanf(" %[^\n]", tituloMusica);
                getchar();
                deixarMaiusculo(tituloMusica);

                Arvore *album = buscarAlbumDeArtista(biblioteca, tituloAlbum);
                if(album != NULL){
                    if(removerMusica(&album->dado.ALBUM.musicas, tituloMusica)){
                        album->dado.ALBUM.numeroMusicas--;
                        printf("\n Música removida com sucesso!\n");
                    } else {
                        printf("\n Música não encontrada.\n");
                    }
                } else {
                    printf("\n Álbum ou artista não encontrado.\n");
                }
                break;
            }

            case 13: {
                char nomeAlbum[100];
                printf("Digite o nome do album a ser removido: ");
                scanf(" %[^\n]", nomeAlbum);
                getchar();
                deixarMaiusculo(nomeAlbum);
                Arvore *Aremover = buscarAlbumDeArtista(biblioteca, nomeAlbum);
                if(Aremover != NULL){
                    removerArvRubroNegra(&biblioteca, nomeAlbum);
                    printf("Álbum %s removido com sucesso!\n", nomeAlbum);
                } else {
                    printf("Álbum não encontrado.\n");
                }
            }

            case 14:{
                char nomeArtista[100];
                printf("Digite o nome do Artista que deseja apagar: ");
                scanf(" %[^\n]", nomeArtista);
                getchar();
                deixarMaiusculo(nomeArtista);
                Arvore *Aremover = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(Aremover != NULL){
                    removerArvRubroNegra(&biblioteca, nomeArtista);
                    printf("Artista %s removido com sucesso!\n", nomeArtista);
                }else{
                    printf("Artista não encontrado.\n");
                }
            }

            case 0:
                printf("\n Encerrando o programa...\n");
                break;

            default:
                printf("\n Opção inválida!\n");
        }

    } while(opcao != 0);

    liberarArvore(biblioteca);
    return 0;
}