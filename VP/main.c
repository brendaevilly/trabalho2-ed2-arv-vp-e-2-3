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
    printf("13 - (Futuro) Remover álbum de um artista\n");
    printf("14 - (Futuro) Remover artista\n");
    printf("15 - (Futuro) Remover música de um artista\n");
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
                    if(inserirArvRubroNegra(&biblioteca, novoArtista))
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
                deixarMaiusculo(nomeArtista);

                Arvore *artista = buscarArvRubroNegra(biblioteca, nomeArtista);
                if(artista && artista->tipo == ARTISTA){
                    Arvore *novoAlbum = alocarTree(ALBUM);
                    if(novoAlbum != NULL){
                        preencherNo(novoAlbum);
                        if(inserirArvRubroNegra(&artista->dado.ARTISTA.albuns, novoAlbum)){
                            artista->dado.ARTISTA.numeroAlbuns++;
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
                deixarMaiusculo(nomeArtista);

                printf("Digite o título do álbum: ");
                scanf(" %[^\n]", tituloAlbum);
                deixarMaiusculo(tituloAlbum);

                Album *album = buscarAlbumDeArtista(biblioteca, nomeArtista, tituloAlbum);
                if(album != NULL){
                    Musica novaMusica;
                    preencherMusica(&novaMusica);
                    if(inserirMusica(&album->musicas, novaMusica)){
                        album->numeroMusicas++;
                        printf("\n Música cadastrada com sucesso!\n");
                    } else {
                        printf("\n Erro ao cadastrar música.\n");
                    }
                } else {
                    printf("\n Álbum ou artista não encontrado!\n");
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
                deixarMaiusculo(nome);
                mostrarAlbunsDeArtista(biblioteca, nome);
                break;
            }

            case 6: { // MOSTRAR MÚSICAS DE UM ÁLBUM
                char nome[100], album[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nome);
                deixarMaiusculo(nome);
                printf("Digite o título do álbum: ");
                scanf(" %[^\n]", album);
                deixarMaiusculo(album);
                mostrarMusicasDeAlbum(biblioteca, nome, album);
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
                printf(" Comparações realizadas: %d\n", comparacoes);
                break;
            }

            case 9: { // BUSCAR MÚSICA DE ARTISTA
                char nomeArtista[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
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
                deixarMaiusculo(estilo);
                mostrarArtistasPorEstilo(biblioteca, estilo);
                break;
            }

            case 12: { // REMOVER MÚSICA
                char nomeArtista[100], tituloAlbum[100], tituloMusica[100];
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]", nomeArtista);
                deixarMaiusculo(nomeArtista);
                printf("Digite o título do álbum: ");
                scanf(" %[^\n]", tituloAlbum);
                deixarMaiusculo(tituloAlbum);
                printf("Digite o título da música a remover: ");
                scanf(" %[^\n]", tituloMusica);
                deixarMaiusculo(tituloMusica);

                Album *album = buscarAlbumDeArtista(biblioteca, nomeArtista, tituloAlbum);
                if(album != NULL){
                    if(removerMusica(&album->musicas, tituloMusica)){
                        album->numeroMusicas--;
                        printf("\n Música removida com sucesso!\n");
                    } else {
                        printf("\n Música não encontrada.\n");
                    }
                } else {
                    printf("\n Álbum ou artista não encontrado.\n");
                }
                break;
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
