#ifndef ESTRUTURACINEMA_H
#define ESTRUTURACINEMA_H

typedef struct {
    int idFilme;
    int idSessao;
    int idCadeira;
    int status;
    char comprador[50];
} Registro;

typedef struct {
    int idSessao;
    int idCadeira;
} Assento;

typedef struct {
    Assento* assentos;
    int quantidadeAssentos;
    int capacidade;
} ListaAssentos;


Registro* obterSessoes(char* nomeArquivo, int* quantidadeSessoes);
void adicionarSessoes(char* nomeArquivo);
int cadeiraOcupada(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira);
char* obterNomeComprador(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira);
void reservarCadeira(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira, char* nomeArquivo);
void exibirSessoes(Registro* sessoes, int quantidadeSessoes);

ListaAssentos* criarListaAssentos(int capacidade);
void destruirListaAssentos(ListaAssentos* lista);
void adicionarAssento(ListaAssentos* lista, int idSessao, int idCadeira);
void removerAssento(ListaAssentos* lista, int idSessao, int idCadeira);
void finalizarPedido(ListaAssentos* lista, Registro* sessoes, int quantidadeSessoes, char* nomeArquivo);
void exibirListaAssentos(ListaAssentos* lista);

/*-------------------------------------------------*/
int buscaCadeira(int idFilme, int idSessao, int numero);
void reservaCarrinho(Carrinho* carrinho, char comprador[50]);

#endif  // ESTRUTURACINEMA_H
