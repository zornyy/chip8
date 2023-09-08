//
// Created by Student on 07.09.2023.
//

#ifndef ALEXZORN_CHIP8_STACK_H
#define ALEXZORN_CHIP8_STACK_H
#include <stdint.h>

struct Stack {
    uint8_t content[64];
    int top;
};

_Bool isEmpty( struct Stack stack );

_Bool isFull( struct Stack stack );

int push( struct Stack stack, uint8_t value );

int pop( struct Stack stack );

uint8_t top( struct Stack stack );

#endif //ALEXZORN_CHIP8_STACK_H





