/*
  Programa: Reserva de Cadeiras
  Descrição: Simula a reserva de cadeiras dentro de uma sala.
*/


#include <stdio.h>
#include <stdlib.h>

// Estrutura Cadeira
typedef struct cadeira
{
    int idFilme; // 1 a 4
    int idSessao; // 1 a 4      
    int numero; // 1 a 64
    char comprador[50];
    struct cadeira* prox;
} Cadeira;

// Estrura Carrinho de Compras
typedef struct
{
    Cadeira* inicio;
    Cadeira* fim;
} Carrinho;

void cria(Carrinho* c);
int insere(Carrinho* c, int idFilme, int idSessao, int numero);
int retira(Carrinho* c, int id);
void reserva(Carrinho* c, char comprador[50]);
int estaVazia(Carrinho c);
void mostra(Carrinho c);

int main()
{
    Carrinho c;
    int idFilme, idSessao, numero, sucesso;
    char comprador[50];

    cria(&c);

   insere(&c, idFilme, idSessao, numero);

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
