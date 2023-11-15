//
// Created by Zorny on 06.09.2023.
//

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "keyboard.h"
#include "../cpu/cpu.h"

bool keyboardState[16];



void initKeyboard( ) {
    // Set keys to false
    for ( int i=0; i<16; i++ ) {
        keyboardState[i] = false;
    }
    SDL_Log("Keyboard is initialized");
}

void pressKey( int keyCode ) {
    SDL_Log("Key pressed: %d", keyCode);
    
    int keyValue;

    switch ( keyCode ) {
        case 49 :
          keyboardState[0] = true;
          keyValue = 0;  
          break;
        case SDLK_2 :
          keyboardState[1] = true;
          keyValue = 1;
          break;
        case SDLK_3 :
          keyboardState[2] = true;
          keyValue = 2;
          break;
        case SDLK_4 :
          keyboardState[3] = true;
          keyValue = 3;
          break;
        case SDLK_q :
          keyboardState[4] = true;
          keyValue = 4;
          break;
        case SDLK_w :
          keyboardState[5] = true;
          keyValue = 5;
          break;
        case SDLK_e :
          keyboardState[6] = true;
          keyValue = 6;
          break;
        case SDLK_r :
          keyboardState[7] = true;
          keyValue = 7;
          break;
        case SDLK_a :
          keyboardState[8] = true;
          keyValue = 8;
          break;
        case SDLK_s :
          keyboardState[9] = true;
          keyValue = 9;
          break;
        case SDLK_d :
          keyboardState[10] = true;
          keyValue = 10;
          break;
        case SDLK_f :
          keyboardState[11] = true;
          keyValue = 11;
          break;
        case SDLK_y :
          keyboardState[12] = true;
          keyValue = 12;
          break;
        case SDLK_x :
          keyboardState[13] = true;
          keyValue = 13;
          break;
        case SDLK_c :
          keyboardState[14] = true;
          keyValue = 14;
          break;
        case SDLK_v :
          keyboardState[15] = true;
          keyValue = 15;
          break;
    }

  if ( CHIP8.paused > 15 ) {
    CHIP8.V[CHIP8.pauseRegister] = keyValue;
    CHIP8.paused = false;
  }
}

void releaseKey( int keyCode ) {
    SDL_Log("Key released: %d", keyCode);
    switch ( keyCode ) {
        case SDLK_1 :
            keyboardState[0] = false;
            break;
        case SDLK_2 :
            keyboardState[1] = false;
            break;
        case SDLK_3 :
            keyboardState[2] = false;
            break;
        case SDLK_4 :
            keyboardState[3] = false;
            break;
        case SDLK_q :
            keyboardState[4] = false;
            break;
        case SDLK_w :
            keyboardState[5] = false;
            break;
        case SDLK_e :
            keyboardState[6] = false;
            break;
        case SDLK_r :
            keyboardState[7] = false;
            break;
        case SDLK_a :
            keyboardState[8] = false;
            break;
        case SDLK_s :
            keyboardState[9] = false;
            break;
        case SDLK_d :
            keyboardState[10] = false;
            break;
        case SDLK_f :
            keyboardState[11] = false;
            break;
        case SDLK_y :
            keyboardState[12] = false;
            break;
        case SDLK_x :
            keyboardState[13] = false;
            break;
        case SDLK_c :
            keyboardState[14] = false;
            break;
        case SDLK_v :
            keyboardState[15] = false;
            break;
    }
}
