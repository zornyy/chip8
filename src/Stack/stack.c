//
// Created by Student on 07.09.2023.
//

#include "SDL.h"
#include "stack.h"
#include <stdbool.h>




bool isEmpty( Stack *stack ) {
    if ( stack->top == 0 ) {
        SDL_Log("Stack is empty");
        return true;
    }
    SDL_Log("Stack is not empty");
    return false;
}

bool isFull( Stack *stack ) {
    if ( stack->top == 17 ) {
        SDL_Log("Stack is FULL");
        return true;
    }
    SDL_Log("Stack is not FULL");
    return false;
}

int push( Stack *stack, uint8_t value ) {
    if ( !isFull( stack ) ) {
        stack->content[stack->top] = value;
        SDL_Log("Pushed '%x' on top of the stack", stack->content[stack->top]);
        stack->top++;
        SDL_Log("Stack top = %d", stack->top);
        return 0;
    }
    return 1;
}

int pop( Stack *stack ) {
    if ( !isEmpty( stack ) ) {
        stack->top--;
        SDL_Log("Popped '%x' from the top of the stack", stack->content[stack->top]);
        return stack->content[stack->top];
    }
    return -1;
}

uint8_t top( Stack *stack ) {
    return stack->top - 1;
}
