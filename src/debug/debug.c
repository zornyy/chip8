#include <string.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "debug.h"
#include "../cpu/cpu.h"
#include "../Stack/stack.h"

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

SDL_Rect keypadRect;
SDL_Surface* keypadSurface;
SDL_Texture* keypadTexture;

SDL_Rect logoRect;
SDL_Surface* logoSurface;
SDL_Texture* logoTexture;

// Initialization FUNCTION
void initDebug( int pxSize, int sizeH, int sizeV ) {
  pixelSize = pxSize;

  // Setting the position of the outlines & text areas
  outlines.x = 0, outlines.y = 0, outlines.w = pxSize * 64, outlines.h = pxSize * 32;
  vDebug.x = pxSize * 64, vDebug.y = 0, vDebug.w = pxSize * sizeV, vDebug.h = pxSize * 32;
  hDebug.x = 0, hDebug.y = pxSize * 32, hDebug.w = pxSize * ( 64 + sizeV ), hDebug.h = pxSize * sizeH;
  
  keypadRect.x = pixelSize * 40, keypadRect.y = pixelSize * 37;
  keypadRect.h = pixelSize * 18, keypadRect.w = pixelSize * 18;
  keypadSurface = SDL_LoadBMP( "./src/static/Keypad.bmp" );

  logoRect.x = pixelSize * ( 61 + sizeV );
  logoRect.y = pixelSize * ( 29 + sizeH );
  logoRect.w = pixelSize * 2.5;
  logoRect.h = pixelSize * 2.5;
  logoSurface = SDL_LoadBMP( "./src/static/logo.bmp" );

  // Setting up the variables for SDL_ttf
  nerdFont = TTF_OpenFont( "./src/static/JetBrainsMonoNL-Medium.ttf", 64 );
  if ( !nerdFont ) {
    SDL_Log("Font could not be loaded, check the specified path");
  } else {
    SDL_Log("Font has been successfully loaded");
  }

  SDL_Log("Debug initialized");
}


// Private functions
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
    length = snprintf( NULL, 0, "Register %d = %x", i, CHIP8.V[i] );
    Rstr = ( char* )malloc( length + 1 );
    sprintf( Rstr, "Register %d = %x", i, CHIP8.V[i] );

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
  char* pauseStr = "Space -> enter / exit the step by step mode";
  char* forwardStr = "J -> Move to the next instruction";
  char* forward10Str = "K -> Move forward 10 instructions";

  t_Text TextKeyboardInfo = {keyboardInfo, pixelSize * 2, pixelSize * 34, pixelSize * 2, Green, nerdFont};
  drawText( &TextKeyboardInfo, renderer );

  TextKeyboardInfo.text = pauseStr;
  TextKeyboardInfo.y = pixelSize * 37;
  TextKeyboardInfo.fontSize = pixelSize * 1.5;
  TextKeyboardInfo.color = White;
  drawText( &TextKeyboardInfo, renderer );

  TextKeyboardInfo.text = forwardStr;
  TextKeyboardInfo.y = pixelSize * 39;
  drawText( &TextKeyboardInfo, renderer );

  TextKeyboardInfo.text = forward10Str;
  TextKeyboardInfo.y = pixelSize * 41;
  drawText( &TextKeyboardInfo, renderer );

}

