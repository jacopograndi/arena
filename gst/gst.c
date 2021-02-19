#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>

#include <gst.h>

void gst_init (gamestate *gst) {
    map_init(&gst->map_editor, MAXMAP, MAXMAP, 32);
    army_init(gst->army_bp, &gst->map_editor);
    gst->playernum = 1;
    gst->state = 0;
    gst->starttime = FLT_MAX;
    gst->turn = 0;
    gst->coveredtime = 0;
    gst->turnspeed = 0.35f;
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

void gst_tobattle (gamestate *gst) {
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
    gst->starttime = FLT_MAX;
    gst->turn = 0;
    gst->coveredtime = 0;
}

void gst_process (gamestate *gst, infos *info, float t) {
    if (gst->starttime > t) gst->starttime = t; 
    float t_elapsed = t-gst->starttime;
    if (t_elapsed > gst->coveredtime) {
        gst->coveredtime += gst->turnspeed;
        gst->turn ++;
        map *m; army *ar;
        gst_get_maparmy(gst, &m, &ar);
        army_move(info, ar, m);
        army_fire(info, ar, m);
        army_upkeep(info, ar, m);
    }
}