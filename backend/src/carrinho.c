/*
  Programa: Reserva de Cadeiras
  Descrição: Simula a reserva de cadeiras dentro de uma sala.
*/

//Terminal na pasta Backend
//gcc src/carrinho.c lib/funcoesCarrinho.c -I include -o carrinho
//./carrinho

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/carrinho.h"

int main()
{
    Carrinho c;
    int idFilme, idSessao, numero, sucesso;
    char comprador[50];

    cria(&c);

   insere(&c, 1, 1, 52);
   insere(&c, 2, 2, 34);
   insere(&c, 3, 3, 22);
   insere(&c, 4, 4, 11);

    if (!estaVazia(c))    // exibindo a lista
    {
        printf("\n-------------------------------------------------\n");
        printf("Carrinho: \n");
        mostra(c);
        printf("\n");
    }

    strcpy(comprador, "Felipe");

    reserva(&c, comprador);

    // 1,1,1,0,NULL
    // 1,1,2,0,NULL

    mostra(c);

    return 0;
}