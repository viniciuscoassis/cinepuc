#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cadastro.h"

void cadastrarUsuario(Usuario* usuario) {
    /*Pede para o usuario digitar um email e senha*/
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

    // Cria uma struct Usuario e preenche os campos com os dados da cadastrarUsuario()
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

    //loop para printar usuarios existentes
    while (fread(&usuario, sizeof(Usuario), 1, arquivo) == 1) {
        printf("Email: %s\n", usuario.email);
        printf("Senha: %s\n", usuario.senha);
        printf("\n");
    }

    fclose(arquivo);
}

int validarUsuario(const char email[50], const char senha[20]) {
    //Abre o arquivo de cadastro
    FILE* arquivo = fopen("backend/src/cadPessoa.bin", "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Usuario usuario;
    rewind(arquivo);
    //loop para percorrer e verificar se os dados foram digitados corretamente
    while (fread(&usuario, sizeof(Usuario), 1, arquivo) == 1) {
        if (strcmp(usuario.email, email) == 0 && strcmp(usuario.senha, senha) == 0) {
            fclose(arquivo);
            return 1; // Validacao bem-sucedida / Login SUCESS
        }
    }
    fclose(arquivo);
    return 0; // Validacao falhou / Login FAILED
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