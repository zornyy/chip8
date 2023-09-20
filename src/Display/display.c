#include <SDL.h>
#include <stdbool.h>

#include "stack.h"
#include "cpu.h"
#include "display.h"
#include "keyboard.h"


// SDL Utility Variables
SDL_Window *window;
SDL_Renderer *renderer;

SDL_Rect drawingRect;


// Framerate variables
const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint64 frameStart;
Uint64 frameTime;


// Screen matrix
int screenState[64][32];



// Display variables
int width, height;
int pxSize;

void clearDisplay( ) {
    SDL_RenderClear( renderer );
}

void resetBackground( ) {
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderPresent( renderer );
}

void drawRect( ) {
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
    SDL_RenderFillRect( renderer, &drawingRect );
}

void eraseRect( ) {
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0);
    SDL_RenderFillRect( renderer, &drawingRect );
}

int setPixel( int x, int y ) {

    // wrap the pixel on the other side of the screen if out display
    if ( x > 63 ) {
        x -= 64;
    } else if ( x < 0 ) {
        x += 64;
    }

    if ( y > 31 ) {
        y -= 32;
    } else if ( y < 0 ) {
        y += 32;
    }

    // Set the coordinates to draw the pixel
    drawingRect.x = x * pxSize;
    drawingRect.y = y * pxSize;

    // update the screenState
    screenState[x][y] ^= 1;

    // Draw or erase the pixel
    if ( screenState[x][y] == 1 ) {
        drawRect( );
        return 0;
    } else {
        eraseRect( );
        return 1;
    }
}

void fixFramerate(  ) {
    frameTime = SDL_GetTicks() - frameStart;

    if ( frameDelay > frameTime ) {
        SDL_Delay( frameDelay - frameTime );
    }
}


void initDisplay( int pixelSize ) {
    SDL_Init( SDL_INIT_EVERYTHING );
    pxSize = pixelSize;
    drawingRect.h = pixelSize, drawingRect.w = pixelSize;
    width = pixelSize * 64, height = pixelSize * 32;


    // init screen matrix
    for ( int x = 0; x < 64; x++ ) {
        for ( int y = 0; y < 32; y++ ) {
            screenState[x][y] = 0;
        }
    }

    SDL_Log("Display initialized with 64x32px\n");
}

void programCycle( ) {
    // clearDisplay( );


    // Cycle actions
    cpuCycle( );
    // drawScreen( );


    // resetBackground( );
}

int showWindow( ) {
    window = SDL_CreateWindow( "AlexZorn - Chip-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_ALLOW_HIGHDPI );
    if ( NULL == window )
    {
        SDL_Log("Could not create window");
        return 1;
    }

    renderer = SDL_CreateRenderer( window, -1, 0 );
    if ( NULL == renderer ) {
        SDL_Log("Could not create renderer");
        return 1;
    }


    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    SDL_Event windowEvent;

    bool isRunning = true;

    SDL_Log("Starting Main loop fixed at %dHz\n", FPS);
    // Main Program LOOP
    while ( isRunning )
    {
        frameStart = SDL_GetTicks();
        while ( SDL_PollEvent( &windowEvent ) )
        {
            // Check for QUIT Event
            if ( SDL_QUIT == windowEvent.type )
            {
                isRunning = false;
            } else if ( SDL_KEYDOWN == windowEvent.type ) {
                pressKey( windowEvent.key.keysym.sym );
            } else if ( SDL_KEYUP == windowEvent.type ) {
                releaseKey( windowEvent.key.keysym.sym );
            }

            // Content of the program cycle
            programCycle( );

            SDL_RenderPresent( renderer );

            // Fix the framerate
            fixFramerate( );
        }
    }

    SDL_DestroyWindow( window );
    SDL_Quit( );

    return EXIT_SUCCESS;
}

