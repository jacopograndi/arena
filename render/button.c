#include <stdlib.h>
#include <stdio.h>
#include "button.h"

#include "../umath/intersect.h"

int mouse_in_button (float pt[], txtd *t, button *b) {
    int width = get_text_width(b->txt, t);
    float size[2] = { width+b->pad*2, 10+b->pad*2 };
    if (pt_rect(pt, b->pos, size)) return 1;
    return 0;
}

void render_button (SDL_Renderer* rend, txtd *t, button *b) {
    int width = get_text_width(b->txt, t);
    SDL_Rect rect = { b->pos[0], b->pos[1], width+b->pad*2, 10+b->pad*2 };
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &rect);
    float offpad[2] = { b->pos[0]+b->pad, b->pos[1]+b->pad };
    render_text(rend, b->txt, offpad, t);
}