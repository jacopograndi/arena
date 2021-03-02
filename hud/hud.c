#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "hud.h"
#include "hud_views.h"

#include "../umath/intersect.h"

// TODO: make sound module mabye?
#define SOUND_MOUSE_OVER 0
#define SOUND_MOUSE_CLICK_0 1
#define SOUND_MOUSE_WHEEL 2
#define SOUND_SUCCESS 3


void SDL_Rect_init (SDL_Rect *rect, float x, float y, float w, float h) {
    rect->x = x; rect->y = y; rect->w = w; rect->h = h;
}


// INIT
void init_overlay_battle (graphic_settings *gs, overlay_battle *ob,
    txtd *t) 
{
    int w = 400, h = 100;
    int x = gs->resx-w-10, y = 10;
    SDL_Rect_init(&ob->rect_back, x, y, w, h);
}

void init_sel_chassis (graphic_settings *gs, hud_sel *sc, txtd *t, 
    SDL_Rect back, float start) 
{
    sc->start = start;
    sc->nav = 0;
    sc->rect_back = back;
}

void init_form_new_unit (graphic_settings *gs, form_new_unit *fnu, txtd *t) {
    int w = 290+200+300+20*2+10*4 + 30*3 + 200, h = 145+300+20*2+10*1 + 30*2;
    int x = gs->resx/2-w/2, y = gs->resy/2-h/2;
    SDL_Rect_init(&fnu->rect_back, x, y, w, h);
    
    SDL_Rect_init(&fnu->rect_chassis, x+20, y+20+145+10+30, 300, 300);
    SDL_Rect_init(&fnu->rect_brain, x+20, y+20+30, 145, 145);
    SDL_Rect_init(&fnu->rect_battery, 0+25, y+20+30, 145, 145);
    for (int i=0; i<8; i++) {
        SDL_Rect_init(&fnu->rect_weapons[i], 
            x+20+300+10+30, y+20+120*i +30, 200, 110);
    }
    for (int i=0; i<8; i++) {
        SDL_Rect_init(&fnu->rect_armor[i], 
            x+20+500+20+30*2, y+20+60*i +30, 150, 50);
    }
    for (int i=0; i<8; i++) {
        SDL_Rect_init(&fnu->rect_augs[i],
            x+20+500+20+160+30*2, y+20+60*i +30, 150, 50);
    }
    
    SDL_Rect_init(&fnu->rect_stats, x+w-20-200, y+20, 200, h-70 );
    
    int width = get_text_width("save", t);
    button bdone = { "save", 4, { x+w-4*2-width-20, y+h-4*2-11-20 } };
    fnu->done = bdone;
}

void init_overlay_game (graphic_settings *gs, overlay_game *og, txtd *t) {
    int w = 250, h = gs->resy-20;
    SDL_Rect_init(&og->rect_templates, 10, gs->resy-10-h, w, h);
    
    float wnu = get_text_width("new template", t);
    button b = { "new template", 4, { w-wnu-4*2+5, 10+5 } };
    og->new_template = b;
    
    float wst = get_text_width("save templates", t);
    button b2 = { "save templates", 4, { w-wst-4*2+5, gs->resy-20-4*2-5 } };
    og->save_templates = b2;
    
    int warmy = 250, harmy = gs->resy-20;
    SDL_Rect_init(&og->rect_army, 
        gs->resx-warmy-10, gs->resy-harmy-10, warmy, harmy);
    button b3 = { "save army", 4, { gs->resx-warmy-5, gs->resy-20-4*2-5 } };
    og->save_army = b3;
    
    { 
        int w = get_text_width("delete army", t);
        button b = { "delete army", 4, { 
            gs->resx-w-4*2-15, gs->resy-20-4*2-5 } };
        og->delete_army = b; 
    }
    
    { 
        int w = get_text_width("new army", t);
        button b = { "new army", 4, { 
            gs->resx-warmy-5, gs->resy-15-4*4-20-5 } };
        og->new_army = b; 
    }
    
    int wbattle = 400, hbattle = 100;
    SDL_Rect_init(&og->rect_battle, gs->resx/2-wbattle/2, 10, wbattle, hbattle);
    
    float wsb = get_text_width("start battle", t);
    button b1 = { "start battle", 4, 
        { og->rect_battle.x+wbattle-wsb-4*2-5, hbattle-4*2-5 } };
    og->start_battle = b1;
    
    button b4 = { "host game", 4, 
        { og->rect_battle.x+5, hbattle-4*2-5 } };
    og->host_game = b4;
    
    button b5 = { "join game", 4, 
        { og->rect_battle.x+5, hbattle-4*2-5-25 } };
    og->join_game = b5;
}

void hud_reset (graphic_settings *gs, hud *h, txtd *t) {
    init_form_new_unit(gs, &h->fnu, t);
    init_overlay_battle(gs, &h->ob, t);
    init_overlay_game(gs, &h->og, t);
    SDL_Rect clip = { h->fnu.rect_back.x, h->sc.ref->y,
        h->fnu.rect_back.w, 300 };
    float start = h->sc.ref->x - h->fnu.rect_back.x;
    init_sel_chassis(gs, &h->sc, t, clip, start);
}

void hud_init (graphic_settings *gs, hud *h, txtd *t) {    
    h->fnu.sel = 0; h->fnu.ind = 0;
    h->sc.ref = &h->fnu.rect_chassis;
    h->og.temp_place = -1;
    info_unit_init(&h->fnu.uinfo);
    h->og.army_listcur = 0;
    strcpy(h->og.playername, "");
    strcpy(h->og.ip, "192.168.1.255");
    strcpy(h->ob.oppo, "cpu");
    h->nameedit = NULL;
    h->og.battle_state = 0;
    h->og.input_playername = 0;
    h->og.input_ip = 0;
    h->og.input_army = -1;
    h->og.input_temp = -1;
    h->og.start_battle_flag = 0;
    hud_reset(gs, h, t);
}

void hud_resize (graphic_settings *gs, hud *h, txtd *t) {
    hud_reset(gs, h, t);
}


