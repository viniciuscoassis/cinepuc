#ifndef ESTRUTURACINEMA_H
#define ESTRUTURACINEMA_H

typedef struct {
    int idSessao;
    int idCadeira;
    int status;
    char comprador[50];
} Registro;


Registro* obterSessoes(char* nomeArquivo, int* quantidadeSessoes);
void adicionarSessoes(char* nomeArquivo);
int cadeiraOcupada(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira);
char* obterNomeComprador(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira);
void reservarCadeira(Registro* sessoes, int quantidadeSessoes, int idSessao, int idCadeira);
void exibirSessoes(Registro* sessoes, int quantidadeSessoes);

#endif  // ESTRUTURACINEMA_H
