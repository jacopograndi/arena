#ifndef GST_H
#define GST_H

#include "info.h"
#include "units.h"
#include "map.h"
#include "fxs.h"
#include "../render/render_text.h"
#include "../mkb/mkb.h"


#include <SDL2/SDL.h> 
#include <SDL2/SDL_mixer.h> 

#define MAXMAP 10

typedef struct {
    map map_editor;
    army army_bp[8];
    int playernum;
    map map_battle;
    army ar;
    army ar_past[2];
    stats_unit ustats[MAXUNITS];
    
    int state;
    float cam[2];
    float starttime;
    float coveredtime;
    int turn;
    float turnspeed;
    int turn_until_finish;
    int over;
    int waitstep;
} gamestate;

void gst_init (gamestate *gst);
void gst_destroy (gamestate *gst);
void gst_get_maparmy(gamestate *gst, map **m, army **ar);

void gst_tobattle (gamestate *gst, infos *info);
void gst_toeditor (gamestate *gst);

void gst_next_turn (gamestate *gst, infos *info, fxs *fx, float t);
int gst_check_victory (gamestate *gst);

void gst_process (gamestate *gst, infos *info, MKb *mkb, fxs *fx, float t);
void gst_render (SDL_Renderer *rend, SDL_Texture *txsprites, txtd *textd, 
    gamestate *gst, infos *info, float t);

#endif