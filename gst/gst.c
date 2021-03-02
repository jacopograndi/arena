#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#include "gst.h"

void gst_init (gamestate *gst) {
    map_init(&gst->map_editor, MAXMAP, MAXMAP, 32);
    army_init(gst->army_bp, &gst->map_editor);
    gst->playernum = 1;
    gst->state = 0;
    gst->starttime = FLT_MAX;
    gst->turn = 0;
    gst->coveredtime = 0;
    gst->turnspeed = 0.75;
}

void gst_destroy (gamestate *gst) {
    army_destory(gst->army_bp+0);
    map_destroy(&gst->map_editor);
}

void gst_get_maparmy(gamestate *gst, map **m, army **ar) {
    if (gst->state == 0) {
        *m = &gst->map_editor;
        *ar = gst->army_bp+0;
    } 
    if (gst->state == 1) {
        *m = &gst->map_battle;
        *ar = &gst->ar;
    }
}

void gst_lastpos (gamestate *gst) {
    for (int i=0; i<gst->ar.uslen; i++) {
        gst->ar_lastpos[i][0] = gst->ar.us[i].pos[0];
        gst->ar_lastpos[i][1] = gst->ar.us[i].pos[1];
    }
}

void gst_compute_stats (gamestate *gst, infos *info) {
    for (int i=0; i<gst->ar.uslen; i++) {
        stats_unit_compute(info, &gst->ar.us[i].info, gst->ustats +i);
    }
}

void gst_tobattle (gamestate *gst, infos *info) {
    if (gst->playernum == 1) {
        //info_load_army(gst->army_bp+1, "army");
        gst->army_bp[1] = gst->army_bp[0];
        gst->playernum = 2;
    }
    gst->state = 1;
    int maxx = MAXMAP, maxy = MAXMAP;
    int dx = 5, dy = 10;
    int sx = dx*2+maxx, sy = dy+maxy*2;
    map_init(&gst->map_battle, sx, sy, 32);
    army_init(&gst->ar, &gst->map_battle);
    unit u;
    for (int i=0; i<gst->playernum; i++) {
        for (int j=0; j<gst->army_bp[i].uslen; j++) {
            u = gst->army_bp[i].us[j];
            u.owner = i;
            if (i==0) {
                u.gridpos[0] += dx;
                u.gridpos[1] += dy+maxy;
            }
            if (i==1) {
                u.gridpos[0] = maxx-u.gridpos[0]+dx;
                u.gridpos[1] = maxy-u.gridpos[1];
            }
            u.pos[0] = u.gridpos[0]*gst->map_battle.ts;
            u.pos[1] = u.gridpos[1]*gst->map_battle.ts;
            army_spawn(&gst->ar, &gst->map_battle, u);
            gst->ar.us[gst->ar.uslen].owner = i;
        }
    }
    
    gst_compute_stats(gst, info);
    gst_lastpos(gst);
    gst->starttime = FLT_MAX;
    gst->turn = 0;
    gst->coveredtime = 0;
    gst->turn_until_finish = 5;
    gst->over = 0;
}

void gst_toeditor(gamestate *gst) {
    gst->playernum = 1;
}

void gst_spawn_bullets (gamestate *gst, fxs *fx, a_dmg dmgs[], int dmgslen,
    float time) 
{
    map *m; army *ar;
    gst_get_maparmy(gst, &m, &ar);
    
    bullet b;
    
    // for every unit count how many shots
    int counts[MAXUNITS]; 
    for (int j=0;j<MAXUNITS; counts[j]=0, j++);
    for (int i=0; i<ar->uslen; i++) {
        for (int j=0; j<dmgslen; j++) {
            unit *u = ar->us+i;
            if (dmgs[j].u == u) { counts[i]++; }
        }
    } 
    
    // then spawn a bullet for every shot
    int curr[MAXUNITS]; 
    for (int j=0;j<MAXUNITS; curr[j]=0, j++);
    for (int i=0; i<ar->uslen; i++) {
        for (int j=0; j<dmgslen; j++) {
            unit *u = ar->us+i;
            if (dmgs[j].u == u) {
                unit *t = dmgs[j].t;
                b.from[0] = u->pos[0]+16;
                b.from[1] = u->pos[1]+16;
                b.to[0] = t->pos[0]+16;
                b.to[1] = t->pos[1]+16;
                float n = (float)curr[i]/counts[i];
                float travel_time = 0.1;
                float shot_time = time + n*gst->turnspeed;
                b.starttime = shot_time;
                b.endtime = shot_time + travel_time;
                if (u->owner == 0) {
                    b.color[0] = 0; b.color[1] = 255; b.color[2] = 0;
                } else {
                    b.color[0] = 255; b.color[1] = 0; b.color[2] = 0;
                }
                fx_add_bullet(fx, &b);
                curr[i] ++;
            }
        }
    }
    
}

