CC=gcc
CFLAGS=-g -ggdb3 -std=c11 -Wall -pedantic -lpthread -ldl -lm
BIN=sqlitec
OBJ=sqlite3.o 

all: sqlite3.o
	${CC} ${CFLAGS} src/protCadastro.c src/sqlite_utils.c sqlite3.o -o ${BIN}

sqlite3.o:
	${CC} ${CFLAGS} -c src/sqlite3.c -o $@

format:
	clang-format -i --verbose src/protCadastro.c
	
clean: 
	${RM} -r ${OBJ}
	${RM} ${BIN} *.db
	echo "Great Success"