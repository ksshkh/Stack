#include "stack.hpp"

const char* errors_names[] = {"NO_ERROR", "SIZE_ERROR", "STACK_UNDERFLOW",
                              "NO_STACK", "BAD_CAPACITY", "NO_DATA",
                              "BAD_DATA_RIGHT_CANARY", "BAD_DATA_LEFT_CANARY",
                              "BAD_STACK_RIGHT_CANARY", "BAD_STACK_LEFT_CANARY", "BAD_HASH"}; //hpp?????????

Errors StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line) {
    if(!stk) {
        printf("%s\n", errors_names[NO_STACK]);
        return NO_STACK;
    }

    else if(!initCapacity) {
        printf("%s\n", errors_names[BAD_CAPACITY]);
        return BAD_CAPACITY;
    }

    ON_DEBUG(stk->file = file;)
    ON_DEBUG(stk->func = func;)
    ON_DEBUG(stk->line = line;)

    stk->data = (StackElem_t *) calloc(initCapacity ON_DEBUG(+ 2), sizeof(StackElem_t));
    assert(stk->data);

    stk->capacity = initCapacity;
    stk->position = 0;

    PoisonMaker(stk);

    ON_DEBUG(stk->data[0] = DATA_CANARY;)
    ON_DEBUG(stk->data[stk->capacity + 1] = DATA_CANARY;)

    ON_DEBUG(stk->hash = Hash(stk);)

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

    stk->data[stk->position ON_DEBUG(+ 1)] = el;
    stk->position++;

    ON_DEBUG(stk->hash = Hash(stk);)

    STACK_ASSERT(stk);
    return NO_ERROR;
}

Errors StackPop(Stack_t* stk, StackElem_t* x) {
    STACK_ASSERT(stk);

    if(!stk->position) {
        printf("%s\n", errors_names[STACK_UNDERFLOW]);
        return STACK_UNDERFLOW;
    }

    stk->position--;
    *x = stk->data[stk->position ON_DEBUG(+ 1)];
    stk->data[stk->position ON_DEBUG(+ 1)] = POISON;

    ON_DEBUG(stk->hash = Hash(stk);)

    if(stk->capacity == stk->position * 4) {
        StackReallocation(stk, POP_ID);
    }

    STACK_ASSERT(stk);
    return NO_ERROR;
}

void StackDump(Stack_t* stk, const char* file, const char* func, int line) {
    if(!stk) {
        printf("%s\n", errors_names[NO_STACK]);
        return;
    }
    printf("------------------------------------\n");
    printf("Stack_t [%p]\n", stk);
    printf("called from %s: %d (%s)\n", file, line, func);
    ON_DEBUG(printf("name born at %s: %d (%s)\n", stk->file, stk->line, stk->func);)
    ON_DEBUG(printf("hash: %llu\n", stk->hash);)
    ON_DEBUG(printf("stack left canary: %#llX\n", stk->left_canary);)
    printf("capacity = %lu\n", stk->capacity);
    printf("position = %lu\n", stk->position);
    printf("data [%p]\n", stk->data);
    if(!stk->data) {
        printf("%s\n", errors_names[STACK_UNDERFLOW]);
        return;
    }
    printf("{\n");
    ON_DEBUG(printf("data left canary: %#X\n", stk->data[0]);)
    for(size_t i = 0 ON_DEBUG(+ 1); i < stk->capacity ON_DEBUG(+ 1); i++) {
        if(i < stk->position ON_DEBUG(+ 1)) {
           printf("    *[%lu] = %d\n", i ON_DEBUG(- 1), stk->data[i]);
        }
        else {
            printf("    [%lu] = %d(POISON)\n", i ON_DEBUG(- 1), stk->data[i]);
        }
    }
    ON_DEBUG(printf("data right canary: %#X\n", stk->data[stk->capacity + 1]);)
    printf("}\n");
    ON_DEBUG(printf("stack right canary: %#llX\n", stk->right_canary);)
    printf("------------------------------------\n");
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

    ON_DEBUG(else if(stk->hash != Hash(stk)) {                       \
                printf("%s\n", errors_names[BAD_HASH]);              \
                return BAD_HASH;                                     \
    })                                                               \

    ON_DEBUG(else if(stk->data[0] != DATA_CANARY) {                  \
                printf("%s\n", errors_names[BAD_DATA_LEFT_CANARY]);  \
                return BAD_DATA_LEFT_CANARY;                         \
    })                                                               \

    ON_DEBUG(else if(stk->data[stk->capacity + 1] != DATA_CANARY) {  \
                printf("%s\n", errors_names[BAD_DATA_RIGHT_CANARY]); \
                return BAD_DATA_RIGHT_CANARY;                        \
    })                                                               \

    ON_DEBUG(else if(stk->left_canary != STACK_CANARY) {             \
                printf("%s\n", errors_names[BAD_STACK_LEFT_CANARY]); \
                return BAD_STACK_LEFT_CANARY;                        \
    })                                                               \

    ON_DEBUG(else if(stk->right_canary != STACK_CANARY) {            \
                printf("%s\n", errors_names[BAD_STACK_RIGHT_CANARY]);\
                return BAD_STACK_RIGHT_CANARY;                       \
    })                                                               \

    return NO_ERROR;
}

void StackReallocation(Stack_t* stk, FunkId id) {
    STACK_ASSERT(stk);

    ON_DEBUG(stk->data[stk->capacity + 1] = 0;) //delete right canary

    if(id == PUSH_ID) {
        stk->capacity *= 2;
        stk->data = (StackElem_t *) realloc(stk->data, sizeof(StackElem_t) * (stk->capacity ON_DEBUG(+ 2)));
        assert(stk->data);
        ON_DEBUG(stk->data[stk->capacity + 1] = DATA_CANARY;)
    }
    else if(id == POP_ID) {
        stk->capacity /= 2;
        stk->data = (StackElem_t *) realloc(stk->data, sizeof(StackElem_t) * (stk->capacity ON_DEBUG(+ 2)));
        assert(stk->data);
        ON_DEBUG(stk->data[stk->capacity + 1] = DATA_CANARY;)
    }
    PoisonMaker(stk);

    ON_DEBUG(stk->hash = Hash(stk);)

    STACK_ASSERT(stk);
}

void PoisonMaker(Stack_t* stk) {
    for(size_t i = stk->position ON_DEBUG(+ 1); i < stk->capacity ON_DEBUG( +1); i++) {
        stk->data[i] = POISON;
    }
}

Hash_t Hash(const Stack_t* stk) {
    Hash_t hash = 0, high;
    for (size_t i = 0 ON_DEBUG( +1); i < stk->position ON_DEBUG( +1); i++) {
        hash = (hash << 4) + stk->data[i];
        if (high = hash & 0xF0000000) {
            hash ^= high >> 24;
        }
        hash &= ~high;
    }
    return hash;
}