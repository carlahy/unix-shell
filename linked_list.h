typedef struct node {
	char *dir;
	struct node *next;
} NODE ;

NODE *root;

NODE* newNode(char* directory, NODE *next);

NODE* addNodeToEnd(NODE* head, NODE* new);
