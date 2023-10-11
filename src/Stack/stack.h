//
// Created by Student on 07.09.2023.
//

#ifndef ALEXZORN_CHIP8_STACK_H
#define ALEXZORN_CHIP8_STACK_H
#include <stdint.h>

typedef struct {
    uint8_t content[16];
    int top;
} Stack;

/*
 * Param: stack ( struct Stack ) Stack that will be checked
 * Returns: <true> if <stack> is empty, else returns <false>
 */
_Bool isEmpty( Stack *stack );

/*
 * Param: stack ( struct Stack ) Stack that will be checked
 * Returns: <true> if <stack> is full, else returns <false>
 */
_Bool isFull( Stack *stack );

/*
 * Param: stack ( struct Stack ) stack in which <value> will be pushed
 * Param: value ( unit8_t ) Value to be added to the stack
 * Returns: 0 if <value> has been added to <stack>, if <stack> is already full, returns 1
 */
int push( Stack *stack, uint8_t value );

/*
 * Param: stack ( struct Stack ) stack from which the last element will be removed
 * Returns: the value at the top of the stack before removing it, if <stack> is already empty, returns -1
 */
int pop( Stack *stack );

/*
 * Param: stack ( struct Stack ) Stack that will be checked
 * Returns: the <index> of the top element of the <stack>
 */
uint8_t top( Stack *stack );

#endif //ALEXZORN_CHIP8_STACK_H