void drawCurrentOpcode( SDL_Renderer* renderer ) {
  char* opcodeStr;
  int length = snprintf( NULL, 0, "Current opcode: %x", opcode.content );
  opcodeStr = ( char* )malloc( length + 1 );
  sprintf( opcodeStr, "Current opcode: %x", opcode.content );
  t_Text opcodeText = {opcodeStr, pixelSize * 2, pixelSize * 44, pixelSize * 1.5, Red, nerdFont};
  drawText( &opcodeText, renderer );
  free( opcodeStr );

  length = snprintf( NULL, 0, "Opcode nnn: %x", opcode.nnn );
  opcodeStr = ( char* )malloc( length + 1 );
  sprintf( opcodeStr, "Opcode nnn: %x", opcode.nnn );
  opcodeText.text = opcodeStr;
  opcodeText.y = 46 * pixelSize;
  drawText( &opcodeText, renderer );
  free( opcodeStr );

  length = snprintf( NULL, 0, "Opcode kk: %x", opcode.kk );
  opcodeStr = ( char* )malloc( length + 1 );
  sprintf( opcodeStr, "Opcode kk: %x", opcode.kk );
  opcodeText.text = opcodeStr;
  opcodeText.y = 48 * pixelSize;
  drawText( &opcodeText, renderer );
  free( opcodeStr );

  length = snprintf( NULL, 0, "Opcode x: %x", opcode.x );
  opcodeStr = ( char* )malloc( length + 1 );
  sprintf( opcodeStr, "Opcode x: %x", opcode.x );
  opcodeText.text = opcodeStr;
  opcodeText.y = 50 * pixelSize;
  drawText( &opcodeText, renderer );
  free( opcodeStr );

  length = snprintf( NULL, 0, "Opcode y: %d", opcode.y );
  opcodeStr = ( char* )malloc( length + 1 );
  sprintf( opcodeStr, "Opcode y: %x", opcode.y );
  opcodeText.text = opcodeStr;
  opcodeText.y = 52 * pixelSize;
  drawText( &opcodeText, renderer );
  free( opcodeStr );

  length = snprintf( NULL, 0, "Opcode n: %x", opcode.n );
  opcodeStr = ( char* )malloc( length + 1 );
  sprintf( opcodeStr, "Opcode n: %x", opcode.n );
  opcodeText.text = opcodeStr;
  opcodeText.y = 54 * pixelSize;
  drawText( &opcodeText, renderer );
  free( opcodeStr );
}

void drawKeyboard( SDL_Renderer* renderer ) {
  char* StrKeypad = "Keypad :";

  t_Text TitleKeypad = {StrKeypad, pixelSize * 40, pixelSize * 34, pixelSize * 2, Green, nerdFont};
  drawText( &TitleKeypad, renderer );
  keypadTexture = SDL_CreateTextureFromSurface( renderer, keypadSurface );
  SDL_RenderCopy( renderer, keypadTexture, NULL, &keypadRect );
  SDL_DestroyTexture( keypadTexture );
}

void drawStackInfo( SDL_Renderer* renderer ) {
  char* stackStr = "Stack informations :";
  t_Text stackText = {stackStr, pixelSize * 65, pixelSize * 34, pixelSize * 2, Green, nerdFont};
  drawText( &stackText, renderer ); 

  int length = snprintf( NULL, 0, "Stack top is at index %d of 16", top( &CHIP8.stack ) );
  stackStr = ( char* )malloc( length + 1 );
  sprintf( stackStr, "Stack top is at index %d of 16", top( &CHIP8.stack ) );
  stackText.text = stackStr;
  stackText.y = 37 * pixelSize;
  stackText.color = White;
  stackText.fontSize = 1.5 * pixelSize;
  drawText( &stackText, renderer );
  free( stackStr );

  length = snprintf( NULL, 0, "Value on top of the stack: %x", CHIP8.stack.content[top( &CHIP8.stack )]);
  stackStr = ( char* )malloc( length + 1 );
  sprintf( stackStr, "Value on top of the stack: %x", CHIP8.stack.content[top( &CHIP8.stack )]);
  stackText.text = stackStr;
  stackText.y = 39 * pixelSize;
  drawText( &stackText, renderer );
  free( stackStr );
}

void drawLogo( SDL_Renderer* renderer ) {
  logoTexture = SDL_CreateTextureFromSurface( renderer, logoSurface );
  SDL_RenderCopy( renderer, logoTexture, NULL, &logoRect );
  SDL_DestroyTexture( logoTexture );
}

// PUBLIC FUNTION
void displayDebugInfo( SDL_Renderer *renderer ) {
  drawOutlines( renderer );
  displayRegisters( renderer );
  displayKeyInfo( renderer );
  drawCurrentOpcode( renderer );
  drawKeyboard( renderer );
  drawStackInfo( renderer );
  drawLogo( renderer );
}