// PROCESS
int hud_fnu_check (info_unit *u, infos *info) {
    if (u->chassis == -1) return 1;
    if (u->battery == -1) return 2;
    if (u->brain == -1) return 3;
    int chassis_lvl = u->levels[LEVEL_CHASSIS];
        
    stats_unit base;
    stats_unit_compute(info, u, &base);
    if (base.frame.weight > base.frame.weight_max) { return 4; }
    return 0;
}

void hud_map_sel (info_unit *u, infos *info, int sel, int ind, 
    int8_t **n, int *bound, int size[]) 
{
    *bound = info->statslen[sel];
    if (sel == 0) { 
        *n = &u->chassis; 
        size[0] = 300; size[1] = 300;
    }
    if (sel == 1) { 
        *n = &u->brain; 
        size[0] = 145; size[1] = 145;
    }
    if (sel == 2) { 
        *n = &u->battery; 
        size[0] = 145; size[1] = 145;
    }
    if (sel == 3) { 
        *n = &u->weapons[ind]; 
        size[0] = 200; size[1] = 170;
    }
    if (sel == 4) { 
        *n = &u->armor[ind]; 
        size[0] = 150; size[1] = 200;
    }
    if (sel == 5) { 
        *n = &u->augs[ind]; 
        size[0] = 150; size[1] = 200;
    }
}

void hud_process_sel (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, Mix_Chunk *sounds[]) 
{
    if (mkb->mwheeldelta != 0) {
        Mix_PlayChannel( -1, sounds[SOUND_MOUSE_WHEEL], 0 );
    }
    int8_t *n = NULL; int bound = 0; int size[2];
    hud_map_sel(&h->fnu.uinfo, info, h->fnu.sel, h->fnu.ind, &n, &bound, size);
    h->sc.nav += mkb->mwheeldelta*size[0];
    if (h->sc.nav > (*n+1)*size[0]) {
        h->sc.nav = (*n+1)*size[0];
    }
    if (h->sc.nav < (*n-bound+1)*size[0]) {
        h->sc.nav = (*n-bound+1)*size[0];
    }
    float mousepos[2] = { mkb->mx, mkb->my };
    if (mkb->mheld[0] == 1) {
        float possc[2] = { h->sc.rect_back.x, h->sc.rect_back.y };
        float sizesc[2] = { h->sc.rect_back.w, h->sc.rect_back.h };
        if (!pt_rect(mousepos, possc, sizesc)) {
            h->state = 1;
        } else {
            int i, inav = (int)h->sc.nav/size[0];
            for (int j=-7;j<h->sc.rect_back.w/size[0]+1; j++) {
                i = *n+j-inav; 
                if (i<-1) continue; 
                if (i>=bound) break;
                float x = h->sc.rect_back.x+size[0]*j
                    +(int)h->sc.nav%size[0]+h->sc.start;
                float y = h->sc.rect_back.y;
                float pos[2] = {x, y}, s[2] = { size[0], size[1] };
                if (pt_rect(mousepos, pos, s)) {
                    *n = i;
                    h->state = 1;
                    Mix_PlayChannel( -1, sounds[SOUND_MOUSE_CLICK_0], 0 );
                    break;
                }
            }
        }
    }
}


void hud_edit_close(hud *h, infos *info) {
    if (h->og.input_playername != 0) {
        info_save_playername(h->og.playername);
    }
    h->og.input_playername = 0;
    h->og.input_ip = 0;
    if (h->og.input_army != -1) {
        info_army_rename(h->og.army_rename, h->nameedit);
    }
    h->og.input_army = -1;
    if (h->og.input_temp != -1) {
        info_save_templates(info, "default");
    }
    h->og.input_temp = -1;
    h->nameedit = NULL;
}

void hud_open_fnu (hud *h, infos *info, info_unit *u, int i) {
    h->state = 1;
    h->fnu.uinfo = *u;
    h->og.temp_modify = i;
    hud_edit_close(h, info);
}

void hud_close_fnu (hud *h, infos *info) {
    if (hud_fnu_check(&h->fnu.uinfo, info) > 0) return;
    if (h->og.temp_modify == -1) {
        info_template_add(info, &h->fnu.uinfo);
    } else {
        info->templates[h->og.temp_modify] = h->fnu.uinfo;
    }
    info_save_templates(info, "default");
    h->state = 0;
}


void hud_og_place(hud *h, int i) {
    h->og.temp_place = i;
}


void hud_open_sel (graphic_settings *gs, hud *h, 
    txtd *t, infos *info, SDL_Rect *ref) 
{
    int8_t *n = NULL; int bound = 0; int size[2];
    hud_map_sel(&h->fnu.uinfo, info, h->fnu.sel, 
        h->fnu.ind, &n, &bound, size);
    h->sc.nav = 0; if (*n == -1) h->sc.nav -= size[0];
    h->sc.ref = ref;
    hud_resize(gs, h, t);
    h->sc.rect_back.h = size[1];
}

int hud_mouse_level (float *mp, float *pos, float *size) {
    for (int i=0; i<MAXLEVEL; i++) {
        float p[2] = { 
            pos[0]+size[0]-5-MAXLEVEL*9-1 + 9*i, 
            pos[1]+size[1]-5-9-1 -1 };
        float s[2] = { 10, 10 };
        if (pt_rect(mp, p, s)) { return i; }
    }
    return -1;
}