int gst_check_victory (gamestate *gst) {
    int counts[gst->playernum], max=-1, imax = -1;
    for (int i=0; i<gst->ar.uslen; i++) {        
        if (gst->ar.us[i].hp <= 0) continue;
        if (gst->ar.us[i].charge <= 0) continue;
        counts[gst->ar.us[i].owner] ++;
        if (counts[gst->ar.us[i].owner] > max) {
            imax = gst->ar.us[i].owner;
            max = counts[gst->ar.us[i].owner];
        }
    }
    return imax;
}

void gst_next_turn (gamestate *gst, infos *info, fxs *fx, float t) {
    gst_lastpos(gst);
    gst->coveredtime += gst->turnspeed;
    gst->turn ++;
    map *m; army *ar;
    gst_get_maparmy(gst, &m, &ar);
    int move = army_move(info, ar, m, gst->ustats);
    
    a_dmg dmgs[1024*8];
    int fire = army_fire(info, ar, m, dmgs, gst->ustats);
    army_upkeep(info, ar, m, gst->ustats);
    if (move == 0 && fire == 0) {
        gst->turn_until_finish--;
    } else { gst->turn_until_finish = 5; }
    if (gst->turn_until_finish <= 0) {
        gst->over = 1;
    }
    gst_spawn_bullets(gst, fx, dmgs, fire, t);
}

void gst_process (gamestate *gst, infos *info, fxs *fx, float t) {
    if (gst->state == 1) {
        if (gst->starttime > t) gst->starttime = t; 
        float t_elapsed = t-gst->starttime;
        if (t_elapsed >= gst->coveredtime) {
            gst_next_turn(gst, info, fx, t);
        }
    }
}

void gst_render (SDL_Renderer *rend, SDL_Texture *txsprites, txtd *textd,
    gamestate *gst, infos *info, float t) 
{  
    float ts = 32;
    int posx = gst->cam[0];
    int posy = gst->cam[1];
    map *m; army *ar;
    gst_get_maparmy(gst, &m, &ar);
    
    // render map
    for (int y=0; y<m->sy; y++) {
        for (int x=0; x<m->sx; x++) {
            float px = x*ts;
            float py = y*ts;
            SDL_Rect srcRect = { 1*ts, 0*ts, ts, ts };
            SDL_Rect dstRect = { (int)px-posx, (int)py-posy, ts, ts };
            SDL_RenderCopy(rend, txsprites, &srcRect, &dstRect);
        }
    }
    
    /* assuming one turn = 1s, amt domain = [0, 1) */
    float amt = t - (gst->starttime+gst->coveredtime);
    amt = (amt / gst->turnspeed) + 1;
    if (amt > 1) amt = 1; if (amt < 0) amt = 0; // clamping away fuzzyness
    
    // render units
    for (int i=0; i<ar->uslen; i++) {
        if (ar->us[i].hp <= 0) continue;
        float present_x = ar->us[i].pos[0];
        float present_y = ar->us[i].pos[1];
        
        float x = present_x, y = present_y;
        if (gst->state == 1) {
            x = present_x*(amt) + gst->ar_lastpos[i][0]*(1-amt);
            y = present_y*(amt) + gst->ar_lastpos[i][1]*(1-amt);
        } 
        
        SDL_Rect srcRect = { ar->us[i].info.chassis*ts, ts, ts, ts };
        SDL_Rect dstRect = { (int)x-posx, (int)y-posy, ts, ts };
        SDL_RenderCopy(rend, txsprites, &srcRect, &dstRect);
        
        stats_unit base; stats_unit_compute(info, &ar->us[i].info, &base);
        float amt = ar->us[i].hp / base.frame.hp;
        SDL_Rect hprect = { 
            (int)x-posx, (int)y-posy+ts-5, 
            ts*amt, 6 };
        int sw = 1 ? ar->us[i].owner : 0;
        SDL_SetRenderDrawColor(rend, 255*sw, 255*(1-sw), 0, 255);
        SDL_RenderFillRect(rend, &hprect);
        
        SDL_SetTextureColorMod(textd->tex_small, sw*100, 100*(1-sw), 0);
        char shp[32]; sprintf(shp, "%.0f", ar->us[i].hp);
        float php[2] = { (int)x-posx, (int)y-posy+ts-5 };
        render_text_small(rend, shp, php, textd);
        
        SDL_SetTextureColorMod(textd->tex_small, 255, 160, 0);
        char sch[32]; sprintf(sch, "%.0f", ar->us[i].charge);
        float pch[2] = { (int)x-posx, (int)y-posy+ts+1 };
        render_text_small(rend, sch, pch, textd);
        SDL_SetTextureColorMod(textd->tex_small, 0, 0, 0);
    }
}