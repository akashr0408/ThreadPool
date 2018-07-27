CC=gcc
FLAGS=-pthread -Wall

threadmake: threadUse.c
	$(CC) $(FLAGS) -o thread threadUse.c
