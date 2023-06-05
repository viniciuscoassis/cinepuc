#ifndef SQLITE_UTILS_H
#define SQLITE_UTILS_H

#include "sqlite3.h"

// Função para inicializar o SQLite
void initialize_sqlite();

// Função para fechar o banco de dados
void close_sqlite();

// Função para executar uma consulta SQL
void execute_query(const char* query);


#endif /* SQLITE_UTILS_H */
