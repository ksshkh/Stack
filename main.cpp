#include "stack.hpp"

int main(void) {

    Stack_t stk = {};
    Errors err = NO_ERROR;
    StackElem_t x = 0;

    CHECKED_ STACK_CTOR(&stk, 1);

    for(int i = 0; i < 10; i++) {
        CHECKED_ StackPush(&stk, i * 10);
    }
    // stk.position = 0;
    // stk.data[1] = 900; // for bad hash
    // stk.data = NULL; // for no data

    // for(int i = 0; i < 10; i++) {
    //     CHECKED_ StackPush(&stk, i * 10);
    //     STACK_DUMP(&stk);
    // }

    // stk.capacity = 0; // for bad capacity

    for(int i = 0; i < 10; i++) {
        CHECKED_ StackPop(&stk, &x);
        fprintf(stdout, "pop: %d\n", x);
    }
    // CHECKED_ StackPop(&stk, &x); // for error stack underflow

    CHECKED_ StackDtor(&stk);

    return 0;
}