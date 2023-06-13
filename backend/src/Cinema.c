#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int verificaExistencia(Carrinho c, int id, int numero);

int main()
{
    Carrinho c;
    int id, numero, sucesso;
    char comprador[50];
    int opcao;

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

    do
    {
        printf("Digite a opcao desejada:\n");
        printf("1 - Adicionar cadeira\n");
        printf("2 - Remover cadeiras existentes\n");
        printf("3 - Encerrar programa\n");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            printf("Digite o ID da cadeira: ");
            scanf("%d", &id);
            printf("\nDigite o numero da cadeira: ");
            scanf("%d", &numero);

            if (verificaExistencia(c, id, numero))
            {
                printf("ID ou numero de cadeira ja existem.\n\n");
                printf("Escolha uma opcao:\n");
                printf("1 - Adicionar uma nova cadeira\n");
                printf("2 - Remover a cadeira existente\n");
                scanf("%d", &opcao);

                if (opcao == 1)
                {
                    printf("Digite o novo ID da cadeira: ");
                    scanf("%d", &id);
                    printf("Digite o novo n�mero da cadeira: ");
                    scanf("%d", &numero);
                    sucesso = insere(&c, id, numero);
                }
                else if (opcao == 2)
                {
                    sucesso = retira(&c, id);
                }
                else
                {
                    printf("Op��o inv�lida.\n");
                }
            }
            else
            {
                sucesso = insere(&c, id, numero);
            }
        }
        else if (opcao == 2)
        {
            printf("Digite o ID da cadeira a ser removida: ");
            scanf("%d", &id);
            sucesso = retira(&c, id);
        }
        else if (opcao == 3)
        {
            printf("Encerrando o programa...\n");
            break;
        }
        else
        {
            printf("Op��o inv�lida.\n");
        }

        if (!estaVazia(c))    // exibindo a lista atualizada
        {
            printf("\n-------------------------------------------------\n");
            printf("Carrinho atualizado: \n");
            mostra(c);
            printf("\n");
        }
    } while (1);

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
        // O elemento n�o foi encontrado
        return 0;
    }

    if (anterior == NULL)
    {
        // Remo��o do primeiro elemento
        c->inicio = atual->prox;

        if (c->fim == atual)
        {
            // O elemento removido era o �ltimo
            c->fim = NULL;
        }
    }
    else
    {
        anterior->prox = atual->prox;

        if (c->fim == atual)
        {
            // O elemento removido era o �ltimo
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
        printf("O carrinho est� vazio.\n");
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
        printf("O carrinho est� vazio.\n");
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

int verificaExistencia(Carrinho c, int id, int numero)
{
    Cadeira* atual = c.inicio;

    while (atual != NULL)
    {
        if (atual->id == id || atual->numero == numero)
            return 1;
        atual = atual->prox;
    }

    return 0;
}

