#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#include "gst.h"
#include "../umath/vec.h"

void gst_init (gamestate *gst) {
    map_init(&gst->map_editor, MAXMAP, MAXMAP, 32);
    army_init(gst->army_bp, &gst->map_editor);
    gst->playernum = 1;
    gst->state = 0;
    gst->starttime = FLT_MAX;
    gst->turn = 0;
    gst->coveredtime = 0;
    gst->turnspeed = 0.75;
    gst->waitstep = 0;
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

void gst_ar_past_cycle (gamestate *gst) {
    gst->ar_past[1] = gst->ar_past[0];
    gst->ar_past[0] = gst->ar;
}

void gst_compute_stats (gamestate *gst, infos *info) {
    for (int i=0; i<gst->ar.uslen; i++) {
        stats_unit_compute(info, &gst->ar.us[i].info, gst->ustats +i);
    }
}

void gst_tobattle (gamestate *gst, infos *info) {
    if (gst->playernum == 1) {
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
    gst_ar_past_cycle(gst);
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
                float n = (float)curr[i]/counts[i];
                
                float u_past_x = gst->ar_past[1].us[i].pos[0];
                float u_past_y = gst->ar_past[1].us[i].pos[1];
                float u_pres_x = gst->ar_past[0].us[i].pos[0];
                float u_pres_y = gst->ar_past[0].us[i].pos[1];
                
                int t_i = 0;
                for (int k=0; k<ar->uslen; k++) {
                    if (ar->us+k == t) { t_i = k; break; }
                }
                
                float t_past_x = gst->ar_past[1].us[t_i].pos[0];
                float t_past_y = gst->ar_past[1].us[t_i].pos[1];
                float t_pres_x = gst->ar_past[0].us[t_i].pos[0];
                float t_pres_y = gst->ar_past[0].us[t_i].pos[1];
                
                // interpolate from the past
                b.from[0] = u_pres_x*(n) + u_past_x*(1-n)+16 +rand()%8-4;
                b.from[1] = u_pres_y*(n) + u_past_y*(1-n)+16 +rand()%8-4;
                b.to[0] = t_pres_x*(n) + t_past_x*(1-n)+16 +rand()%16-8;
                b.to[1] = t_pres_y*(n) + t_past_y*(1-n)+16 +rand()%16-8;
                
                float travel_time = 0.3;
                float shot_time = time + n*gst->turnspeed;
                b.starttime = shot_time;
                b.endtime = shot_time + travel_time;
                
                float colors[2][3] = { {0,255,0}, {255,0,0} };
                int selcol = 0;
                
                if (u->owner == 0) { selcol = 0; } 
                else { selcol = 1; }
                b.color[0] = colors[selcol][0]; 
                b.color[1] = colors[selcol][1]; 
                b.color[2] = colors[selcol][2]; 
                fx_add_bullet(fx, &b);
                
                
                { /* shooting particles */ 
                    float vel[2]; vec2_sub(vel, b.to, b.from);
                    float force[2] = { 0, 0 };
                    explosion e;
                    fx_explosion_init(fx, &e, b.from, vel, colors[selcol], 
                        force, 0.1, 2, 4, b.starttime, 0.2);
                    fx_add_explosion(fx, &e);
                }
                
                { /* hit particles */
                    float vel[2]; vec2_sub(vel, b.to, b.from);
                    float force[2] = { 0, 0 };
                    explosion e;
                    fx_explosion_init(fx, &e, b.to, vel, colors[1-selcol], 
                        force, 0.15, 4, 8, b.endtime, 0.8);
                    fx_add_explosion(fx, &e);
                }
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
    gst_ar_past_cycle(gst);
    gst_spawn_bullets(gst, fx, dmgs, fire, t);
}

void gst_process (gamestate *gst, infos *info, MKb *mkb, fxs *fx, float t) {
    if (gst->state == 1) {
        if (gst->waitstep) {
            if (mkb_search(mkb, SDL_SCANCODE_SPACE)) {
                gst_next_turn(gst, info, fx, t);
            }
        } else {
            if (gst->starttime > t) gst->starttime = t; 
            float t_elapsed = t-gst->starttime;
            if (t_elapsed >= gst->coveredtime) {
                gst_next_turn(gst, info, fx, t);
            }
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
    
    army *past = ar;
    army *present = ar;
    if (gst->state == 1) {
        past = gst->ar_past +1;
        present = gst->ar_past +0;
    }
    
    // render units
    for (int i=0; i<ar->uslen; i++) {
        if (present->us[i].hp <= 0) continue;
        
        float past_x = past->us[i].pos[0];
        float past_y = past->us[i].pos[1];
        
        float present_x = present->us[i].pos[0];
        float present_y = present->us[i].pos[1];
        
        float x = present_x*(amt) + past_x*(1-amt);
        float y = present_y*(amt) + past_y*(1-amt);
        
        SDL_Rect srcRect = { 
            present->us[i].info.chassis*ts, ts, ts, ts };
        SDL_Rect dstRect = { (int)x-posx, (int)y-posy, ts, ts };
        SDL_RenderCopy(rend, txsprites, &srcRect, &dstRect);
        
        stats_unit base; 
        stats_unit_compute(info, &present->us[i].info, &base);
        float amt = present->us[i].hp / base.frame.hp;
        SDL_Rect hprect = { 
            (int)x-posx, (int)y-posy+ts-5, 
            ts*amt, 6 };
        int sw = 1 ? ar->us[i].owner : 0;
        SDL_SetRenderDrawColor(rend, 255*sw, 255*(1-sw), 0, 255);
        SDL_RenderFillRect(rend, &hprect);
        
        SDL_SetTextureColorMod(textd->tex_small, sw*100, 100*(1-sw), 0);
        char shp[32]; sprintf(shp, "%.0f", present->us[i].hp);
        float php[2] = { (int)x-posx, (int)y-posy+ts-5 };
        render_text_small(rend, shp, php, textd);
        
        SDL_SetTextureColorMod(textd->tex_small, 255, 160, 0);
        char sch[32]; sprintf(sch, "%.0f", present->us[i].charge);
        float pch[2] = { (int)x-posx, (int)y-posy+ts+1 };
        render_text_small(rend, sch, pch, textd);
        SDL_SetTextureColorMod(textd->tex_small, 0, 0, 0);
    }
}