void hud_process_form_new_unit (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, Mix_Chunk *sounds[]) 
{
    float mousepos[2] = { mkb->mx, mkb->my };
    if (mkb->mheld[0] == 1) {
        if (mouse_in_button(mousepos, t, &h->fnu.done)) {
            hud_close_fnu(h, info);
            Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
            /*
            for (int i=0; i<ar->uslen; i++) {
                unit *u = ar->us+i;
                unit_init(info, ar, m, u->gridpos[0], u->gridpos[1], 
                        &u->info, 0, u);
            }*/
        }
        float possc[2] = { h->fnu.rect_chassis.x, h->fnu.rect_chassis.y };
        float sizesc[2] = { h->fnu.rect_chassis.w, h->fnu.rect_chassis.h };
        if (pt_rect(mousepos, possc, sizesc)) {
            int lvl = hud_mouse_level(mousepos, possc, sizesc);
            if (lvl != -1) h->fnu.uinfo.levels[LEVEL_CHASSIS] = lvl;
            else {
                h->fnu.sel = 0; h->state = 2;
                hud_open_sel(gs, h, t, info, &h->fnu.rect_chassis);
            }
        }
        float possb[2] = { h->fnu.rect_battery.x, h->fnu.rect_battery.y };
        float sizesb[2] = { h->fnu.rect_battery.w, h->fnu.rect_battery.h };
        if (pt_rect(mousepos, possb, sizesb)) {
            int lvl = hud_mouse_level(mousepos, possb, sizesb);
            if (lvl != -1) h->fnu.uinfo.levels[LEVEL_BATTERY] = lvl;
            else {
                h->fnu.sel = 2; h->state = 2;
                hud_open_sel(gs, h, t, info, &h->fnu.rect_battery);
            }
        }
        float possbr[2] = { h->fnu.rect_brain.x, h->fnu.rect_brain.y };
        float sizesbr[2] = { h->fnu.rect_brain.w, h->fnu.rect_brain.h };
        if (pt_rect(mousepos, possbr, sizesbr)) {
            int lvl = hud_mouse_level(mousepos, possbr, sizesbr);
            if (lvl != -1) h->fnu.uinfo.levels[LEVEL_BRAIN] = lvl;
            else {
                h->fnu.sel = 1; h->state = 2;
                hud_open_sel(gs, h, t, info, &h->fnu.rect_brain);
            }
        }
        if (h->fnu.uinfo.chassis != -1) {
            int lc = h->fnu.uinfo.levels[LEVEL_CHASSIS];
            stats_comp *comp = 
                info->stats[STATS_CHASSIS] +h->fnu.uinfo.chassis;
                
            for (int i=0; i<comp->base[lc].slot_armor; i++) {
                float possa[2] = { 
                    h->fnu.rect_armor[i].x, h->fnu.rect_armor[i].y };
                float sizesa[2] = { 
                    h->fnu.rect_armor[i].w, h->fnu.rect_armor[i].h };
                if (pt_rect(mousepos, possa, sizesa)) {
                    int lvl = hud_mouse_level(mousepos, possa, sizesa);
                    if (lvl != -1) h->fnu.uinfo.levels[LEVEL_ARMOR+i] = lvl;
                    else {
                        h->fnu.sel = 4; h->fnu.ind = i; h->state = 2;
                        hud_open_sel(gs, h, t, info, &h->fnu.rect_armor[i]);
                    }
                }
            }
            for (int i=0; i<comp->base[lc].slot_weapon; i++) {
                float possa[2] = { 
                    h->fnu.rect_weapons[i].x, h->fnu.rect_weapons[i].y };
                float sizesa[2] = { 
                    h->fnu.rect_weapons[i].w, h->fnu.rect_weapons[i].h };
                if (pt_rect(mousepos, possa, sizesa)) {
                    int lvl = hud_mouse_level(mousepos, possa, sizesa);
                    if (lvl != -1) h->fnu.uinfo.levels[LEVEL_WEAPONS+i] = lvl;
                    else {
                        h->fnu.sel = 3; h->fnu.ind = i; h->state = 2;
                        hud_open_sel(gs, h, t, info, &h->fnu.rect_weapons[i]);
                    }
                }
            }
            for (int i=0; i<comp->base[lc].slot_armor; i++) {
                float possa[2] = { 
                    h->fnu.rect_augs[i].x, h->fnu.rect_augs[i].y };
                float sizesa[2] = { 
                    h->fnu.rect_augs[i].w, h->fnu.rect_augs[i].h };
                if (pt_rect(mousepos, possa, sizesa)) {
                    int lvl = hud_mouse_level(mousepos, possa, sizesa);
                    if (lvl != -1) h->fnu.uinfo.levels[LEVEL_AUGS+i] = lvl;
                    else {
                        h->fnu.sel = 5; h->fnu.ind = i; h->state = 2;
                        hud_open_sel(gs, h, t, info, &h->fnu.rect_augs[i]);
                    }
                }
            }
        }
    }
}

void hud_process_overlay_battle (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, gamestate *gst, 
    Mix_Chunk *sounds[]) 
{
    if (gst->over == 1) {
        h->state = 0;
        gst->state = 0;
        gst->cam[0] = -gs->resx/2+gst->map_editor.sx*gst->map_editor.ts/2;
        gst->cam[1] = -gs->resy/2+gst->map_editor.sy*gst->map_editor.ts/2;
        gst_toeditor(gst);
    }
}

