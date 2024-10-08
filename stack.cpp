#include "stack.hpp"

const char * DEBUG_FILE_NAME = "./dump.txt";

Errors StackCtor(Stack_t* stk, size_t initCapacity, const char* file, const char* func, int line) {

    MY_ASSERT(stk != NULL, PTR_ERROR);
    MY_ASSERT(initCapacity != 0, SIZE_ERROR);

    ON_DEBUG(stk->file = file;)
    ON_DEBUG(stk->func = func;)
    ON_DEBUG(stk->line = line;)

    stk->debug_file_name = DEBUG_FILE_NAME;

#ifdef DEBUG

    stk->data = (StackElem_t*)calloc(initCapacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t), sizeof(char));
    MY_ASSERT (stk->data != NULL, PTR_ERROR)

    *((Canary_t*)stk->data) = DATA_CANARY;
    stk->data = (StackElem_t*)((char*) stk->data + sizeof(Canary_t));
    *((Canary_t*)(stk->data + initCapacity)) = DATA_CANARY;

    stk->left_canary = STACK_CANARY;
    stk->right_canary = STACK_CANARY;

#else

    stk->data = (StackElem_t*)calloc(initCapacity * sizeof(StackElem_t), sizeof(StackElem_t));
    MY_ASSERT(stk->data != NULL);

#endif

    stk->capacity = initCapacity;
    stk->position = 0;

    PoisonMaker(stk);

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return NO_ERROR;
}

Errors StackDtor(Stack_t* stk) {
    STACK_ASSERT(stk);

    ON_DEBUG(stk->data = (StackElem_t *) ((char *) stk->data - sizeof(Canary_t));)
    free(stk->data);
    stk->data = NULL;

    stk->capacity = 0;
    stk->position = 0;

    #ifdef DEBUG
    stk->data_hash = 0;
    stk->file = 0;
    stk->func = 0;
    stk->line = 0;
    stk->left_canary = 0;
    stk->right_canary = 0;
    stk->stack_hash = 0;
    stk->debug_file_name = NULL;
    #endif

    return NO_ERROR;
}

Errors StackPush(Stack_t* stk, StackElem_t el) {
    STACK_ASSERT(stk);

    if(stk->position == stk->capacity) {
        StackReallocation(stk, PUSH_ID);
    }

    stk->data[stk->position] = el;
    stk->position++;

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return NO_ERROR;
}

Errors StackPop(Stack_t* stk, StackElem_t* x) {
    STACK_ASSERT(stk);

    MY_ASSERT(stk->position != 0, STACK_UNDERFLOW)

    if(stk->capacity > (stk->position - 1) * 3) {
        StackReallocation(stk, POP_ID);
    }

    stk->position--;
    *x = stk->data[stk->position];
    stk->data[stk->position] = POISON;

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return NO_ERROR;
}

void StackDump(Stack_t* stk, const char* file, const char* func, int line, Errors err) {

    FILE* debug_file = fopen(stk->debug_file_name, "a");

    if(debug_file != NULL) {

        fprintf(debug_file, "------------------------------------\n");
        fprintf(debug_file, "called from %s: %d (%s)\n", file, line, func);

        if(err != NO_ERROR) {
            fprintf(debug_file, ERR("%s\n"), errors_names[err]);
        }

        fprintf(debug_file, "Stack_t [%p]\n", stk);

        if(stk != NULL) {

            ON_DEBUG(fprintf(debug_file, "stack born at %s: %d (%s)\n", stk->file, stk->line, stk->func);)

            ErrorPrint(stk, debug_file);

            ON_DEBUG(fprintf(debug_file, "stack hash: %llu\n", stk->stack_hash);)
            ON_DEBUG(fprintf(debug_file, "data hash: %llu\n", stk->data_hash);)

            ON_DEBUG(fprintf(debug_file, "stack left canary: %#llX\n", stk->left_canary);)

            fprintf(debug_file, "capacity = %lu\n", stk->capacity);
            fprintf(debug_file, "position = %lu\n", stk->position);
            fprintf(debug_file, "data [%p]\n", stk->data);

            if(stk->data != NULL) {
                fprintf(debug_file, "{\n");

                ON_DEBUG(fprintf(debug_file, "\tdata left canary: %#llX\n", *((Canary_t*) stk->data - 1));)

                if(stk->capacity != 0) {
                    for(size_t i = 0; i < stk->capacity; i++) {
                        if(i < stk->position) {
                            fprintf(debug_file, "\t*[%lu] = %d\n", i, stk->data[i]);
                        }
                        else {
                            fprintf(debug_file, "\t[%lu] = %d(POISON)\n", i, stk->data[i]);
                        }
                    }
                }
                else {
                    fprintf(debug_file, ERR("%s\n"), errors_names[BAD_CAPACITY]);
                }

                ON_DEBUG(fprintf(debug_file, "\tdata right canary: %#llX\n", *((Canary_t*)(stk->data + stk->capacity)));)
                fprintf(debug_file, "}\n");
            }
            else {
                fprintf(debug_file, ERR("%s\n"), errors_names[NO_DATA]);
            }

            ON_DEBUG(fprintf(debug_file, "stack right canary: %#llX\n", stk->right_canary);)
        }
        else {
            fprintf(debug_file, ERR("%s\n"), errors_names[NO_STACK]);
        }

        fprintf(debug_file, "------------------------------------\n");

        fclose(debug_file);
    }
    else {
        fprintf(stderr, "file did not open\n");
    }
}

