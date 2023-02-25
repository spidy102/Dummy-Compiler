
typedef struct st_el
{
    char NTOrT;
    struct st_el *next;
} st_element;

typedef struct STACK
{
    st_element *top;
} stack;

stack *initStack();

char top(stack *st);

bool push(stack *st, char NTOrT);

bool isEmpty(stack *st);

char pop(stack *st);