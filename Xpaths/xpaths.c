#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <err.h>
#include <string.h>

enum{
	size = 1024
};

struct data{
	char* paths;
	int count;
}Data;

struct data data[size];

void
insertpaths(char *path){
	int i= 0;
	int found = 0;

	while ((data[i].count != 0) && (!found)) {
		if(strcmp(data[i].paths, path) == 0){
			data[i].count++;
			found = 1;
		}
		i++;
	}

	if(!found){
		data[i].paths = strdup(path);
		data[i].count = 1;
	}
}

int
getpaths(char *file){
	FILE *fd;
	char words[size];

	fd = fopen(file, "r");
	if (fd == NULL){
		return -1;
	}

	while (fgets(words, size, fd)!= NULL) {
		words[strlen(words)-1] = 0;
		if(access(words, W_OK) != 0){
			fclose(fd);
			return -1;
		}else{
			insertpaths(words);
		}
	}

	fclose(fd);
	return 0;
}

char*
getmaxpath(){
	int max=0;
	int i=0, j = 0;
	char* path;

	while (data[i].count != 0) {
		if(data[i].count > max){
			max = data[i].count;
		}
		i++;
	}

	while (data[j].count != 0) {
		if(data[j].count == max){
			path = data[j].paths;
		}
		j++;
	}
	return path;
}

void
dops() {
	execl("/bin/ps", "ps", NULL);
	err(EXIT_FAILURE, "exec ha dado error");
}

int
main(int argc, char *argv[]) {
	char* path;
	int i ,n,exitmode, fd;
	pid_t pid;

	argv++;
	argc--;

	for (i = 0; i < argc; i++) {
		n = getpaths(argv[i]);
		if(n < 0){
			exitmode = 1;
		}
	}
	path= getmaxpath();
	switch (pid = fork()){
		case -1:
			err(EXIT_FAILURE, "fork error");
		case 0:
			fd = open(path, O_RDWR|O_CREAT|O_TRUNC, 0640);
			if (fd < 0){
				err(EXIT_FAILURE, "can´t open");
			}
			if(exitmode){
				dup2(2,1);
			}else{
				dup2(fd, 1);
			}
				dops();
				close(fd);
	}
}
