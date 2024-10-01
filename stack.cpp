#include "stack.hpp"

const char* errors_names[] = {"NO_ERROR", "SIZE_ERROR", "STACK_UNDERFLOW", "NO_STACK", "BAD_CAPACITY", "NO_DATA"}; //hpp?????????

Errors StackCtor(Stack_t* stk, size_t initCapacity) {
    if(!stk) {
        printf("%s\n", errors_names[NO_STACK]);
        return NO_STACK;
    }
    else if(!initCapacity) {
        printf("%s\n", errors_names[BAD_CAPACITY]);
        return BAD_CAPACITY;
    }
    stk->data = (StackElem_t *) calloc(initCapacity, sizeof(StackElem_t));
    assert(stk->data);

    stk->capacity = initCapacity;
    stk->position = 0;

    STACK_ASSERT(stk);
    return NO_ERROR;
}

void StackDtor(Stack_t* stk) { //, int *code_error
    STACK_ASSERT(stk);

    free(stk->data);
    stk->data = NULL;

    stk->capacity = 0;
    stk->position = 0;
}

Errors StackPush(Stack_t* stk, StackElem_t el) {
    STACK_ASSERT(stk);

    if(stk->position == stk->capacity) {
        StackReallocation(stk, PUSH_ID);
    }

    stk->data[stk->position] = el;
    stk->position++;

    STACK_ASSERT(stk);
    return NO_ERROR;
}

Errors StackPop(Stack_t* stk, StackElem_t* x) {
    STACK_ASSERT(stk);

    if(!stk->position) {
        return STACK_UNDERFLOW;
    }

    stk->position--;
    *x = stk->data[stk->position];
    stk->data[stk->position] = 0;

    if(stk->capacity == stk->position * 4) {
        StackReallocation(stk, POP_ID);
    }

    STACK_ASSERT(stk);
    return NO_ERROR;
}

void StackDump(Stack_t* stk) {
    if(!stk) {
        printf("%s\n", errors_names[NO_STACK]);
        return;
    }
    printf("------------------------------------\n");
    printf("capacity = %lu\n", stk->capacity);
    printf("position = %lu\n", stk->position);
    printf("data [%p]\n", stk->data);
    if(!stk->data) {
        printf("%s\n", errors_names[STACK_UNDERFLOW]);
        return;
    }
    printf("{\n");
    for(size_t i = 0; i < stk->capacity; i++) {
        if(i < stk->position) {
           printf("    *[%lu] = %d\n", i, stk->data[i]);
        }
        else {
            printf("    [%lu] = %d\n", i, stk->data[i]);
        }
    }
    printf("}\n------------------------------------\n");
}

Errors StackVerification(const Stack_t* stk) {
    if(!stk) {
        return NO_STACK;
        printf("%s\n", errors_names[NO_STACK]);
    }
    else if(stk->position > 0 && stk->capacity < stk->position) {
        printf("%s\n", errors_names[SIZE_ERROR]);
        return SIZE_ERROR;
    }
    else if(!stk->capacity) {
        printf("%s\n", errors_names[BAD_CAPACITY]);
        return BAD_CAPACITY;
    }
    else if(!stk->data) {
        printf("%s\n", errors_names[STACK_UNDERFLOW]);
        return STACK_UNDERFLOW;
    }
    return NO_ERROR;
}

// void StackAssertFunc(Stack_t* stk, const char* file, int line) {
//     Errors err = StackVerification(stk);
//     if(err) {
//         StackDump(stk);
//         printf("err: %d file: %s, line: %d\n", err, file, line);
//         exit(1);
//     }
// }

void StackReallocation(Stack_t* stk, FunkId id) {
    STACK_ASSERT(stk);

    if(id == PUSH_ID) {
        stk->data = (StackElem_t *) realloc(stk->data, sizeof(StackElem_t) * (2 * stk->capacity));
        assert(stk->data);
        stk->capacity *= 2;
    }
    else if(id == POP_ID) {
        stk->data = (StackElem_t *) realloc(stk->data, sizeof(StackElem_t) * (stk->capacity / 2));
        assert(stk->data);
        stk->capacity /= 2;
    }

    STACK_ASSERT(stk);
}