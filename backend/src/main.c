// COMPILADOR POR ENQUANTO : gcc -o programa src/main.c lib/estruturaCinema.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estruturaCinema.h"

int main() {
    int quantidadeSessoes;
    char nomeArquivo[] = "cadeirasAVT.bin";
    Registro* sessoes;

    // adicionarSessoes(nomeArquivo);     //QUANDO FOR CRIAR NOVA SESSÃO DE FILMES DESCOMENTAR
    // return 0;


    // sessoes = obterSessoes(nomeArquivo, &quantidadeSessoes);
    // if (sessoes != NULL) {
    //     // Imprime as sessões e suas cadeiras
    //     exibirSessoes(sessoes, quantidadeSessoes);
    // }
    // return 1;

    sessoes = obterSessoes(nomeArquivo, &quantidadeSessoes);
    if (sessoes == NULL) {
        return 1;
    }

    ListaAssentos* listaAssentos = criarListaAssentos(10);
    if (listaAssentos == NULL) {
        return 1;
    }

    int opcao;
    do {
        printf("Selecione uma opção:\n");
        printf("1. Adicionar assento\n");
        printf("2. Remover assento\n");
        printf("3. Finalizar pedido\n");
        printf("0. Sair\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        int idSessao, idCadeira;
        switch (opcao) {
            case 1:
                printf("Digite o ID da sessão: ");
                scanf("%d", &idSessao);
                printf("Digite o ID da cadeira: ");
                scanf("%d", &idCadeira);

                if (cadeiraOcupada(sessoes, quantidadeSessoes, idSessao, idCadeira)) {
                    printf("A cadeira (Sessão: %d, Cadeira: %d) já está ocupada.\n", idSessao, idCadeira);
                } else {
                    adicionarAssento(listaAssentos, idSessao, idCadeira);
                    printf("Assento adicionado com sucesso!\n");
                }

                exibirListaAssentos(listaAssentos);
                break;
            case 2:
              if (listaAssentos->quantidadeAssentos > 0) {
                int idSessao, idCadeira;
                printf("Digite o ID da sessão do assento a ser removido: ");
                scanf("%d", &idSessao);
                printf("Digite o ID da cadeira do assento a ser removido: ");
                scanf("%d", &idCadeira);
                removerAssento(listaAssentos, idSessao, idCadeira);
                exibirListaAssentos(listaAssentos);
            } else {
                printf("A lista de assentos está vazia.\n");
            }
                break;
            case 3:
                finalizarPedido(listaAssentos, sessoes, quantidadeSessoes, nomeArquivo);
                opcao = 0;  // Sair do loop
                break;
            case 0:
                destruirListaAssentos(listaAssentos);
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opção inválida.\n");
                break;
        }
    } while (opcao != 0);

    free(sessoes);

    return 0;
}