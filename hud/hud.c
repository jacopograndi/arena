#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <hud.h>
#include <hud_views.h>

#include <intersect.h>

// TODO: make sound module mabye?
#define SOUND_MOUSE_OVER 0
#define SOUND_MOUSE_CLICK_0 1
#define SOUND_MOUSE_WHEEL 2
#define SOUND_SUCCESS 3


// INIT
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
    fnu->rect_back = { x, y, w, h };
    
    fnu->rect_chassis = { x+20, y+20+145+10+30, 300, 300 };
    fnu->rect_brain = { x+20, y+20+30, 145, 145 };
    fnu->rect_battery = { x+150+25, y+20+30, 145, 145 };
    for (int i=0; i<8; i++) {
        fnu->rect_weapons[i] = { x+20+300+10+30, y+20+120*i +30, 200, 110 };
    }
    for (int i=0; i<8; i++) {
        fnu->rect_armor[i] = { x+20+500+20+30*2, y+20+60*i +30, 150, 50 };
    }
    for (int i=0; i<8; i++) {
        fnu->rect_augs[i] = { x+20+500+20+160+30*2, y+20+60*i +30, 150, 50 };
    }
    
    fnu->rect_stats = { x+w-20-200, y+20, 200, h-70 };
    
    int width = get_text_width("save", t);
    button bdone = { "save", 4, { x+w-4*2-width-20, y+h-4*2-11-20 } };
    fnu->done = bdone;
}

void init_overlay_game (graphic_settings *gs, overlay_game *og, txtd *t) {
    int w = 250, h = gs->resy-20;
    og->rect_templates = { 10, gs->resy-10-h, w, h };
    
    float wnu = get_text_width("new template", t);
    button b = { "new template", 4, { w-wnu-4*2, 20 } };
    og->new_template = b;
    
    float wst = get_text_width("save templates", t);
    button b2 = { "save templates", 4, { w-wst-4*2, gs->resy-20-4*2-10 } };
    og->save_templates = b2;
    
    int warmy = 250, harmy = gs->resy-20;
    og->rect_army = { gs->resx-warmy-10, gs->resy-harmy-10, warmy, harmy };
    button b3 = { "save army", 4, { gs->resx-warmy, gs->resy-20-4*2-10 } };
    og->save_army = b3;
    
    int wbattle = 400, hbattle = 100;
    og->rect_battle = { gs->resx/2-wbattle/2, 10, wbattle, hbattle };
    
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
    strcpy(h->og.army_listcur, "army");
    strcpy(h->og.playername, "");
    h->nameedit = NULL;
    h->og.battle_state = 0;
    h->og.edit_playername = 0;
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
    float curweight = info_unit_get_weight(info, u);
    int chassis_lvl = u->levels[LEVEL_CHASSIS];
    float maxweight = info->chassis[u->chassis].weight_max[chassis_lvl];
    if (curweight > maxweight) { return 4; }
    return 0;
}

