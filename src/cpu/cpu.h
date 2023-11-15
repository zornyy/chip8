//
// Created by Zorny on 07.09.2023.
//

#ifndef ALEXZORN_CHIP8_CPU_H
#define ALEXZORN_CHIP8_CPU_H

#include <stdint.h>
#include "../Stack/stack.h"

typedef struct {
    uint16_t content;
    uint16_t nnn;
    uint8_t kk;
    uint8_t x;
    uint8_t y;
    uint8_t n;
} opcode_t;

typedef struct {
    uint8_t ram[4096];
    Stack stack;
    uint8_t V[16];
    uint16_t I;
    uint16_t PC;
    uint8_t delay_timer;
    uint8_t sound_timer;
    _Bool paused;
    uint8_t pauseRegister; 
    _Bool stepMode;
    int opcodesToExecute;
    int speed;
} chip8_t;

typedef struct {
    uint8_t code[3584];
    size_t size;
} program;

// Global Variables
extern chip8_t CHIP8;
extern opcode_t opcode;

/*
 * Function initializes the CPU values for program start
 */
void initCPU( );

/*
 * Function loads CHIP-8 font sprites into memory starting at address 0x000
 */
void loadSpritesIntoMemory();

/*
 * Function loads the rom specified in the cpu.c file and loads it into memory starting at address 0x200
 */
int loadRom( char *ROM_PATH );

/*
 * Function executes the CPU action for one cycle of the CHIP-8 emulator (runs at 60Hz)
 */
void cpuCycle( );

#endif //ALEXZORN_CHIP8_CPU_H
