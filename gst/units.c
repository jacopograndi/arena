#include <stdlib.h>
#include <stdio.h>
#include <units.h>
#include <string.h>

#include <vec.h>
#include <jsonparse.h>

void army_grid_init(army *ar) {
    if (ar->grid != NULL) free(ar->grid);
    ar->grid = (unit**)malloc(sizeof(unit*)*ar->sx*ar->sy);
    memset(ar->grid, NULL, sizeof(unit*)*ar->sx*ar->sy);
    for (int i=0; i<ar->uslen; i++) {
        unit *u = ar->us+i;
        // not using ptoi, don't have map, why have map anyway?
        ar->grid[ar->sx*u->gridpos[1]+u->gridpos[0]] = u;
    }
}

void army_init (army *ar, map *m) {
    ar->uslen = 0;
    ar->sx = m->sx; ar->sy = m->sy;
    strcpy(ar->name, "-");
    army_grid_init(ar);
}

void army_destory(army *ar) {
    free(ar->grid);
}

void unit_init (infos *info, army *ar, map *m, 
    int x, int y, info_unit *iu, int owner, unit *u) 
{
    u->pos[0] = x*m->ts; u->pos[1] = y*m->ts;
    u->gridpos[0] = x; u->gridpos[1] = y;
    u->info = *iu;
    u->owner = owner;
    u->hp = info_unit_get_health(info, iu);
    u->move_points = 0;
    u->charge = info_unit_get_capacity(info, iu);
    for (int i=0; i<8; u->cooldown[i] = 1, i++);
}

void army_spawn (army *ar, map *m, unit u) {
    ar->us[ar->uslen] = u;
    ar->grid[ptoi(m, u.gridpos)] = ar->us+ar->uslen;
    ar->uslen++;
}

void unit_move (army *ar, map *m, unit *u, int *dest) {
    ar->grid[ptoi(m, u->gridpos)] = NULL;
    u->gridpos[0] = dest[0];
    u->gridpos[1] = dest[1];
    ar->grid[ptoi(m, u->gridpos)] = u;
    u->pos[0] = dest[0]*m->ts;
    u->pos[1] = dest[1]*m->ts;
}

void unit_remove (army *ar, map *m, unit *u) {
    unit *t = ar->us+ar->uslen-1;
    int ux = u->gridpos[0], uy = u->gridpos[1];
    int tx = t->gridpos[0], ty = t->gridpos[1];
    *u = *t;
    ar->uslen--;
    ar->grid[xytoi(m, tx, ty)] = u;
    ar->grid[xytoi(m, ux, uy)] = NULL;
    printf("ar->uslen: %d\n", ar->uslen);
}

void unit_dead (army *ar, map *m, unit *u) {
    ar->grid[ptoi(m, u->gridpos)] = NULL;
}

void unit_search (infos *info, army *ar, map *m, unit *u, 
    unit **t, float range)
{
    for (int f=0; f<32; t[f] = NULL, f++);
    int mult[4][2] = { {1, 1},{1,-1},{-1,-1},{-1,1} }, x, y, dx, dy, tmp;
    *t = NULL;
    for (int r=1; r<range*1.42 && *t==NULL; r++) {
    for (int k=0; k<4 && *t==NULL; k++) {
    for (int j=0; j<r && *t==NULL; j++) {
        if (k%2==1) { // invert them if k=1 || k=3
            dx = (j)*mult[k][0]; dy = (r-j)*mult[k][1]; 
        } else { 
            dx = (r-j)*mult[k][0]; dy = (j)*mult[k][1]; 
        }
        x = u->gridpos[0]+dx; y = u->gridpos[1]+dy;
        //printf("  (%d, %d) -> (%d, %d), %d, %d, %d\n", u->gridpos[0], u->gridpos[1], x, y, r, k, j);
        if (!(x>=0 && y>=0 && x<m->sx && y<m->sy)) continue; // oob
        *t = ar->grid[xytoi(m, x, y)];
        if (*t!=NULL && (*t)->owner == u->owner) *t = NULL; // owner check
        if (*t!=NULL && (*t)->hp <= 0) *t = NULL; // owner check
        if (*t!=NULL) {
            // range check
            float diff[2] = {
                u->gridpos[0] - (*t)->gridpos[0],
                u->gridpos[1] - (*t)->gridpos[1]
            }; float mag = vec2_mag(diff);
            if (mag > range) *t = NULL;
        }
    }}}
}


