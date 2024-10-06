#ifndef ERRORS_HPP
#define ERRORS_HPP

#define ERR(str) "\033[31m" str "\033[0m"

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
    BAD_DATA_HASH,
    BAD_STACK_HASH,
    BAD_HASH,
    BAD_DATA_CANARIES,
    BAD_STACK_CANARIES
};

const static char* errors_names[] = {"NO_ERROR",
                              "SIZE_ERROR",
                              "STACK_UNDERFLOW",
                              "NO_STACK", "BAD_CAPACITY", "NO_DATA",
                              "BAD_DATA_RIGHT_CANARY", "BAD_DATA_LEFT_CANARY",
                              "BAD_STACK_RIGHT_CANARY", "BAD_STACK_LEFT_CANARY", "BAD_HASH"};

#define CHECKED_ if(!err) err =

#define STACK_ASSERT(stk) {                             \
    if (StackVerification(stk) != NO_ERROR) {           \
            STACK_DUMP(stk);                            \
            exit(1);                                    \
    }                                                   \
}

#define MY_ASSERT(expression) if(!(expression)) {                                                           \
    printf(ERR("%s: %d (%s) My assertion failed: \"" #expression "\"\n"), __FILE__, __LINE__, __func__);    \
    exit(1);                                                                                                \
}                                                                                                           \

#endif // ERRORS_HPP