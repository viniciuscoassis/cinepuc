#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/estruturaCinema.h"

int buscaCadeira(int idFilme, int idSessao, int numero) {

    FILE* arquivo;

    switch (idFilme)
    {
    case 1:
        arquivo = fopen("cadeirasSMB.bin", "rb");
        break;
    case 2:
        arquivo = fopen("cadeirasGDG.bin", "rb");
        break;
    case 3:
        arquivo = fopen("cadeirasVFX.bin", "rb");
        break;
    case 4:
        arquivo = fopen("cadeirasAVT.bin", "rb");
        break;
    }

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Registro reg;
    rewind(arquivo);
    while (fread(&reg, sizeof(Registro), 1, arquivo) == 1) {
        if (reg.idSessao == idSessao && reg.idCadeira == numero) {
            if (reg.status == 1) //Ocupada
            {
                fclose(arquivo);
                return 1;
            }            
            fclose(arquivo);
            return 0; // Validacao bem-sucedida
        }
    }
}

/*---------------------------------------------------------------------------------------------*/

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

    int idSessao, idCadeira, idFilme;

    idFilme = 4;

    // Loop para adicionar 4 sessões
    for (idSessao = 1; idSessao <= 4; idSessao++) {
        // Loop para adicionar 30 cadeiras para cada sessão
        for (idCadeira = 0; idCadeira <= 63; idCadeira++) {
            // Preenche os dados do registro
            registro.idFilme = idFilme;
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

void reservarCadeira(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira, char* nomeArquivo) {
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
                FILE* arquivo = fopen(nomeArquivo, "rb+");
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
        printf("Filme: %d, Sessão: %d, Cadeira: %d, Status: %s, Comprador: %s\n",
            sessoes[i].idFilme,
            sessoes[i].idSessao,
            sessoes[i].idCadeira,
            sessoes[i].status == 1 ? "Ocupada" : "Disponível",
            sessoes[i].status == 1 ? sessoes[i].comprador : "");
    }
}


// LISTA DE ASSENTOS

ListaAssentos* criarListaAssentos(int capacidade) {
    ListaAssentos* lista = (ListaAssentos*)malloc(sizeof(ListaAssentos));
    if (lista == NULL) {
        printf("Erro ao alocar memória.\n");
        return NULL;
    }
    lista->capacidade = capacidade;
    lista->quantidadeAssentos = 0;
    lista->assentos = (Assento*)malloc(capacidade * sizeof(Assento));
    if (lista->assentos == NULL) {
        printf("Erro ao alocar memória.\n");
        free(lista);
        return NULL;
    }
    return lista;
}

void destruirListaAssentos(ListaAssentos* lista) {
    free(lista->assentos);
    free(lista);
}

void adicionarAssento(ListaAssentos* lista, int idSessao, int idCadeira) {
    if (lista->quantidadeAssentos == lista->capacidade) {
        printf("A lista de assentos está cheia.\n");
        return;
    }
    Assento novoAssento;
    novoAssento.idSessao = idSessao;
    novoAssento.idCadeira = idCadeira;
    lista->assentos[lista->quantidadeAssentos++] = novoAssento;
}

void removerAssento(ListaAssentos* lista, int idSessao, int idCadeira) {
    int i;
    for (i = 0; i < lista->quantidadeAssentos; i++) {
        Assento assento = lista->assentos[i];
        if (assento.idSessao == idSessao && assento.idCadeira == idCadeira) {
            // Deslocar os elementos à direita do elemento removido
            for (int j = i; j < lista->quantidadeAssentos - 1; j++) {
                lista->assentos[j] = lista->assentos[j + 1];
            }
            lista->quantidadeAssentos--;
            printf("Assento removido com sucesso!\n");
            return;
        }
    }
    printf("O assento não foi encontrado na lista.\n");
}

void finalizarPedido(ListaAssentos* lista, Registro* sessoes, int quantidadeSessoes, char* nomeArquivo) {
    for (int i = 0; i < lista->quantidadeAssentos; i++) {
        Assento assento = lista->assentos[i];
        if (!cadeiraOcupada(sessoes, quantidadeSessoes, assento.idSessao, assento.idCadeira)) {
            reservarCadeira(sessoes, quantidadeSessoes, assento.idSessao, assento.idCadeira, nomeArquivo);
        } else {
            printf("O assento (Sessão: %d, Cadeira: %d) já está ocupado.\n", assento.idSessao, assento.idCadeira);
        }
    }
    printf("Assentos adicionados com sucesso!\n");
    destruirListaAssentos(lista);
}


void exibirListaAssentos(ListaAssentos* lista) {
    printf("Assentos selecionados:\n");
    for (int i = 0; i < lista->quantidadeAssentos; i++) {
        Assento assento = lista->assentos[i];
        printf("Sessão: %d, Cadeira: %d\n", assento.idSessao, assento.idCadeira);
    }
    printf("\n");
}
