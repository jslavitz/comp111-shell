#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

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
void handle_line(char *buf, ssize_t len, char *path);


//commands
void clear();



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
        } else if(argc == 2) {
                printf("BATCH MODE WITH FILE: %s\n", argv[1]);
		batch(argv[1], cwd);
        } else {
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
                printf("%s", PROMPT);
                bytes_read = getline(&buf, &len, stdin);             
                handle_line(buf, bytes_read, path);
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
void handle_line(char *buf, ssize_t len, char *path)
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
                        printf("%s\n", token);
                } else if (strncmp(token, "clr", 3) == 0) {
                        clear();
                } else if (strncmp(token, "dir", 3) == 0) {
                        printf("dir\n");
                } else if (strncmp(token, "environ", 7) == 0) {
                        printf("environ\n");
                } else if (strncmp(token, "echo", 4) == 0) {
                        printf("echo %s\n", token);
                } else if (strncmp(token, "help", 4) == 0) {
                        printf("help\n");
                } else if (strncmp(token, "pause", 5) == 0) {
                        printf("pause\n");
                } else if (strncmp(token, "quit", 4) == 0) {
                        printf("quit\n");
                } else if (strncmp(token, ";", 1) != 0 &&
                           strncmp(token, "\n", 1) != 0) {
                        printf("run: %s\n", token);
                        //run the token as an executable
                }
                token = strtok(NULL, delim);
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



/*
void runInBackground(char*[] args){
	//fork and call handleline()?
}
*/
