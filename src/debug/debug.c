#include <string.h>
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
TTF_Font* nerdFont;


// Initialization FUNCTION
void initDebug( int pxSize, int sizeH, int sizeV ) {
  pixelSize = pxSize;

  // Setting the position of the outlines & text areas
  outlines.x = 0, outlines.y = 0, outlines.w = pxSize * 64, outlines.h = pxSize * 32;
  vDebug.x = pxSize * 64, vDebug.y = 0, vDebug.w = pxSize * sizeV, vDebug.h = pxSize * 32;
  hDebug.x = 0, hDebug.y = pxSize * 32, hDebug.w = pxSize * ( 64 + sizeV ), hDebug.h = pxSize * sizeH;
  
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

void allocateStringPointer( char** str, int i ) {

}

void displayRegister( SDL_Renderer *renderer ) {
  char* registerString = "Registers:";
  t_Text registerText = {registerString, pixelSize * 67, pixelSize * 2, pixelSize * 3, White, nerdFont };
  drawText( &registerText, renderer );
}

// PUBLIC FUNTION
void displayDebugInfo( SDL_Renderer *renderer ) {
  clearDebugScreen( renderer );
  drawOutlines( renderer );
  displayRegister( renderer );
}
