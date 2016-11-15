typedef struct node {
	char *dir;
	struct node *next;
} NODE ;

NODE *root;

NODE* newDir(char* directory, NODE *next);

NODE* addDirToList(NODE* head, NODE* new);

NODE* processDir(char** directory, char* path);

char* findInDirectories(NODE* root, char* command);

int isInDirectory(char *prog, char *dir);

