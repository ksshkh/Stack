#ifndef STACK_HPP
#define STACK_HPP

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "errors.hpp"

#define DEBUG

#ifdef DEBUG
    #define ON_DEBUG(code) code
#else
    #define ON_DEBUG(code)
#endif

#define STACK_ASSERT(stk) {                                           \
    Errors err = StackVerification(stk);                              \
    if (err != NO_ERROR) {                                            \
            STACK_DUMP(stk);                                          \
            exit(1);                                                  \
    }                                                                 \
}

#define STACK_CTOR(stk, initCapacity) StackCtor((stk), (initCapacity), __FILE__, __func__, __LINE__, (output))

#define STACK_DUMP(stk) StackDump((stk), __FILE__, __func__, __LINE__, err)

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

// add file in struct

struct Stack_t {

    ON_DEBUG(FILE* debug_file_name = NULL;)

    ON_DEBUG(Canary_t left_canary = 0;)

    ON_DEBUG(Hash_t stack_hash = 0;)
    ON_DEBUG(Hash_t data_hash = 0;)

    ON_DEBUG(const char* file = 0;)
    ON_DEBUG(const char* func = 0;)
    ON_DEBUG(int line = 0;)

    StackElem_t* data = NULL;
    size_t position = 0;
    size_t capacity = 0;

    ON_DEBUG(Canary_t right_canary = 0;)
};

Errors StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line, FILE* output);

void StackDtor(Stack_t* stk);

Errors StackPush(Stack_t* stk, StackElem_t el);

Errors StackPop(Stack_t* stk, StackElem_t* x);

void StackDump(Stack_t* stk, const char* file, const char* func, int line, Errors err);

Errors StackVerification(const Stack_t* stk);

void StackReallocation(Stack_t* stk, FunkId id);

void PoisonMaker(Stack_t* stk);

Hash_t DataHash(const Stack_t* stk);

Hash_t StackHash(const Stack_t* stk);

#endif // STACK_HPP