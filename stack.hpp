#ifndef STACK_HPP
#define STACK_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(code) code
#else
    #define ON_DEBUG(code)
#endif

#define CHECKED_ if(!err) err =

#define STACK_ASSERT(stk) {                             \
    if (StackVerification(stk) != NO_ERROR) {  \
            StackDump(stk);                            \
            abort();                                    \
        }                                               \
    }

typedef int StackElem_t;

enum FunkId {
    PUSH_ID,
    POP_ID
};

enum Errors {
    NO_ERROR,
    SIZE_ERROR,
    STACK_UNDERFLOW,
    NO_STACK,
    BAD_CAPACITY,
    NO_DATA
};

struct Stack_t {
    StackElem_t* data = NULL;
    size_t position = 0;
    size_t capacity = 0;
};

Errors StackCtor(Stack_t* stk, size_t initCapacity);

void StackDtor(Stack_t* stk);

Errors StackPush(Stack_t* stk, StackElem_t el);

Errors StackPop(Stack_t* stk, StackElem_t* x);

void StackDump(Stack_t* stk);

Errors StackVerification(const Stack_t* stk);

//void StackAssertFunc(Stack_t* stk, const char* file, int line);

void StackReallocation(Stack_t* stk, FunkId id);

#endif // STACK_HPP