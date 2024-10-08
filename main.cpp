#include "stack.hpp"

int main(void) {

    Stack_t stk = {};
    Errors err = NO_ERROR;
    StackElem_t x = 0;

    FILE * output = fopen("dump.txt", "w+");
    assert(output);

    CHECKED_ STACK_CTOR(&stk, 1);
    STACK_DUMP(&stk);

    for(int i = 0; i < 10; i++) {
        CHECKED_ StackPush(&stk, i * 10);
        STACK_DUMP(&stk);
    }

    // stk.data[1] = 900; // for bad hash
    // stk.data = NULL; // for no data

    for(int i = 0; i < 10; i++) {
        CHECKED_ StackPop(&stk, &x);
        fprintf(output, "pop: %d\n", x);
        STACK_DUMP(&stk);
    }

    // CHECKED_ StackPop(&stk, &x); // for error stack underflow

    StackDtor(&stk);

    return 0;
}