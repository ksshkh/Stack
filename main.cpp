#include "stack.hpp"

int main(void) {

    Stack_t stk = {};
    Errors err = NO_ERROR;
    StackElem_t x = 0;

    CHECKED_ STACK_CTOR(&stk, 5);

    for(int i = 0; i < 10; i++) {
        CHECKED_ StackPush(&stk, i * 10);
    }

    for(int i = 0; i < 10; i++) {
        CHECKED_ StackPop(&stk, &x);
    }

    CHECKED_ StackDtor(&stk);

    return 0;
}