
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
			return 0;
		}
	}
	fclose(fp);
	return 0;
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

int forkProcess(char* directory, char* prog, char* params[]){
	int pid;
	if ((pid=fork()) >= 0) {
		if (pid == 0) {
			char *torun = strcat(strcat(directory,"/"), prog);
			execv(torun, params);
			exit(1);
		} else {
			wait(&pid);
		}
	} else {
		perror("Fork failed");
		return -1;
	}
	return 0;	
}

int changeDirectories(char* home, char* params[]) {
	if (params[1] == NULL) {
		int r = chdir(home);
		if(r==-1){
			perror("Could not change to home directory");
			return -1;
		}
	} else {
		chdir(params[1]);
	}
	return 0;
}

