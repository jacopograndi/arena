#ifndef HUD_VIEWS_H
#define HUD_VIEWS_H

#include <SDL2/SDL.h> 

#include "../render/render_text.h"
#include "../render/button.h"
#include "../render/graphicsettings.h"
#include "../mkb/mkb.h"
#include "../gst/info.h"
#include "../gst/units.h"
#include "hud.h"

void render_view_stats (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, info_unit *tm);


void render_view_level (SDL_Renderer *rend, int px, int py, int lvl);


void render_view_chassis (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int chassis, int lvl, SDL_Texture *sprites);
    
void render_view_battery (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int batt, int lvl);
    
void render_view_armor (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int armor, int lvl);
void render_view_armor_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int armor, int lvl);

void render_view_weapon (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int weapon, int lvl);
void render_view_weapon_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int weapon, int lvl);

void render_view_aug (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug, int lvl);
void render_view_aug_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug, int lvl);
    
void render_view_brain (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int brain, int lvl);


void render_view_template (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int temp, int lvl);


#endif