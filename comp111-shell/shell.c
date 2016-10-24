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
 * - I/O redirection
 * - Error reporting
 * - WAIT FOR RUNNING PROCESSES BEFORE QUIT
 */



//validates command input
bool check_command(char *input, char *cmd);

//interactive mode
void interactive();

//batch mode
void batch(const char *fname);

//handle line
void handle_line(char *buf, ssize_t len);

//commands
void clear();

//change directory
void change_directory(char *input_path);

//list directory
void list_dir_contents(char *input_path);

//echo user's input
void echo(char* buf);

void run_exe(char* buf, char* delim);

//prints environ strings
void environ();

//help manual
void help();

int main(int argc, char* argv[]){
	if(argc == 1) {
		interactive();
	} 
	else if(argc == 2) {
		batch(argv[1]);
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
void interactive()
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
		handle_line(buf, bytes_read);
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
void batch(const char *fname) 
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
		handle_line(buf, bytes_read);
	}
}

/*
 *
 * handle_line
 *
 * purpose: handles one 'line' of command line input
 *
 */
void handle_line(char *buf, ssize_t len)
{
	const char delim[2] = ";";
	char *token;

	//get first 'token'
	token = strtok(buf, delim);

	while (token != NULL) {
		//check command, if not in list then
		//attempt to run as executable
                if (check_command(token, "cd")) {
			change_directory(token);
		} 
		else if (check_command(token, "clr")) {
			clear();
		} 
		else if (check_command(token, "dir")) {
			list_dir_contents(token);
		} 
		else if (check_command(token, "environ")) {
                        environ();
		} 
		else if (check_command(token, "echo")) {
			echo(token);
		} 
		else if (check_command(token, "help")) {
			help();
		} 
		else if (check_command(token, "pause")) {
			char* buf = NULL;
			size_t len = 0;
			ssize_t bytes_read = getline(&buf, &len, stdin); 
			(void) bytes_read;
		} 
		else if (check_command(token, "quit")) {
                        //TODO WAIT FOR RUNNING CHILDREN
                        if(buf != NULL) free(buf);
			exit(EXIT_SUCCESS);
		} 
		else if (strncmp(token, ";", 1) != 0 &&
			     strncmp(token, "\n", 1) != 0) {
                        printf("invalid command!\n");
			//run_exe(buf, " \n");
		}
		token = strtok(NULL, delim);
	}
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


//echos the text
//text starts with "echo"
void echo(char *buf)
{
        int i, len = strlen(buf);
        for (i = 5; i < len; ++i) {
                printf("%c", (char)buf[i]);
        }
}


//attempts to change the shells directory
void change_directory(char *input_path)
{
        int len = strlen(input_path);
        int i;
        char *path;
        path = getenv("PWD");


        //either print current directory or change
        if ((len < 3) || (input_path[2] == 10)) {
                if (path != NULL) printf("%s\n", path);  
        } else {
                //skip over whitespace 
                i = 2; 
                while (i < len && input_path[i] == ' ') ++i;
                if (input_path[i] == 10) {
                        if (path != NULL) printf("%s\n", path);  
                } else {
                        //make new string holding desired environment
                        path = malloc(sizeof(char) * len);
                        int j = 0;
                        for ( ; i < len && input_path[i] != 10 
                                        && input_path[i] != '\0'; ++i) {
                                path[j] = input_path[i];
                                j++;
                        }
                        path[i] = '\0';

                        DIR *dir;
                        //attempt to set new pwd
                        if ((dir = opendir(path)) != NULL) {
                                closedir(dir);
                                setenv("PWD", path, 1);
                        } else {
                                perror("");
                        }
                        if (path != NULL) free(path);
                }
        }
}


//checks to see if the user's input matches
//an actual command
bool check_command(char *input, char *cmd)
{
        int input_len = (int)strlen(input);
        int cmd_len = (int)strlen(cmd);

        if (input_len < cmd_len) {
                return false;
        }

        if (strncmp(input, cmd, cmd_len) == 0) {
                if (input_len == cmd_len) {
                        return true;
                }
                if (cmd_len + 1 <= input_len) {
                        if (input[cmd_len] == ' ') return true;
                        if (input[cmd_len] == '\r') return true;
                        if (input[cmd_len] == 10) return true; //line feed
                } else {
                        return false;
                }
        }
        return false;
}


//lists the dir contents of a 
//directory name pointed to by path
void list_dir_contents(char *path)
{
	DIR *dir;
	struct dirent *ent;
        int len = strlen(path);
        char *dir_name;
        int i;

        //get directory name
        if ((len < 4) || (path[3] == 10)) {
                dir_name = getenv("PWD");
        } else {
                //skip over whitespace 
                i = 3; 
                while (i < len && path[i] == ' ') ++i;
                if (path[i] == 10) {
                        dir_name = getenv("PWD");
                } else {
                        //make new string holding desired directory
                        dir_name = malloc(sizeof(char) * len);
                        int j = 0;
                        for ( ; i < len && path[i] != 10 
                                        && path[i] != '\0'; ++i) {
                                dir_name[j] = path[i];
                                j++;
                        }
                        dir_name[i] = '\0';
                }
        }

        //now list the contents
	if ((dir = opendir(dir_name)) != NULL) {
		while((ent = readdir (dir)) != NULL) {
			printf("%s\n", ent->d_name);
		}
		closedir(dir);
                if (dir_name != getenv("PWD") && dir_name != NULL)
                        free(dir_name);
	} else {
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


//prints environment strings
void environ()
{
        char *pwd = getenv("PWD");
        if (pwd != NULL) {
                printf("parent=%s/shell\n", pwd);
        } else {
                fprintf(stderr, "ERROR GETTING ENV\n");
        }
}


//shows the user's help manual
void help()
{
        clear();
        printf("COMP 111 BASIC SHELL MANUAL\nInstructions:\ncd <directory>"
               " - change working directory to <directory>. If <directory> is not"
               " present, prints working directory\nclr - clears the screen\n"
               "dir <directory> - list contents of <directory>\nenviron - list"
               "all the environ strings\necho <comment> - display comment on stdout"
               "help - display this manual\nquit - quit the shell\n");

}
