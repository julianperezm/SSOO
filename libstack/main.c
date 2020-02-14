#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"


int main(int argc, char *argv[]) {
    char i, j;
    int n;
    char *strp;
    int error = 0;
  
    Stack *s =  malloc(sizeof(Stack));
    s -> First = NULL;
    
    for (i ='a' ; i <= 'z'; i++) {
        for (j ='a' ; j <= 'z'; j++) {
            char str[] = {i,j, '\0'};
            
            if(push(str, s) != 0){
                printf("Push error");
            }
        }
    }

    if (isempty(s) == 1){
        printf("Empty stack\n");
    }else{
        printf("Full stack\n");
    }

    while (! isempty(s)) {
        strp = s->First->Value;
        if (strp[0] == strp[1]){
            n = drop(s,5);
            if (n < 0){
                error = 1;
                printf("%s \n", pop(s));
            }
        }else{
            printf("%s \n", pop(s));
        }
    }
    if (error){
        printf("superado numero max de eliminaciones\n");
    }
    
    exit(EXIT_SUCCESS);
 
}


