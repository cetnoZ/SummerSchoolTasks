PROJECT=solution

CC=g++
LL=g++

FILES?=solution.cpp

CFLAG = -std=c++17 
CFLAG+= -O3

all:
	@rm -f $(PROJECT)
	@make $(PROJECT)

$(PROJECT):$(FILE)
	@$(CC) $(CFLAG) $(FILES) -o $(PROJECT)

start:$(PROJECT)
	./$(PROJECT)

clean:
	rm -f $(PROJECT)