void hud_process_overlay_game (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, gamestate *gst, 
    net_client *netc, net_server *nets, Mix_Chunk *sounds[]) 
{
    if (h->og.input_army != -1) {
        // hack to not pass gst to everything
        strcpy(gst->army_bp[0].name, h->nameedit);
    }
    if (h->og.battle_state == 1) {
        if (nets->sock_client == NULL) {
            net_server_accept(nets);
        } else {
            char buffer[1024*64];
            int len = net_server_recv(nets, buffer);
            if (len != -1) {
                h->og.battle_state = 3;

                int armysize = sizeof(unit)*gst->army_bp[0].uslen + 64;
                char data[armysize];
                memcpy(data, gst->army_bp[0].us, armysize);
                memcpy(data+armysize-64, h->og.playername, 32);
                memcpy(data+armysize-32, gst->army_bp[0].name, 32);
                
                net_server_send(nets, data, armysize);
                printf("send (%d)\n", armysize);
            
                int arlen = len-64;
                memcpy(gst->army_bp[1].us, buffer, arlen);
                gst->army_bp[1].uslen = arlen/sizeof(unit);
                memcpy(h->ob.oppo, buffer+arlen, 32);
                memcpy(gst->army_bp[1].name, buffer+arlen+32, 32);
                gst->playernum = 2;
                gst_tobattle(gst, info);
                gst->cam[0] = -gs->resx/2+gst->map_battle.sx*gst->map_battle.ts/2;
                gst->cam[1] = -gs->resy/2+gst->map_battle.sy*gst->map_battle.ts/2;
                h->state = 3;
                net_server_close(nets);
                hud_edit_close(h, info);
            }
        }
    }
    if (h->og.battle_state == 2) {
        char buffer[1024*64];
        int len = net_client_recv(netc, buffer);
        if (len != -1) {
            h->og.battle_state = 3;
            
            int arlen = len-64;
            memcpy(gst->army_bp[1].us, buffer, arlen);
            gst->army_bp[1].uslen = arlen/sizeof(unit);
            memcpy(h->ob.oppo, buffer+arlen, 32);
            memcpy(gst->army_bp[1].name, buffer+arlen+32, 32);
            gst->playernum = 2;
            gst_tobattle(gst, info);
            gst->cam[0] = -gs->resx/2+gst->map_battle.sx*gst->map_battle.ts/2;
            gst->cam[1] = -gs->resy/2+gst->map_battle.sy*gst->map_battle.ts/2;
            h->state = 3;
            net_client_close(netc);
            hud_edit_close(h, info);
        }
    }
            
    float mousepos[2] = { mkb->mx, mkb->my };
    if (mkb->mheld[0] == 1) {
        if (mouse_in_button(mousepos, t, &h->og.new_template)) {
            info_unit u; info_unit_init(&u);
            hud_open_fnu(h, info, &u, -1);
            h->og.temp_place = -1;
        }
        
        if (mouse_in_button(mousepos, t, &h->og.save_templates)) {
            info_save_templates(info, "default");
            Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
        }
        
        if (mouse_in_button(mousepos, t, &h->og.join_game)) {
            h->og.battle_state = 0;
            net_server_close(nets);
            printf("open client\n");
            net_client_open(netc);
            int conn = net_client_connect(netc, h->og.ip, SERVER_PORT);
            if (conn == 0) {
                int armysize = sizeof(unit)*gst->army_bp[0].uslen + 64;
                char data[armysize];
                memcpy(data, gst->army_bp[0].us, armysize);
                memcpy(data+armysize-64, h->og.playername, 32);
                memcpy(data+armysize-32, gst->army_bp[0].name, 32);
                net_client_send(netc, data, armysize);
                printf("send (%d)\n", armysize);
                h->og.battle_state = 2;
            } else {
                printf("close client");
                net_client_close(netc);
            }
        }
        
        if (mouse_in_button(mousepos, t, &h->og.host_game)) {
            net_client_close(netc);
            printf("open server\n");
            net_server_open(nets, "127.0.0.1", SERVER_PORT);
            h->og.battle_state = 1;
        }
        
        if (mouse_in_button(mousepos, t, &h->og.save_army)) {
            int cur = h->og.army_listcur;
            info_save_army(gst->army_bp+0, h->og.army_list[cur]);
            h->og.army_listlen = info_army_get_list(h->og.army_list);
        }
        
        if (mouse_in_button(mousepos, t, &h->og.new_army)) {
            army ar; ar.uslen = 0; ar.sx = 10; ar.sy = 10;
            char name[32]; strcpy(name, "new army");
            char namecmp[32]; strcpy(namecmp, name);
            for (int i=0; i<128; i++) {
                int flag = 0;
                for (int j=0; j<h->og.army_listlen; j++) {
                    sprintf(namecmp, "%s %d", name, i);
                    if (strcmp(namecmp, h->og.army_list[j]) == 0) {
                        flag = 1; break;
                    }
                }
                if (flag == 0) break;
            }
            info_save_army(&ar, namecmp);
            h->og.army_listlen = info_army_get_list(h->og.army_list);
            for (int i=0; i<h->og.army_listlen; i++) {
                if (strcmp(namecmp, h->og.army_list[i]) == 0) {
                    h->og.army_listcur = i; break;
                }
            }
        }
        
        if (mouse_in_button(mousepos, t, &h->og.delete_army)) {
            if (h->og.army_listlen > 1) {
                if (h->og.input_army != -1) {
                    h->og.input_army = -1;
                    hud_edit_close(h, info);
                }
                int cur = h->og.army_listcur;
                info_army_remove(h->og.army_list[cur]);
                h->og.army_listlen = info_army_get_list(h->og.army_list);
                if (cur >= 0) h->og.army_listcur--;
                cur = h->og.army_listcur;
                info_load_army(gst->army_bp+0, h->og.army_list[cur]);
            }
        }
        
        if (h->og.temp_place != -1) {
            int x = (int)((mkb->mx+gst->cam[0])/32);
            int y = (int)((mkb->my+gst->cam[1])/32);
            if (x >= 0 && y >= 0 && x < m->sx && y < m->sy) {
                if (ar->grid[xytoi(m,x,y)] == NULL) {
                    unit u;
                    unit_init(info, ar, m, x, y, 
                        info->templates+h->og.temp_place, 0, &u);
                    
                    army_spawn(ar, m, u);
                }
            } else {
                h->og.temp_place = -1;
            }
        }

        for (int i=0; i<info->templateslen; i++) {
            float x = h->og.rect_templates.x+5;
            float y = h->og.rect_templates.y+5 + i*20 + 30;
            float wedit = get_text_width("edit", t);
            float possa[2] = { x, y };
            float sizesa[2] = { wedit+4*2, 11+4*2 };
            if (pt_rect(mousepos, possa, sizesa)) {
                hud_open_fnu(h, info, info->templates+i, i);
                h->og.temp_place = -1;
            }
            float wplace = get_text_width("place", t);
            float posp[2] = { x+wedit+4*2+5, y };
            float sizep[2] = { wplace+4*2, 11+4*2 };
            if (pt_rect(mousepos, posp, sizep)) {
                hud_og_place(h, i);
            }
            if (h->nameedit == NULL) {
                float pn[2] = { x+wedit+5+wplace+4*4, y+4 };
                char *sname = info->templates[i].name;
                float w = get_text_width(sname, t);
                if (w==0) w = 5;
                float sizen[2] = { w, 10 };
                if (pt_rect(mousepos, pn, sizen)) {
                    h->nameedit = sname;
                    h->og.input_temp = i;
                }
            } 
        }

        for (int i=0; i<h->og.army_listlen; i++) {
            float x = h->og.rect_army.x+5;
            float y = h->og.rect_army.y+5 + i*20 + 50;
            float wload = get_text_width("load", t)+4*2;
            float posp[2] = { x, y };
            float sizep[2] = { wload, 11+4*2 };
            if (pt_rect(mousepos, posp, sizep)) {
                if (h->og.start_battle_flag == 1) {
                    // start battle
                    h->og.start_battle_flag = 0;
                    hud_edit_close(h, info);
                    info_load_army(gst->army_bp+1, h->og.army_list[i]);
                    gst->playernum = 2;
                    gst_tobattle(gst, info);
                    gst->cam[0] = -gs->resx/2
                        +gst->map_battle.sx*gst->map_battle.ts/2;
                    gst->cam[1] = -gs->resy/2
                        +gst->map_battle.sy*gst->map_battle.ts/2;
                    h->og.battle_state = 3;
                    h->state = 3;
                    Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
                } else {
                    int cur = h->og.army_listcur;
                    info_save_army(gst->army_bp+0, h->og.army_list[cur]);
                    h->og.army_listcur = i;
                    info_load_army(gst->army_bp+0, h->og.army_list[i]);
                    Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
                }
            }
            else if (h->nameedit == NULL) {
                float pn[2] = { x+wload+5, y+4 };
                float w = get_text_width(h->og.army_list[i], t);
                if (w==0) w = 5;
                float sizen[2] = { w, 10 };
                if (pt_rect(mousepos, pn, sizen)) {
                    h->nameedit = h->og.army_list[i];
                    strcpy(h->og.army_rename, h->nameedit);
                    h->og.input_army = i;
                }
            } 
        }
        
        if (h->nameedit == NULL) {
            float pn[2] = { h->og.rect_battle.x+5, h->og.rect_battle.y+5 };
            char sn[64]; sprintf(sn, "PLAYER NAME: %s", h->og.playername);
            float sizen[2] = { get_text_width(sn, t), 10 };
            if (pt_rect(mousepos, pn, sizen)) {
                h->nameedit = h->og.playername;
                h->og.input_playername = 1;
            } 
        } 
        
        if (h->nameedit == NULL) {
            float wjoin = get_text_width("join game", t);
            float pn[2] = { h->og.join_game.pos[0]+wjoin + 4*2+10, 
                h->og.join_game.pos[1]+4 };
            char sn[64]; sprintf(sn, "IP: %s", h->og.playername);
            float sizen[2] = { get_text_width(sn, t), 10 };
            if (pt_rect(mousepos, pn, sizen)) {
                h->nameedit = h->og.ip;
                h->og.input_ip = 1;
            }
        } 
        if (h->og.start_battle_flag == 1) {
            h->og.start_battle_flag = 0;
        }
        
        if (mouse_in_button(mousepos, t, &h->og.start_battle)) {
            h->og.start_battle_flag = 1;
        }
    }
    
    // rm unit
    if (mkb->mheld[2] == 1) {
        Mix_PlayChannel( -1, sounds[SOUND_MOUSE_CLICK_0], 0 );
        int x = (int)((mkb->mx+gst->cam[0])/32);
        int y = (int)((mkb->my+gst->cam[1])/32);
        if (x >= 0 && y >= 0 && x < m->sx && y < m->sy) {
            if (ar->grid[xytoi(m,x,y)] != NULL) {
                unit_remove(ar, m, ar->grid[xytoi(m,x,y)]);
            }
        } 
    }
}

