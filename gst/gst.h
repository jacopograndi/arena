#ifndef GST_H
#define GST_H

#include <info.h>
#include <units.h>
#include <map.h>

#define MAXMAP 10

typedef struct {
    map map_editor;
    army army_bp[8];
    int playernum;
    map map_battle;
    army ar;
    int state;
    float cam[2];
    float starttime;
    float coveredtime;
    int turn;
    float turnspeed;
} gamestate;

void gst_init (gamestate *gst);
void gst_destroy (gamestate *gst);
void gst_get_maparmy(gamestate *gst, map **m, army **ar);
void gst_tobattle (gamestate *gst);
void gst_process (gamestate *gst, infos *info, float t);

#endif