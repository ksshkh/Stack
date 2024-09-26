#ifndef STACK
#define STACK

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define CHECKED_ if(!err) err = 

typedef int StackElem_t;

enum FunkId {
    PUSH_ID,
    POP_ID,
};

enum Errors {
    NO_ERROR,
    SIZE_ERROR,
    EMPTY_STACK,
};

struct Stack_t {
    StackElem_t* data;
    size_t size;
    size_t capacity;
};

Errors StackCtor(Stack_t* stk, size_t initCapacity);

void StackDtor(Stack_t* stk);

Errors StackPush(Stack_t* stk, StackElem_t el);

Errors StackPop(Stack_t* stk, StackElem_t* x);

void StackDump(Stack_t* stk);

Errors StackOK(Stack_t* stk);

void StackAssertFunc(Stack_t* stk, const char* file, int line);

void StackReallocation(Stack_t* stk, FunkId id);

#endif