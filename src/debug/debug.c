#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../cpu/cpu.h"

int pixelSize;
SDL_Rect outlines;
SDL_Rect vDebug;
SDL_Rect hDebug;


// Public funtions
void initDebug( int pxSize, int sizeH, int sizeV ) {
  pixelSize = pxSize;
  outlines.x = 0, outlines.y = 0, outlines.w = pxSize * 64, outlines.h = pxSize * 32;
  vDebug.x = pxSize * 64, vDebug.y = 0, vDebug.w = pxSize * sizeV, vDebug.h = pxSize * 32;
  hDebug.x = 0, hDebug.y = pxSize * 32, hDebug.w = pxSize * ( 64 + sizeV ), hDebug.h = pxSize * sizeH;
  SDL_Log("Debug initialized");
}

void displayDebugInfo( SDL_Renderer *renderer ) {
  drawOutlines( renderer );
  SDL_RenderPresent( renderer );
}


// Private functions
void clearDebugScreen( SDL_Renderer *renderer ) {
  SDL_SetRenderDrawColor( renderer, 0, 0, 0, 100 );
  SDL_RenderFillRect( renderer, &vDebug );
  SDL_RenderFillRect( renderer, &hDebug );
}


void drawOutlines( SDL_Renderer *renderer ) {
  SDL_RenderDrawRect( renderer, &outlines );
}

void displayRegister( SDL_Renderer *renderer ) {
    
}
