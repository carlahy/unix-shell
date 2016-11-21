CC = gcc
FILES = shell.c execute.c directories.c parse.c
OUT_EXE = shell

build: $(FILES) 
	$(CC) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o core

rebuild: clean build
