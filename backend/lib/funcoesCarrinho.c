#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/carrinho.h"

void cria(Carrinho* c)
{
    c->inicio = NULL;
    c->fim = NULL;
}

int insere(Carrinho* c, int idFilme, int idSessao, int numero, char* comprador)
{
    Cadeira* aux = (Cadeira*)malloc(sizeof(Cadeira));
    if (aux == NULL)
        return 0;

    aux->idFilme = idFilme;
    aux->idSessao = idSessao;
    aux->numero = numero;
    strcpy(aux->comprador, comprador);
    aux->prox = NULL;

    if (c->inicio == NULL)
    {
        c->inicio = aux;
        c->fim = aux;
    }
    else
    {
        c->fim->prox = aux;
        c->fim = aux;
    }
    return 1;
}

int retira(Carrinho* c, int numero)
{
    if (estaVazia(c))
        return 0;

    Cadeira* atual = c->inicio;
    Cadeira* anterior = NULL;

    while (atual != NULL && atual->numero != numero)
    {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL)
    {
        // O elemento não foi encontrado
        return 0;
    }

    if (anterior == NULL)
    {
        // Remoção do primeiro elemento
        c->inicio = atual->prox;

        if (c->fim == atual)
        {
            // O elemento removido era o último
            c->fim = NULL;
        }
    }
    else
    {
        anterior->prox = atual->prox;

        if (c->fim == atual)
        {
            // O elemento removido era o último
            c->fim = anterior;
        }
    }

    free(atual);
    return 1;
}

int esvaziaCarrinho(Carrinho* c)
{
    if(estaVazia(c)){
        return 0;
    }
    Cadeira* atual = c->inicio;
    Cadeira* proximo;
    int contador = 0;

    while (atual != NULL) {
        proximo = atual->prox;
        free(atual);
        atual = proximo;
        contador++;
    }

    c->inicio = NULL;
    c->fim = NULL;

    return contador;
}

void mostra(Carrinho c)
{
    if (estaVazia(&c))
    {
        printf("O carrinho está vazio.\n");
        return;
    }

    Cadeira* atual = c.inicio;

    while (atual != NULL)
    {
        printf("Numero da Cadeira: %d \n", atual->numero);
        printf("Comprador: %s \n", atual->comprador);
        printf("Sessao: %d \n", atual->idSessao);
        printf("Filme: %d \n\n", atual->idFilme);
        atual = atual->prox;
    }
}

void reserva(Carrinho* c, char comprador[50])
{
    if (estaVazia(c))
    {
        printf("O carrinho está vazio.\n");
        return;
    }

    Cadeira* atual = c->inicio;

    while (atual != NULL)
    {
        strcpy(atual->comprador, comprador);
        atual = atual->prox;
    }
}

int estaVazia(Carrinho* c)
{
    return c->inicio == NULL;
}

void adicionarArquivo(Carrinho* carrinho) {
    if (estaVazia(carrinho))
    {
        printf("O carrinho está vazio.\n");
        return;
    }
    FILE* arquivo;
    switch (carrinho->inicio->idFilme)
    {
    case 1:
        arquivo = fopen("./arquivos/cadeirasSMB.bin", "ab");
        break;
    case 2:
        arquivo = fopen("./arquivos/cadeirasGDG.bin", "ab");
        break;
    case 3:
        arquivo = fopen("./arquivos/cadeirasVFX.bin", "ab");
        break;
    case 4:
        arquivo = fopen("./arquivos/cadeirasAVT.bin", "ab");
        break;
    }
    Registro registro;

    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return;
    }

    Cadeira* cadeiraAtual = carrinho->inicio;

    while (cadeiraAtual != NULL)
    {
        registro.idFilme = cadeiraAtual->idFilme;
        registro.idSessao = cadeiraAtual->idSessao;
        registro.idCadeira = cadeiraAtual->numero;
        registro.status = 1; // Status disponível
        strcpy(registro.comprador, cadeiraAtual->comprador ); // Comprador vazio
        fwrite(&registro, sizeof(Registro), 1, arquivo);
        cadeiraAtual = cadeiraAtual->prox;
    }

    printf("Sessões adicionadas com sucesso!\n");

    // Fecha o arquivo
    fclose(arquivo);
}

int lerArquivo(int idFilme, int idSessao, int numeroCadeira) {
    FILE* arquivo;
    printf("%d", idFilme);
    switch (idFilme)
    {
    case 1:
        arquivo = fopen("./arquivos/cadeirasSMB.bin", "rb");
        break;
    case 2:
        arquivo = fopen("./arquivos/cadeirasGDG.bin", "rb");
        break;
    case 3:
        arquivo = fopen("./arquivos/cadeirasVFX.bin", "rb");
        break;
    case 4:
        arquivo = fopen("./arquivos/cadeirasAVT.bin", "rb");
        break;
    }

    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return -1;
    }

    //Cadeira cadeira;
    Registro registro;

    // while (fread(&registro, sizeof(Registro), 1, arquivo) == 1) {
    //     printf("ID Filme: %d\n", registro.idFilme);
    //     printf("ID Sessão: %d\n", registro.idSessao);
    //     printf("ID Cadeira: %d\n", registro.idCadeira);
    //     printf("Status: %d\n", registro.status);
    //     printf("Comprador: %s\n", registro.comprador);
    //     printf("--------------------\n");
    // }
    while (fread(&registro, sizeof(Registro), 1, arquivo) == 1) {
        if(registro.idFilme == idFilme && registro.idSessao == idSessao && registro.idCadeira == numeroCadeira){
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}


