#include <stdlib.h>
#include <stdio.h>
#include "render_text.h"

void char_width_init (int *char_width) {
    for (int i=0; i<128; char_width[i++]=5);
    char_width['('] = 2; char_width[')'] = 2;
    char_width['['] = 2; char_width[']'] = 2;
    char_width['{'] = 3; char_width['}'] = 3;
    char_width[','] = 1; char_width['-'] = 3; char_width['.'] = 1; 
    char_width['/'] = 4; char_width['!'] = 1;
    char_width[':'] = 1; char_width[';'] = 1;
    char_width['<'] = 3; char_width['>'] = 3; char_width['='] = 4;
    char_width['I'] = 1;
    char_width['f'] = 4; char_width['i'] = 1; char_width['j'] = 3;
    char_width['l'] = 1; char_width['k'] = 4; char_width['t'] = 4;
}

int get_text_width (char str[], txtd *t) {
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) { 
        width += t->cw[str[i]]; 
        if (str[i+1]!='\0') width++;
    }
    return width;
}

void render_text (SDL_Renderer* gRenderer, char str[], float off[], txtd *t) 
{
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) {
        int char_i = str[i];
        SDL_Rect srcRect = { (char_i%32)*6+1, (char_i/32)*12+1, 5, 11 };
        SDL_Rect dstRect = { off[0]+width, off[1], 5, 11 };
        SDL_RenderCopy(gRenderer, t->tex, &srcRect, &dstRect);
        width += t->cw[char_i]+1;
    }
}

void render_text_scaled (SDL_Renderer* rend, char str[], 
    float off[], txtd *t, float scale) 
{
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) {
        int char_i = str[i];
        SDL_Rect srcRect = { (char_i%32)*6+1, (char_i/32)*12+1, 5, 11 };
        SDL_Rect dstRect = { off[0]+width, off[1], 5*scale, 11*scale };
        SDL_RenderCopy(rend, t->tex, &srcRect, &dstRect);
        width += t->cw[char_i]*scale+1*scale;
    }
}

void render_text_small (SDL_Renderer* rend, char str[], float off[], txtd *t)
{
    int width = 0;
    for (int i=0; str[i]!='\0'; i++) {
        int char_i = str[i];
        SDL_Rect srcRect = { (char_i%32)*5+1, (char_i/32)*7+1, 4, 6 };
        SDL_Rect dstRect = { off[0]+width, off[1], 4, 6 };
        SDL_RenderCopy(rend, t->tex_small, &srcRect, &dstRect);
        width += 5;
    }
}