#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#include "execute.h"
#include "directories.h"
#include "parse.h"

#define WHITE  "\x1B[37m"
#define CYAN "\x1B[36m"


//Open file and read lines to find HOME and PATH variables. Assign them to program variables
int processProfile(char **home, char **path) {

	FILE *fp = fopen("profile", "r");
	
	//Opening profile file failed, terminates program
	if(fp == NULL){
		perror("File does not exist");
		exit(EXIT_FAILURE);
		return 0;
	}

	//Opening profile file succeeded
	char *str = malloc(sizeof(char) * 100);

	int hasPath = 0;
	int hasHome = 0;

	//Parse file for PATH and HOME and assign the variables
	while( fgets(str, 100, fp) != NULL) {
		if (strstr(str,"PATH")) {
			hasPath = 1;
			*path =  assignVariable(str, "=");
		} 	
		if (strstr(str,"HOME")) {
			hasHome = 1;
			*home =  assignVariable(str, "=");
		}
	}

	//Checking that both HOME and PATH exist
	if (hasPath == 0 || hasHome == 0) {
		perror("PATH or HOME variable does not exist");
		return 0;
	}

	fclose(fp);
	return 1;
}

//Ask the user for input
void promptUser() {
	char cwd[100];
	printf(CYAN "%s > ",getcwd(cwd, 100)); 
	printf(WHITE "");
}

//Creating a child process using fork(), and running the command with execv()
int forkProcess(char* directory, char* prog, char* params[]){
	int pid;
	//Fork a new process
	if ((pid=fork()) >= 0) {
		//If we are in the child process
		if (pid == 0) {
			char *torun = strcat(strcat(directory,"/"), prog);
			execv(torun, params);
			exit(1);
		}
		//If we are in the parent process, wait for child process to return 
		else {
			wait(&pid);
		}
	} else {
		perror("Fork failed");
		return 0;
	}
	return 1;	
}

//Using chdir() to change directories in the parent process
int changeDirectories(char* home, char* params[]) {
	if (params[1] == NULL) {
		int r = chdir(home);
		if(r==-1){
			perror("Could not change to home directory");
			return 0;
		}
	} else {
		chdir(params[1]);
	}
	return 1;
}

void executeShell() {
	getCommand(&command);

	if(command == NULL) {
		return;
	}

	getParameters(params, &command);

	//Iterate through directories to find command

	directory = findInDirectories(root, command);

	//If the command does not exist in a directory
	if (directory == NULL) {
		printf("Command not found\n");	
		return;
	}
	//If the command exists in directory 
	else {
			
		//If changing directories
		if (isCommand(command, "cd")) {
			changeDirectories(HOME, params);
		}

		//If reassigning environment variables
		else if (isCommand(command, "$HOME")) {
			HOME = assignVariable(params[0], "=");
		} else if(isCommand(command, "$PATH")) {
			PATH = assignVariable(params[0], "=");
		}

		//Otherwise, fork the process
		else {
			forkProcess(directory, command, params);
		}
	}
	return;			
}


