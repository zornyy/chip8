//
// Created by Zorny on 07.09.2023.
//


#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <time.h>
// #include <windows.h>

#include "../cpu/cpu.h"
#include "../Display/display.h"
#include "../Keyboard/keyboard.h"

int sum;
int carry;

chip8_t CHIP8;
opcode_t opcode;

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
    CHIP8.stepMode = false;
    CHIP8.opcodesToExecute = 0;
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

    CHIP8.PC += 2;

    // Switch case for the opcodes
    switch ( opcode->content >> 12 ) {
        case 0x0:
            switch ( opcode->content ) {
                case 0x00E0: // 00E0 - CLS -> clear display
                    clearDisplay();
                    break;
                case 0x00EE: // 00EE - RET -> Return from subroutine
                    CHIP8.PC = pop( &CHIP8.stack );
                    break;
            }
            break;
        case 0x1: // 1nnn - JP addr -> set PC to nnn
            CHIP8.PC = opcode->nnn;
            break;
        case 0x2: // 2nnn - CALL addr -> call subroutine at nnn
            push( &CHIP8.stack, CHIP8.PC );
            CHIP8.PC = opcode->nnn;
            break;
        case 0x3: // 3xkk - SE Vx, byte -> Skip next if Vx = kk
            if ( CHIP8.V[opcode->x] == opcode->kk ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x4: // 4xkk - SNE Vx, byte -> Skip next if Vx != kk
            if ( CHIP8.V[opcode->x] != opcode->kk ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x5: // 5xy0 - SE Vx, Vy -> Skip next if Vx = Vy
            if ( CHIP8.V[opcode->x] == CHIP8.V[opcode->y] ) {
                CHIP8.PC += 2;
            }
            break;
        case 0x6: // 6xkk - LS Vx, byte -> Set Vx = kk
            CHIP8.V[opcode->x] = opcode->kk;
            break;
        case 0x7: // 7xkk - ADD Vx, byte -> Set Vx = Vx + Vy
            CHIP8.V[opcode->x] += opcode->kk;
            break;
        case 0x8:
            switch ( opcode->content & 0xF ) {
                case 0x0: // 8xy0 - LD Vx, Vy -> Set  Vx = Vy
                    CHIP8.V[opcode->x] = CHIP8.V[opcode->y];
                    break;
                case 0x1: // 8xy1 - OR Vx, Vy -> Set Vx = Vx OR Vy
                    CHIP8.V[opcode->x] |= CHIP8.V[opcode->y];
                    break;
                case 0x2: // 8xy2 - AND Vx, Vy -> Set Vx = Vx AND Vy
                    CHIP8.V[opcode->x] &= CHIP8.V[opcode->y];
                    break;
                case 0x3: // 8xy3 - XOR Vx, Vy -> Set Vx = Vx XOR vy
                    CHIP8.V[opcode->x] ^= CHIP8.V[opcode->y];
                    break;
                case 0x4: // 8xy4 - ADD Vx, Vy -> Set Vx = Vx, set VF = carry
                    sum = CHIP8.V[opcode->x] + CHIP8.V[opcode->y];
                    CHIP8.V[opcode->x] = sum;

                    if ( sum > 0xFF ) {
                        CHIP8.V[0xF] = 1;
                    } else {
                        CHIP8.V[0xF] = 0;
                    }
                    break;
                case 0x5: // 8xy5 - SUB Vx, Vy -> Set Vx = Vx - Vy, set VF = NOT borrow
                    carry = CHIP8.V[opcode->x] >= CHIP8.V[opcode->y];

                    CHIP8.V[opcode->x] -= CHIP8.V[opcode->y];
                    CHIP8.V[0xF] = carry;
                    break;
                case 0x6: // 8xy6 - SHR Vx {, Vy} -> Set Vx = Vx SHR 1
                    carry = CHIP8.V[opcode->y] & 1;
                    CHIP8.V[opcode->x] >>= 1;

                    CHIP8.V[0xF] = carry;
                    break;
                case 0x7: // 8xy7 - SUBN Vx, Vy -> Set Vx = Vy - Vx, set VF = NOT borrow
                    carry = CHIP8.V[opcode->y] > CHIP8.V[opcode->x];

                    CHIP8.V[opcode->x] = CHIP8.V[opcode->y] - CHIP8.V[opcode->x];
                    CHIP8.V[0xF] = carry;
                    break;
                case 0xE: // 8xyE - SHL Vx {, Vy} -> Set Vx = Vx SHL 1
                    carry = (CHIP8.V[opcode->x] & 0x80 ) >> 7;
                    CHIP8.V[opcode->x] <<= 1;

                    CHIP8.V[0xF] = carry;
                    break;
            }

            break;
        case 0x9: // 9xy0 - SNE Vx, Vy -> Skip next if Vx != Vy
            if ( CHIP8.V[opcode->x] != CHIP8.V[opcode->y] ) {
                CHIP8.PC += 2;
            }
            break;
        case 0xA: // Annn - LD I, addr -> Set I = nnn
            CHIP8.I = opcode->nnn;
            break;
        case 0xB: // Bnnn - JP V0, addr -> Jump to location NNN + V0
            CHIP8.PC = opcode->nnn + CHIP8.V[0];
            break;
        case 0xC: // Cxkk - RND Vx, byte -> Set Vx = random byte AND kk
            CHIP8.V[opcode->x] = generateRandom( ) & opcode->kk;

            break;
        case 0xD: // Dxyn - DRW Vx, Vy, nibble -> Display n-bytes sprite starting at I at (Vx, Vy), set VF = collision
            
            spriteWidth = 8; // Sprites are always 8 pixels wide
            spriteHeight = opcode->n; // Height of the sprite is provided by n

            for ( int row = 0; row < spriteHeight; row++ ) { // loop through the rows
                sprite = CHIP8.ram[CHIP8.I + row];

                for ( int col = 0; col < spriteWidth; col++ ) { // loop through the pixels
                    if (( sprite & 0x80 ) > 0 ) { // If pixel is TRUE in the sprite
                        CHIP8.V[0xF] = setPixel( CHIP8.V[opcode->x] + col, CHIP8.V[opcode->y] + row ); // set pixel
                    }

                    sprite <<= 1;
                }
            }
            break;
        case 0xE: 
            switch ( opcode->content & 0xFF ) {
                case 0x9E: // Ex9E - SKP Vx -> Skip next if key Vx is pressed
                    if ( keyboardState[CHIP8.V[opcode->x]] == true ) {
                        CHIP8.PC += 2;
                    }
                    break;
                case 0xA1: // ExA1 - SKNP Vx -> Skip next if key Vx is not pressed
                    if ( keyboardState[CHIP8.V[opcode->x]] == false ) {
                        CHIP8.PC += 2;
                    }
                    break;
            }

            break;
        case 0xF: 
            switch ( opcode->content & 0xFF ) {
                case 0x07: // Fx07 - LD Vx, DT -> Set Vx = delay timer value
                    CHIP8.V[opcode->x] = CHIP8.delay_timer;
                    break;
                case 0x0A: // Fx0A - LD Vx, K -> Wait for keypress, store keypressed in Vx
                    CHIP8.paused = true;
                    CHIP8.pauseRegister = opcode->x;
                    SDL_Log("paused");
                    break;
                case 0x15: // Fx15 - LD DT, Vx -> Set delay timer = Vx
                    CHIP8.delay_timer = CHIP8.V[opcode->x];
                    break;
                case 0x18: // Fx18 - LD ST, Vx -> Set soundTimer = Vx
                    CHIP8.sound_timer = CHIP8.V[opcode->x];
                    break;
                case 0x1E: // Fx1E - ADD I, Vx -> Set I = I + Vx
                    CHIP8.I = CHIP8.I + CHIP8.V[opcode->x];
                    break;
                case 0x29: // Fx29 - LD F, Vx -> Set I = location of sprite for digit Vx
                    CHIP8.I = CHIP8.V[opcode->x] * 5; // * 5 since every digit is 5 byte long
                    break;
                case 0x33: // Fx33 - LD F, Vx -> Store BCD representation of Vx in memory locations I, I+1, and I+2
                    CHIP8.ram[CHIP8.I] = CHIP8.V[opcode->x] / 100 % 10; // Hundreds digit
                    CHIP8.ram[CHIP8.I + 1] = CHIP8.V[opcode->x] / 10 % 10; // Tens digit
                    CHIP8.ram[CHIP8.I + 2] = CHIP8.V[opcode->x] % 10; // Ones digit
                    break;
                case 0x55: // Fx55 - LD [i], Vx -> Store registers V0 through Vx into memory at location I
                    for ( int idx = 0; idx <= opcode->x; idx++ ) {
                        CHIP8.ram[CHIP8.I + idx] = CHIP8.V[idx];
                    }
                    break;
                case 0x65: // Fx65 - LD Vx, [I] -> Read registers V0 through Vx from memory starting at location I
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
        if ( !CHIP8.stepMode || CHIP8.opcodesToExecute > 0 ) {
          if ( CHIP8.stepMode ) {
            CHIP8.opcodesToExecute --;
          }
          
          setOpcodeValues( &opcode );

          // Do not execute opcode if out of the program
          if ( CHIP8.PC <= Program.size + 0x200 - 1 && CHIP8.PC >= 0x200 ) {
            executeOpcode( &opcode );
          }
        }
      }
    }

    if ( !CHIP8.paused ) {
        updateTimers( );
    }
    playSound( );
}
