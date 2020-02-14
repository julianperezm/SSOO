#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int push(char *str, Stack *s) {
    Node* new;

    new = malloc(sizeof(Node));

    if (s != NULL && strlen(str) < max_str){
        strncpy(new ->Value, str, max_str);
        new -> Next = s -> First;
        s -> First = new;
        return 0;
    }else{
        return -1;
    }
}

char*  pop(Stack *s){
    Node* new;
    char* str;
    
    new = s -> First;
    
    if (new == NULL){
        return NULL;
    }else{
        s -> First = new -> Next;
        str = strdup(new -> Value);
        free(new);
        return str;
    }
}

int drop(Stack *s, int n){
    Node* new;
    int i;
    
    if (s != NULL && n > 0 && n < max_drops){
        for (i = 0; i < n && s -> First != NULL; i++) {
            new = s -> First;
            s -> First = new -> Next;
            free(new);
        }
        return 0;
    }
    else{
        return -1;
    }
}


int isempty(Stack*s){

    if (s ->First == NULL){
        return 1;
    }else{
        return 0;
    }
}

