#include "stack.hpp"

Errors StackCtor(Stack_t* stk, size_t initCapacity) {
    StackElem_t* tempPtr = (int*)calloc(initCapacity, sizeof(StackElem_t));
    assert(tempPtr);

    stk->data = tempPtr;
    stk->capacity = initCapacity;
    stk->size = 0;

    StackAssertFunc(stk, __FILE__, __LINE__);
    return NO_ERROR;
}

void StackDtor(Stack_t* stk) {
    free(stk->data);
    stk->data = NULL;
    stk->capacity = 0;
    stk->size = 0;
}

Errors StackPush(Stack_t* stk, StackElem_t el) {
    if (stk->size == stk->capacity) {
        StackReallocation(stk, PUSH_ID);
    }

    stk->data[stk->size] = el;
    stk->size++;

    StackAssertFunc(stk, __FILE__, __LINE__);
    return NO_ERROR;
}

Errors StackPop(Stack_t* stk, StackElem_t* x) {
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
    printf("Address: %p\n", stk->data);
    printf("Capacity: %lu\n", stk->capacity);
    printf("Size: %lu\n", stk->size);
    printf("Stack:\n");
    for(size_t i = 0; i < stk->size; i++) {
        printf("[%d]\n", stk->data[i]);
    }
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
    StackElem_t* tempPtr = 0;
    if(id == PUSH_ID) {
        tempPtr = (int*)realloc(stk->data, 2 * stk->capacity);
        assert(tempPtr);
        stk->data = tempPtr;
        stk->capacity *= 2;
    }
    else if(id == POP_ID) {
        tempPtr = (int*)realloc(stk->data, stk->capacity / 2);
        assert(tempPtr);
        stk->data = tempPtr;
        stk->capacity = stk->capacity / 2;
    }
}