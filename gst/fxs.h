#ifndef FXS_H
#define FXS_H

#include <SDL2/SDL.h> 

typedef struct {
    float from[2];
    float to[2];
    float starttime;
    float endtime;
    float size;
    int color[3];
} bullet;

typedef struct {
    bullet *bullets;
    int bulletslen;
} fxs;

void fx_init (fxs *fx);
void fx_add_bullet (fxs *fx, bullet *b);
void fx_process (fxs *fx, float time);
void fx_render (SDL_Renderer *rend, fxs *fx, float cam[], float time);

#endif