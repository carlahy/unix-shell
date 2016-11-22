#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include "directories.h"
#include "parse.h"

/*Contains the logic for a singly linked list to store the directories so they are easily iterable, 
check whether a command belongs to a directory,
find what directory a command belongs to.
*/

//Create a new directory node in the list
NODE* newDir(char* directory, NODE *next) {
	NODE* new = malloc(sizeof(NODE));
	new->dir = directory;
	new->next = next;
	return new;
}

//Append a directory to the list
NODE* addDirToList(NODE* head, NODE* new) {
	NODE* current = head;
	while(current->next != NULL) {
		current = current->next;
	}
	current->next = new;
	return head;
}

//Return a list with all directories from PATH
NODE* processDir(char** directory, char* path) {
	getDir(directory, path, ":");
	root = newDir(*directory,NULL);

	while( getNextDir(directory, ":") ){
		addDirToList(root, newDir(*directory,NULL) );		
	}
	return root;
}

//Check if program belongs to directory using opendir() and readdir()
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

//Iterate through list of directories to find command
char* findInDirectories(NODE* root, char* command) {
	while(root != NULL) {
		if (isInDirectory(command,root->dir) == 1) {
			return root->dir;
		}
		root = root->next;
	}
	return NULL;
}
