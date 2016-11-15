#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"

/*
Contains function to parse the directories and the user input, identify commands and identify what directory the command is in. 
The directories are parsed based on the PATH profile variable. 
The user input is parsed as a command followed by one or multiple parameters.
*/

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
void getParameters(char* params[], char** command) {
	char *arg = strtok(*command, " ");
	int c = 0;
	while (arg != NULL) {
		params[c++] = arg;
		arg = strtok(NULL, " ");
	}
	*command = params[0];
	params[c] = NULL;
}

//Check is stdin command is equal to cmd
int isCommand(char* string, char* cmd) {
	if (strstr(string, cmd)) {
		return 1;
	}
	return 0;
}

//Assign value to a variable after a delimeter
char* assignVariable(char* value, char* DELIM) {
	return strdup( strtok(&value[strcspn(value,DELIM)+1], "\n"));
}


