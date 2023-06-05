CC=gcc
CFLAGS=-g -ggdb3 -std=c11 -Wall -pedantic -lpthread -ldl -lm
BIN=sqlitec
OBJ=sqllite3.o 

all: sqlite3.o
	${CC} ${CFLAGS} src/main.c sqlite3.o -o sqlitec

sqlite3.o:
	${CC} ${CFLAGS} -c src/sqlite3.c -o $@

format:
	clang-format -i --verbose src/main.c
	
clean: 
	${RM} -r ${OBJ}
	${RM} ${BIN} *.db
	echo "Great Success"