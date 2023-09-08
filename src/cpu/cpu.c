//
// Created by Student on 07.09.2023.
//

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "cpu.h"
#include "stack.h"



// Whole memory of the CHIP-8 computer
uint8_t memory[4096];

// Registers
uint8_t v[16];

// Memory pointers
int i = 0;

// Timers
int delayTimer = 0;
int soundTimer = 0;

// PC Program Counter, current executing address
int pc = 0x200;

// Stack
struct Stack stack;

// Execution flow
bool isPaused = false;

int speed = 10;


int loadSpritesIntoMemory( ) {
    // TODO
    return 0;
}

void loadProgramIntoMemory( ) {
    // TODO
}

int loadRom( ) {
    // TODO
    return 0;
}