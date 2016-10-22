#include <stdlib.h>
#include <stdio.h>
//test comment

void quitHandler(   );
void interactive();
void batch(char* name);
void handleLine(char* line);
void runInBackground(char*[] args);

int main(int argc, char* argv[]){
	if(argc == 1)
		interactive();
	else if(argc == 2)
		batch(argv[2]);
	else
		fprintf(stderr, "%s", "Too many files");
}

void quitHandler(   ){
	exit(0);
}

void interactive(){
	char* env = //();

	while(true){
		printf("%s", "prompt: ");
		batch(getline());
		printf("%s", "\n");
	}
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
		default: /*run external program*/ break;
	}
}

void runInBackground(char*[] args){
	//fork and call handleline()?
}
