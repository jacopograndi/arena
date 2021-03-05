#ifndef HUD_H
#define HUD_H


#include <SDL2/SDL.h> 
#include <SDL2/SDL_mixer.h> 

#include "../render/render_text.h"
#include "../render/button.h"
#include "../render/graphicsettings.h"
#include "../mkb/mkb.h"
#include "../gst/info.h"
#include "../gst/units.h"
#include "../gst/map.h"
#include "../gst/gst.h"
#include "../net/net.h"

typedef struct {
    float start;
    float nav;
    SDL_Rect rect_back;
    SDL_Rect *ref;
} hud_sel;

typedef struct {
    SDL_Rect rect_back;
    char oppo[32];
} overlay_battle;

typedef struct {
    button new_template;
    button save_templates;
    button save_army;
    button delete_army;
    button new_army;
    button start_battle;
    button host_game;
    button join_game;
    SDL_Rect rect_battle;
    SDL_Rect rect_templates;
    SDL_Rect rect_army;
    int temp_modify;
    int temp_place;
    char army_list[64][32];
    int army_listlen;
    int army_listcur;
    int battle_state;
    char playername[32];
    int input_playername;
    char ip[32];
    int input_ip;
    int input_army;
    int input_temp;
    char army_rename[32];
    int start_battle_flag;
} overlay_game;

typedef struct {
    SDL_Rect rect_back;
    SDL_Rect rect_chassis;
    SDL_Rect rect_brain;
    SDL_Rect rect_battery;
    SDL_Rect rect_weapons[8];
    SDL_Rect rect_armor[8];
    SDL_Rect rect_augs[8];
    SDL_Rect rect_stats;
    SDL_Rect rect_gen_cost;
    button done;
    button randomize;
    info_unit uinfo;
    info_unit *uinfo_ptr;
    int sel, ind;
    float gen_cost_max;
    float nav_stats;
} form_new_unit;

typedef struct {
    hud_sel sc;
    overlay_battle ob;
    overlay_game og;
    form_new_unit fnu;
    int state;
    char *nameedit;
} hud;

void hud_init(graphic_settings *gs, hud *h, txtd *t);

void hud_map_sel (info_unit *u, infos *info, int sel, int ind, 
    int8_t **n, int *bound, int size[]);

void hud_resize (graphic_settings *gs, hud *h, txtd *t);
void hud_process (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, gamestate *gst, 
    net_client *netc, net_server *nets, Mix_Chunk *sounds[]);
void hud_render (hud *h, SDL_Renderer* rend, txtd *t, MKb *mkb, infos *info, 
    SDL_Texture *sprites, gamestate *gst, float time);

#endif