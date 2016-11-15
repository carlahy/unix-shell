#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parsing.h"

int cmdlen = 100;


//Parse directory from environment variables
int getDir(char** result, char* string, char* delim) {
	*result = strtok(string,delim);
	if (*result != NULL) return 1;
	return 0;
}

//Continue parsing directories from environment variables
int getNextDir(char** result, char* delim) {
	*result = strtok(NULL, delim);
	if (*result != NULL) return 1;
	return 0;
}


//Retrieve input from prompt
void getCommand(char** command) {
	fgets(*command, cmdlen, stdin);
	*command = strtok(*command, "\n");
}

//Parse input into command and parameters
void parseCommand(char* params[], char** command) {
	char *arg = strtok(*command, " ");
	int c = 0;
	while (arg != NULL) {
		params[c++] = arg;
		arg = strtok(NULL, " ");
	}
	*command = params[0];
	params[c] = NULL;
}

