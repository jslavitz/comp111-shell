#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

//shell prompt
#define PROMPT "$> "


/*
 * THINGS LEFT TODO
 * - run in background &
 * - implement all native commands
 * - keep track of environ somehow
 * - I/O redirection
 * - Error reporting
 * - WAIT FOR RUNNING PROCESSES BEFORE EXIT
 * TODO FREE CWD/PATH ARRAY
 */

/* TODO...
void runInBackground(char*[] args);
*/


//interactive mode
void interactive(char *path);

//batch mode
void batch(const char *fname, char *path);

//handle line
//returns the path we have navigated to
char* handle_line(char *buf, ssize_t len, char *path);


//commands
void clear();

char* change_directory(char* cur_path, char* buf, char* delim);

void list_dir_contents(char* path);

void echo(char* buf, char* delim);

void run_exe(char* buf, char* delim);


int main(int argc, char* argv[]){
	char *cwd = malloc(256);

	if (getcwd(cwd, 256) == NULL) {
		fprintf(stderr, "Unable to get current directory\n");
		exit(EXIT_FAILURE);
	}
	printf("%s\n", cwd);

	if(argc == 1) {
		printf("INTERACTIVE MODE\n");
		interactive(cwd);
	} 
	else if(argc == 2) {
		printf("BATCH MODE WITH FILE: %s\n", argv[1]);
		batch(argv[1], cwd);
	} 
	else {
		fprintf(stderr, "INVALID ARGUMENTS\nUSAGE: "
							"./shell [batchfile]\n");
		exit(EXIT_FAILURE);
	}
}


/*
 * interactive mode
 * purpose: runs the command shell in interactive mode until 
 *          CTRL-D (EOF) is signaled 
 * arguments: NONE
 * returns: NONE
 *
 * NOTE: should not return (program will exit from this function)
 */
void interactive(char *path)
{
	ssize_t bytes_read;
	size_t len = 0;
	char *buf = NULL; 

	while (true) {
		//check for EOF (end of mode)
		if (feof(stdin)) {
			printf("\n");
			if (buf != NULL) {
					free(buf);
			}
			exit(EXIT_SUCCESS);
		}

		//prompt and process input
		printf("%s %s", path, PROMPT);
		bytes_read = getline(&buf, &len, stdin);             
		path = handle_line(buf, bytes_read, path);
	}
}


/*
 * batch mode
 *
 * purpose: processes shell commands from a batch file
 * arguments: file name of batch file
 * returns: none 
 *
 * NOTE: function will not return (exits the 'shell' once the file
 *       is processed)
 */
void batch(const char *fname, char *path) 
{
	ssize_t bytes_read;
	size_t len = 0;
	char *buf = NULL; 
	FILE *fp;

	//open file
	fp = fopen(fname, "r"); 
	if (fp == NULL) {
		fprintf(stderr, "INVALID FILENAME\n");
		exit(EXIT_FAILURE);
	}

	while (true) {
		//check for EOF
		if (feof(fp)) {
			if (buf != NULL) {
					free(buf);
			}
			fclose(fp);
			exit(EXIT_SUCCESS);
		}
		bytes_read = getline(&buf, &len, fp);             
		handle_line(buf, bytes_read, path);
	}
}

/*
 *
 * handle_line
 *
 * purpose: handles one 'line' of command line input
 *
 *
 * TODO (actually run the input commands, check for invalid
 *       ones and report appropriate errors)
 * TODO remove the printf statements 
 */
char* handle_line(char *buf, ssize_t len, char *path)
{
	const char delim[2] = ";";
	char *token;

	//get first 'token'
	token = strtok(buf, delim);
	 //if(args[length(args) - 1]  == "&") runInBackground(args);

	while (token != NULL) {
		//check command, if not in list then
		//attempt to run as executable
		if (strncmp(token, "cd", 2) == 0) {
			return change_directory(path, buf, " \n");
		} 
		else if (strncmp(token, "clr", 3) == 0) {
			clear();
		} 
		else if (strncmp(token, "dir", 3) == 0) {
			list_dir_contents(path);
		} 
		else if (strncmp(token, "environ", 7) == 0) {
			printf("environ\n");
		} 
		else if (strncmp(token, "echo", 4) == 0) {
			echo(buf, " \n");
			return path;
		} 
		else if (strncmp(token, "help", 4) == 0) {
			printf("help\n");
		} 
		else if (strncmp(token, "pause", 5) == 0) {
			char* buf = NULL;
			size_t len = 0;
			ssize_t bytes_read = getline(&buf, &len, stdin); 
			(void) bytes_read;
		} 
		else if (strncmp(token, "quit", 4) == 0) {
			exit(EXIT_SUCCESS);
		} 
		else if (strncmp(token, ";", 1) != 0 &&
			     strncmp(token, "\n", 1) != 0) {
			run_exe(buf, " \n");
		}
		token = strtok(NULL, delim);
	}
	return path;
}


void run_exe(char* buf, char* delim)
{
	char* token = malloc(sizeof(char) * 255);
	strcpy(token, buf);
	char* lengthstr = strtok (buf, delim);
	int length = 0;

	while ((lengthstr = strtok (NULL, delim)) != NULL){
		length++;
	}

	char* argv[length + 1];
	argv[length] = NULL;

	char* pipingFile = malloc(sizeof(char) * 255);
	bool runInBackground = false;
	token = strtok(token, delim);

	int i = 0;
	while (token != NULL){
		if (token != NULL){
			if (strcmp(token, "<") == 0){
				i = -1;
			}
			else if (strcmp(token, ">") == 0){
				i = -1;
			}
			else if (strcmp(token, "&") == 0){
				i = -2;
				runInBackground = true;
			}
			else if (i >= 0){
				argv[i] = token;
				i++;
			}
			else if(i == -1){
				strcpy(pipingFile, token);
			}
		}
		token = strtok (NULL, delim);
	}

    int pid = fork();
    char* s = (char *) 0;

    //for some reason without this line we get exec bad acess????
    printf("");

    if ( pid == 0 ) {
        int e = execve( argv[0], argv, &s);
    
        if(e == -1)
            perror("execve error");
        printf("Executed%i\n", e);
    }

    waitpid(pid, NULL, 0);
    free(token);
}


void echo(char* buf, char* delim)
{
	char* token = strtok(buf, delim);

	while((token = strtok(NULL, delim)) != NULL){
		printf("%s ", token);
	}
	printf("\n");
}


char* change_directory(char* cur_path, char* buf, char* delim)
{
	char* token = strtok(buf, delim);

	token = strtok(NULL, delim);
	char* new_dir = malloc(sizeof(char) * 255);
	strcpy(new_dir, cur_path);
	strcat(new_dir, "/");
	strcat(new_dir, token);

	DIR* dir;

	if((dir = opendir(new_dir)) != NULL){
		closedir(dir);
	}
	else{
		perror("");
	}
	
	return new_dir;
}

void list_dir_contents(char* path)
{
	DIR* dir;
	struct dirent* ent;

	if((dir = opendir(path)) != NULL){
		while((ent = readdir (dir)) != NULL){
			printf("%s\n", ent->d_name);
		}
		closedir(dir);
	}
	else{
		perror("Could not open directory");
	}
}

/*
 * clear
 * purpose: clears the shell screen
 *
 * NOTE: this is an ansi escape sequences
 */
void clear()
{
	printf("\033c");
}
