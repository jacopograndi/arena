#ifndef MKB_H
#define MKB_H

#include <SDL2/SDL.h> 

// SDL2 mouse and keyboard

typedef struct {
    SDL_Scancode kb[128]; /* keyboard state, reset every frame */
    int kbnum; 
    int mx; /* mouse x pos */
    int my; /* mouse y pos */
    int mclick[3];
    int mheld[3];
    int mwheeldelta;
} MKb;

void mkb_init(MKb *mkb);
int mkb_search(MKb *mkb, SDL_Scancode scancode);
int mkb_statesearch(MKb *mkb, SDL_Scancode scancode);
void mkb_event(MKb *mkb, SDL_Event *e);
void mkb_process(MKb *mkb);

#endif