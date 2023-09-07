//
// Created by Student on 07.09.2023.
//

#include "stack.h"
#include <stdbool.h>




bool isEmpty( struct Stack stack ) {
    if ( stack.top == 0 ) {
        return true;
    }
    return false;
}

bool isFull( struct Stack stack ) {
    if ( stack.top == 16 ) {
        return true;
    }
    return false;
}

int push( struct Stack stack, uint8_t value ) {
    if ( !isFull( stack ) ) {
        stack.content[stack.top] = value;
        stack.top++;
        return 0;
    }
    return 1;
}

int pop( struct Stack stack ) {
    if ( !isEmpty( stack ) ) {
        stack.top--;
        return 0;
    }
    return 1;
}

uint8_t top( struct Stack stack ) {
    return stack.content[stack.top - 1];
}

void test( ) {
    struct Stack st;
    st.top = 0;
}


