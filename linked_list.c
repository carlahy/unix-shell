#include <stdio.h>
#include <stdlib.h>

#include "linked_list.h"

NODE* newNode(char* directory, NODE *next) {
	NODE* new = malloc(sizeof(NODE));
	new->dir = directory;
	new->next = next;
	return new;
}

NODE* addNodeToEnd(NODE* head, NODE* new) {
	NODE* current = head;
	while(current->next != NULL) {
		current = current->next;
	}
	current->next = new;
	return head;
}