void hud_map_sel (info_unit *u, infos *info, int sel, int ind, 
    int8_t **n, int *bound, int size[]) 
{
    if (sel == 0) { 
        *n = &u->chassis; 
        *bound = info->chassislen; 
        size[0] = 300; size[1] = 300;
    }
    if (sel == 1) { 
        *n = &u->battery; 
        *bound = info->batterieslen; 
        size[0] = 145; size[1] = 145;
    }
    if (sel == 2) { 
        *n = &u->armor[ind]; 
        *bound = info->armorslen; 
        size[0] = 150; size[1] = 250;
    }
    if (sel == 3) { 
        *n = &u->weapons[ind]; 
        *bound = info->weaponslen; 
        size[0] = 200; size[1] = 110;
    }
    if (sel == 4) { 
        *n = &u->augs[ind]; 
        *bound = info->augslen; 
        size[0] = 150; size[1] = 200;
    }
    if (sel == 5) { 
        *n = &u->brain; 
        *bound = info->brainslen; 
        size[0] = 145; size[1] = 145;
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


void hud_edit_close(hud *h) {
    h->nameedit = NULL;
    h->og.edit_playername = 0;
}

void hud_open_fnu (hud *h, info_unit *u, int i) {
    h->state = 1;
    h->fnu.uinfo = *u;
    h->og.temp_modify = i;
    hud_edit_close(h);
}

void hud_close_fnu (hud *h, infos *info) {
    if (hud_fnu_check(&h->fnu.uinfo, info) > 0) return;
    if (h->og.temp_modify == -1) {
        info_template_add(info, &h->fnu.uinfo);
    } else {
        info->templates[h->og.temp_modify] = h->fnu.uinfo;
    }
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
            h->fnu.sel = 0; h->state = 2;
            hud_open_sel(gs, h, t, info, &h->fnu.rect_chassis);
        }
        float possb[2] = { h->fnu.rect_battery.x, h->fnu.rect_battery.y };
        float sizesb[2] = { h->fnu.rect_battery.w, h->fnu.rect_battery.h };
        if (pt_rect(mousepos, possb, sizesb)) {
            h->fnu.sel = 1; h->state = 2;
            hud_open_sel(gs, h, t, info, &h->fnu.rect_battery);
        }
        float possbr[2] = { h->fnu.rect_brain.x, h->fnu.rect_brain.y };
        float sizesbr[2] = { h->fnu.rect_brain.w, h->fnu.rect_brain.h };
        if (pt_rect(mousepos, possbr, sizesbr)) {
            h->fnu.sel = 5; h->state = 2;
            hud_open_sel(gs, h, t, info, &h->fnu.rect_brain);
        }
        if (h->fnu.uinfo.chassis != -1) {
            int lc = h->fnu.uinfo.levels[LEVEL_CHASSIS];
            for (int i=0; 
                i<info->chassis[h->fnu.uinfo.chassis].slot_armor[lc]; i++) 
            {
                float possa[2] = { 
                    h->fnu.rect_armor[i].x, h->fnu.rect_armor[i].y };
                float sizesa[2] = { 
                    h->fnu.rect_armor[i].w, h->fnu.rect_armor[i].h };
                if (pt_rect(mousepos, possa, sizesa)) {
                    h->fnu.sel = 2; h->fnu.ind = i; h->state = 2;
                    hud_open_sel(gs, h, t, info, &h->fnu.rect_armor[i]);
                }
            }
            for (int i=0; 
                i<info->chassis[h->fnu.uinfo.chassis].slot_weapon[lc]; i++) 
            {
                float possa[2] = { 
                    h->fnu.rect_weapons[i].x, h->fnu.rect_weapons[i].y };
                float sizesa[2] = { 
                    h->fnu.rect_weapons[i].w, h->fnu.rect_weapons[i].h };
                if (pt_rect(mousepos, possa, sizesa)) {
                    h->fnu.sel = 3; h->fnu.ind = i; h->state = 2;
                    hud_open_sel(gs, h, t, info, &h->fnu.rect_weapons[i]);
                }
            }
            for (int i=0; 
                i<info->chassis[h->fnu.uinfo.chassis].slot_aug[lc]; i++) 
            {
                float possa[2] = { 
                    h->fnu.rect_augs[i].x, h->fnu.rect_augs[i].y };
                float sizesa[2] = { 
                    h->fnu.rect_augs[i].w, h->fnu.rect_augs[i].h };
                if (pt_rect(mousepos, possa, sizesa)) {
                    h->fnu.sel = 4; h->fnu.ind = i; h->state = 2;
                    hud_open_sel(gs, h, t, info, &h->fnu.rect_augs[i]);
                }
            }
        }
    }
}

