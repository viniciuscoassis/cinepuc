#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estruturaCinema.h"

int main() {
    char nomeArquivo[] = "cadeirasAVT.bin";
    
    // adicionarSessoes(nomeArquivo);     //QUANDO FOR CRIAR NOVA SESSÃO DE FILMES DESCOMENTAR
    // return 0;

    Registro* sessoes;
    int quantidadeSessoes, i, idSessao, idCadeira;

    // Obtém as sessões do arquivo
    sessoes = obterSessoes(nomeArquivo, &quantidadeSessoes);

    if (sessoes != NULL) {
        // Imprime as sessões e suas cadeiras
        exibirSessoes(sessoes, quantidadeSessoes);

        // Solicita o ID da sessão e da cadeira para verificar se está ocupada
        printf("Digite o ID da sessão: ");
        scanf("%d", &idSessao);
        printf("Digite o ID da cadeira: ");
        scanf("%d", &idCadeira);

        // Verifica se a cadeira está ocupada
        int ocupada = cadeiraOcupada(sessoes, quantidadeSessoes, idSessao, idCadeira);
        if (ocupada == 1) {
            char* nomeComprador = obterNomeComprador(sessoes, quantidadeSessoes, idSessao, idCadeira);
            printf("A cadeira está ocupada pelo comprador: %s\n", nomeComprador);

        } else if (ocupada == 0) {
            // Reserva a cadeira
            reservarCadeira(sessoes, quantidadeSessoes, idSessao, idCadeira);
        } else {
            printf("Cadeira não encontrada.\n");
        }

        // Libera a memória alocada para o vetor de sessões
        free(sessoes);
    }

    return 0;
}
