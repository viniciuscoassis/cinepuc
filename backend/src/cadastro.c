/*
  Programa: Cadastro de Usuários e "Assistir Filme"
  Descrição: Este programa permite cadastrar usuários, fazer login, escolher um filme para assistir e exibir uma mensagem informando o filme escolhido.
*/

/*
    Para executar:
        -Abra o terminal na pasta backend
        -Compile:
            gcc src/cadastro.c lib/funcoesCadastro.c -I include -o cadastro
        -Rode:
            ./cadastro
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cadastro.h"
//#include <Windows.h>

int main() {
    int escolha = 0;
    char email[50];
    char senha[20];

    while (escolha != 3) {
        printf("===== MENU =====\n");
        printf("1. Cadastrar usuarios\n");
        printf("2. Fazer login\n");
        printf("3. Sair\n");
        printf("Digite a opcao desejada: ");
        scanf("%d", &escolha);
        getchar();

        switch (escolha) {
            case 1:
                // Cadastrar usuarios
                printf("===== CADASTRO DE USUARIOS =====\n");
 

                while (1) {
                    Usuario novoUsuario;
                    cadastrarUsuario(&novoUsuario);
                    salvarUsuarioArquivo(novoUsuario);
                    printf("Usuario cadastrado com sucesso.\n");

                    printf("Deseja cadastrar outro usuario? (1 - Sim, 0 - Nao): ");
                    int cadastrarOutro;
                    scanf("%d", &cadastrarOutro);
                    getchar();

                    if (cadastrarOutro == 0)
                        break;
                }
                break;
            case 2:
                // Fazer login
                printf("Digite o email: ");
                scanf("%s", email);
                printf("Digite a senha: ");
                scanf("%s", senha);

                if (validarUsuario(email, senha)) {
                    printf("Login realizado com sucesso.\n");
                    // Sleep(1000);
                    printf("Escolha o filme:\n");
                    printf("1. Guardioes da Galaxia 3\n");
                    printf("2. Velozes e Furiosos X\n");
                    printf("3. Pequena Sereia\n");
                    printf("4. O Demonio dos Mares\n");

                    int filmeEscolhido;
                    printf("Digite o numero do filme que deseja assistir: ");
                    scanf("%d", &filmeEscolhido);

                    switch (filmeEscolhido) {
                        case 1:
                            printf("Voce esta assistindo Guardioes da Galaxia 3.\n");
                            break;
                        case 2:
                            printf("Voce esta assistindo Velozes e Furiosos X.\n");
                            break;
                        case 3:
                            printf("Voce esta assistindo Pequena Sereia.\n");
                            break;
                        case 4:
                            printf("Voce esta assistindo O Demonio dos Mares.\n");
                            break;
                        default:
                            printf("Opcao invalida.\n");
                            break;
                    }

                    return 0;  // Finaliza o programa apos assistir o filme
                } else {
                    printf("Usuario invalido.\n");
                }
                break;
            case 3:
                // Sair
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida. Digite novamente.\n");
                break;
        }

        printf("\n");
    }

    return 0;
}