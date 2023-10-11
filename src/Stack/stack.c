//
// Created by Student on 07.09.2023.
//

#include "SDL.h"
#include "stack.h"
#include <stdbool.h>




bool isEmpty( Stack *stack ) {
    if ( stack->top == 0 ) {
        return true;
    }
    return false;
}

bool isFull( Stack *stack ) {
    if ( stack->top == 17 ) {
        return true;
    }
    return false;
}

int push( Stack *stack, uint16_t value ) {
    if ( !isFull( stack ) ) {
        stack->content[stack->top] = value;
        stack->top++;
        return 0;
    }
    return 1;
}

uint16_t pop( Stack *stack ) {
    if ( !isEmpty( stack ) ) {
        stack->top--;
        return stack->content[stack->top];
    }
    return -1;
}

int top( Stack *stack ) {
    return stack->top - 1;
}
