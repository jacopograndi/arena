#ifndef RENDER_TEXT_H
#define RENDER_TEXT_H

#include <SDL.h>

typedef struct {
    SDL_Texture *tex;
    int cw[128];
} txtd;

void char_width_init (int *char_width);
int get_text_width (char str[], txtd *t);
    
void render_text (SDL_Renderer* gRenderer, char str[], float off[], txtd *t);
void render_text_scaled (SDL_Renderer* gRenderer, char str[], 
    float off[], txtd *t, float scale);
    
#endif