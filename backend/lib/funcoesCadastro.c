#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cadastro.h"

void cadastrarUsuario(Usuario* usuario) {
    printf("Digite o email: ");
    scanf("%s", usuario->email);
    getchar();

    printf("Digite a senha: ");
    scanf("%s", usuario->senha);
    getchar();
}

void salvarUsuarioArquivo(char email[50], char senha[20]) {
    // Abre o arquivo para salvar dados
    FILE* arquivo = fopen("backend/src/cadPessoa.bin", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Escreve no arquivo
    fprintf(arquivo, "%s,%s\n", email, senha);
    fclose(arquivo);
}

void lerUsuarios() {
    // Abre o arquivo para ler os dados
    FILE* arquivo = fopen("backend/src/cadPessoa.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Usuario usuario;

    while (fscanf(arquivo, "%[^,],%[^,]\n", usuario.email, usuario.senha) != EOF) {
        printf("Email: %s\n", usuario.email);
        printf("Senha: %s\n", usuario.senha);
        printf("\n");
    }

    fclose(arquivo);
}

int validarUsuario(const char email[50], const char senha[20]) {
    FILE* arquivo = fopen("backend/src/cadPessoa.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Usuario usuario;

    while (fscanf(arquivo, "%[^,],%[^,]\n", usuario.email, usuario.senha) != EOF) {
        if (strcmp(usuario.email, email) == 0 && strcmp(usuario.senha, senha) == 0) {
            fclose(arquivo);
            return 1; // Validacao bem-sucedida
        }
    }

    fclose(arquivo);
    return 0; // Validacao falhou
}
