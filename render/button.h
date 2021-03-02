#ifndef BUTTON_H
#define BUTTON_H

#include <SDL2/SDL.h> 
#include "render_text.h"

typedef struct {
    char txt[32];
    int pad;
    float pos[2];
} button;

int mouse_in_button (float pt[], txtd *t, button *b);
void render_button (SDL_Renderer* rend, txtd *t, button *b);
    
#endif