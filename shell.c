#include <stdio.h>
#include <stdlib.h>
#include "execute.h"
#include "directories.h"
#include "parse.h"

/*The main function makes high level calls to the other source files that handle the shell functionalities.*/

int main(int argc, char *args[]) {

	//Read the HOME and PATH variables from profile file
	processProfile(&HOME, &PATH);

	//Store directories in list with root as the head
	root = processDir(&directory, PATH);

	//Start user prompt

	while(1) {
		//Assign memory to the command 
		command = malloc(sizeof(char) * cmdlen);

		//Get input from stdin and parse into command and parameters
		promptUser();

		//Run the command in the shell
		executeShell();
		
		//Free command memory to avoid segmentation faults
		free(command);
	}
	return 0;
}
