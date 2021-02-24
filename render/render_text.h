#ifndef RENDER_TEXT_H
#define RENDER_TEXT_H

#include <SDL.h>

typedef struct {
    SDL_Texture *tex;
    SDL_Texture *tex_small;
    int cw[128];
} txtd;

void char_width_init (int *char_width);
int get_text_width (char str[], txtd *t);
    
void render_text (SDL_Renderer* gRenderer, char str[], float off[], txtd *t);
void render_text_scaled (SDL_Renderer* gRenderer, char str[], 
    float off[], txtd *t, float scale);
    
void render_text_small (SDL_Renderer* rend, char str[], float off[], txtd *t);
    
#endif