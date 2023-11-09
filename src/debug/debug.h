#ifndef ALEXZORN_DEBUG_H
#define ALEXZORN_DEBUG_H

typedef struct {
  char *text;
  int x;
  int y;
  int fontSize;
  SDL_Color color;
  TTF_Font *font;
} t_Text;


void initDebug( int pxSize, int sizeH, int sizeV ); 

void displayDebugInfo( SDL_Renderer *renderer );


#endif //ALEXZORN_DEBUG_H
