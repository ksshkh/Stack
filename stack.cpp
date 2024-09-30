#include "stack.hpp"

Errors StackCtor(Stack_t* stk, size_t initCapacity) {
    StackAssertFunc(stk, __FILE__, __LINE__);
    StackElem_t* tempPtr = (int*)calloc(initCapacity, sizeof(StackElem_t));
    assert(tempPtr);

    stk->data = tempPtr;
    stk->capacity = initCapacity;
    stk->size = 0;

    StackAssertFunc(stk, __FILE__, __LINE__);
    return NO_ERROR;
}

void StackDtor(Stack_t* stk) {
    StackAssertFunc(stk, __FILE__, __LINE__);
    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    stk->size = 0;
    StackAssertFunc(stk, __FILE__, __LINE__);
}

Errors StackPush(Stack_t* stk, StackElem_t el) {
    StackAssertFunc(stk, __FILE__, __LINE__);
    if (stk->size == stk->capacity) {
        StackReallocation(stk, PUSH_ID);
    }

    stk->data[stk->size] = el;
    stk->size++;

    StackAssertFunc(stk, __FILE__, __LINE__);
    return NO_ERROR;
}

Errors StackPop(Stack_t* stk, StackElem_t* x) {
    StackAssertFunc(stk, __FILE__, __LINE__);
    if (stk->size == 0) {
        return EMPTY_STACK;
    }
    stk->size--;
    *x = stk->data[stk->size];
    if (stk->size == stk->capacity / 4) {
        StackReallocation(stk, POP_ID);
    }

    StackAssertFunc(stk, __FILE__, __LINE__);
    return NO_ERROR;
}

void StackDump(Stack_t* stk) {
    printf("------------------------------------\n");
    printf("capacity = %lu\n", stk->capacity);
    printf("size = %lu\n", stk->size);
    printf("data [%p]\n{\n", stk->data);
    for(size_t i = 0; i < stk->capacity; i++) {
        if(i < stk->size) {
           printf("    *[%lu] = %d\n", i, stk->data[i]);
        }
        else {
            printf("    [%lu] = %d\n", i, stk->data[i]);
        }
    }
    printf("}\n------------------------------------\n");
}

Errors StackOK(Stack_t* stk) {
    if(!(stk->size > 0 && stk->capacity < stk->size)) {
        return SIZE_ERROR;
    }
    return NO_ERROR;
}

void StackAssertFunc(Stack_t* stk, const char* file, int line) {
    if(!StackOK(stk)) {
        StackDump(stk);
        printf("file: %s, line: %d\n", file, line);
        assert(0);
    }
}

void StackReallocation(Stack_t* stk, FunkId id) {
    StackAssertFunc(stk, __FILE__, __LINE__);
    StackElem_t* tempPtr = 0;
    if(id == PUSH_ID) {
        tempPtr = (StackElem_t*)realloc(stk->data, sizeof(StackElem_t) * (2 * stk->capacity));
        assert(tempPtr);
        stk->data = tempPtr;
        stk->capacity *= 2;
    }
    else if(id == POP_ID) {
        tempPtr = (int*)realloc(stk->data, sizeof(StackElem_t) * (stk->capacity / 2));
        assert(tempPtr);
        stk->data = tempPtr;
        stk->capacity = stk->capacity / 2;
    }
    StackAssertFunc(stk, __FILE__, __LINE__);
}