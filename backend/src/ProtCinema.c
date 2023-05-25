#include <stdio.h>
#include <stdlib.h>

typedef struct cadeira
{
    int id;
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
int insere(Carrinho* c, int id, int numero);
int retira(Carrinho* c, int id);
void reserva(Carrinho* c, char comprador[50]);
int estaVazia(Carrinho c);
void mostra(Carrinho c);

int main()
{
    Carrinho c;
    int id, numero, sucesso;
    char comprador[50];

    cria(&c);

    sucesso = insere(&c, 1, 16);
    sucesso = insere(&c, 2, 17);
    sucesso = insere(&c, 3, 18);
    sucesso = insere(&c, 4, 19);
    sucesso = insere(&c, 5, 20);

    if (!estaVazia(c))    // exibindo a lista
    {
        printf("\n-------------------------------------------------\n");
        printf("Carrinho: \n");
        mostra(c);
        printf("\n");
    }

    strcpy(comprador, "Felipe");

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

int insere(Carrinho* c, int id, int numero)
{
    Cadeira* aux = (Cadeira*)malloc(sizeof(Cadeira));
    if (aux == NULL)
        return 0;

    aux->id = id;
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

int retira(Carrinho* c, int id)
{
    if (estaVazia(*c))
        return 0;

    Cadeira* atual = c->inicio;
    Cadeira* anterior = NULL;

    while (atual != NULL && atual->id != id)
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
        printf("CadeiraID: %d \n", atual->id);
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
