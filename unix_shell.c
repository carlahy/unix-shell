#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "unix_shell.h"

#define WHITE  "\x1B[37m"
#define CYAN "\x1B[36m"

struct node {
	char *dir;
	struct node *next;
};

int main(int argc, char *args[]) {

	char cwd[100];
	
	char *HOME;
	char *PATH;

	//Initialise HOME and PATH variables

	if ( getEnvVariables("profile", &HOME, &PATH) == -1) {
		perror("Terminating");
		return -1;
	}


	int cmdlen = 100;
	char *command = malloc(sizeof(char) * cmdlen);

	//Store directories in linked list

	char *directory = strtok(PATH, ":");
	struct node *root = malloc(sizeof(struct node));
	root->dir = directory;
	root->next = NULL;
	struct node *conductor = root;

	while( (directory = strtok(NULL, ":")) != NULL ) {
		conductor->next = malloc(sizeof(struct node));
		conductor = conductor->next;
		conductor->dir = directory;
		conductor->next = NULL;
	}

	//Start user prompt

	while(1) {

		//Get command from prompt

		printf(CYAN "%s > ",getcwd(cwd, 100)); printf(WHITE "");
		fgets(command, cmdlen, stdin);
		command = strtok(command, "\n");

		//Parse command

		char *params[10];
		char *arg = strtok(command, " ");
		int c = 0;  

		while (arg != NULL) {
			params[c++] = arg;
			arg = strtok(NULL, " ");
		}
		params[c] = NULL;
	
		//Check directories for command

		conductor = root;
		directory = NULL;
		int cmdFound = -1;
			
		while(conductor != NULL) {	
			if (isInDirectory(params[0],conductor->dir) == 1) {
				cmdFound = 1;
				directory = conductor->dir;
				break;
			}
			conductor = conductor->next;
		}

		//Change directories
		
		if (strcmp(params[0], "cd") == 0) {
			changeDirectories(HOME, params);
			continue;
		}

		//Assignment to profile variables

		if (strstr(params[0], "$HOME") != NULL) {
			//HOME = strdup( strtok(&prog[5], "\n"));
			//printf("New home is %s", HOME);
			//char *assignment =  strdup( strtok(&prog[5], "\n"));
			//setEnvVariables("profile", &HOME, assignment);
		} else if(strstr(params[0], "$PATH") != NULL) {
			//PATH = strdup(prog[5], "\n"); 
			//printf("New path is %s", PATH);
			//char *assignment = ;
			//setEnvVariables(PATH, );
		}

		//Fork the process

		if(cmdFound == 1) {
			forkProcess(directory, params[0], params);
		} else {
			printf("Command not found\n");
		}
	}
	return 0;

}
