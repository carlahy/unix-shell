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
			return 1;
		}
	}
	fclose(fp);
	return 1;
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

int main(int argc, char *args[]) {

	char cwd[100];
	int pid;
	
	char *HOME;
	char *PATH;

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

		printf(CYAN "%s > ",getcwd(cwd, 100));
		printf(WHITE "");
		fgets(command, cmdlen, stdin);
		command = strtok(command, "\n");

		//Parse command
		char *params[10];
		char *arg = strtok(command, " ");
		char *prog = arg;
		params[0] = prog;

		//Get arguments 
		int c = 1;  
		arg = strtok(NULL, " ");
	
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
			if (isInDirectory(prog,conductor->dir) == 1) {
				cmdFound = 1;
				directory = conductor->dir;
				break;
			}
			conductor = conductor->next;
		}

		//Change directories
		
		if (strcmp(prog, "cd") == 0) {
			if (params[1] == NULL) {
				int r = chdir(HOME);
				if(r==-1){
					perror("Could not change to home directory");
				}
			} else {
				chdir(params[1]);
			}
			continue;
		}

		//Assign to profile variables

		if (strstr(prog, "$HOME") != NULL) {
			//HOME = strdup( strtok(&prog[5], "\n"));
			printf("New home is %s", HOME);
			char *assignment =  strdup( strtok(&prog[5], "\n"));
			setEnvVariables("profile", &HOME, assignment);
		} else if(strstr(prog, "$PATH") != NULL) {
		//	PATH = strdup(prog[5], "\n"); 
		//	printf("New path is %s", PATH);
			//char *assignment = ;
			//setEnvVariables(PATH, );
		}

		//Fork the process

		if(cmdFound == 1) {
			if ((pid=fork()) >= 0) {
				if (pid == 0) {
					//printf("##########Child process, ");
					char *torun = strcat(strcat(directory,"/"), prog);
					//printf("running process: %s with args %s\n", torun, params[1]);
					execv(torun, params);//&argvs[0]);
					exit(1);
				} else {
					wait(&pid);
					//printf("##########Parent process\n");
				}
			} else {
				perror("Fork failed");
			}	
		} else {
			printf("Command not found\n");
		}
	}
	return 0;

}
