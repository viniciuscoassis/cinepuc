#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/carrinho.h"

void cria(Carrinho* c)
{
    c->inicio = NULL;
    c->fim = NULL;
}

int insere(Carrinho* c, int idFilme, int idSessao, int numero)
{
    Cadeira* aux = (Cadeira*)malloc(sizeof(Cadeira));
    if (aux == NULL)
        return 0;

    aux->idFilme = idFilme;
    aux->idSessao = idSessao;
    aux->numero = numero;
    strcpy(aux->comprador, "");
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
    if (estaVazia(*c))
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

void mostra(Carrinho c)
{
    if (estaVazia(c))
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
    if (estaVazia(*c))
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

int estaVazia(Carrinho c)
{
    return c.inicio == NULL;
}