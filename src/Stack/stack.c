//
// Created by Student on 07.09.2023.
//

#include "stack.h"
#include <stdbool.h>




bool isEmpty( Stack stack ) {
    if ( stack.top == 0 ) {
        return true;
    }
    return false;
}

bool isFull( Stack stack ) {
    if ( stack.top == 16 ) {
        return true;
    }
    return false;
}

int push( Stack stack, uint8_t value ) {
    if ( !isFull( stack ) ) {
        stack.content[stack.top] = value;
        stack.top++;
        return 0;
    }
    return 1;
}

int pop( Stack stack ) {
    if ( !isEmpty( stack ) ) {
        stack.top--;
        return stack.content[stack.top];
    }
    return -1;
}

uint8_t top( Stack stack ) {
    return stack.content[stack.top - 1];
}
