CC = gcc
FILES = unix_shell.c
OUT_EXE = output

build: $(FILES) 
	$(CC) -o $(OUT_EXE) $(FILES)

clean:
	rm -f *.o core

rebuild: clean build
