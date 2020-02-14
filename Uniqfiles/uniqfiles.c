#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct file{
    char* word;
    pid_t pid;
    int status;
}files;

int checkArguments(int num_args){
    if (num_args != 0) {
        return 1;
    }else{
        return 0;
    }
}

void compareFile(char* fileToCompare1, char* fileToCompare2){
    execl("/usr/bin/cmp", "cmp","-s", fileToCompare1, fileToCompare2, NULL);
    err(EXIT_FAILURE, "exec ha dado error");
}

int createChilds(char* file, char* filesToCompare[], int num_rep){
    int i;
    pid_t pid;
    pid_t pidCompare;
    int isEqual=0;
    int notEqual=0;
    int notExist=0;
    int status;

    for (i=0; i<num_rep; i++) {
        if (file != filesToCompare[i]){
            switch (pid = fork()){
                case -1:
                    err(EXIT_FAILURE, "fork ha dado error");
                case 0:
                    compareFile(file, filesToCompare[i]);
                break;
            }
        }
    }

    while((pidCompare= wait(&status))> 0){
        if (pidCompare < 0){
            err(EXIT_FAILURE, "wait ha dado error");
        }
        if (WEXITSTATUS (status) == 0){
            isEqual = 1;
        }else if (WEXITSTATUS (status) == 1){
            notEqual = 1;
        }else if (WEXITSTATUS (status) == 2){
            notExist = 1;
        }
    }
	
    if(isEqual && !notExist){
		return 0;
	}else if(notEqual && !notExist){
		return 1;
	}else{
		return 2;
	}
}

void checkSts(int status, char* word){
    if (WEXITSTATUS (status) == 1){
        printf("%s\n", word);
    }
}

int main(int argc, char * argv[]) {
    int i;
    int j = 0;
    pid_t pid;
    pid_t child_pids;
    int status;
    int result;
    char* filesArray[argc -1];
    struct file file[(argc -1)];

    if (checkArguments(argc - 1)){
        for (i = 1; i <argc ; i++) {
            filesArray[j] = argv[i];
            j++;
        }

        for (i=0; i<(argc-1); i++) {
            switch (pid = fork()){
            case -1:
                err(EXIT_FAILURE, "fork ha dado error");
            case 0:
                result = createChilds(filesArray[i], filesArray, ((argc -1)));
                if (result == 0){
                    exit(0);
                }else if (result == 1){
                    exit(1);
                }else{
                    exit(2);
                }
            default:
                file[i].word = filesArray[i];
                file[i].pid = pid;
            }
        }

        while((child_pids = wait(&status))> 0){
            for (i = 0; i < (argc -1); i++) {
                if (child_pids == file[i].pid) {
                    file[i].status = status;
                }
            }
        }

        if (WEXITSTATUS(file[0].status)!= 2){
            for (i = 0; i < (argc-1); i++) {
                checkSts(file[i].status, file[i].word);
            }
        }else{
            printf("usage: uniqfiles[files ...]\n");
            exit(2);
        }
    }
        exit(EXIT_SUCCESS);
    }
