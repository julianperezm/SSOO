#include <stdlib.h>
#include <stdio.h>
#include <string.h>

enum{
	max_str = 31
};

typedef struct Node{
	char Value[max_str];
	struct Node *Next;
} TypeNode;

typedef TypeNode *pNode;
typedef TypeNode *Stack;

void push(char *str, Stack *s) {
	pNode new;

	new = malloc(sizeof(TypeNode));

	if (new != NULL){
		strncpy(new -> Value,str, max_str);
		new -> Next = *s;
		*s = new;
	}
}

char*  pop(Stack *s){
	pNode new;

	new = *s;

	if (!new) return 0;

	*s = new -> Next;
	char *str = strdup(new -> Value);
	free(new);
	return str;
}

int main(int argc, char const *argv[]) {

	Stack stack = NULL;

	char i, j ;

	for (i ='a' ; i <= 'z'; i++) {
		for (j ='a' ; j <= 'z'; j++) {
			char str[] = {i,j, '\0'};
			push(str, &stack);
		}
	}

	while (stack != NULL) {
		printf("%s \n", pop(&stack));
	}

	exit(0);
}