void hud_edit_name(hud *h, infos *info, MKb *mkb, Mix_Chunk *sounds[]) {
    if (h->nameedit == NULL) return;
    for (int i=0; i<mkb->kbnum; i++) {
        if (strlen(h->nameedit) < 31) {
            if (mkb->kb[i] >= SDL_SCANCODE_A
             && mkb->kb[i] <= SDL_SCANCODE_Z) {
                char c = mkb->kb[i]-SDL_SCANCODE_A+'a';
                if (SDL_GetModState() & KMOD_SHIFT) {
                    c = mkb->kb[i]-SDL_SCANCODE_A+'A';
                }
                sprintf(h->nameedit, "%s%c", h->nameedit, c);
            }
            if (mkb->kb[i] >= SDL_SCANCODE_1
             && mkb->kb[i] <= SDL_SCANCODE_9) {
                char c = mkb->kb[i]-SDL_SCANCODE_1+'1';
                sprintf(h->nameedit, "%s%c", h->nameedit, c);
            }
            if (mkb->kb[i] >= SDL_SCANCODE_KP_1
             && mkb->kb[i] <= SDL_SCANCODE_KP_9) {
                char c = mkb->kb[i]-SDL_SCANCODE_KP_1+'1';
                sprintf(h->nameedit, "%s%c", h->nameedit, c);
            }
            if (mkb->kb[i] == SDL_SCANCODE_KP_0
             || mkb->kb[i] == SDL_SCANCODE_0) {
                sprintf(h->nameedit, "%s0", h->nameedit);
            }
            if (mkb->kb[i] == SDL_SCANCODE_KP_PERIOD
             || mkb->kb[i] == SDL_SCANCODE_PERIOD) {
                sprintf(h->nameedit, "%s.", h->nameedit);
            }
        }
    }
    if (mkb_search(mkb, SDL_SCANCODE_SPACE)) {
        Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
        if (strlen(h->nameedit) < 31) {
            sprintf(h->nameedit, "%s ", h->nameedit);
        }
    }
    if (mkb_search(mkb, SDL_SCANCODE_BACKSPACE)) {
        if (strlen(h->nameedit) > 0)
        h->nameedit[strlen(h->nameedit)-1] = '\0';
    }
    if (mkb_search(mkb, SDL_SCANCODE_ESCAPE) 
     || mkb_search(mkb, SDL_SCANCODE_RETURN)) {
        Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
        hud_edit_close(h, info); return;
    }
}

void hud_process (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, gamestate *gst, 
    net_client *netc, net_server *nets, Mix_Chunk *sounds[]) 
{
    if (h->nameedit != NULL) { hud_edit_name(h, info, mkb, sounds); }
    switch (h->state) {
        case 0: 
            hud_process_overlay_game(gs, h, mkb, info, ar, m, t, gst, 
                netc, nets, sounds); 
            break;
        case 1: hud_process_form_new_unit(gs, h, mkb, info, ar, m, t, sounds); 
            break;
        case 2: hud_process_sel(gs, h, mkb, info, ar, m, t, sounds); 
            break;
        case 3: hud_process_overlay_battle(gs, h, mkb, info, ar, m, t, gst, 
                sounds);
            break;
    }
}



