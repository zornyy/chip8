//
// Created by Zorny on 07.09.2023.
//


#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#include "cpu.h"
#include "display.h"
#include "keyboard.h"

int sum;
int carry;

chip8_t CHIP8;

program Program;


int spriteWidth;
int spriteHeight;
int sprite;

void initCPU( ) {
    CHIP8.PC = 0x200;
    CHIP8.speed = 10;
    CHIP8.delay_timer = 0;
    CHIP8.sound_timer = 10;
    CHIP8.paused = false;
    CHIP8.stack.top = 0;

    srand(time(0));

    SDL_Log("CPU initialized");
}

int generateRandom( ) {
    int lower = 0x00;
    int higher = 0xFF;

    int num = (rand() % ( higher - lower +1)) + lower;
    return num;
}

void setOpcodeValues( opcode_t *opcode ) {
    opcode->content = (CHIP8.ram[CHIP8.PC] << 8 | CHIP8.ram[CHIP8.PC + 1] );
    opcode->x = (opcode->content & 0x0F00) >> 8;
    opcode->y = (opcode->content & 0x00F0) >> 4;
    opcode->kk = opcode->content & 0xFF;
    opcode->n = opcode->content & 0xF;
    opcode->nnn = opcode->content & 0xFFF;
}

void loadSpritesIntoMemory( ) {

    // Characters sprites
    uint8_t sprites[80] = {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // load sprites into memory
    // load sprites starting at 0x000
    for ( int i = 0; i < 80; i++ ) {
        CHIP8.ram[i] = sprites[i];
    }
    SDL_Log( "Sprites loaded into RAM\n" );
}

void loadProgramIntoMemory( program loadingProgram ) {
    SDL_Log( "ROM loading started" );
    for ( int i = 0; i < loadingProgram.size; i++ ) {
        CHIP8.ram[i + 0x200] = loadingProgram.code[i];
    }
    SDL_Log( "ROM successfully loaded into RAM" );
}


int loadRom( char *ROM_PATH ) {

    // Get the rom
    FILE *rom = fopen( ROM_PATH, "rb" );
    if (!rom) {
        SDL_Log( "Rom file %s is misspelled or does not exist\n", ROM_PATH );
        return 1;
    }

    // Get rom size
    fseek( rom, 0, SEEK_END );
    Program.size = ftell( rom );
    // the program can't exceed the 4kb of memory and starts at 0x200
    const size_t max_size = sizeof CHIP8.ram - 0x200;
    rewind( rom );

    if ( Program.size > max_size ) {
        SDL_Log( "Rom file %s is too big and cannot fit into memory\n", ROM_PATH );
        return 1;
    }

    if ( fread(&Program.code[0], Program.size, 1, rom ) != 1 ) {
        SDL_Log( "Could not read from file %s", ROM_PATH );
        return 1;
    }
    loadProgramIntoMemory( Program );

    return 0;
}

int executeOpcode( opcode_t *opcode ) {
    SDL_Log( "Executing opcode %x", opcode->content );

    CHIP8.PC += 2;

    // Switch case for the opcodes
    switch ( opcode->content >> 12 ) {
        case 0x0:
            switch ( opcode->content ) {
                case 0x00E0:
                    clearDisplay();
                    break;
                case 0x00EE:
                    CHIP8.PC = pop( &CHIP8.stack );
                    break;
            }
            break;
        case 0x1:
            CHIP8.PC = opcode->nnn;
            break;
        case 0x2:
            push( &CHIP8.stack, CHIP8.PC );
            CHIP8.PC = opcode->nnn;
            break;
        case 0x3:
            if ( CHIP8.V[opcode->x] == opcode->kk ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x4:
            if ( CHIP8.V[opcode->x] != opcode->kk ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x5:
            if ( CHIP8.V[opcode->x] == CHIP8.V[opcode->y] ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x6:
            CHIP8.V[opcode->x] = opcode->kk;
            break;
        case 0x7:
            CHIP8.V[opcode->x] += opcode->kk;
            break;
        case 0x8:
            switch ( opcode->content & 0xF ) {
                case 0x0:
                    CHIP8.V[opcode->x] = CHIP8.V[opcode->y];
                    break;
                case 0x1:
                    CHIP8.V[opcode->x] |= CHIP8.V[opcode->y];
                    break;
                case 0x2:
                    CHIP8.V[opcode->x] &= CHIP8.V[opcode->y];
                    break;
                case 0x3:
                    CHIP8.V[opcode->x] ^= CHIP8.V[opcode->y];
                    break;
                case 0x4:
                    sum = CHIP8.V[opcode->x] + CHIP8.V[opcode->y];
                    CHIP8.V[opcode->x] = sum;

                    if ( sum > 0xFF ) {
                        CHIP8.V[0xF] = 1;
                    } else {
                        CHIP8.V[0xF] = 0;
                    }
                    break;
                case 0x5:
                    carry = CHIP8.V[opcode->x] >= CHIP8.V[opcode->y];

                    CHIP8.V[opcode->x] -= CHIP8.V[opcode->y];
                    CHIP8.V[0xF] = carry;
                    break;
                case 0x6:
                    carry = CHIP8.V[opcode->y] & 1;
                    CHIP8.V[opcode->x] >>= 1;

                    CHIP8.V[0xF] = carry;
                    break;
                case 0x7:
                    carry = CHIP8.V[opcode->y] > CHIP8.V[opcode->x];

                    CHIP8.V[opcode->x] = CHIP8.V[opcode->y] - CHIP8.V[opcode->x];
                    CHIP8.V[0xF] = carry;
                    break;
                case 0xE:
                    carry = (CHIP8.V[opcode->x] & 0x80 ) >> 7;
                    CHIP8.V[opcode->x] <<= 1;

                    CHIP8.V[0xF] = carry;
                    break;
            }

            break;
        case 0x9:
            if ( CHIP8.V[opcode->x] != CHIP8.V[opcode->y] ) {
                CHIP8.PC += 2;
            }
            break;
        case 0xA:
            CHIP8.I = opcode->nnn;
            break;
        case 0xB:
            CHIP8.PC = opcode->nnn + CHIP8.V[0];
            break;
        case 0xC:
            CHIP8.V[opcode->x] = generateRandom( ) & opcode->kk;

            break;
        case 0xD:
            spriteWidth = 8; // Sprites are always 8 pixels wide
            spriteHeight = opcode->n; // Height of the sprite according to it's size

            for ( int row = 0; row < spriteHeight; row++ ) { // loop through the rows
                sprite = CHIP8.ram[CHIP8.I + row];

                for ( int col = 0; col < spriteWidth; col++ ) { // loop through the pixels
                    if (( sprite & 0x80 ) > 0 ) { // if pixel is white in sprite
                        CHIP8.V[0xF] = setPixel( CHIP8.V[opcode->x] + col, CHIP8.V[opcode->y] + row ); // set pixel
                    }

                    sprite <<= 1;
                }
            }
            break;
        case 0xE:
            switch ( opcode->content & 0xFF ) {
                case 0x9E:
                    if ( keyboardState[CHIP8.V[opcode->x]] == true ) {
                        CHIP8.PC += 2;
                    }
                    break;
                case 0xA1:
                    if ( keyboardState[CHIP8.V[opcode->x]] == false ) {
                        CHIP8.PC += 2;
                    }
                    break;
            }

            break;
        case 0xF:
            switch ( opcode->content & 0xFF ) {
                case 0x07:
                    CHIP8.V[opcode->x] = CHIP8.delay_timer;
                    break;
                case 0x0A:
                    // TODO: Pause until keypress instruction
                    CHIP8.paused = true;
                    SDL_Log("paused");
                    break;
                case 0x15:
                    CHIP8.delay_timer = CHIP8.V[opcode->x];
                    break;
                case 0x18:
                    CHIP8.sound_timer = CHIP8.V[opcode->x];
                    break;
                case 0x1E:
                    CHIP8.I = CHIP8.I + CHIP8.V[opcode->x];
                    break;
                case 0x29:
                    CHIP8.I = CHIP8.V[opcode->x] * 5;
                    break;
                case 0x33:
                    CHIP8.ram[CHIP8.I] = CHIP8.V[opcode->x] / 100 % 10;
                    CHIP8.ram[CHIP8.I + 1] = CHIP8.V[opcode->x] / 10 % 10;
                    CHIP8.ram[CHIP8.I + 2] = CHIP8.V[opcode->x] % 10;
                    break;
                case 0x55:
                    for ( int idx = 0; idx <= opcode->x; idx++ ) {
                        CHIP8.ram[CHIP8.I + idx] = CHIP8.V[idx];
                    }
                    break;
                case 0x65:
                    for ( int idx = 0; idx <= opcode->x; idx++ ) {
                        CHIP8.V[idx] = CHIP8.ram[CHIP8.I + idx];
                    }
                    break;
            }
            break;
        default:
            SDL_Log( "Unknown opcode %x", opcode->content );
    }

    return 0;
}

void playSound( ) {
    if ( CHIP8.sound_timer > 0 ) {
        // Beep( 1000, 1000 );
        // Beep function must be better understood outside of class because of noise
        // Todo: work on beep
    }
}

void updateTimers( ) {
    if ( CHIP8.delay_timer > 0 ) {
        CHIP8.delay_timer --;
    }
    if ( CHIP8.sound_timer > 0 ) {
        CHIP8.sound_timer --;
    }
}

void cpuCycle( ) {
    for ( int i = 0; i < CHIP8.speed; i++ ) {
        if ( !CHIP8.paused ) {
            opcode_t opcode;
            setOpcodeValues( &opcode );

            // Do not execute opcode if out of the program
            if ( CHIP8.PC <= Program.size + 0x200 - 1 && CHIP8.PC >= 0x200 ) {
                executeOpcode( &opcode );
            }
        }
    }

    if ( !CHIP8.paused ) {
        updateTimers();
    }
    playSound( );
}