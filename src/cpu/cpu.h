//
// Created by Student on 07.09.2023.
//

#ifndef ALEXZORN_CHIP8_CPU_H
#define ALEXZORN_CHIP8_CPU_H

#include <stdint.h>

extern uint8_t memory[4096];

extern uint8_t v[16];

extern int i;

extern int delayTimer;
extern int soundTimer;

extern int pc;

extern struct Stack stack;

extern _Bool isPaused;

extern int speed;

#endif //ALEXZORN_CHIP8_CPU_H
