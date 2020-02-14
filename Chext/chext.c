#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <err.h>

enum{
	size = 1024
};

void
buildPath(char* path, char* dir, struct dirent *ent) {
	 snprintf(path, size, "%s/%s", dir, ent->d_name);
}

void
swapExtension(char* ext1, char*  ext2, char* path){
	char* extension;
	char newPath [size] = "";
	int bytesToCopy;

	extension = strrchr(path,'.');

	if(extension != NULL){
		extension++;
		if(strcmp(extension, ext1)== 0){
			bytesToCopy = strlen(path) - strlen(extension);
			strncpy(newPath, path, bytesToCopy);
			strcat(newPath, ext2);
			if(rename(path, newPath)<0){
				err(EXIT_FAILURE, "can´t rename");
			}
		}
	}
}

void
listDir(char* ext1, char* ext2, char *dir){
	DIR *d;
	struct dirent *ent;
	struct stat st;
	char path[size];

	d = opendir(dir);
	if (d == NULL){
		err(EXIT_FAILURE, "can´t open");
	}

	while ((ent = readdir(d)) != NULL) {
		if ((strcmp(ent -> d_name, ".")!= 0) && (strcmp(ent->d_name, "..")!= 0)){
			buildPath(path , dir, ent);
			if (stat(path, &st) == -1){
				err(EXIT_FAILURE, "stat error");
			}
			if(S_ISDIR(st.st_mode)){
				listDir(ext1, ext2, path);
			}
			if(S_ISREG(st.st_mode)){
				if(st.st_mode & (S_IRUSR | S_IRGRP | S_IROTH)){
					swapExtension(ext1, ext2, path);
				}
			}
		}
	}
	closedir(d);
}

int
main(int argc, char *argv[]) {
	argv++;
	argc--;
	char* ext1 = argv[0];
	char* ext2 = argv[1];
	char* path = argv[2];

	if(argc == 3){
		listDir(ext1, ext2, path);
	}else{
		err(EXIT_FAILURE, "invalid arguments");
	}

	exit(EXIT_SUCCESS);
}