void hud_process_overlay_game (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, gamestate *gst, 
    net_client *netc, net_server *nets, Mix_Chunk *sounds[]) 
{
    if (h->og.battle_state == 1) {
        if (nets->sock_client == NULL) {
            net_server_accept(nets);
        } else {
            char buffer[1024*64];
            int len = net_server_recv(nets, buffer);
            if (len != -1) {
                h->og.battle_state = 3;
                
                int armysize = sizeof(unit)*gst->army_bp[0].uslen;
                char data[armysize];
                memcpy(data, gst->army_bp[0].us, armysize);
                net_server_send(nets, data, armysize);
                printf("send (%d)\n", armysize);
            
                memcpy(gst->army_bp[1].us, buffer, len);
                gst->army_bp[1].uslen = len/sizeof(unit);
                gst->playernum = 2;
                gst_tobattle(gst);
                gst->cam[0] = -gs->resx/2+gst->map_battle.sx*gst->map_battle.ts/2;
                gst->cam[1] = -gs->resy/2+gst->map_battle.sy*gst->map_battle.ts/2;
                h->state = 4;
            }
        }
    }
    if (h->og.battle_state == 2) {
        char buffer[1024*64];
        int len = net_client_recv(netc, buffer);
        if (len != -1) {
            h->og.battle_state = 3;
            
            memcpy(gst->army_bp[1].us, buffer, len);
            gst->army_bp[1].uslen = len/sizeof(unit);
            gst->playernum = 2;
            gst_tobattle(gst);
            gst->cam[0] = -gs->resx/2+gst->map_battle.sx*gst->map_battle.ts/2;
            gst->cam[1] = -gs->resy/2+gst->map_battle.sy*gst->map_battle.ts/2;
            h->state = 4;
        }
    }
            
    float mousepos[2] = { mkb->mx, mkb->my };
    if (mkb->mheld[0] == 1) {
        if (mouse_in_button(mousepos, t, &h->og.new_template)) {
            info_unit u; info_unit_init(&u);
            hud_open_fnu(h, &u, -1);
            h->og.temp_place = -1;
        }
        
        if (mouse_in_button(mousepos, t, &h->og.save_templates)) {
            info_save_templates(info, "default");
            Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
        }
        
        if (mouse_in_button(mousepos, t, &h->og.start_battle)) {
            /*
            army_move(info, ar, m);
            army_fire(info, ar, m);*/
            gst_tobattle(gst);
            gst->cam[0] = -gs->resx/2+gst->map_battle.sx*gst->map_battle.ts/2;
            gst->cam[1] = -gs->resy/2+gst->map_battle.sy*gst->map_battle.ts/2;
            h->og.battle_state = 3;
            h->state = 4;
            Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
        }
        
        if (mouse_in_button(mousepos, t, &h->og.join_game)) {
            printf("open client\n");
            net_client_open(netc);
            int conn = net_client_connect(netc, "127.0.0.1", SERVER_PORT);
            if (conn == 0) {
                int armysize = sizeof(unit)*gst->army_bp[0].uslen;
                char data[armysize];
                memcpy(data, gst->army_bp[0].us, armysize);
                net_client_send(netc, data, armysize);
                printf("send (%d)\n", armysize);
            }
            h->og.battle_state = 2;
        }
        
        if (mouse_in_button(mousepos, t, &h->og.host_game)) {
            printf("open server\n");
            net_server_open(nets, SERVER_PORT);
            h->og.battle_state = 1;
        }
        
        if (mouse_in_button(mousepos, t, &h->og.save_army)) {
            info_save_army(gst->army_bp+0, h->og.army_listcur);
            h->og.army_listlen = info_army_get_list(h->og.army_list);
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
                hud_open_fnu(h, info->templates+i, i);
                h->og.temp_place = -1;
            }
            float wplace = get_text_width("place", t);
            float posp[2] = { x+wedit+4*2+5, y };
            float sizep[2] = { wplace+4*2, 11+4*2 };
            if (pt_rect(mousepos, posp, sizep)) {
                hud_og_place(h, i);
            }
        }
        
        for (int i=0; i<h->og.army_listlen; i++) {
            float x = h->og.rect_army.x+5;
            float y = h->og.rect_army.y+5 + i*20 + 30;
            float wload = get_text_width("load", t);
            float posp[2] = { x, y };
            float sizep[2] = { wload+4*2, 11+4*2 };
            if (pt_rect(mousepos, posp, sizep)) {
                strcpy(h->og.army_listcur, h->og.army_list[i]);
                info_load_army(gst->army_bp+0, h->og.army_listcur);
                Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
            }
        }
        
        if (h->nameedit == NULL) {
            float pn[2] = { h->og.rect_battle.x+5, h->og.rect_battle.y+5 };
            char sn[64]; sprintf(sn, "PLAYER NAME: %s", h->og.playername);
            float sizen[2] = { get_text_width(sn, t), 10 };
            if (pt_rect(mousepos, pn, sizen)) {
                h->nameedit = h->og.playername;
                printf("editing this %s\n", h->nameedit);
                h->og.edit_playername = 1;
            }
        } else {
            Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
            hud_edit_close(h);
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

void hud_edit_name(hud *h, MKb *mkb, Mix_Chunk *sounds[]) {
    for (int i=0; i<mkb->kbnum; i++) {
        if (mkb->kb[i] >= SDL_SCANCODE_A
         && mkb->kb[i] <= SDL_SCANCODE_Z) {
            if (strlen(h->nameedit) < 31) {
                char c = mkb->kb[i]-SDL_SCANCODE_A+'a';
                if (SDL_GetModState() & KMOD_SHIFT) {
                    c = mkb->kb[i]-SDL_SCANCODE_A+'A';
                }
                sprintf(h->nameedit, "%s%c", h->nameedit, c);
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
    if (mkb_search(mkb, SDL_SCANCODE_ESCAPE)) {
        Mix_PlayChannel( -1, sounds[SOUND_SUCCESS], 0 );
        hud_edit_close(h); return;
    }
}

void hud_process (graphic_settings *gs, hud *h, MKb *mkb, 
    infos *info, army *ar, map *m, txtd *t, gamestate *gst, 
    net_client *netc, net_server *nets, Mix_Chunk *sounds[]) 
{
    if (h->nameedit != NULL) { hud_edit_name(h, mkb, sounds); }
    switch (h->state) {
        case 0: 
            hud_process_overlay_game(gs, h, mkb, info, ar, m, t, gst, 
                netc, nets, sounds); 
            break;
        case 1: hud_process_form_new_unit(gs, h, mkb, info, ar, m, t, 
            sounds); break;
        case 2: hud_process_sel(gs, h, mkb, info, ar, m, t, sounds); break;
    }
}



// RENDER
void hud_render_sel (hud_sel *sc, MKb *mkb, info_unit *u,
    SDL_Renderer* rend, SDL_Texture *sprites, txtd *t, infos *info, 
    int sel, int ind) 
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
        if (sel==0) render_view_chassis(rend, t, x, y, info, i, 0, sprites);
        if (sel==1) render_view_battery(rend, t, x, y, info, i, 0);
        if (sel==2) render_view_armor_detail(rend, t, x, y, info, i, 0);
        if (sel==3) render_view_weapon_detail(rend, t, x, y, info, i, 0);
        if (sel==4) render_view_aug_detail(rend, t, x, y, info, i, 0);
        if (sel==5) render_view_brain(rend, t, x, y, info, i, 0);
    }
    SDL_RenderSetClipRect(rend, NULL);
}

void hud_render_form_new_unit (form_new_unit *fnu, MKb *mkb,
    SDL_Renderer* rend, txtd *t, infos *info, SDL_Texture *sprites)
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
        for (int i=0; i<info->chassis[fnu->uinfo.chassis].slot_weapon[lc]; i++) {
            SDL_SetRenderDrawColor(rend, 200, 100, 255, 255);
            SDL_RenderFillRect(rend, fnu->rect_weapons+i);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderDrawRect(rend, fnu->rect_weapons+i);
            render_view_weapon(rend, t, 
                fnu->rect_weapons[i].x, 
                fnu->rect_weapons[i].y, info, fnu->uinfo.weapons[i], 
                fnu->uinfo.levels[LEVEL_WEAPONS+i]); 
        }
        for (int i=0; i<info->chassis[fnu->uinfo.chassis].slot_armor[lc]; i++) {
            SDL_SetRenderDrawColor(rend, 200, 200, 255, 255);
            SDL_RenderFillRect(rend, fnu->rect_armor+i);
            SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
            SDL_RenderDrawRect(rend, fnu->rect_armor+i);
            render_view_armor(rend, t, 
                fnu->rect_armor[i].x, 
                fnu->rect_armor[i].y, info, fnu->uinfo.armor[i], 
                fnu->uinfo.levels[LEVEL_ARMOR+i]); 
        }
        for (int i=0; i<info->chassis[fnu->uinfo.chassis].slot_aug[lc]; i++) {
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
    SDL_Renderer* rend, txtd *t, infos *info, SDL_Texture *sprites)
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
        float x = og->rect_templates.x+5;
        float y = og->rect_templates.y+5 + i*20 + 30;
        render_view_template(rend, t, x, y, info, i, 0);
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
    
    render_button(rend, t, &og->save_army);
    
    button b = { "load", 4, { 0, 0 } };
    for (int i=0; i<og->army_listlen; i++) {
        float x = og->rect_army.x+5;
        float y = og->rect_army.y+5 + i*20 + 30;
        float bw = get_text_width("load", t)+4*2;
        b.pos[0] = x; b.pos[1] = y;
        render_button(rend, t, &b);
        float pa[2] = { x+bw+5, y+4 };
        render_text_scaled(rend, og->army_list[i], pa, t, 1);
        if (strcmp(og->army_listcur, og->army_list[i]) == 0) {
            float w = get_text_width(og->army_list[i], t);
            float pe[2] = { x+bw+5+w+5, y+4 };
            render_text_scaled(rend, "<- editing", pe, t, 1);
        }
    }
    
    SDL_SetRenderDrawColor(rend, 250, 60, 60, 255);
    SDL_RenderFillRect(rend, &og->rect_battle);
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderDrawRect(rend, &og->rect_battle);
    
    float pn[2] = { og->rect_battle.x+5, og->rect_battle.y+5 };
    char sn[64]; 
    if (og->edit_playername == 1) {
        sprintf(sn, "PLAYER NAME: %s_", og->playername);
        printf("looool\n");
    } else {
        sprintf(sn, "PLAYER NAME: %s", og->playername);
    }
    render_text_scaled(rend, sn, pn, t, 1);
    
    render_button(rend, t, &og->host_game);
    render_button(rend, t, &og->join_game);
    render_button(rend, t, &og->start_battle);
    
    if (og->temp_place != -1) {
        SDL_Rect srcRect = { 
            info->templates[og->temp_place].chassis*32, 32, 32, 32 };
        SDL_Rect dstRect = { mkb->mx - 16, mkb->my - 16, 32, 32 };
        SDL_RenderCopy(rend, sprites, &srcRect, &dstRect);
    }
}

void hud_render (hud *h, SDL_Renderer* rend, txtd *t, MKb *mkb, infos *info, 
    SDL_Texture *sprites) 
{
    switch (h->state) {
        case 0: 
            hud_render_overlay_game(&h->og, mkb, rend, t, info, sprites); 
            break;
        case 1: 
            hud_render_overlay_game(&h->og, mkb, rend, t, info, sprites); 
            hud_render_form_new_unit(&h->fnu, mkb, rend, t, info, sprites); 
            break;
        case 2:
            hud_render_overlay_game(&h->og, mkb, rend, t, info, sprites); 
            hud_render_form_new_unit(&h->fnu, mkb, rend, t, info, sprites); 
            hud_render_sel(&h->sc, mkb, &h->fnu.uinfo, 
                rend, sprites, t, info, h->fnu.sel, h->fnu.ind); 
            break;
    }
}