//
// Created by Student on 07.09.2023.
//


#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <windows.h>

#include "cpu.h"


chip8_t CHIP8;

char ROM_PATH[] = "E:/EPTM/Modules/PR_PROJ/PRO/ch8_Decoder/ibmLogo.ch8";

program Program;

void initCPU( ) {
    CHIP8.PC = 0x200;
    CHIP8.speed = 10;
    CHIP8.delay_timer = 0;
    CHIP8.sound_timer = 10;
    CHIP8.paused = false;

    SDL_Log("CPU initialized");
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
    // sprites are stored in the memory starting at address 0x000
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
    const size_t max_size = sizeof CHIP8.ram - 0x200; // the program can't exceed the 4kb of memory and starts at 0x200
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
                    break;
                case 0x00EE:
                    break;
            }

            break;
        case 0x1:
            break;
        case 0x2:
            break;
        case 0x3:
            break;
        case 0x4:
            break;
        case 0x5:
            break;
        case 0x6:
            break;
        case 0x7:
            break;
        case 0x8:
            switch ( opcode & 0xF ) {
                case 0x0:
                    break;
                case 0x1:
                    break;
                case 0x2:
                    break;
                case 0x3:
                    break;
                case 0x4:
                    break;
                case 0x5:
                    break;
                case 0x6:
                    break;
                case 0x7:
                    break;
                case 0xE:
                    break;
            }

            break;
        case 0x9:
            break;
        case 0xA:
            break;
        case 0xB:
            break;
        case 0xC:
            break;
        case 0xD:
            break;
        case 0xE:
            switch ( opcode & 0xFF ) {
                case 0x9E:
                    break;
                case 0xA1:
                    break;
            }

            break;
        case 0xF:
            switch ( opcode & 0xFF ) {
                case 0x07:
                    break;
                case 0x0A:
                    break;
                case 0x15:
                    break;
                case 0x18:
                    break;
                case 0x1E:
                    break;
                case 0x29:
                    break;
                case 0x33:
                    break;
                case 0x55:
                    break;
                case 0x65:
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
        // Todo: worlk on beep
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
            if ( CHIP8.PC <= Program.size + 0x200 - 1 ) {
                executeOpcode( opcode );
            }
        }
    }

    if ( !CHIP8.paused ) {
        updateTimers();
    }
    playSound( );
}