#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse(char* Word){
    int length;
    int i;

    length = strlen(Word);

    for (i = length; i>= 0; i--){
        printf("%c", Word[i]);
    }

    printf("\n");
}

char *cutWord(char * Word){
    char * cuttedword = malloc(5);
    int i;

    for (i = 0; i < 5; i++) {
        cuttedword[i] = Word[i];
    }

    return cuttedword;
}

void wordRepeat(char *Word, int count, int change){

    if (strlen(Word) > 5){
        Word = cutWord(Word);
    }

    char* newword = (char*)malloc(strlen(Word) * count);
    strcpy(newword, Word);

    while (--count > 0) {
        strcat(newword,Word);
    }

    if (change){
        reverse(newword);
    }else{
        printf("%s\n",newword);
    }
}

int main(int argc, char *argv[]) {

    int i;
    int repeater;

    if (argc == 1) {
        printf("usage: [-r] n [String ...]\n");
    }

    if (argc >= 2) {
        if (strcmp(argv[1],"-r")== 0) {
            if (argv[2] != 0){
                repeater = atoi(argv[2]);
                for (i = 3; i < argc; i++) {
                    wordRepeat(argv[i], repeater, 1);
                }
            }
        }else{

            if (argv[1] != 0){
                repeater = atoi(argv[1]);
                for (i = 2; i < argc; i++) {
                    wordRepeat(argv[i], repeater, 0);
                }
            }
        }
    }
    exit(EXIT_SUCCESS);
}


