//
// Created by Zorny on 06.09.2023.
//

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "keyboard.h"
#include "../cpu/cpu.h"

bool keyboardState[16];

int keypadRefTable[16];

void loadKeyboardLayout( int layout[] ) {
  for ( int i = 0; i < 16; i++ ) {
    keypadRefTable[i] = layout[i];
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
    
    // Change execution mode
    switch ( keyCode ) {
      case SDLK_SPACE :
        CHIP8.stepMode = !CHIP8.stepMode;
      case SDLK_k :
        CHIP8.opcodesToExecute += 10;
      case SDLK_j :
        CHIP8.opcodesToExecute += 1;
    }
    
    for ( int i = 0; i < 16; i++ ) {
      if ( keypadRefTable[i] == keyCode ) {
        keyboardState[i] = true;
        keyValue = i;
      }
    }
    
    // Unpause & store key if needed
    if ( CHIP8.paused ) {
      CHIP8.V[CHIP8.pauseRegister] = keyValue;
      CHIP8.paused = false;
    }
}

void releaseKey( int keyCode ) {
  
  for ( int i = 0; i < 16; i++ ) {
    if ( keypadRefTable[i] == keyCode ) {
      keyboardState[i] = false;
    }  
  }
}
