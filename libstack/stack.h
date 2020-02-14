
#ifndef stack_h
#define stack_h

enum{
    max_str = 31,
    max_drops = 30
};

typedef struct Node{
    char Value[max_str];
    struct Node *Next;
} Node;


typedef struct Stack{
    Node* First;
}Stack;

int push(char *str, Stack *s) ;
char*  pop(Stack *s);
int drop(Stack *s, int n);
int isempty(Stack*s);


#endif /* stack_h */
