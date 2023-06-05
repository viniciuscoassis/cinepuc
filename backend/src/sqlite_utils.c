#include "sqlite_utils.h"
#include <stdio.h>
#include "sqlite3.h"

sqlite3 *db;


void initialize_sqlite() {
    int rc = sqlite3_open("database.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(db));
    }
}

void execute_query(const char* query) {
    char *errMsg;
    int rc = sqlite3_exec(db, query, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao executar a consulta: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
}

void close_sqlite() {
    int rc = sqlite3_close(db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao fechar o banco de dados: %s\n", sqlite3_errmsg(db));
    }
}

