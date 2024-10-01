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
    if (StackVerification(stk) != NO_ERROR) {           \
            STACK_DUMP(stk);                            \
            abort();                                    \
    }                                                   \
}

#define STACK_CTOR(stk, initCapacity) StackCtor((stk), (initCapacity), __FILE__, __func__, __LINE__)

#define STACK_DUMP(stk) StackDump((stk), __FILE__, __func__, __LINE__)

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

    ON_DEBUG(const char* stack_name = 0;)
    ON_DEBUG(const char* file = 0;)
    ON_DEBUG(const char* func = 0;)
    ON_DEBUG(int line = 0;)

    StackElem_t* data = NULL;
    size_t position = 0;
    size_t capacity = 0;
};

Errors StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line);

void StackDtor(Stack_t* stk);

Errors StackPush(Stack_t* stk, StackElem_t el);

Errors StackPop(Stack_t* stk, StackElem_t* x);

void StackDump(Stack_t* stk, const char* file, const char* func, int line);

Errors StackVerification(const Stack_t* stk);

//void StackAssertFunc(Stack_t* stk, const char* file, int line);

void StackReallocation(Stack_t* stk, FunkId id);

#endif // STACK_HPP