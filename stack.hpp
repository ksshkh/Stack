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

#define STACK_CTOR(stk, initCapacity) StackCtor((stk), (initCapacity), __FILE__, __func__, __LINE__, (output))

#define STACK_DUMP(stk, output) StackDump((stk), __FILE__, __func__, __LINE__, (output))

#define STACK_PUSH(stk, el) StackPush((stk), (el), (output))

#define STACK_POP(stk, x) StackPop((stk), (x), (output))

#define STACK_DTOR(stk) StackDtor((stk), (output))

#define STACK_REALLOCATION(stk, id) StackReallocation((stk), (id), (output))

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

struct Stack_t {

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

void StackDtor(Stack_t* stk, FILE* output);

Errors StackPush(Stack_t* stk, StackElem_t el, FILE* output);

Errors StackPop(Stack_t* stk, StackElem_t* x, FILE* output);

void StackDump(Stack_t* stk, const char* file, const char* func, int line, FILE* output);

Errors StackVerification(const Stack_t* stk, FILE* output);

void StackReallocation(Stack_t* stk, FunkId id, FILE* output);

void PoisonMaker(Stack_t* stk);

Hash_t DataHash(const Stack_t* stk);

Hash_t StackHash(const Stack_t* stk);

#endif // STACK_HPP