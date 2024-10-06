#include "stack.hpp"

const char* errors_names[] = {"NO_ERROR",
                              "SIZE_ERROR",
                              "STACK_UNDERFLOW",
                              "NO_STACK", "BAD_CAPACITY", "NO_DATA",
                              "BAD_DATA_RIGHT_CANARY", "BAD_DATA_LEFT_CANARY",
                              "BAD_STACK_RIGHT_CANARY", "BAD_STACK_LEFT_CANARY", "BAD_HASH"}; //hpp?????????

// add hash_struct

Errors StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line) {
    if(!stk) {      // my_assert
        printf(RED("%s\n"), errors_names[NO_STACK]);
        return NO_STACK;
    }

    if(!initCapacity) {
        printf(RED("%s\n"), errors_names[BAD_CAPACITY]);
        return BAD_CAPACITY;
    }

    ON_DEBUG(stk->file = file;)
    ON_DEBUG(stk->func = func;)
    ON_DEBUG(stk->line = line;)


    #ifdef DEBUG

    stk->data = (StackElem_t *) calloc (initCapacity * sizeof (StackElem_t) + 2 * sizeof (Canary_t), sizeof (char));
    // my_assert (stk->data != NULL)

    *((Canary_t *) stk->data) = DATA_CANARY;
    stk->data = (StackElem_t *) ((char *) stk->data + sizeof(Canary_t));
    *((Canary_t *)(stk->data + initCapacity)) = DATA_CANARY;

    stk->left_canary = STACK_CANARY;
    stk->right_canary = STACK_CANARY;

    #else

    stk->data = (StackElem_t *) calloc(initCapacity, sizeof(StackElem_t));

    #endif

    assert(stk->data);

    stk->capacity = initCapacity;
    stk->position = 0;

    PoisonMaker(stk);

    ON_DEBUG(stk->hash = Hash(stk);)

    STACK_ASSERT(stk);
    return NO_ERROR;
}

void StackDtor(Stack_t* stk) { //, int *code_error
    STACK_ASSERT(stk);

    ON_DEBUG(stk->data = (StackElem_t *) ((char *) stk->data - sizeof(Canary_t));)
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

    ON_DEBUG(stk->hash = Hash(stk);)

    STACK_ASSERT(stk);
    return NO_ERROR;
}

Errors StackPop(Stack_t* stk, StackElem_t* x) {
    STACK_ASSERT(stk);

    if(!stk->position) {
        printf(RED("%s\n"), errors_names[STACK_UNDERFLOW]);
        return STACK_UNDERFLOW;
    }

    stk->position--;
    *x = stk->data[stk->position];
    stk->data[stk->position] = POISON;

    ON_DEBUG(stk->hash = Hash(stk);)

    if(stk->capacity == stk->position * 4) {
        StackReallocation(stk, POP_ID);
    }

    // ON_DEBUG(stk->hash = Hash(stk);)        // after realloc

    STACK_ASSERT(stk);
    return NO_ERROR;
}

