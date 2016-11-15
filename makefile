CC = gcc
FILES = main.c environment.c linked_list.c parsing.c
OUT_EXE = output

build: $(FILES) 
	$(CC) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o core

rebuild: clean build
