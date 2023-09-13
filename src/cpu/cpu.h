//
// Created by Student on 07.09.2023.
//

#ifndef ALEXZORN_CHIP8_CPU_H
#define ALEXZORN_CHIP8_CPU_H

#include <stdint.h>
typedef struct {
    uint8_t ram[4096];
    uint16_t stack[12];
    uint8_t V[16];
    uint16_t I;
    uint16_t PC;
    uint8_t delay_timer;
    uint8_t sound_timer;
    int speed;
} chip8_t;

typedef struct {
    uint8_t code[3584];
    size_t size;
} program;

// Global Variables
extern chip8_t CHIP8;
extern char ROM_PATH[100];

void initCPU( );

void loadSpritesIntoMemory();

int loadRom( );

#endif //ALEXZORN_CHIP8_CPU_H
