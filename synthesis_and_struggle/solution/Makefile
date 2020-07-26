PROJECT=solution

CC=g++
LL=g++

FILES=infection.cpp

CFLAG = -std=c++17 
CFLAG+= -Wall 
CFLAG+= -O3
CFLAG+= -g

all:$(PROJECT)

$(PROJECT):$(FILE)
	$(CC) $(CFLAG) $(FILES) -o $(PROJECT)

start:$(PROJECT)
	./$(PROJECT)
	python script.py
clean:
	rm -f $(PROJECT)