typedef struct { unit *u; int *dir; int done; } mcom;
int army_move_step (infos *info, army *ar, map *m) {
    int dirs[4][2] = { {1, 0},{0, 1},{-1,0},{0,-1} };
    mcom mcs[ar->uslen];
    int mclen = 0;
    float diff[2];
    int orders = 0;
    // planning
    for (int i=0; i<ar->uslen; i++) {
        unit *u = ar->us+i;
        if (u->move_points <= 0) continue;
        if (u->hp <= 0) continue;
        if (u->charge <= 0) continue;
        // search target
        unit *t[32];
        unit_search(info, ar, m, u, t, 100);
        if (t[0] != NULL) {
            // in range to shoot
            diff[0] = u->gridpos[0] - t[0]->gridpos[0];
            diff[1] = u->gridpos[1] - t[0]->gridpos[1];
            if (vec2_mag(diff) > /*info->units[u->who].range*/1) {
                // movement command issued
                float dist[4] = { 9999, 9999, 9999, 9999 };
                for (int j=0; j<4; j++) {
                    diff[0] = u->gridpos[0]+dirs[j][0] 
                        - t[0]->gridpos[0];
                    diff[1] = u->gridpos[1]+dirs[j][1] 
                        - t[0]->gridpos[1];
                    dist[j] = vec2_mag(diff);
                }
                float min = dist[0]; int minj = 0;
                for (int j=1; j<4; j++) {
                    if (dist[j] < min) { min = dist[j]; minj = j; }
                }
                mcs[mclen].u = u;
                mcs[mclen].dir = dirs[minj];
                mcs[mclen].done = 0;
                mclen++;
            }
        }
    }
    // execution
    int sum = 0, lastsum = -1, step = 0;
    for (; step<MAXSOLVESTEPS; step++) {
        if (sum == lastsum) { break; }
        lastsum = sum;
        sum = 0;
        for (int i=0; i<mclen; i++) {
            int dest[2] = {
                mcs[i].u->gridpos[0]+mcs[i].dir[0], 
                mcs[i].u->gridpos[1]+mcs[i].dir[1]
            };
            if (!mcs[i].done && ar->grid[ptoi(m, dest)] == NULL) {
                unit_move(ar, m, mcs[i].u, dest);
                mcs[i].done = 1;
                mcs[i].u->move_points -= 1;
                orders++;
            }
            sum += mcs[i].done;
        }
    }
    if (step == MAXSOLVESTEPS) { printf("army: max steps reached\n"); }
    if (orders > 0) return 0;
    else return 1;
}

int army_move (infos *info, army *ar, map *m) {
    for (int i=0; i<ar->uslen; i++) {
        ar->us[i].move_points += info_unit_get_speed(info, &ar->us[i].info);
    }
    int iter = 0, finished = army_move_step(info, ar, m);
    for (; iter<5 && !finished; iter++) {
        finished = army_move_step(info, ar, m);
    }
    return iter;
}

int army_fire (infos *info, army *ar, map *m) {
    for (int i=0; i<ar->uslen; i++) {
        unit *u = ar->us+i;
        int lw = u->info.levels[LEVEL_CHASSIS];
        for (int j=0; j<info->chassis[u->info.chassis].slot_weapon[lw]; j++) {
            u->cooldown[j] += 1;
        }
    }
    struct dmg { unit *u; float dam; } dmgs[1024*8]; int dmgslen = 0;
    unit *t[32];
    for (int i=0; i<ar->uslen; i++) {
        unit *u = ar->us+i;
        if (u->hp <= 0) continue;
        if (u->charge <= 0) continue;
        int lw = u->info.levels[LEVEL_CHASSIS];
        for (int j=0; j<info->chassis[u->info.chassis].slot_weapon[lw]; j++) {
            if (u->info.weapons[j] == -1) continue;
            if (u->cooldown[j] <= 0) continue;
            float range = info_unit_get_range(info, &u->info, j);
            unit_search(info, ar, m, u, t, range);
            if (t[0]!=NULL) {
                dmgs[dmgslen].u = t[0];
                dmgs[dmgslen].dam = info_unit_get_damage_target(
                    info, &u->info, j, &t[0]->info);
                dmgslen++;
                u->cooldown[j] -= info_unit_get_cooldown(info, &u->info, j);
                u->charge -= info_unit_get_charge_per_shot(info, &u->info, j);
                if (u->charge < 0) u->charge = 0;
            }
        }
    }
    for (int i=0; i<dmgslen; i++) {
        dmgs[i].u->hp -= dmgs[i].dam;
        if (dmgs[i].u->hp <= 0) {
            unit_dead(ar, m, dmgs[i].u);
        }
    }
    return dmgslen;
}

void army_upkeep (infos *info, army *ar, map *m) {
    // battery drain or recharge
    for (int i=0; i<ar->uslen; i++) {
        unit *u = ar->us+i;
        if (u->hp <= 0) continue;
        u->charge -= info_unit_get_upkeep(info, &u->info);
        if (u->charge < 0) u->charge = 0;
    }
}