// Estrutura Cadeira
typedef struct cadeira
{
    int idFilme; // 1 a 4
    int idSessao; // 1 a 4      
    int numero; // 1 a 64
    int status; // Disponivel (0), ocupado (1) ou selecionado (2)
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
int retira(Carrinho* c, int numero);
void reserva(Carrinho* c, char comprador[50]);
int estaVazia(Carrinho* c);
void mostra(Carrinho c);
int esvaziaCarrinho(Carrinho* c);