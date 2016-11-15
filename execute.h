int cmdlen;
char *command;	
char *HOME;
char *PATH;
char *directory;
char *params[10];

int processProfile(char **home, char **path);

void promptUser();

int forkProcess(char* directory, char* prog, char* params[]);

int changeDirectories(char* home, char* params[]);

void executeShell();
