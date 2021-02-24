#include <stdlib.h>
#include <stdio.h>
#include <units.h>
#include <vec.h>

#include <fxs.h>

void fx_init (fxs *fx) {
    fx->bullets = (bullet*)malloc(sizeof(bullet)*1024);
    fx->bulletslen = 0;
}

void fx_add_bullet (fxs *fx, bullet *b) {
    fx->bullets[fx->bulletslen] = *b;
    fx->bulletslen++;
}

void fx_process (fxs *fx, float time) {
    for (int i=0; i<fx->bulletslen; i++) {
        bullet *b = fx->bullets+i;
        if (time > b->endtime) {
            // remove by replacing with last, dont care for order
            *b = fx->bullets[fx->bulletslen-1];
            fx->bulletslen--;
        }
    }
}


void fx_render (SDL_Renderer *rend, fxs *fx, float cam[], float time) {
    for (int i=0; i<fx->bulletslen; i++) {
        bullet *b = fx->bullets+i;
        float travel_time = b->endtime - b->starttime;
        float interval_time = time - b->starttime;
        float amt = interval_time / travel_time;
        if (amt < 0) amt = 0; if (amt > 1) amt = 1;
        float pos[2] = {
            b->from[0] *(1-amt) + b->to[0] *amt,
            b->from[1] *(1-amt) + b->to[1] *amt
        };
        float head[2]; vec2_sub(head, b->from, b->to);
        vec2_norm(head); vec2_mul(head, head, 10);
        SDL_SetRenderDrawColor(rend, 
            b->color[0], 
            b->color[1], 
            b->color[2],
            255);
        SDL_RenderDrawLine(rend, 
            -cam[0] + pos[0], 
            -cam[1] + pos[1],
            -cam[0] + pos[0] + head[0], 
            -cam[1] + pos[1] + head[1]
        );
    }
}
