#include <string.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "debug.h"
#include "../cpu/cpu.h"

int pixelSize;
SDL_Rect outlines;
SDL_Rect vDebug;
SDL_Rect hDebug;


SDL_Rect textArea;
SDL_Surface* textSurface;
SDL_Texture* textTexture;
SDL_Color White = {255, 255, 255};
SDL_Color Green = {0, 255, 0};
SDL_Color Red = {255, 0, 0};
SDL_Color Blue = {0, 0, 255};
TTF_Font* nerdFont;

SDL_Rect memoryRect;
SDL_Rect PCrep;

// Initialization FUNCTION
void initDebug( int pxSize, int sizeH, int sizeV ) {
  pixelSize = pxSize;

  // Setting the position of the outlines & text areas
  outlines.x = 0, outlines.y = 0, outlines.w = pxSize * 64, outlines.h = pxSize * 32;
  vDebug.x = pxSize * 64, vDebug.y = 0, vDebug.w = pxSize * sizeV, vDebug.h = pxSize * 32;
  hDebug.x = 0, hDebug.y = pxSize * 32, hDebug.w = pxSize * ( 64 + sizeV ), hDebug.h = pxSize * sizeH;
 
  memoryRect.x = pxSize * 3, memoryRect.y = pxSize * 35, memoryRect.w = pxSize * 5, memoryRect.h = pxSize * 20;
  PCrep.x = pxSize * 8.5, PCrep.y = 0, PCrep.w = pxSize * 1, PCrep.h = pxSize * 0.25;

  // Setting up the variables for SDL_ttf
  nerdFont = TTF_OpenFont( "./src/fonts/JetBrainsMonoNL-Medium.ttf", 64 );
  if ( !nerdFont ) {
    SDL_Log("Font could not be loaded, check the specified path");
  } else {
    SDL_Log("Font has been successfully loaded");
  }

  SDL_Log("Debug initialized");
}


// Private functions
void clearDebugScreen( SDL_Renderer *renderer ) {
  SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
  SDL_RenderFillRect( renderer, &vDebug );
  SDL_RenderFillRect( renderer, &hDebug );
}


void drawOutlines( SDL_Renderer *renderer ) {
  SDL_SetRenderDrawColor( renderer, 255, 255, 255, SDL_ALPHA_OPAQUE );
  SDL_RenderDrawRect( renderer, &outlines );
  SDL_RenderDrawRect( renderer, &vDebug );
  SDL_RenderDrawRect( renderer, &hDebug );
}

void drawText( t_Text *text, SDL_Renderer *renderer ) {
  textArea.x = text->x;
  textArea.y = text->y;
  textArea.h = text->fontSize;
  textArea.w = text->fontSize * strlen(text->text) / 2;
  
  textSurface = TTF_RenderText_Solid( text->font, text->text, text->color );
  textTexture = SDL_CreateTextureFromSurface( renderer, textSurface );
  SDL_FreeSurface( textSurface );
  SDL_RenderCopy( renderer, textTexture, NULL, &textArea );
  
  SDL_DestroyTexture( textTexture );
}

void displayRegisters( SDL_Renderer *renderer ) {
  
  // initializing length utility variable
  int length;

  // Display registers title
  char* registerString = "Registers & Memory :";
  t_Text registerText = {registerString, pixelSize * 66, pixelSize * 2, pixelSize * 3, White, nerdFont };
  drawText( &registerText, renderer );
  
  // Display all 16 registers Value
  char* Rstr;
  for ( int i = 0; i < 16; i++ ) {
    length = snprintf( NULL, 0, "Register %d = %d", i, CHIP8.V[i] );
    Rstr = ( char* )malloc( length + 1 );
    sprintf( Rstr, "Register %d = %d", i, CHIP8.V[i] );

    t_Text myText = {Rstr, pixelSize * 67, pixelSize * 6 + 1.5 * i* pixelSize, pixelSize * 1.5, White, nerdFont};
    drawText( &myText, renderer );
  }
  free( Rstr );
  
  // Display PC 
  char* PCstr;
  length = snprintf( NULL,0, "PC = %d", CHIP8.PC );
  PCstr = ( char* )malloc( length + 1 );
  sprintf( PCstr, "PC = %d", CHIP8.PC );

  t_Text PCtext = {PCstr, pixelSize * 83, pixelSize * 6, pixelSize * 2, Red, nerdFont};
  drawText( &PCtext, renderer );
  free( PCstr );

  // Display I value
  char* Istr;
  length = snprintf( NULL, 0, "I = %d", CHIP8.I );
  Istr = ( char* )malloc( length + 1 );
  sprintf( Istr, "I = %d", CHIP8.I );

  t_Text Itext = {Istr, pixelSize * 83, pixelSize * 9, pixelSize * 2, Red, nerdFont};
  drawText( &Itext, renderer );
  free( Istr );
}

void drawPerformances( SDL_Renderer* renderer, int frameTime, int targetFPS ) {
  // Display FPS & current load on the virtual CPU
  int fps = 60;
  if ( frameTime > 1000 / targetFPS ) {
    fps = 1000 / frameTime;
  }

  char* FPSstr;
  int length = snprintf( NULL, 0, "FPS = %d", fps );
  FPSstr = ( char* )malloc( length + 1 );
  sprintf( FPSstr, "FPS = %d", fps );

  t_Text FPStext = {FPSstr, pixelSize * 83, pixelSize * 28, pixelSize * 2, Green, nerdFont};
  drawText( &FPStext, renderer );
  free( FPSstr );
}

void displayKeyInfo( SDL_Renderer *renderer ) {
  char* keyboardInfo = "Keyboard debug keys :";
  t_Text TextKeyboardInfo = {keyboardInfo, pixelSize * 2, pixelSize * 34, pixelSize * 2, Green, nerdFont};
  drawText( &TextKeyboardInfo, renderer );
}

void drawMemory( SDL_Renderer* renderer ) {

}

// PUBLIC FUNTION
void displayDebugInfo( SDL_Renderer *renderer ) {
  clearDebugScreen( renderer );
  drawOutlines( renderer );
  displayRegisters( renderer );
  displayKeyInfo( renderer );
}
