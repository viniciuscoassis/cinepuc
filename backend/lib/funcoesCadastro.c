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

int salvarUsuarioArquivo(char email[50], char senha[20]) {
    if(verificaUsuario(email) == 0){
        return 0;
    }
    // Abre o arquivo para salvar dados
    FILE* arquivo = fopen("backend/src/cadPessoa.bin", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    // Cria uma struct Usuario e preenche os campos com os dados
    Usuario usuario;
    strcpy(usuario.email, email);
    strcpy(usuario.senha, senha);

    // Escreve a struct no arquivo
    fwrite(&usuario, sizeof(Usuario), 1, arquivo);

    fclose(arquivo);
    return 1;
}

void lerUsuarios() {
    // Abre o arquivo para ler os dados
    FILE* arquivo = fopen("backend/src/cadPessoa.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    Usuario usuario;

    while (fread(&usuario, sizeof(Usuario), 1, arquivo) == 1) {
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
    rewind(arquivo);
    while (fread(&usuario, sizeof(Usuario), 1, arquivo) == 1) {
        if (strcmp(usuario.email, email) == 0 && strcmp(usuario.senha, senha) == 0) {
            printf("1");
            fclose(arquivo);
            return 1; // Validacao bem-sucedida
        }
    }
    fclose(arquivo);
    return 0; // Validacao falhou
}

int verificaUsuario(char email[50]) {
    // Abre o arquivo para ler os dados
    FILE* arquivo = fopen("backend/src/cadPessoa.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    Usuario usuario;
    //Loop para ver todos os usuarios
    while (fread(&usuario, sizeof(Usuario), 1, arquivo) == 1) {
        //Condicao para ver se o email ja existe
        if(strcmp(usuario.email, email) == 0){
            return 0;
        }
    } 

    fclose(arquivo);
    return 1;
}