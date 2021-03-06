#ifndef FXS_H
#define FXS_H

#include <SDL2/SDL.h> 

#define FXS_BULLET_MAX 4096
#define FXS_EXPLOSION_MAX 2048

typedef struct {
    float from[2];
    float to[2];
    float starttime;
    float endtime;
    float size;
    float color[3];
} bullet;

typedef struct {
    float pos[2];
    float vel[2];
    float color[3];
    float size;
    float mass;
    float lifetime;
} particle;

typedef struct {
    float pos[2];
    float force[2];
    float friction;
    particle parts[32];
    int partslen;
    float starttime;
} explosion;

typedef struct {
    bullet *bullets;
    int bulletslen;
    explosion *explosions;
    int explosionslen;
} fxs;

void fx_init (fxs *fx);

void fx_add_bullet (fxs *fx, bullet *b);

void fx_explosion_init (fxs *fx, explosion *e, 
    float pos[], float vel[], float color[], float force[],
    float spread, float speed, int n, float time, float lifetime);
void fx_add_explosion (fxs *fx, explosion *e);

void fx_process (fxs *fx, float time);
void fx_render (SDL_Renderer *rend, fxs *fx, float cam[], float time);

#endif