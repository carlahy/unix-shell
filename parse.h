int getDir(char** result, char* string, char* delim);

int getNextDir(char** result, char* delim);

void getCommand(char** command);

void getParameters(char* params[], char** command);

int isCommand(char* string, char* cmd);

char* assignVariable(char* value, char* DELIM);
