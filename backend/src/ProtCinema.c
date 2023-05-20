#include <stdio.h>
#include <stdlib.h>

/*-------------------------------------------------
Organização prototipo do projeto Cinema contendo:
- Estrutura base para reserva de cadeiras (Lista);
- Função para selecionar uma cadeira e adicionar/ retirar na lista(Carrinho);
- Reservar as Cadeiras selecionadas (Alterar o registro da lista)
-------------------------------------------------*/

typedef struct cadeira
{
    int id;
    int numero;
    char comprador;
    struct no* prox;
} Cadeira;

typedef struct
{
    Cadeira* inicio;
    Cadeira* fim;
    int tam;
} Carrinho;

// Funções de manipulação
void cria(Carrinho* c);
int insere(Carrinho* c, int id, int numero);
int retira(Carrinho* c, int id);
int reserva(Carrinho* c, char comprador);
int estaVazia(Carrinho c);

//Funções de visualização
void mostra(Carrinho c);

int main()
{
    Carrinho c;
    int id, numero, sucesso;
    char comprador;

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

    comprador = "Felipe";

    reserva(c, comprador);
}

void cria(Carrinho* c)
{
    c->inicio = NULL;
    c->fim = NULL;
    c->tam = 0;
}

int insere(Carrinho* l, int id, int numero)
{
    Cadeira* aux = (Cadeira*)malloc(sizeof(Cadeira));
    if (aux == NULL)
        return 0;

    aux->id = id;
    aux->numero = numero;
    aux->prox = NULL;

    if (l->inicio == NULL)
    {
        l->inicio = aux;
        l->fim = aux;
    }
    else
    {
        l->fim->prox = aux;
        l->fim = aux;
    }

    l->tam++;
    return 1;
}

int retira(Carrinho* c, int id) {
    if (estaVazia(*c))
        return 0;

    Cadeira* atual = c->inicio;
    Cadeira* anterior = NULL;

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        // O elemento não foi encontrado
        return 0;
    }

    if (anterior == NULL) {
        // Remoção do primeiro elemento
        c->inicio = atual->prox;

        if (c->fim == atual) {
            // O elemento removido era o último
            c->fim = NULL;
        }
    } else {
        anterior->prox = atual->prox;

        if (c->fim == atual) {
            // O elemento removido era o último
            c->fim = anterior;
        }
    }

    free(atual);
    c->tam--;
    return 1;
}

void mostra(Carrinho c) {
    if (estaVazia(c)) {
        printf("O carrinho está vazio.\n");
        return;
    }

    Cadeira* atual = c.inicio;

    while (atual != NULL) {
        printf("CadeiraID: %d \n", atual->id);
        printf("Numero da Cadeira: %d \n", atual->numero);
        printf("Comprador: %c \n", atual->comprador);
        atual = atual->prox;
    }
}

void reserva(Carrinho c, char comprador) {
    if (estaVazia(c)) {
        printf("O carrinho está vazio.\n");
        return;
    }

    Cadeira* atual = c.inicio;

    while (atual != NULL) {
        atual->comprador = comprador;
        atual = atual->prox;
    }
}

int estaVazia(Carrinho c) {
    return c.inicio == NULL;
}

