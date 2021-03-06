#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../umath/vec.h"

#include "fxs.h"
#include "units.h"

void fx_init (fxs *fx) {
    fx->bullets = (bullet*)malloc(sizeof(bullet)*FXS_BULLET_MAX);
    fx->bulletslen = 0;
    fx->explosions = (explosion*)malloc(sizeof(explosion)*FXS_EXPLOSION_MAX);
    fx->explosionslen = 0;
}

void fx_add_bullet (fxs *fx, bullet *b) {
    if (fx->bulletslen >= FXS_BULLET_MAX-1) return;
    fx->bullets[fx->bulletslen] = *b;
    fx->bulletslen++;
}

void fx_spread_to_head (float spread, float head[]) {
    head[0] = 1-spread; if (spread<0.5) head[0] = spread;
    head[1] = 1-spread*2;
    vec2_norm(head);
}

/*
 * param: pos is the center of the explosion
 * param: vel is the heading of the explosion
 * param: force is added to vel and scaled by mass
 * param: spread [0, 1] is half the angle of the explosion
 * param: speed [0, inf] is the initial speed
 */
void fx_explosion_init (fxs *fx, explosion *e, 
    float pos[], float vel[], float color[], float force[],
    float spread, float speed, int n, float time, float lifetime) 
{
    float normvel[2] = { vel[0], vel[1] }; vec2_norm(normvel);
    float perpvel[2] = { normvel[1], -normvel[0] };
    e->pos[0] = pos[0]; e->pos[1] = pos[1];
    e->partslen = n;
    e->force[0] = force[0]; e->force[1] = force[1];
    e->starttime = time;
    e->friction = 0.95;
    for (int j=0; j<n; j++) {
        particle *p = e->parts+j;
        float sample = rand()%1000/1000.0 * spread;
        float head[2]; fx_spread_to_head(sample, head);
        if (j%2 == 0) head[0] *= -1;
        float dir[2] = {
            -head[0]*normvel[1] - head[1]*perpvel[1],
            head[0]*normvel[0] + head[1]*perpvel[0]
        }; 
        p->lifetime = lifetime;
        p->mass = 1;
        p->pos[0] = pos[0]; p->pos[1] = pos[1];
        p->vel[0] = dir[0]; p->vel[1] = dir[1];
        vec2_mul(p->vel, p->vel, speed+(rand()%1000)/1000.0*speed/2);
        p->color[0] = color[0];
        p->color[1] = color[1];
        p->color[2] = color[2];
    }
}

void fx_add_explosion (fxs *fx, explosion *e) {
    if (fx->explosionslen >= FXS_EXPLOSION_MAX-1) return;
    fx->explosions[fx->explosionslen] = *e;
    fx->explosionslen++;
}

void fx_process (fxs *fx, float time) {
    for (int i=0; i<fx->bulletslen; i++) {
        bullet *b = fx->bullets+i;
        if (b->starttime > time) continue;
        if (time > b->endtime) {
            // remove by replacing with last, dont care for order
            *b = fx->bullets[fx->bulletslen-1];
            fx->bulletslen--;
        }
    }
    
    for (int i=0; i<fx->explosionslen; i++) {
        explosion *e = fx->explosions+i;
        if (e->starttime > time) continue;
        int sum = e->partslen;
        for (int j=0; j<e->partslen; j++) {
            if (e->starttime + e->parts[j].lifetime >= time) {
                vec2_add(e->parts[j].pos, e->parts[j].pos, e->parts[j].vel);
                float forcemass[2]; 
                vec2_mul(forcemass, e->force, e->parts[j].mass);
                vec2_add(e->parts[j].vel, e->parts[j].vel, forcemass);
                vec2_mul(e->parts[j].vel, e->parts[j].vel, e->friction);
            } else { sum -= 1; }
        }
        if (sum == 0) {
            // remove by replacing with last, dont care for order
            *e = fx->explosions[fx->explosionslen-1];
            fx->explosionslen--;
        }
    }
}


void fx_render (SDL_Renderer *rend, fxs *fx, float cam[], float time) {
    for (int i=0; i<fx->bulletslen; i++) {
        bullet *b = fx->bullets+i;
        if (b->starttime > time) continue;
        float travel_time = b->endtime - b->starttime;
        float interval_time = time - b->starttime;
        float amt = interval_time / travel_time;
        if (amt < 0) amt = 0; if (amt > 1) amt = 1;
        float pos[2] = {
            b->from[0] *(1-amt) + b->to[0] *amt,
            b->from[1] *(1-amt) + b->to[1] *amt
        };
        float head[2]; vec2_sub(head, b->from, b->to);
        float mag = 10;
        float dist = amt*vec2_mag(head);
        if (dist < 10) { mag = dist; }
        vec2_norm(head); vec2_mul(head, head, mag);
        SDL_SetRenderDrawColor(rend, 
            b->color[0], b->color[1], b->color[2], 255);
        SDL_RenderDrawLine(rend, 
            -cam[0] + pos[0], 
            -cam[1] + pos[1],
            -cam[0] + pos[0] + head[0], 
            -cam[1] + pos[1] + head[1]
        );
    }
    
    for (int i=0; i<fx->explosionslen; i++) {
        explosion *e = fx->explosions+i;
        if (e->starttime > time) continue;
        for (int j=0; j<e->partslen; j++) {
            particle *p = e->parts+j;
            if (e->starttime + p->lifetime >= time) {
                SDL_SetRenderDrawColor(rend, 
                    p->color[0], p->color[1],  p->color[2], 255);
                SDL_RenderDrawPoint(rend, 
                    -cam[0] + p->pos[0], -cam[1] + p->pos[1]);
            }
        }
    }
}
