#include <mkb.h>

bool mkb_search(MKb *mkb, SDL_Scancode scancode) {
    for (int i=0; i<mkb->kbnum; i++) {
        if (mkb->kb[i] == scancode) 
            return true;
    }
    return false;
}

bool mkb_statesearch (MKb *mkb, SDL_Scancode scancode) {
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[scancode]) return true;
    return false;
}

void mkb_init(MKb *mkb) {
    mkb->kbnum = 0;
    for (int i=0;i<3;mkb->mheld[i]=0,mkb->mclick[i]=0,i++);
    mkb->mwheeldelta = 0;
}
    
void mkb_process(MKb *mkb) {
    mkb->kbnum = 0;
    SDL_GetMouseState(&(mkb->mx), &(mkb->my));
    for (int i=0;i<3;i++) {
        if (mkb->mclick[i] > 0) mkb->mheld[i] += 1;
        else if (mkb->mheld[i] > 0) mkb->mheld[i] = -1;
        else mkb->mheld[i] = 0;
    }
    mkb->mwheeldelta = 0;
}

void mkb_event(MKb *mkb, SDL_Event *e) {
    if(e->type == SDL_KEYDOWN && e->key.repeat == 0) {
        mkb->kb[mkb->kbnum++] = e->key.keysym.scancode;
    }
    if(e->type == SDL_KEYUP) {
    }
    if(e->type == SDL_MOUSEBUTTONDOWN) {
        if (e->button.button == SDL_BUTTON_LEFT) { mkb->mclick[0] = 1; }
        if (e->button.button == SDL_BUTTON_MIDDLE) { mkb->mclick[1] = 1; }
        if (e->button.button == SDL_BUTTON_RIGHT) { mkb->mclick[2] = 1; }
    }
    if(e->type == SDL_MOUSEBUTTONUP) {
        if (e->button.button == SDL_BUTTON_LEFT) { mkb->mclick[0] = 0; }
        if (e->button.button == SDL_BUTTON_MIDDLE) { mkb->mclick[1] = 0; }
        if (e->button.button == SDL_BUTTON_RIGHT) { mkb->mclick[2] = 0; }
    }
    if(e->type == SDL_MOUSEWHEEL) {
        mkb->mwheeldelta = e->wheel.y;
    }
}