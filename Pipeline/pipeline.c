#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <err.h>

enum{
	size = 1024
};

typedef struct cmd{
	char exe[size];
	char* path[size];
}cmd;

typedef struct p{
	int pipe[2];
}p;

void
createPipes(int numPipes, p pipes[]) {
	int i;

	for (i = 0; i < numPipes; i++) {
		pipe(pipes[i].pipe);
	}
}

void
closePipes(int numPipes, p pipes[]) {
	int i;

	for (i = 0; i < numPipes; i++) {
		close(pipes[i].pipe[0]);
		close(pipes[i].pipe[1]);
	}
}

void
createPath(int pos, cmd command[], char* arg)
{
	char *aux;
	char *token;
	int i = 0;
	char* delimiter = " ";

	while ((token = strtok_r(arg, delimiter, &aux))){
		command[pos].path[i] = token;
		i++;
		arg = aux;
	}
	command[pos].path[i] = NULL;
}

void
checkAccess(char* pathBin, char* pathUsrBin, cmd command[], int pos) {

	if(access(pathBin, F_OK) == 0){
		strcpy(command[pos].exe, pathBin);
	}else if(access(pathUsrBin, F_OK) == 0){
		strcpy(command[pos].exe, pathUsrBin);
	}else{
		err(EXIT_FAILURE, "can`t access");
	}
}

void
createExe(cmd command[], int pos)
{
	char pathBin[size];
	char pathUsrBin[size];

	snprintf(pathBin, size, "%s/%s", "/bin", command[pos].path[0]);
	snprintf(pathUsrBin, size, "%s/%s", "/usr/bin", command[pos].path[0]);

	checkAccess(pathBin, pathUsrBin,command, pos);
}

void
doCommand(cmd command [], int pos){
	if(execv(command[pos].exe, command[pos].path) < 0){
		err(EXIT_FAILURE, "can´t command");
	}
}

void
firstPipe(int pos, cmd command[], p pipes[], int numPipes, int positionPipe) {
	switch (fork()) {
		case -1:
			err(EXIT_FAILURE, "fork error");
		case 0:
			dup2(pipes[positionPipe].pipe[1], 1);
			closePipes(numPipes,pipes);
			doCommand(command, pos);
	}
}

pid_t
lastPipe(int pos, cmd command[], p pipes[], int numPipes, int positionPipe) {
	pid_t pid;

	pid = fork();
	switch (pid) {
		case -1:
			err(EXIT_FAILURE, "fork error");
		case 0:
			dup2(pipes[positionPipe].pipe[0], 0);
			closePipes(numPipes,pipes);
			doCommand(command, pos);
	}
	return pid;
}

void
medPipe(int pos, cmd command[], p pipes[], int numPipes, int positionPipe) {
	switch (fork()) {
		case -1:
			err(EXIT_FAILURE, "fork error");
		case 0:
			dup2(pipes[positionPipe].pipe[0], 0);
			dup2(pipes[positionPipe + 1].pipe[1], 1);
			closePipes(numPipes,pipes);
			doCommand(command, pos);
	}
}

int
main(int argc, char **argv)
{
	argv++;
	argc--;

	int i, status, positionPipe = 0;
	int first, last, med;
	int lastPos = argc -1;
	int childs = argc;
	int numPipes = childs-1;
	pid_t lastPid;
	int lastSts;
	struct cmd command[childs];
	struct p pipes[numPipes];

	if(argc != 3){
		err(EXIT_FAILURE, "wrong arguments");
	}else{
		for (i = 0; i < argc; i++) {
			createPath(i,command, argv[i]);
			createExe(command, i);
		}

		createPipes(numPipes,pipes);

		for (i = 0; i < argc; i++) {
			first = 0;
			last = 0;
			med = 0;
			if(i == 0){
				first = 1;
			}else if( i == lastPos){
				last = 1;
			}else{
				med = 1;
			}

			if (first){
				firstPipe(i,command, pipes, numPipes,positionPipe);
			}else if(med){
				medPipe(i, command, pipes, numPipes, positionPipe);
			}else if(last){
				lastPid = lastPipe(i, command, pipes, numPipes, positionPipe);
				if (lastPid < 0) {
					err(EXIT_FAILURE, "last pipe error");
				}
			}

			if (i != 0) {
				positionPipe ++;
			}
		}

		closePipes(numPipes, pipes);

		for (i = 0; i < argc; i++)
		 	if(lastPid == wait(&status)){
				lastSts = status;
			}

		exit(WEXITSTATUS(lastSts));

	 }
}
