#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hud_views.h>

#define LABEL(x, y, s, d) {\
    float p[2]={x,y};\
    render_text_scaled(rend, s, p, t, d);\
}

#define LABEL_F3(x, y, s, q, v, d) {\
    float p[2]={x,y}; char ss[64]; \
    if (lvl == 0) \
        sprintf(ss, "%s: (%."#q"f) %."#q"f %."#q"f", s, v[0], v[1], v[2]);\
    if (lvl == 1) \
        sprintf(ss, "%s: %."#q"f (%."#q"f) %."#q"f", s, v[0], v[1], v[2]);\
    if (lvl == 2) \
        sprintf(ss, "%s: %."#q"f %."#q"f (%."#q"f)", s, v[0], v[1], v[2]);\
    render_text_scaled(rend, ss, p, t, d);\
}

#define LABEL_I3(x, y, s, v, d) {\
    float p[2]={x,y}; char ss[64]; \
    if (lvl == 0) sprintf(ss, "%s: (%d) %d %d", s, v[0], v[1], v[2]);\
    if (lvl == 1) sprintf(ss, "%s: %d (%d) %d", s, v[0], v[1], v[2]);\
    if (lvl == 2) sprintf(ss, "%s: %d %d (%d)", s, v[0], v[1], v[2]);\
    render_text_scaled(rend, ss, p, t, 1);\
}


void render_view_comp (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, stats_comp *comp, int lvl) 
{
    float pad = 0;
    float h = 0;
    
    char arr[32][64];
    { 
        int n = stats_frame_sprintf(info, comp->base +lvl, arr); 
        for (int i=0; i<n; i++) { LABEL(px+pad, py+h, arr[i], 1); h += 15; }
    }
    { 
        int n = stats_frame_sprintf(info, comp->perc +lvl, arr); 
        for (int i=0; i<n; i++) { LABEL(px+pad, py+h, arr[i], 1); h += 15; }
    }
    { 
        int n = stats_weapon_sprintf(info, comp->base_weapon +lvl, arr);
        for (int i=0; i<n; i++) { LABEL(px+pad, py+h, arr[i], 1); h += 15; }
    }
    { 
        int n = stats_weapon_sprintf(info, comp->perc_weapon +lvl, arr);
        for (int i=0; i<n; i++) { LABEL(px+pad, py+h, arr[i], 1); h += 15; }
    }
}

void render_view_stats_unit (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, stats_unit *u) 
{
    float pad = 0;
    float h = 0;
    
    char arr[32][64];
    { 
        int n = stats_frame_sprintf(info, &u->frame, arr); 
        for (int i=0; i<n; i++) { LABEL(px+pad, py+h, arr[i], 1); h += 15; }
    }
    h += 10;
    for (int i=0; i<u->weaponlen; i++) { 
        LABEL(px+pad, py+h, "  WEAPON", 1); h += 15;
        int n = stats_weapon_sprintf(info, u->weapon +i, arr); 
        for (int i=0; i<n; i++) { LABEL(px+pad, py+h, arr[i], 1); h += 15; }
    }
}


void render_view_stats (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, info_unit *tm) 
{
    float h = 10;
    LABEL(px+10, py+h, "STATS", 2); h += 35;
    LABEL(px+10, py+h, tm->name, 1); h += 15;
    
    stats_unit base;
    stats_unit_compute(info, tm, &base);
    render_view_stats_unit(rend, t, px+10, py+h, info, &base);
}


void render_view_level (SDL_Renderer *rend, int px, int py, int lvl) {
    // position is of bottom right corner
    for (int i=0; i<MAXLEVEL; i++) {
        int sel = 0;
        if (lvl == i) {
            SDL_SetRenderDrawColor(rend, 240, 250, 240, 255);
            sel = 1;
        } else {
            SDL_SetRenderDrawColor(rend, 140, 140, 140, 255);
            sel = 0;
        }
        SDL_Rect rect = { 
            px-MAXLEVEL*9-1 + 9*i, 
            py-9-1-sel*2, 
            10, 
            10+sel*2 };
        SDL_RenderFillRect(rend, &rect);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderDrawRect(rend, &rect);
    }
}


void render_view_chassis (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int chassis, int lvl, SDL_Texture *sprites) 
{
    if (chassis != -1) {
        info_chassis *ch = info->chassis+chassis;
        int h = 10;
        
        stats_comp *comp = info->stats[STATS_CHASSIS]+chassis;
        LABEL(px+10, py+h, comp->name, 2); h += 30;
        render_view_comp(rend, t, px+10, py+h, info, comp, lvl);

        info_unit u; int8_t *_; int __; int size[2];
        hud_map_sel(&u, info, 0, 0, &_, &__, size);
        size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a chassis...", pname, t, 1);
    }
}

void render_view_battery (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int batt, int lvl) 
{
    if (batt != -1) {
        info_battery *battery = info->batteries+batt;
        int h = 10;
        
        stats_comp *comp = info->stats[STATS_BATTERY]+batt;
        LABEL(px+10, py+h, comp->name, 1); h += 30;
        render_view_comp(rend, t, px+10, py+h, info, comp, lvl);
        
        float size[2] = {145, 145}; size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a battery...", pname, t, 1);
    }
}


void render_view_armor (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int armor, int lvl) 
{
    if (armor != -1) {
        info_armor *arm = info->armors+armor;
        int h = 10;
        
        stats_comp *comp = info->stats[STATS_ARMOR]+armor;
        LABEL(px+10, py+h, comp->name, 1); h += 20;
        
        float size[2] = { 150, 50 }; size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select an armor...", pname, t, 1);
    }
}

void render_view_armor_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int armor, int lvl) 
{
    if (armor != -1) {
        info_armor *arm = info->armors+armor;
        int h = 10;
                
        stats_comp *comp = info->stats[STATS_ARMOR]+armor;
        LABEL(px+10, py+h, comp->name, 1); h += 30;
        render_view_comp(rend, t, px+10, py+h, info, comp, lvl);
        
        info_unit u; int8_t *_; int __; int size[2];
        hud_map_sel(&u, info, 2, 0, &_, &__, size);
        size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        render_view_weapon(rend, t, px, py, info, armor, lvl);
    }
}


