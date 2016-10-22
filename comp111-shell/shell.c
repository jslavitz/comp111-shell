#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


//shell prompt
#define PROMPT "$> "
/*
void quitHandler(   );
void interactive();
void batch(char* name);
void handleLine(char* line);
void runInBackground(char*[] args);
*/
//interactive mode
void interactive();

//handle line
void handle_line(char *buf, ssize_t len);

int main(int argc, char* argv[]){

	if(argc == 1) {
                printf("INTERACTIVE MODE\n");
		interactive();
        } else if(argc == 2) {
                printf("BATCH MODE WITH FILE: %s\n", argv[1]);
		//batch(argv[2]);
        } else {
		fprintf(stderr, "%s", "INVALID ARGUMENTS\nUSAGE: "
                                "./shell [batchfile]\n");
                exit(EXIT_FAILURE);
        }
}



//https://www.gnu.org/software/libc/manual/html_node/Line-Input.html
/*
 * interactive mode
 * purpose: runs the command shell in interactive mode until 
 *          CTRL-D (EOF) is signaled 
 * arguments: NONE
 * returns: NONE
 *
 * NOTE: should not return (program will exit from this function)
 */
void interactive() 
{
        ssize_t bytes_read;
        size_t len = 0;
        char *buf = NULL; 

        while (true) {
                //check for EOF
                if (feof(stdin)) {
                        printf("\n");
                        free(buf);
                        exit(EXIT_SUCCESS);
                        //TODO
                        //JUST CALL EXIT FUNCTION IMPLEMENTED LATER
                        //with memory cleaned up
                }
                printf("%s", PROMPT);
                bytes_read = getline(&buf, &len, stdin);             
                handle_line(buf, bytes_read);
        }
}


void handle_line(char *buf, ssize_t len)
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
                        printf("clr\n");
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
void quitHandler(   ){
	exit(0);
}

void batch(char* name){
	char*[] lines = split (get(name, ";"));

	int l = lenght(lines);
	for(int i = 0; i < l; i++)
		handle(lines[i]);
}

void handleLine(char* line){
	char*[] args = split(line, " ");

	if(args[length(args) - 1]  == "&")
		runInBackground(args);

	switch(args[0]){
		case "cd": break;
		case "clr": break;
		case "dir": break;
		case "environ": break;
		case "echo":  break;
		case "help": break;
		case "pause": break;
		case "quit": break;
		default: //run external program break;
	}
}

void runInBackground(char*[] args){
	//fork and call handleline()?
}
*/
