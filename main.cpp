#include "stack.hpp"

int main(void) {
    ON_DEBUG(printf("ggfgfg\n");)
    Stack_t stk = {};
    Errors err = NO_ERROR;
    StackElem_t x = 0;

    CHECKED_ STACK_CTOR(&stk, 1);
    stk.capacity = 0;
    STACK_DUMP(&stk);
    CHECKED_ StackPush(&stk, 100);
    STACK_DUMP(&stk);
    CHECKED_ StackPush(&stk, 200);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);

    return 0;
}