void render_view_weapon (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int weapon, int lvl) 
{
    if (weapon != -1) {
        info_weapon *weap = info->weapons+weapon;
        int h = 10;

        stats_comp *comp = info->stats[STATS_WEAPONS]+weapon;
        LABEL(px+10, py+h, comp->name, 1); h += 30;
        render_view_comp(rend, t, px+10, py+h, info, comp, lvl);
        
        float size[2] = { 200, 110 }; size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a weapon...", pname, t, 1);
    }
    
}

void render_view_weapon_detail (SDL_Renderer* rend, txtd *t, 
    int px, int py, infos *info, int weapon, int lvl)
{
    if (weapon != -1) {
        info_weapon *weap = info->weapons+weapon;
        int h = 10;

        stats_comp *comp = info->stats[STATS_WEAPONS]+weapon;
        LABEL(px+10, py+h, comp->name, 1); h += 30;
        render_view_comp(rend, t, px+10, py+h, info, comp, lvl);
        
        info_unit u; int8_t *_; int __; int size[2];
        hud_map_sel(&u, info, 3, 0, &_, &__, size);
        size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        render_view_weapon(rend, t, px, py, info, weapon, lvl);
    }
}


void render_view_aug (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug, int lvl) 
{
    if (aug != -1) {
        info_aug *augm = info->augs+aug;
        int h = 10;
        
        stats_comp *comp = info->stats[STATS_AUGS]+aug;
        LABEL(px+10, py+h, comp->name, 1); h += 20;
        
        float size[2] = { 150, 50 }; size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select an augment...", pname, t, 1);
    }
}

void render_view_aug_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug, int lvl) 
{
    if (aug != -1) {
        info_aug *augm = info->augs+aug;
        int h = 10;
        
        stats_comp *comp = info->stats[STATS_AUGS]+aug;
        LABEL(px+10, py+h, comp->name, 1); h += 20;
        render_view_comp(rend, t, px+10, py+h, info, comp, lvl);
        
        info_unit u; int8_t *_; int __; int size[2];
        hud_map_sel(&u, info, 4, 0, &_, &__, size);
        size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        render_view_aug(rend, t, px, py, info, aug, lvl);
    }
}


void render_view_brain (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int brain, int lvl) 
{
    if (brain != -1) {
        float pname[2] = { px+10, py+10 };
        int h = 10;
        
        stats_comp *comp = info->stats[STATS_BRAIN]+brain;
        LABEL(px+10, py+h, comp->name, 1); h += 20;
        render_view_comp(rend, t, px+10, py+h, info, comp, lvl);
        
        float size[2] = { 145, 145 }; size[0] -= 5; size[1] -= 5;
        render_view_level(rend, px+size[0], py+size[1], lvl);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a controller...", pname, t, 1);
    }
}


void render_view_template (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int temp, int lvl) 
{
    { button b = { "edit", 4, { px, py } };
    render_button(rend, t, &b); }
    float wedit = get_text_width("edit", t); 
    
    { button b = { "place", 4, { px+wedit+2*4+5, py } };
    render_button(rend, t, &b); }
    float wplace = get_text_width("place", t); 
    
    float pname[2] = { wplace+wedit+4*4+10+px, py+4 };
    char *sname = info->templates[temp].name;
    render_text_scaled(rend, sname, pname, t, 1);
}