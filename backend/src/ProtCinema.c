/*
  Programa: Reserva de Cadeiras
  Descrição: Simula a reserva de cadeiras dentro de uma sala.
*/


#include <stdio.h>
#include <stdlib.h>

typedef struct cadeira
{
    int idFilme;
    int idSessao;
    int numero;
    char comprador[50];
    struct cadeira* prox;
} Cadeira;

typedef struct
{
    Cadeira* inicio;
    Cadeira* fim;
    int tam;
} Carrinho;

void cria(Carrinho* c);
int insere(Carrinho* c, int idFilme, int idSessao, int numero);
int retira(Carrinho* c, int numero);
void reserva(Carrinho* c, char comprador[50]);
int estaVazia(Carrinho c);
void mostra(Carrinho c);

int main()
{
    Carrinho c;
    int idFilme, idSessao, numero, sucesso;
    char comprador[50];

    idFilme = 1;
    idSessao = 1;

    cria(&c);

    sucesso = insere(&c, idFilme, idSessao, 1);
    sucesso = insere(&c, idFilme, idSessao, 2);
    sucesso = insere(&c, idFilme, idSessao, 3);

    if (!estaVazia(c))    // exibindo a lista
    {
        printf("\n-------------------------------------------------\n");
        printf("Carrinho: \n");
        mostra(c);
        printf("\n");
    }

    strcpy(comprador, "felipemareca@gmail.com");

    reserva(&c, comprador);

    mostra(c);

    return 0;
}

void cria(Carrinho* c)
{
    c->inicio = NULL;
    c->fim = NULL;
    c->tam = 0;
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

    c->tam++;
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
    c->tam--;
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
