//
// Created by Student on 07.09.2023.
//


#include <stdio.h>
#include <SDL.h>

#include "cpu.h"


chip8_t CHIP8;

char ROM_PATH[] = "D:/EPTM/Modules/PR_PROJ/PRO/ch8_Decoder/ibmLogo.ch8";

void initCPU( ) {
    CHIP8.PC = 0x200;
    CHIP8.speed = 10;
    CHIP8.delay_timer = 0;
    CHIP8.sound_timer = 0;

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
    SDL_Log( "Sprites loaded into ram\n" );
}

void loadProgramIntoMemory( program loadingProgram ) {
    SDL_Log( "Rom loading started" );
    for ( int i = 0; i < loadingProgram.size; i++ ) {
        CHIP8.ram[i + 0x200] = loadingProgram.code[i];
    }
    SDL_Log( "ROM successfully loaded into RAM" );
}


int loadRom( ) {

    program loadingProgram;

    // Get the rom
    FILE *rom = fopen( ROM_PATH, "rb" );
    if (!rom) {
        SDL_Log( "Rom file %s is misspelled or does not exist\n", ROM_PATH );
        return 1;
    }

    // Get rom size
    fseek( rom, 0, SEEK_END );
    loadingProgram.size = ftell( rom );
    const size_t max_size = sizeof CHIP8.ram - 0x200; // the program can't exceed the 4kb of memory and starts at 0x200
    rewind( rom );

    if ( loadingProgram.size > max_size ) {
        SDL_Log( "Rom file %s is too big and cannot fit into memory\n", ROM_PATH );
        return 1;
    }

    if ( fread(&loadingProgram.code[0], loadingProgram.size, 1, rom ) != 1 ) {
        SDL_Log( "Could not read from file %s", ROM_PATH );
        return 1;
    }

    SDL_Log("Secon part\n" );

    loadProgramIntoMemory( loadingProgram );

    return 0;
}