Errors StackVerification(const Stack_t* stk) {
    if(!stk) {
        return NO_STACK;
    }

    else if(!stk->capacity) {
        return BAD_CAPACITY;
    }

    else if(stk->capacity < stk->position) {
        return SIZE_ERROR;
    }

    else if(!stk->data) {
        return NO_DATA;
    }

#ifdef DEBUG

    else if(stk->data_hash != DataHash(stk) && stk->stack_hash != StackHash(stk)) {
        return BAD_HASH;
    }

    else if(stk->data_hash != DataHash(stk)) {
        return BAD_DATA_HASH;
    }

    else if(stk->stack_hash != StackHash(stk)) {
        return BAD_STACK_HASH;
    }

    else if(stk->left_canary != STACK_CANARY && stk->right_canary != STACK_CANARY) {
        return BAD_STACK_CANARIES;
    }

    else if(stk->left_canary != STACK_CANARY) {
        return BAD_STACK_LEFT_CANARY;
    }

    else if(stk->right_canary != STACK_CANARY) {
        return BAD_STACK_RIGHT_CANARY;
    }

    else if(*((Canary_t *) stk->data - 1) != DATA_CANARY && *((Canary_t *)(stk->data + stk->capacity)) != DATA_CANARY) {
        return BAD_DATA_CANARIES;
    }

    else if(*((Canary_t *) stk->data - 1) != DATA_CANARY) {
        return BAD_DATA_LEFT_CANARY;
    }

    else if(*((Canary_t *)(stk->data + stk->capacity)) != DATA_CANARY) {
        return BAD_DATA_RIGHT_CANARY;
    }

    #endif

    return NO_ERROR;
}

Errors StackReallocation(Stack_t* stk, FunkId id) {
    STACK_ASSERT(stk);

    if(id == PUSH_ID) {
        stk->capacity *= 2;

#ifdef DEBUG

        stk->data = (StackElem_t*)((char*)stk->data - sizeof(Canary_t));
        stk->data = (StackElem_t*)realloc(stk->data, stk->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t));
        MY_ASSERT(stk->data != NULL, NO_DATA);
        stk->data = (StackElem_t*)((char*)stk->data + sizeof(Canary_t));
        *((Canary_t*)(stk->data + stk->capacity)) = DATA_CANARY;

#else

        stk->data = (StackElem_t*) realloc(stk->data, sizeof(StackElem_t) * stk->capacity);
        MY_ASSERT(stk->data != NULL);

#endif
    }
    else if(id == POP_ID) {
        stk->capacity /= 2;

        #ifdef DEBUG

        stk->data = (StackElem_t*)((char*)stk->data - sizeof(Canary_t));
        stk->data = (StackElem_t*)realloc(stk->data, stk->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t));
        MY_ASSERT(stk->data != NULL, NO_DATA);
        stk->data = (StackElem_t*)((char*)stk->data + sizeof(Canary_t));
        *((Canary_t*)(stk->data + stk->capacity)) = DATA_CANARY;

        #else

        stk->data = (StackElem_t*)realloc(stk->data, sizeof(StackElem_t) * stk->capacity);
        MY_ASSERT(stk->data != NULL);

        #endif
    }

    PoisonMaker(stk);

    ON_DEBUG(stk->data_hash = DataHash(stk);)
    ON_DEBUG(stk->stack_hash = StackHash(stk);)

    STACK_ASSERT(stk);
    return NO_ERROR;
}

void PoisonMaker(Stack_t* stk) {
    for(size_t i = stk->position; i < stk->capacity; i++) {
        stk->data[i] = POISON;
    }
}

Hash_t DataHash(const Stack_t* stk) {
    Hash_t hash = 0, high = 0;
    for (size_t i = 0; i < stk->position; i++) {
        hash = (hash << 4) + stk->data[i];
        if (high = hash & 0xF0000000) {
            hash ^= high >> 24;
        }
        hash &= ~high;
    }
    return hash;
}

Hash_t StackHash(const Stack_t* stk) {
    Hash_t hash = 0;
    StackElem_t data_sum = 0;
    for (size_t i = 0; i < stk->position; i++) {
        data_sum += stk->data[i];
    }

    hash += data_sum;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += stk->capacity;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += stk->position;
    hash += hash << 10;
    hash ^= hash >> 6;

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

void ErrorSave(Stack_t* stk, Errors err) {
    stk->error_list |= (1 << err);
}

void ErrorPrint(Stack_t* stk, FILE* debug_file) {
    int n = stk->error_list;
    fprintf(debug_file, "errors: ");
    for(int i = sizeof(n) - 1; i >= 0; i--) {
        fprintf(debug_file, "%u", (n >> i) & 1);
    }
    fprintf(debug_file, "\n");
}