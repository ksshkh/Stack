#include "stack.hpp"

int main(void) {

    Stack_t stk = {};
    Errors err = NO_ERROR;
    StackElem_t x = 0;

    CHECKED_ STACK_CTOR(&stk, 1);
    STACK_DUMP(&stk);
    CHECKED_ StackPush(&stk, 100);
    STACK_DUMP(&stk);
    CHECKED_ StackPush(&stk, 200);
    STACK_DUMP(&stk);
    // stk.data[1] = 200;
    CHECKED_ StackPush(&stk, 300);
    STACK_DUMP(&stk);
    CHECKED_ StackPush(&stk, 400);
    STACK_DUMP(&stk);
    CHECKED_ StackPush(&stk, 500);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);
    CHECKED_ StackPop(&stk, &x);
    printf("%d\n", x);
    STACK_DUMP(&stk);
    StackDtor(&stk);

    return 0;
}