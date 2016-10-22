#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
//test comment






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
/*
void interactive(){
	char* env = //();

	while(true){
		printf("%s", "prompt: ");
		batch(getline());
		printf("%s", "\n");
	}
}
*/

//https://www.gnu.org/software/libc/manual/html_node/Line-Input.html
void interactive() 
{
        ssize_t bytes_read;
        size_t len = 0;
        char *buf = NULL; 

        while (true) {
                printf("%s", PROMPT);
                //read from command prompt
                bytes_read = getline(&buf, &len, stdin);             
                printf("%s", "\n");
                printf("%s", buf);
                printf("%s", "\n");
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