// RENDER
void hud_render_sel (hud_sel *sc, MKb *mkb, info_unit *u,
    SDL_Renderer* rend, SDL_Texture *sprites, txtd *t, infos *info, 
    int sel, int ind, float time) 
{
    int8_t *n = NULL; int bound = 0; int size[2];
    hud_map_sel(u, info, sel, ind, &n, &bound, size);
    SDL_SetRenderDrawColor(rend, 150, 200, 0, 255);
    SDL_RenderFillRect(rend, &sc->rect_back);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &sc->rect_back);
    SDL_RenderSetClipRect(rend, &sc->rect_back);
    int i;
    for (int j=-7;j<sc->rect_back.w/size[0]+1; j++) {
        i = *n+j - (int)sc->nav/size[0]; 
        if (i<-1) continue; 
        if (i>=bound) break;
        float x = sc->rect_back.x+size[0]*j+(int)sc->nav%size[0]+sc->start;
        float y = sc->rect_back.y;
        SDL_Rect r = { x, y, size[0]+1, size[1] };
        SDL_SetRenderDrawColor(rend, 150, 200, 120, 255);
        SDL_RenderFillRect(rend, &r);
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderDrawRect(rend, &r);
        if (sel==0) {
            int lvl = u->levels[LEVEL_CHASSIS];
            render_view_chassis(rend, t, x, y, info, i, lvl, sprites);
        }
        if (sel==1) {
            int lvl = u->levels[LEVEL_BRAIN];
            render_view_brain(rend, t, x, y, info, i, lvl);
        }
        if (sel==2) {
            int lvl = u->levels[LEVEL_BATTERY];
            render_view_battery(rend, t, x, y, info, i, lvl);
        }
        if (sel==3) {
            int lvl = u->levels[LEVEL_WEAPONS+ind];
            render_view_weapon_detail(rend, t, x, y, info, i, lvl);
        }
        if (sel==4) {
            int lvl = u->levels[LEVEL_ARMOR+ind];
            render_view_armor_detail(rend, t, x, y, info, i, lvl);
        }
        if (sel==5) {
            int lvl = u->levels[LEVEL_AUGS+ind];
            render_view_aug_detail(rend, t, x, y, info, i, lvl);
        }
    }
    SDL_RenderSetClipRect(rend, NULL);
}

void hud_render_form_new_unit (form_new_unit *fnu, MKb *mkb,
    SDL_Renderer* rend, txtd *t, infos *info, SDL_Texture *sprites, float time)
{    
    SDL_SetRenderDrawColor(rend, 150, 200, 255, 255);
    SDL_RenderFillRect(rend, &fnu->rect_back);
    
    SDL_SetRenderDrawColor(rend, 200, 0, 255, 255);
    SDL_RenderFillRect(rend, &fnu->rect_chassis);
    SDL_RenderFillRect(rend, &fnu->rect_brain);
    SDL_RenderFillRect(rend, &fnu->rect_battery);
    
    SDL_SetRenderDrawColor(rend, 255, 150, 50, 255);
    SDL_RenderFillRect(rend, &fnu->rect_stats);
    
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &fnu->rect_back);
    SDL_RenderDrawRect(rend, &fnu->rect_chassis);
    SDL_RenderDrawRect(rend, &fnu->rect_brain);
    SDL_RenderDrawRect(rend, &fnu->rect_battery);
    SDL_RenderDrawRect(rend, &fnu->rect_stats);
    
    render_view_stats(rend, t, fnu->rect_stats.x, fnu->rect_stats.y, 
        info, &fnu->uinfo);
    
    int lc = fnu->uinfo.levels[LEVEL_CHASSIS];
    if (fnu->uinfo.chassis != -1) {
        stats_comp *comp = 
            info->stats[STATS_CHASSIS] +fnu->uinfo.chassis;
            
        for (int i=0; i<comp->base[lc].slot_weapon; i++) {
            SDL_SetRenderDrawColor(rend, 200, 100, 255, 255);
            SDL_RenderFillRect(rend, fnu->rect_weapons+i);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderDrawRect(rend, fnu->rect_weapons+i);
            render_view_weapon(rend, t, 
                fnu->rect_weapons[i].x, 
                fnu->rect_weapons[i].y, info, fnu->uinfo.weapons[i], 
                fnu->uinfo.levels[LEVEL_WEAPONS+i]); 
        }
        for (int i=0; i<comp->base[lc].slot_armor; i++) {
            SDL_SetRenderDrawColor(rend, 200, 200, 255, 255);
            SDL_RenderFillRect(rend, fnu->rect_armor+i);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderDrawRect(rend, fnu->rect_armor+i);
            render_view_armor(rend, t, 
                fnu->rect_armor[i].x, 
                fnu->rect_armor[i].y, info, fnu->uinfo.armor[i], 
                fnu->uinfo.levels[LEVEL_ARMOR+i]); 
        }
        for (int i=0; i<comp->base[lc].slot_aug; i++) {
            SDL_SetRenderDrawColor(rend, 200, 200, 255, 255);
            SDL_RenderFillRect(rend, fnu->rect_augs+i);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderDrawRect(rend, fnu->rect_augs+i);
            render_view_aug(rend, t, 
                fnu->rect_augs[i].x, 
                fnu->rect_augs[i].y, info, fnu->uinfo.augs[i], 
                fnu->uinfo.levels[LEVEL_AUGS+i]); 
        }
    }
    
    char schassis[32] = "CHASSIS"; 
    float wchassis = get_text_width(schassis, t)*2;
    float pchassis[2] = { 
        fnu->rect_chassis.x+fnu->rect_chassis.w/2 - wchassis / 2, 
        fnu->rect_chassis.y+fnu->rect_chassis.h + 7
    };
    render_text_scaled(rend, schassis, pchassis, t, 2);
    
    char sbrain[32] = "CONTROLLER"; 
    float wbrain = get_text_width(sbrain, t)*2;
    float pbrain[2] = { 
        fnu->rect_brain.x+fnu->rect_brain.w/2 - wbrain / 2, 
        fnu->rect_brain.y - 16-7
    };
    render_text_scaled(rend, sbrain, pbrain, t, 2);
    
    char sbattery[32] = "BATTERY"; 
    float wbattery = get_text_width(sbattery, t)*2;
    float pbattery[2] = { 
        fnu->rect_battery.x+fnu->rect_battery.w/2 - wbattery / 2, 
        fnu->rect_battery.y - 16-7
    };
    render_text_scaled(rend, sbattery, pbattery, t, 2);
    
    char sweapons[32] = "WEAPONS"; 
    float wweapons = get_text_width(sweapons, t)*2;
    float pweapons[2] = { 
        fnu->rect_weapons[0].x+fnu->rect_weapons[0].w/2 - wweapons / 2, 
        fnu->rect_weapons[0].y - 16-7
    };
    render_text_scaled(rend, sweapons, pweapons, t, 2);
    
    char sarmor[32] = "ARMOR"; 
    float warmor = get_text_width(sarmor, t)*2;
    float parmor[2] = { 
        fnu->rect_armor[0].x+fnu->rect_armor[0].w/2 - warmor / 2, 
        fnu->rect_armor[0].y - 16-7
    };
    render_text_scaled(rend, sarmor, parmor, t, 2);
    
    char saugs[32] = "AUGMENTS"; 
    float waugs = get_text_width(saugs, t)*2;
    float pagus[2] = { 
        fnu->rect_augs[0].x+fnu->rect_augs[0].w/2 - waugs / 2, 
        fnu->rect_augs[0].y - 16-7
    };
    render_text_scaled(rend, saugs, pagus, t, 2);
    
    render_button(rend, t, &fnu->done);
    
    int err = hud_fnu_check(&fnu->uinfo, info);
    char serr[32] = "ok"; 
    if (err==1) strcpy(serr, "select a chassis");
    if (err==2) strcpy(serr, "select a battery");
    if (err==3) strcpy(serr, "select a controller");
    if (err==4) strcpy(serr, "overburdened, remove weight");
    float werr = get_text_width(serr, t);
    float perr[2] = { 
        fnu->done.pos[0]-werr-10, 
        fnu->done.pos[1]+fnu->done.pad
    };
    render_text_scaled(rend, serr, perr, t, 1);

    render_view_chassis(rend, t, 
        fnu->rect_chassis.x, fnu->rect_chassis.y, 
        info, fnu->uinfo.chassis, 
        fnu->uinfo.levels[LEVEL_CHASSIS], sprites); 
    render_view_battery(rend, t, 
        fnu->rect_battery.x, fnu->rect_battery.y, info, fnu->uinfo.battery, 
        fnu->uinfo.levels[LEVEL_BATTERY]);
    render_view_brain(rend, t, 
        fnu->rect_brain.x, fnu->rect_brain.y, info, fnu->uinfo.brain, 
        fnu->uinfo.levels[LEVEL_BRAIN]); 
}

