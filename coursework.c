#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define WHITE  "\x1B[37m"
#define CYAN "\x1B[36m"

struct node {
	char *dir;
	struct node *next;
};

int getEnvVariables(char *filename, char **home, char **path) {

	FILE *fp = fopen(filename, "r");
	
	if(fp == NULL){
		perror("File does not exist");
		return -1;
	}
	int stringlen = 100;
	char *str = malloc(sizeof(char) * stringlen);

	fp = fopen(filename, "r+");

	int hasPath = 0;
	int hasHome = 0;

	//Parse file for PATH and HOME
	while( fgets(str, stringlen, fp) != NULL) {
		if (strstr(str,"PATH")) {
			hasPath = 1;
			*path = strdup( strtok(&str[strcspn(str,"=")+1], "\n") );
		} 	
		if (strstr(str,"HOME")) {
			hasHome = 1;
			*home = strdup( strtok(&str[strcspn(str,"=")+1], "\n") );
		}
	}

	if (hasPath == 0 || hasHome == 0) {
		perror("PATH or HOME variable does not exist");
		return -1;
	}

	fclose(fp);
	return 0;
}

int setEnvVariables(char *filename, char **var, char *assignment) {
	FILE *fp = fopen(filename, "r");

	if(fp == NULL){
		perror("File does not exist");
		return -1;
	}

	int stringlen = 100;
	char *str = malloc(sizeof(char) * stringlen);

	fp = fopen(filename, "r+");

	//Parse file for "var" (PATH or HOME)
	while( fgets(str, stringlen, fp) != NULL) {
		if (strstr(str, *var)) {
			*var = strdup( strtok(&str[5], "\n") );
			fwrite(assignment, 1, sizeof(assignment), fp);
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	return 0;
}

int isInDirectory(char *prog, char *dir) {
	DIR *opndir = opendir(dir);
	struct dirent *rdentry = readdir(opndir);

	while(rdentry) {
		if(strcmp(rdentry->d_name, prog) == 0) {
			return 1;
		}
		rdentry = readdir(opndir);
	}
	
	return 0;
}

int forkProcess(char* directory, char* prog, char* params[]){
	int pid;
	if ((pid=fork()) >= 0) {
		if (pid == 0) {
			char *torun = strcat(strcat(directory,"/"), prog);
			execv(torun, params);
			exit(1);
		} else {
			wait(&pid);
		}
	} else {
		perror("Fork failed");
		return -1;
	}
	return 0;	
}

int changeDirectories(char* home, char* params[]) {
	if (params[1] == NULL) {
		int r = chdir(home);
		if(r==-1){
			perror("Could not change to home directory");
			return -1;
		}
	} else {
		chdir(params[1]);
	}
	return 0;
}

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
