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

typedef unsigned long long int Hash_t;

typedef unsigned long long int Canary_t;

const Canary_t STACK_CANARY = 0XACCE55ED;
const Canary_t DATA_CANARY = 0XC0FFEE;

const StackElem_t POISON = -666;

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
    NO_DATA,
    BAD_DATA_RIGHT_CANARY,
    BAD_DATA_LEFT_CANARY,
    BAD_STACK_RIGHT_CANARY,
    BAD_STACK_LEFT_CANARY,
    BAD_HASH
};

struct Stack_t {

    ON_DEBUG(Canary_t left_canary = STACK_CANARY;)

    ON_DEBUG(Hash_t hash = 0;)

    ON_DEBUG(const char* stack_name = 0;)
    ON_DEBUG(const char* file = 0;)
    ON_DEBUG(const char* func = 0;)
    ON_DEBUG(int line = 0;)

    StackElem_t* data = NULL;
    size_t position = 0;
    size_t capacity = 0;

    ON_DEBUG(Canary_t right_canary = STACK_CANARY;)
};

Errors StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line);

void StackDtor(Stack_t* stk);

Errors StackPush(Stack_t* stk, StackElem_t el);

Errors StackPop(Stack_t* stk, StackElem_t* x);

void StackDump(Stack_t* stk, const char* file, const char* func, int line);

Errors StackVerification(const Stack_t* stk);

void StackReallocation(Stack_t* stk, FunkId id);

void PoisonMaker(Stack_t* stk);

Hash_t Hash(const Stack_t* stk);

#endif // STACK_HPP