void hud_render_overlay_game (overlay_game *og, MKb *mkb, 
    SDL_Renderer* rend, txtd *t, infos *info, SDL_Texture *sprites, 
    gamestate *gst, float time)
{    
    SDL_SetRenderDrawColor(rend, 40, 150, 200, 255);
    SDL_RenderFillRect(rend, &og->rect_templates);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &og->rect_templates);
    char stemp[32] = "TEMPLATES"; 
    float ptemp[2] = { 
        og->rect_templates.x+10, 
        og->rect_templates.y+10
    };
    render_text_scaled(rend, stemp, ptemp, t, 2);
    
    render_button(rend, t, &og->new_template);
    render_button(rend, t, &og->save_templates);
    
    for (int i=0; i<info->templateslen; i++) {
        float px = og->rect_templates.x+5;
        float py = og->rect_templates.y+5 + i*20 + 30;
        { button b = { "edit", 4, { px, py } };
        render_button(rend, t, &b); }
        float wedit = get_text_width("edit", t); 
        
        { button b = { "place", 4, { px+wedit+2*4+5, py } };
        render_button(rend, t, &b); }
        float wplace = get_text_width("place", t); 
        
        float pname[2] = { wplace+wedit+4*4+10+px, py+4 };
        char *sname = info->templates[i].name;
        char sn[64]; 
        if (og->input_temp == i && time - ((long)time) < 0.5) {
            sprintf(sn, "%s_", sname);
        } else {
            sprintf(sn, "%s", sname);
        }
        render_text_scaled(rend, sn, pname, t, 1);
    }
    
    SDL_SetRenderDrawColor(rend, 0, 200, 120, 255);
    SDL_RenderFillRect(rend, &og->rect_army);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &og->rect_army);
    char sarmy[32] = "ARMY"; 
    float parmy[2] = {
        og->rect_army.x+10, 
        og->rect_army.y+10
    };
    render_text_scaled(rend, sarmy, parmy, t, 2);
    
    float cost = 0;
    for (int i=0; i<gst->army_bp[0].uslen; i++) {
        cost += 10;
    }
    float cx = og->rect_army.x+5;
    float cy = og->rect_army.y+5 + 30;
    float p[2] = { cx, cy };
    char s[64]; sprintf(s, "COST: %.0f", cost);
    render_text_scaled(rend, s, p, t, 1); 
    
    button b = { "load", 4, { 0, 0 } };
    for (int i=0; i<og->army_listlen; i++) {
        float x = og->rect_army.x+5;
        float y = og->rect_army.y+5 + i*20 + 50;
        float bw = get_text_width("load", t)+4*2;
        b.pos[0] = x; b.pos[1] = y;
        render_button(rend, t, &b);
        
        char sn[64]; 
        if (og->input_army == i && time - ((long)time) < 0.5) {
            sprintf(sn, "%s_", og->army_list[i]);
        } else {
            sprintf(sn, "%s", og->army_list[i]);
        }
        float pa[2] = { x+bw+5, y+4 };
        render_text_scaled(rend, sn, pa, t, 1);
        if (og->army_listcur == i) {
            float w = get_text_width(sn, t);
            float pe[2] = { x+bw+5+w+5, y+4 };
            render_text_scaled(rend, "<- editing", pe, t, 1);
        }
    }
    
    render_button(rend, t, &og->save_army);
    render_button(rend, t, &og->new_army);
    render_button(rend, t, &og->delete_army);
    
    SDL_SetRenderDrawColor(rend, 250, 60, 60, 255);
    SDL_RenderFillRect(rend, &og->rect_battle);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &og->rect_battle);
    
    float pn[2] = { og->rect_battle.x+5, og->rect_battle.y+5 };
    char sn[64]; 
    if (og->input_playername == 1 && time - ((long)time) < 0.5) {
        sprintf(sn, "PLAYER NAME: %s_", og->playername);
    } else {
        sprintf(sn, "PLAYER NAME: %s", og->playername);
    }
    render_text_scaled(rend, sn, pn, t, 1);
    
    float wjoin = get_text_width("join game", t);
    float pip[2] = { og->join_game.pos[0]+wjoin + 4*2+10, 
        og->join_game.pos[1]+4 };
    char sip[64]; 
    int _;
    if (og->input_ip == 1 && time - ((long)time) < 0.5) {
        sprintf(sip, "IP: %s_", og->ip);
    } else {
        sprintf(sip, "IP: %s", og->ip);
    }
    render_text_scaled(rend, sip, pip, t, 1);
    
    render_button(rend, t, &og->host_game);
    render_button(rend, t, &og->join_game);
    
    if (og->start_battle_flag == 0) {
        render_button(rend, t, &og->start_battle);
    } else {
        char stip[32] = "select an army to fight";
        float wbat = get_text_width("start battle", t);
        float wtip = get_text_width(stip, t);
        float ptip[2] = { 
            og->start_battle.pos[0]-wtip+wbat-5+4*2, 
            og->start_battle.pos[1]+4
        };
        render_text_scaled(rend, stip, ptip, t, 1); 
    }
    
    if (og->temp_place != -1) {
        SDL_Rect srcRect = { 
            info->templates[og->temp_place].chassis*32, 32, 32, 32 };
        SDL_Rect dstRect = { mkb->mx - 16, mkb->my - 16, 32, 32 };
        SDL_RenderCopy(rend, sprites, &srcRect, &dstRect);
    }
}

