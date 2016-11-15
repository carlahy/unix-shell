#include <stdio.h>
#include <stdlib.h>

#include "environment.h"
#include "linked_list.h"
#include "parsing.h"

int main(int argc, char *args[]) {

	command = malloc(sizeof(char) * cmdlen);

	initialiseEnvVariables("profile",&HOME, &PATH);

	//Store directories in linked list

	getDir(&directory, PATH, ":");
	root = newNode(directory,NULL);
	
	//Add all directories to list

	while( getNextDir(&directory, ":") ){
		addNodeToEnd(root, newNode(directory,NULL) );		
	}

	//Start user prompt

	while(1) {

		promptUser();
		//Get command from prompt and parse
		getCommand(&command);

		char *params[10];
		parseCommand(params, &command);

		//Check directories for command

		NODE *dirs = root;
		int cmdFound = -1;

		//Iterate through list until command is found
			
		while(dirs != NULL) {
			if (isInDirectory(command,dirs->dir) == 1) {
				cmdFound = 1;
				directory = dirs->dir;
				break;
			}
			dirs = dirs->next;
		}

		//Change directories
		
		if (isCommand(command, "cd")) {
			changeDirectories(HOME, params);
			continue;
		}
		else if (isCommand(command, "$HOME")) {
			HOME = reassignVariable(params[0], "=");
			continue;
		} else if(isCommand(command, "$PATH")) {
			PATH = reassignVariable(params[0], "=");
			continue;
		}

		//Fork the process

		if(cmdFound == 1) {
			forkProcess(directory, command, params);
		} else {
			printf("Command not found\n");
		}
	}
	return 0;
}
