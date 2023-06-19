#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int idFilme;
    int idSessao;
    int numero;
    int status;
    char comprador[50];
} Cadeira;

void obterDadosReserva(int* idSessao, int* numero) {
    printf("Digite o ID da Sessão: ");
    scanf("%d", idSessao);
    printf("Digite o número da cadeira: ");
    scanf("%d", numero);
}

void reservarCadeiraFilme(int idFilme) {
    int idSessao, numero;
    obterDadosReserva(&idSessao, &numero);

    char nomeArquivo[20];
    switch (idFilme) {
        case 1:
            strcpy(nomeArquivo, "C:/Users/fefem/OneDrive/Documentos/Faculdade/cinepuc/backend/src/protCadeiras.c");
            break;
        case 2:
            strcpy(nomeArquivo, "./cadeirasVFX.bin");
            break;
        case 3:
            strcpy(nomeArquivo, "./cadeirasSMB.bin");
            break;
        case 4:
            strcpy(nomeArquivo, "./cadeirasGDG.bin");
            break;
        default:
            printf("Filme inválido.\n");
            return;
    }

    FILE* arquivoFilme = fopen(nomeArquivo, "rb+");
    if (arquivoFilme == NULL) {
        printf("Erro ao abrir o arquivo do filme.\n");
        return;
    }

    reservaCadeira(arquivoFilme, idFilme, idSessao, numero);

    fclose(arquivoFilme);
}

void reservaCadeira(FILE* arquivo, int idFilme, int idSessao, int numero) {
    fseek(arquivo, 0, SEEK_SET); // Define o ponteiro do arquivo no início
    Cadeira cadeira;

    while (fread(&cadeira, sizeof(Cadeira), 1, arquivo) == 1) {
        if (cadeira.idFilme == idFilme && cadeira.idSessao == idSessao && cadeira.numero == numero) {
            if (cadeira.status == 0) {
                cadeira.status = 1; // Altera o status para 1
                fseek(arquivo, -sizeof(Cadeira), SEEK_CUR); // Volta a posição do ponteiro no arquivo
                fwrite(&cadeira, sizeof(Cadeira), 1, arquivo); // Escreve a cadeira modificada no arquivo
                printf("Reserva efetuada com sucesso.\n");
            } else {
                printf("Cadeira já reservada.\n");
            }
            return; // Sai da função após encontrar a cadeira
        }
    }

    printf("Cadeira não encontrada.\n");
}

void exibirCadeiras(FILE* arquivo) {
    fseek(arquivo, 0, SEEK_SET); // Define o ponteiro do arquivo no início
    Cadeira cadeira;

    while (fread(&cadeira, sizeof(Cadeira), 1, arquivo) == 1) {
        printf("ID do Filme: %d\n", cadeira.idFilme);
        printf("ID da Sessão: %d\n", cadeira.idSessao);
        printf("Número da Cadeira: %d\n", cadeira.numero);
        printf("Status: %d\n", cadeira.status);
        printf("Comprador: %s\n\n", cadeira.comprador);
    }
}

void menu() {
    int opcao;

    do {
        printf("1. Avatar\n");
        printf("2. Velozes e Furiosos X\n");
        printf("3. Super Mario Bross\n");
        printf("4. Guardiões da Galáxia\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                reservarCadeiraFilme(1);
                break;
            case 2:
                reservarCadeiraFilme(2);
                break;
            case 3:
                reservarCadeiraFilme(3);
                break;
            case 4:
                reservarCadeiraFilme(4);
                break;
            case 5:
                printf("Encerrando o programa...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (opcao != 5);
}

int main() {

    menu();

    return 0;
}