void hud_render_overlay_battle (overlay_battle *ob, MKb *mkb, 
    SDL_Renderer* rend, txtd *t, infos *info, gamestate *gst, float time) 
{    
    SDL_SetRenderDrawColor(rend, 240, 240, 240, 255);
    SDL_RenderFillRect(rend, &ob->rect_back);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &ob->rect_back);
    
    float x = ob->rect_back.x, y = ob->rect_back.y;
    float w = ob->rect_back.w;
    
    float h = 10;
    { 
        float p[2] = { x+10, y+h };
        char s[32]; sprintf(s, "vs: %s", ob->oppo);
        render_text_scaled(rend, s, p, t, 2);
    } h += 35;
    { 
        float p[2] = { x+10, y+h };
        char s[64]; sprintf(s, "%s", gst->army_bp[0].name);
        render_text_scaled(rend, s, p, t, 1);
        
        char p2[64]; sprintf(p2, "%s", gst->army_bp[1].name);
        float p2w = get_text_width(p2, t);
        float q[2] = { x+w-10-p2w, y+h };
        render_text_scaled(rend, p2, q, t, 1);
    } h += 20;
    { 
        float cost = 0;
        for (int i=0; i<gst->army_bp[0].uslen; i++) {
            cost += 10;
        }
        float p[2] = { x+10, y+h };
        char s[64]; sprintf(s, "COST: %.0f", cost);
        render_text_scaled(rend, s, p, t, 1); 
        
        float cost2 = 0;
        for (int i=0; i<gst->army_bp[1].uslen; i++) {
            cost2 += 10;
        }
        char p2[64]; sprintf(p2, "COST: %.0f", cost2);
        float p2w = get_text_width(p2, t);
        float q[2] = { x+w-10-p2w, y+h };
        render_text_scaled(rend, p2, q, t, 1);
    } h += 15;
    { 
        float dps = 0;
        for (int i=0; i<gst->ar.uslen; i++) {
            if (gst->ar.us[i].hp <= 0) continue;
            if (gst->ar.us[i].owner == 0) {
                dps += 3;
            }
        }
        float p[2] = { x+10, y+h };
        char s[64]; sprintf(s, "DAMAGE: %.0f", dps);
        render_text_scaled(rend, s, p, t, 1); 
        
        float dps2 = 0;
        for (int i=0; i<gst->ar.uslen; i++) {
            if (gst->ar.us[i].hp <= 0) continue;
            if (gst->ar.us[i].owner == 1) {
                dps2 += 3;
            }
        }
        char p2[64]; sprintf(p2, "DAMAGE: %.0f", dps2);
        float p2w = get_text_width(p2, t);
        float q[2] = { x+w-10-p2w, y+h };
        render_text_scaled(rend, p2, q, t, 1);
    } h += 15;
}

void hud_render (hud *h, SDL_Renderer* rend, txtd *t, MKb *mkb, infos *info, 
    SDL_Texture *sprites, gamestate *gst, float time) 
{
    switch (h->state) {
        case 0: 
            hud_render_overlay_game(&h->og, mkb, rend, t, info, sprites, gst,
                time); 
            break;
        case 1: 
            hud_render_overlay_game(&h->og, mkb, rend, t, info, sprites, gst,
                time); 
            hud_render_form_new_unit(&h->fnu, mkb, rend, t, info, sprites, 
                time); 
            break;
        case 2:
            hud_render_overlay_game(&h->og, mkb, rend, t, info, sprites, gst,
                time); 
            hud_render_form_new_unit(&h->fnu, mkb, rend, t, info, sprites, 
                time); 
            hud_render_sel(&h->sc, mkb, &h->fnu.uinfo, 
                rend, sprites, t, info, h->fnu.sel, h->fnu.ind, time); 
            break;
        case 3: 
            hud_render_overlay_battle(&h->ob, mkb, rend, t, info, gst, 
                time); 
            break;
    }
}