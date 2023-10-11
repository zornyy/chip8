//
// Created by Student on 07.09.2023.
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


chip8_t CHIP8;

char ROM_PATH[] = "../programs/Corax.ch8";

program Program;


// opcodes variables
int x;
int y;

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

    // TODO: Default sprites are not working when used into programs
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


int loadRom( ) {

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

int executeOpcode( uint16_t opcode ) {
    SDL_Log( "Executing opcode %x", opcode );

    // Switch case for the opcodes
    switch ( opcode >> 12 ) {
        case 0x0:
            switch ( opcode ) {
                case 0x00E0:
                    clearDisplay();
                    break;
                case 0x00EE:
                    CHIP8.PC = pop( &CHIP8.stack );
                    break;
            }
            break;
        case 0x1:
            CHIP8.PC = ( opcode & 0xFFF );
            break;
        case 0x2:
            push( &CHIP8.stack, CHIP8.PC );
            CHIP8.PC = ( opcode & 0xFFF );
            break;
        case 0x3:
            x = ( opcode & 0x0F00 ) >> 8;
            if ( CHIP8.V[x] == ( opcode & 0xFF ) ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x4:
            x = ( opcode & 0x0F00 ) >> 8;
            if ( CHIP8.V[x] != ( opcode & 0xFF ) ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x5:
            x = ( opcode & 0x0F00 ) >> 8;
            y = ( opcode & 0x00F0 ) >> 4;
            if ( CHIP8.V[x] == CHIP8.V[y] ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x6:
            x = ( opcode & 0x0F00 ) >> 8;
            CHIP8.V[x] = ( opcode & 0xFF );
            break;
        case 0x7:
            x = ( opcode & 0x0F00 ) >> 8;
            CHIP8.V[x] += ( opcode & 0xFF );
            break;
        case 0x8:
            switch ( opcode & 0xF ) {
                case 0x0:
                    x = ( opcode & 0x0F00 ) >> 8;
                    y = ( opcode & 0x00F0 ) >> 4;
                    CHIP8.V[x] = CHIP8.V[y];
                    break;
                case 0x1:
                    x = ( opcode & 0x0F00 ) >> 8;
                    y = ( opcode & 0x00F0 ) >> 4;
                    CHIP8.V[x] |= CHIP8.V[y];
                    break;
                case 0x2:
                    x = ( opcode & 0x0F00 ) >> 8;
                    y = ( opcode & 0x00F0 ) >> 4;
                    CHIP8.V[x] &= CHIP8.V[y];
                    break;
                case 0x3:
                    x = ( opcode & 0x0F00 ) >> 8;
                    y = ( opcode & 0x00F0 ) >> 4;
                    CHIP8.V[x] ^= CHIP8.V[y];
                    break;
                case 0x4:
                    x = ( opcode & 0x0F00 ) >> 8;
                    y = ( opcode & 0x00F0 ) >> 4;
                    int sum = CHIP8.V[x] + CHIP8.V[y];
                    CHIP8.V[0xF] = 0;

                    if ( sum > 0xFF ) {
                        CHIP8.V[0xF] = 1;
                    }

                    CHIP8.V[x] = sum;
                    break;
                case 0x5:
                    x = ( opcode & 0x0F00 ) >> 8;
                    y = ( opcode & 0x00F0 ) >> 4;

                    CHIP8.V[0xF] = 0;

                    if ( CHIP8.V[x] > CHIP8.V[y] ) {
                        CHIP8.V[0xF] = 1;
                    }

                    CHIP8.V[x] -= CHIP8.V[y];
                    break;
                case 0x6:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.V[0xF] = ( CHIP8.V[x] & 0x1 );

                    CHIP8.V[x] >>= 1;
                    break;
                case 0x7:
                    x = ( opcode & 0x0F00 ) >> 8;
                    y = ( opcode & 0x00F0 ) >> 4;

                    CHIP8.V[0xF] = 0;

                    if ( CHIP8.V[y] > CHIP8.V[x] ) {
                        CHIP8.V[0xF] = 1;
                    }

                    CHIP8.V[x] = CHIP8.V[y] - CHIP8.V[x];
                    break;
                case 0xE:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.V[0xF] = ( CHIP8.V[x] & 0x80 );

                    CHIP8.V[x] <<= 1;
                    break;
            }

            break;
        case 0x9:
            x = ( opcode & 0x0F00 ) >> 8;
            y = ( opcode & 0x00F0 ) >> 4;

            if ( CHIP8.V[x] != CHIP8.V[y] ) {
                CHIP8.PC += 2;
            }
            break;
        case 0xA:
            CHIP8.I = ( opcode & 0xFFF );
            break;
        case 0xB:
            CHIP8.PC = ( opcode & 0xFFF ) + CHIP8.V[0];
            break;
        case 0xC:
            x = ( opcode & 0x0F00 ) >> 8;
            CHIP8.V[x] = generateRandom( ) & ( opcode & 0xFF );

            break;
        case 0xD:
            x = ( opcode & 0x0F00 ) >> 8;
            y = ( opcode & 0x00F0 ) >> 4;

            spriteWidth = 8; // Sprites are always 8 pixels wide
            spriteHeight = ( opcode & 0xF ); // Height of the sprite according to it's size

            for ( int row = 0; row < spriteHeight; row++ ) { // loop through the rows
                sprite = CHIP8.ram[CHIP8.I + row];

                for ( int col = 0; col < spriteWidth; col++ ) { // loop through the pixels
                    if (( sprite & 0x80 ) > 0 ) { // if pixel is white in sprite
                        CHIP8.V[0xF] = setPixel( CHIP8.V[x] + col, CHIP8.V[y] + row ); // set pixel
                    }

                    sprite <<= 1;
                }
            }
            break;
        case 0xE:
            switch ( opcode & 0xFF ) {
                case 0x9E:
                    x = ( opcode & 0x0F00 ) >> 8;
                    if ( keyboardState[CHIP8.V[x]] == true ) {
                        CHIP8.PC += 2;
                    }
                    break;
                case 0xA1:
                    x = ( opcode & 0x0F00 ) >> 8;
                    if ( keyboardState[CHIP8.V[x]] == false ) {
                        CHIP8.PC += 2;
                    }
                    break;
            }

            break;
        case 0xF:
            switch ( opcode & 0xFF ) {
                case 0x07:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.V[x] = CHIP8.delay_timer;
                    break;
                case 0x0A:
                    // TODO: Pause until keypress instruction
                    CHIP8.paused = true;
                    SDL_Log("paused");
                    break;
                case 0x15:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.delay_timer = CHIP8.V[x];
                    break;
                case 0x18:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.sound_timer = CHIP8.V[x];
                    break;
                case 0x1E:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.I = CHIP8.I + CHIP8.V[x];
                    break;
                case 0x29:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.I = CHIP8.V[x] * 5;
                    break;
                case 0x33:
                    x = ( opcode & 0x0F00 ) >> 8;
                    CHIP8.ram[CHIP8.I] = CHIP8.V[x] / 100 % 10;
                    CHIP8.ram[CHIP8.I + 1] = CHIP8.V[x] / 10 % 10;
                    CHIP8.ram[CHIP8.I + 2] = CHIP8.V[x] % 10;
                    break;
                case 0x55:
                    x = ( opcode & 0x0F00 ) >> 8;
                    for ( int idx = 0; idx <= x; idx++ ) {
                        CHIP8.ram[CHIP8.I + idx] = CHIP8.V[idx];
                    }
                    break;
                case 0x65:
                    x = ( opcode & 0x0F00 ) >> 8;
                    for ( int idx; idx <= x; idx++ ) {
                        CHIP8.V[idx]= CHIP8.ram[CHIP8.I + idx];
                    }
                    break;
            }
            break;
        default:
            SDL_Log( "Unknown opcode %x", opcode );
    }


    CHIP8.PC += 2;
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
            uint16_t opcode = (CHIP8.ram[CHIP8.PC] << 8 | CHIP8.ram[CHIP8.PC + 1] );

            // Do not execute opcode if out of the program
            if ( CHIP8.PC <= Program.size + 0x200 - 1 && CHIP8.PC >= 0x200 ) {
                executeOpcode( opcode );
            }
        }
    }

    if ( !CHIP8.paused ) {
        updateTimers();
    }
    playSound( );
}