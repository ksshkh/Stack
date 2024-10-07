#include "stack.hpp"

int main(void) {

    Stack_t stk = {};
    Errors err = NO_ERROR;
    StackElem_t x = 0;

    FILE * output = fopen("dump.txt", "w+");

    CHECKED_ STACK_CTOR(&stk, 1);
    STACK_DUMP(&stk, output);

    for(int i = 0; i < 10; i++) {
        CHECKED_ STACK_PUSH(&stk, i * 10);
        STACK_DUMP(&stk, output);
    }

    // stk.data[1] = 900; for hash error

    for(int i = 0; i < 10; i++) {
        CHECKED_ STACK_POP(&stk, &x);
        fprintf(output, "pop: %d\n", x);
        STACK_DUMP(&stk, output);
    }

    // CHECKED_ STACK_POP(&stk, &x); for error stack underflow

    STACK_DTOR(&stk);

    return 0;
}