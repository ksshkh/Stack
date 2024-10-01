#include "stack.hpp"

int main(void) {
    ON_DEBUG(printf("ggfgfg\n");)
    Stack_t stk = {};
    Errors err = NO_ERROR;
    StackElem_t x = 0;

    CHECKED_ StackCtor(&stk, 1);
    StackDump(&stk);
    CHECKED_ StackPush(&stk, 100);
    StackDump(&stk);
    CHECKED_ StackPush(&stk, 200);
    StackDump(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    StackDump(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    StackDump(&stk);

    return 0;
}