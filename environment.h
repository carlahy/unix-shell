int cmdlen;
char *command;	
char *HOME;
char *PATH;
char *directory;

int getEnvVariables(char *filename, char **home, char **path);

int initialiseEnvVariables(char *profile, char** HOME, char** PATH);

char* reassignVariable(char* value, char* DELIM);

void promptUser();

int isCommand(char* string, char* cmd);

int isInDirectory(char *prog, char *dir);

int forkProcess(char* directory, char* prog, char* params[]);

int changeDirectories(char* home, char* params[]);