void StackDump(Stack_t* stk, const char* file, const char* func, int line) {
    if(!stk) {
        printf(RED("%s\n"), errors_names[NO_STACK]);
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
    ON_DEBUG(printf("\tdata left canary: %#llX\n", *((Canary_t *) stk->data - 1));)
    for(size_t i = 0; i < stk->capacity; i++) {
        if(i < stk->position) {
           printf("\t*[%lu] = %d\n", i, stk->data[i]);
        }
        else {
            printf("\t[%lu] = %d(POISON)\n", i, stk->data[i]);
        }
    }
    ON_DEBUG(printf("\tdata right canary: %#llX\n", *((Canary_t *)(stk->data + stk->capacity)));)
    printf("}\n");
    ON_DEBUG(printf("stack right canary: %#llX\n", stk->right_canary);)
    printf("------------------------------------\n");
}

Errors StackVerification(const Stack_t* stk) {
    if(!stk) {
        return NO_STACK;
        printf(RED("%s\n"), errors_names[NO_STACK]);
    }

    else if(stk->position > 0 && stk->capacity < stk->position) {
        printf(RED("%s\n"), errors_names[SIZE_ERROR]);
        return SIZE_ERROR;
    }

    else if(!stk->capacity) {
        printf(RED("%s\n"), errors_names[BAD_CAPACITY]);
        return BAD_CAPACITY;
    }

    else if(!stk->data) {
        printf(RED("%s\n"), errors_names[STACK_UNDERFLOW]);
        return STACK_UNDERFLOW;
    }

    #ifdef DEBUG

    else if(stk->hash != Hash(stk)) {
        printf(RED("%s\n"), errors_names[BAD_HASH]);
        return BAD_HASH;
    }

    else if(*((Canary_t *) stk->data - 1) != DATA_CANARY) {
        printf(RED("%s\n"), errors_names[BAD_DATA_LEFT_CANARY]);
        return BAD_DATA_LEFT_CANARY;
    }

    else if(*((Canary_t *)(stk->data + stk->capacity)) != DATA_CANARY) {
        printf(RED("%s\n"), errors_names[BAD_DATA_RIGHT_CANARY]);
        return BAD_DATA_RIGHT_CANARY;
    }

    else if(stk->left_canary != STACK_CANARY) {
        printf(RED("%s\n"), errors_names[BAD_STACK_LEFT_CANARY]);
        return BAD_STACK_LEFT_CANARY;
    }

    else if(stk->right_canary != STACK_CANARY) {
        printf(RED("%s\n"), errors_names[BAD_STACK_RIGHT_CANARY]);
        return BAD_STACK_RIGHT_CANARY;
    }

    #endif

    return NO_ERROR;
}

void StackReallocation(Stack_t* stk, FunkId id) {
    STACK_ASSERT(stk);


    // *((Canary_t *)(stk->data + stk->capacity)) = 0;
    ON_DEBUG(stk->data[stk->capacity] = 0;) //delete right canary

    if(id == PUSH_ID) {
        stk->capacity *= 2;
        #ifdef DEBUG

        stk->data = (StackElem_t *) ((char *) stk->data - sizeof(Canary_t));
        stk->data = (StackElem_t *) realloc (stk->data, stk->capacity * sizeof (StackElem_t) + 2 * sizeof (Canary_t));
        // my_assert (stk->data != NULL)
        stk->data = (StackElem_t *) ((char *) stk->data + sizeof(Canary_t));
        *((Canary_t *)(stk->data + stk->capacity)) = DATA_CANARY;

        #else

        stk->data = (StackElem_t *) realloc(stk->data, sizeof(StackElem_t) * stk->capacity);

        #endif

        assert(stk->data);
    }
    else if(id == POP_ID) {
        stk->capacity /= 2;
        #ifdef DEBUG

        stk->data = (StackElem_t *) ((char *) stk->data - sizeof(Canary_t));
        stk->data = (StackElem_t *) realloc (stk->data, stk->capacity * sizeof (StackElem_t) + 2 * sizeof (Canary_t));
        // my_assert (stk->data != NULL)
        stk->data = (StackElem_t *) ((char *) stk->data + sizeof(Canary_t));
        *((Canary_t *)(stk->data + stk->capacity)) = DATA_CANARY;

        #else

        stk->data = (StackElem_t *) realloc(stk->data, sizeof(StackElem_t) * stk->capacity);

        #endif

        assert(stk->data);
    }
    PoisonMaker(stk);

    // ON_DEBUG(stk->hash = Hash(stk);)

    STACK_ASSERT(stk);
}

void PoisonMaker(Stack_t* stk) {
    for(size_t i = stk->position; i < stk->capacity; i++) {
        stk->data[i] = POISON;
    }
}

Hash_t Hash(const Stack_t* stk) {
    Hash_t hash = 0, high;
    for (size_t i = 0; i < stk->position; i++) {
        hash = (hash << 4) + stk->data[i];
        if (high = hash & 0xF0000000) {
            hash ^= high >> 24;
        }
        hash &= ~high;
    }
    return hash;
}