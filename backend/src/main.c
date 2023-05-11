#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

int criaSala(char nome);

int main(){
    int sucesso;
    char nome = "Sala 1";

    /*sucesso = criaSala(nome);

    if(sucesso != 0){
        printf("Error");
    }
    else {
        printf("Success");
    }*/
}

int criaSala(char nome){
    sqlite3 *db;
    char *msg_error = 0;

    int rc = sqlite3_open("cinema.db", &db);

    if( rc != SQLITE_OK){
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
    sprintf("INSERT INTO Sala (nome) VALUES ('%s')", nome);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao inserir nova sala: %s\n", sqlite3_errmsg(db));
        sqlite3_free(msg_error);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
