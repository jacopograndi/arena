#ifndef UNITS_H
#define UNITS_H

#define MAXUNITS 128
#define MAXSOLVESTEPS 128

#include <info.h>

typedef struct unit_ {
    float pos[2];
    int gridpos[2];
    info_unit info;
    int owner;
    float hp;
    float move_points;
    float cooldown[8];
    float charge;
    float reduced_armor[7];
    float stun;
} unit;

typedef struct army_ {
    unit us[MAXUNITS];
    unit **grid;
    int uslen;
    int sx, sy;
    char name[32];
} army;


#include <map.h>

void unit_init (infos *info, army *ar, map *m, 
    int x, int y, info_unit *iu, int owner, unit *u);
void unit_remove (army *ar, map *m, unit *u);

void army_grid_init (army *ar);
void army_init (army *ar, map *m);
void army_destory (army *ar);
void army_spawn (army *ar, map *m, unit u);
int army_move (infos *info, army *ar, map *m, stats_unit *ustats);

typedef struct { unit *u, *t; float dam; } a_dmg;
int army_fire (infos *info, army *ar, map *m, a_dmg dmgs[], 
    stats_unit *ustats);
void army_upkeep (infos *info, army *ar, map *m, stats_unit *ustats);

#endif