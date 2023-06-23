/*
  Programa: Cadastro de Usuários e "Assistir Filme"
  Descrição: Este programa permite cadastrar usuários, fazer login, escolher um filme para assistir e exibir uma mensagem informando o filme escolhido.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <Windows.h>

typedef struct usuario {
    char nome[50];
    int idade;
    char email[50];
    char senha[20];
    char cpf[12];
} Usuario;

// Funcoes de cadastro e leitura de usuarios
void cadastrarUsuario(Usuario* usuario);
void salvarUsuarioArquivo(Usuario usuario);
void lerUsuarios();
int validarUsuario(const char email[50], const char senha[20]);

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

void cadastrarUsuario(Usuario* usuario) {
    printf("Digite o nome: ");
    fgets(usuario->nome, sizeof(usuario->nome), stdin);
    usuario->nome[strcspn(usuario->nome, "\n")] = '\0';

    printf("Digite a idade: ");
    scanf("%d", &usuario->idade);

    printf("Digite o email: ");
    scanf("%s", usuario->email);
    getchar();

    printf("Digite a senha: ");
    scanf("%s", usuario->senha);
    getchar();

    printf("Digite o CPF: ");
    scanf("%s", usuario->cpf);
    getchar();
}

void salvarUsuarioArquivo(Usuario usuario) {
    // Abre o arquivo para salvar dados
    FILE* arquivo = fopen("./cadPessoa.bin", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Escreve no arquivo
    fprintf(arquivo, "%s,%d,%s,%s,%s\n", usuario.nome, usuario.idade, usuario.email, usuario.senha, usuario.cpf);
    fclose(arquivo);
}

void lerUsuarios() {
    // Abre o arquivo para ler os dados
    FILE* arquivo = fopen("./cadPessoa.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Usuario usuario;

    while (fscanf(arquivo, "%[^,],%d,%[^,],%[^,],%[^\n]\n", usuario.nome, &usuario.idade, usuario.email, usuario.senha, usuario.cpf) != EOF) {
        printf("Nome: %s\n", usuario.nome);
        printf("Idade: %d\n", usuario.idade);
        printf("Email: %s\n", usuario.email);
        printf("Senha: %s\n", usuario.senha);
        printf("CPF: %s\n", usuario.cpf);
        printf("\n");
    }

    fclose(arquivo);
}

int validarUsuario(const char email[50], const char senha[20]) {
    FILE* arquivo = fopen("./cadPessoa.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Usuario usuario;

    while (fscanf(arquivo, "%[^,],%d,%[^,],%[^,],%[^\n]\n", usuario.nome, &usuario.idade, usuario.email, usuario.senha, usuario.cpf) != EOF) {
        if (strcmp(usuario.email, email) == 0 && strcmp(usuario.senha, senha) == 0) {
            fclose(arquivo);
            return 1; // Validacao bem-sucedida
        }
    }

    fclose(arquivo);
    return 0; // Validacao falhou
}
