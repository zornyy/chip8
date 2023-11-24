//
// Created by Zorny on 06.09.2023.
//

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "keyboard.h"
#include "../cpu/cpu.h"

bool keyboardState[16];

int keypadRefTable[16][2] = {
  {0, SDLK_1},
  {1, SDLK_2},
  {2, SDLK_3},
  {3, SDLK_4},
  {4, SDLK_q},
  {5, SDLK_w},
  {6, SDLK_e},
  {7, SDLK_r},
  {8, SDLK_a},
  {9, SDLK_s},
  {10, SDLK_d},
  {11, SDLK_f},
  {12, SDLK_y},
  {13, SDLK_x},
  {14, SDLK_c},
  {15, SDLK_v},
};

void loadKeyboardLayout( int layout[] ) {
  for ( int i = 0; i < 16; i++ ) {
    keypadRefTable[i][1] = layout[i];
  }
}

void initKeyboard( int keypadLayout[] ) {
  // Load keypad layout
  loadKeyboardLayout( keypadLayout );

  // Set keys to false
    for ( int i=0; i<16; i++ ) {
        keyboardState[i] = false;
    }
    SDL_Log("Keyboard is initialized");
}

void pressKey( int keyCode ) {
    int keyValue;
    
    // Change keyboard mode
    switch ( keyCode ) {
      case SDLK_SPACE :
        CHIP8.stepMode = !CHIP8.stepMode;
      case SDLK_k :
        CHIP8.opcodesToExecute += 10;
      case SDLK_j :
        CHIP8.opcodesToExecute += 1;
    }
    
    for ( int i = 0; i < 16; i++ ) {
      if ( keypadRefTable[i][1] == keyCode ) {
        keyboardState[i] = true;
      }
    }

    if ( CHIP8.paused ) {
      CHIP8.V[CHIP8.pauseRegister] = keyValue;
      CHIP8.paused = false;
      SDL_Log("Unpaused chip8 execution");
    }
}

void releaseKey( int keyCode ) {
  
  for ( int i = 0; i < 16; i++ ) {
    if ( keypadRefTable[i][1] == keyCode ) {
      keyboardState[i] = false;
    }  
  }
}
