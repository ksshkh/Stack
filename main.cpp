#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef int Element;

struct Stack {
    Element* data;
    int size;
    int capacity;
};

void StackCtor(Stack* stk, int initCapacity);

void StackDtor(Stack* stk);

void StackPush(Stack* stk, Element el);

Element StackPop(Stack* stk);

void StackDump(Stack stk);

int main(void) {
    Stack stk = {};
    int initCapacity = 0;
    scanf("%d", &initCapacity);
    StackCtor(&stk, initCapacity);
    StackPush(&stk, 10);
    StackDump(stk);
    StackPush(&stk, 20);
    StackDump(stk);
    printf("Pop: %d\n", StackPop(&stk));
    StackDump(stk);
    StackDtor(&stk);
    return 0;
}

void StackCtor(Stack* stk, int initCapacity) {
    stk->data = (int*)calloc(initCapacity, sizeof(Element));
    stk->capacity = initCapacity;
    stk->size = 0;
}

void StackDtor(Stack* stk) {
    free(stk->data);
    stk->data = NULL;
    stk->capacity = -1;
    stk->size = -1;
}

void StackPush(Stack* stk, Element el) {
    Element* tempPtr;
    if (stk->size == stk->capacity) {
        tempPtr = (int*)realloc(stk->data, 2 * stk->capacity);
        assert(tempPtr);
        stk->data = tempPtr;
        stk->capacity *= 2;
    }

    stk->data[stk->size] = el;
    stk->size++;
}

Element StackPop(Stack* stk) {
    if (stk->size == 0) {
        return -1;
    }
    stk->size--;
    int el = stk->data[stk->size];
    return el;
}

void StackDump(Stack stk) {
    printf("Address: %p\n", stk.data);
    printf("Capacity: %d\n", stk.capacity);
    printf("Size: %d\n", stk.size);
    printf("Stack:\n");
    for(int i = 0; i < stk.size; i++) {
        printf("[%d]\n", stk.data[i]);
    }
}
