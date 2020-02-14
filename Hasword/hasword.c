#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct Dato{
    char* word;
	pid_t pid;
    int status;
}Datos;

int checkArguments(int num_args){
    if (num_args%2 == 0) {
        return 1;
    }else{
        return 0;
    }
}

void foundWord(char* file, char* word){
    execl("/bin/fgrep", "fgrep", "-q" ,"-s", word, file, NULL);
    err(EXIT_FAILURE, "exec ha dado error");
}

void checkSts(int status, char* word){
    if (WEXITSTATUS (status) == 0){
        printf("%s : si\n", word);
    }else if (WEXITSTATUS (status) == 1){
        printf("%s : no\n", word);
    }else{
        printf("%s: error\n", word);
    }
}

int main(int argc, char * argv[]) {
    int i;
    int j = 0;
    pid_t pid;
    pid_t child_pids;
    int status;
	char* fileArg;
	char* wordArg;

	struct Dato Dato[(argc -1)/2];

    if (checkArguments(argc - 1)){

        for (i=1; i<argc; i++) {
            fileArg = argv[i];
        	wordArg =argv[i + 1];
            i++;

            switch (pid = fork()){
             case -1:
                    perror("fork");
                    exit(1);
            case 0:
                foundWord(fileArg, wordArg);
                break;
            default:
                Dato[j].word = wordArg;
                Dato[j].pid = pid;
                j++;
            }
        }

        while((child_pids = wait(&status))> 0){
            for (i = 0; i < (argc -1)/2; i++) {
                if (child_pids == Dato[i].pid) {
                    Dato[i].status = status;
                }
            }
        }

        for (i = 0; i < (argc-1)/2; i++) {
            checkSts(Dato[i].status, Dato[i].word);
        }
    }else{
        printf("usage: hasword [[file word] ...]\n");
    }

    exit(EXIT_SUCCESS);
}
