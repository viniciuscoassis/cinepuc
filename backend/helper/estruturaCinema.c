#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estruturaCinema.h"

Registro* obterSessoes(char* nomeArquivo, int* quantidadeSessoes) {
    FILE* arquivo;
    Registro registro;
    Registro* sessoes = NULL;
    int totalSessoes = 0;

    // Abre o arquivo em modo binário para leitura
    arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return NULL;
    }

    // Calcula a quantidade total de sessões no arquivo
    fseek(arquivo, 0, SEEK_END);
    long tamanhoArquivo = ftell(arquivo);
    totalSessoes = tamanhoArquivo / sizeof(Registro);

    // Aloca memória para armazenar as sessões
    sessoes = (Registro*)malloc(totalSessoes * sizeof(Registro));
    if (sessoes == NULL) {
        printf("Erro ao alocar memória.\n");
        fclose(arquivo);
        return NULL;
    }

    // Retorna o ponteiro de leitura para o início do arquivo
    rewind(arquivo);

    // Lê as sessões do arquivo e armazena no vetor
    int i = 0;
    while (fread(&registro, sizeof(Registro), 1, arquivo) == 1) {
        sessoes[i++] = registro;
    }

    // Define a quantidade de sessões encontradas
    *quantidadeSessoes = totalSessoes;

    // Fecha o arquivo
    fclose(arquivo);

    return sessoes;
}

void adicionarSessoes(char* nomeArquivo) {
    FILE* arquivo;
    Registro registro;

    // Abre o arquivo em modo binário para escrita
    arquivo = fopen(nomeArquivo, "ab");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return;
    }

    int idSessao, idCadeira;

    // Loop para adicionar 4 sessões
    for (idSessao = 1; idSessao <= 4; idSessao++) {
        // Loop para adicionar 30 cadeiras para cada sessão
        for (idCadeira = 1; idCadeira <= 30; idCadeira++) {
            // Preenche os dados do registro
            registro.idSessao = idSessao;
            registro.idCadeira = idCadeira;
            registro.status = 0; // Status disponível
            strcpy(registro.comprador, ""); // Comprador vazio

            // Escreve o registro no arquivo
            fwrite(&registro, sizeof(Registro), 1, arquivo);
        }
    }

    printf("Sessões adicionadas com sucesso!\n");

    // Fecha o arquivo
    fclose(arquivo);
}

int cadeiraOcupada(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira) {
    int i;
    for (i = 0; i < quantidadeSessoes; i++) {
        if (sessoes[i].idSessao == idSessao && sessoes[i].idCadeira == idCadeira) {
            return sessoes[i].status == 1;
        }
    }
    return -1; // Cadeira não encontrada
}

char* obterNomeComprador(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira) {
    int i;
    for (i = 0; i < quantidadeSessoes; i++) {
        if (sessoes[i].idSessao == idSessao && sessoes[i].idCadeira == idCadeira && sessoes[i].status == 1) {
            return sessoes[i].comprador;
        }
    }
    return NULL; // Cadeira não encontrada ou não reservada
}

void reservarCadeira(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira) {
    int i;
    for (i = 0; i < quantidadeSessoes; i++) {
        if (sessoes[i].idSessao == idSessao && sessoes[i].idCadeira == idCadeira) {
            if (sessoes[i].status == 0) { // Cadeira disponível
                char comprador[50];
                printf("Digite o nome do comprador: ");
                scanf("%s", comprador);
                fflush(stdin);

                sessoes[i].status = 1; // Atualiza o status para reservado
                strcpy(sessoes[i].comprador, comprador); // Atualiza o nome do comprador

                // Reabre o arquivo em modo de gravação
                FILE* arquivo = fopen("cadeirasAVT.bin", "rb+");
                if (arquivo == NULL) {
                    printf("Não foi possível abrir o arquivo.\n");
                    return;
                }

                // Posiciona o ponteiro de leitura/gravação no registro correto
                fseek(arquivo, i * sizeof(Registro), SEEK_SET);

                // Escreve o registro atualizado no arquivo
                fwrite(&sessoes[i], sizeof(Registro), 1, arquivo);

                // Fecha o arquivo
                fclose(arquivo);

                printf("Cadeira reservada com sucesso!\n");
            } else {
                printf("A cadeira já está ocupada.\n");
            }
            return;
        }
    }
    printf("Cadeira não encontrada.\n");
}

void exibirSessoes(Registro* sessoes, int quantidadeSessoes) {
    int i;
    for (i = 0; i < quantidadeSessoes; i++) {
        printf("Sessão: %d, Cadeira: %d, Status: %s, Comprador: %s\n",
               sessoes[i].idSessao,
               sessoes[i].idCadeira,
               sessoes[i].status == 1 ? "Ocupada" : "Disponível",
               sessoes[i].status == 1 ? sessoes[i].comprador : "");
    }
}
