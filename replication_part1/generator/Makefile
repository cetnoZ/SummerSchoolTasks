PROJECT=generator
CC=g++
LL=g++

FILE=repl1_gen.cpp

CFLAG=-std=c++17 -Wall -O3 -g

all:$(PROJECT)

$(PROJECT):$(FILE)
	$(CC) $(CFLAG) $(FILE) -o $(PROJECT)

start:$(PROJECT)
	./$(PROJECT)
