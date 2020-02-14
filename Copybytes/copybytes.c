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

int
main(int argc, char *argv[]) {
	char words[size];
	int readBytes, fileToRead, fileToWrite;
	int offset = atoi(argv[3]);
	int bytesToCopy = atoi(argv[4]);

	if (argc == 5){
		if (strcmp(argv[1], "-") == 0 ){
			fileToRead = 0;
		}else{
			fileToRead = open(argv[1], O_RDONLY);
			if (fileToRead < 0) {
				err(EXIT_FAILURE, "cannot open file");
			}
		}

		if (strcmp(argv[2], "-") == 0) {
			fileToWrite = 1;
		}else{
			fileToWrite = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, 0640);
			if (fileToWrite < 0) {
				err(EXIT_FAILURE, "cannot open file");
			}
		}

		lseek(fileToRead, offset, SEEK_SET);
		while (bytesToCopy != 0) {
			if (size > bytesToCopy){
				readBytes = read(fileToRead, words, bytesToCopy);
				if (readBytes < 0) {
					err(EXIT_FAILURE,"cannot read file");
				}
				bytesToCopy = 0;
			}else{
				readBytes = read(fileToRead, words, bytesToCopy);
				if (readBytes < 0) {
					err(EXIT_FAILURE,"cannot read file");
				}
				bytesToCopy = bytesToCopy - size;
			}

			if((write(fileToWrite, words, readBytes)) != readBytes){
				err(EXIT_FAILURE, "cannot write file");
			}
		}

		close(fileToRead);
		close(fileToWrite);
		exit(EXIT_SUCCESS);
	}else{
		fprintf(stderr,"usage: ./copybytes [fileToRead] [fileToWrite] [offset] [MaxBytes]\n");
		exit(EXIT_FAILURE);
	}

}
