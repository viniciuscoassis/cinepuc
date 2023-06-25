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

//Estrutura para registrar no arquivo
typedef struct {
    int idFilme;
    int idSessao;
    int idCadeira;
    int status;
    char comprador[50];
} Registro;

void cria(Carrinho* c); // Cria lista para ser o carrinho de compras
int insere(Carrinho* c, int idFilme, int idSessao, int numero, char* comprador); // função para inserir no carrinho
int retira(Carrinho* c, int numero); // função para retirar item do carrinho
void reserva(Carrinho* c, char comprador[50]); // função para efetivar uma reserva percorrendo a lista
int estaVazia(Carrinho* c); // verificação se a lista esta vazia
void mostra(Carrinho c); // mostra todos os itens do carrinho
int esvaziaCarrinho(Carrinho* c); // esvazia o carrinho antes de fechar o programa
void adicionarArquivo(Carrinho* carrinho); // adiciona as reservas no arquivo de reservas
int lerArquivo(int idFilme, int idSessao, int numeroCadeira); // le o arquivo de reservas