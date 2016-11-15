CC = gcc
FILES = main.c execute.c directories.c parse.c
OUT_EXE = output

build: $(FILES) 
	$(CC) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o core

rebuild: clean build
