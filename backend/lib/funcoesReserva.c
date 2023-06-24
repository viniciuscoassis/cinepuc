#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include ".h"

int buscaCadeira(int idFilme, int idSessao, int numero) {
    
    switch (idFilme)
    {
    case 1:
        FILE* arquivo = fopen("../cadeirasSMB.bin", "rb");
        break;
    case 2:
        FILE* arquivo = fopen("../cadeirasGDG.bin", "rb");
        break;
    case 3:
        FILE* arquivo = fopen("../cadeirasVFX.bin", "rb");
        break;
    case 4:
        FILE* arquivo = fopen("../cadeirasAVT.bin", "rb");
        break;
    }
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    Registro ;
    rewind(arquivo);
    while (fread(&registro, sizeof(Registro), 1, arquivo) == 1) {
        if (strcmp(usuario.email, email) == 0 && strcmp(usuario.senha, senha) == 0) {
            printf("1");
            fclose(arquivo);
            return 1; // Validacao bem-sucedida
        }
    }
    fclose(arquivo);
    return 0; // Validacao falhou
}