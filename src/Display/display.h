//
// Created by Zorny on 06.09.2023.
//

#ifndef ALEXZORN_CHIP8_DISPLAY_H
#define ALEXZORN_CHIP8_DISPLAY_H

/*
 * This function initializes the display of a window
 *  ( int pixelSize ) => Size of each pixel of the 32x64px display
 */
void initDisplay( int pixelSize );


/*
 * This function displays a new SDL window
 *  !! It should be used after int initDisplay( int w, int h )
 */
int showWindow( );

/*
 *  This function sets a pixel to be ON
 *  ( int x ) => position X of the pixel
 *  ( int y ) => position Y of the pixel
 *  Returns: 1 if a pixel has been erased else, returns 0
 */
int setPixel( int x, int y );

/*
 * Clears the display
 */
void clearDisplay( );

#endif //ALEXZORN_CHIP8_DISPLAY_H