#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "generate.h"

void generate_init () {
    srand(time(NULL));
}

float calc_unit_cost (infos *info, info_unit *u) {
    if (u->chassis == -1) return 0;
    stats_unit base; 
    stats_unit_compute(info, u, &base);
    return stats_compute_cost(&info->cost_weights, &base);
}

int accept_cond (infos *info, info_unit *u, float cost_max) {
    stats_unit base; 
    stats_unit_compute(info, u, &base);
    float cost = stats_compute_cost(&info->cost_weights, &base);
    if (base.frame.weight > base.frame.weight_max) return 0;
    if (cost > cost_max) return 0;
    return 1;
}

int gen_add_comp (infos *info, stats_unit *base, info_unit *u) {
    int sel = rand() % 3;
    if (sel == 0) {
        for (int i=0; i<base->frame.slot_weapon; i++) {
            if (u->weapons[i] == -1) {
                u->weapons[i] = rand() % info->statslen[STATS_WEAPONS];
                break;
            }
        }
    }
    else if (sel == 1) {
        for (int i=0; i<base->frame.slot_armor; i++) {
            if (u->armor[i] == -1) {
                u->armor[i] = rand() % info->statslen[STATS_ARMOR];
                break;
            }
        }
    }
    else if (sel == 2) {
        for (int i=0; i<base->frame.slot_aug; i++) {
            if (u->augs[i] == -1) {
                u->augs[i] = rand() % info->statslen[STATS_AUGS];
                break;
            }
        }
    }
    return sel;
}

// generates randomly a valid unit
void gen_unit_attempt (infos *info, info_unit *u, float cost_max) {
    info_unit_init(u);
    stats_unit base; 
    u->chassis = rand() % info->statslen[STATS_CHASSIS];
    u->battery = rand() % info->statslen[STATS_BATTERY];
    u->brain = rand() % info->statslen[STATS_BRAIN];
    stats_unit_compute(info, u, &base);
    for (int i=0; i<8*8*16; i++) {
        int sel = gen_add_comp(info, &base, u);
        if (sel == 0) {
            if (!accept_cond(info, u, cost_max)) {
                u->weapons[i] = -1; return; 
            }
        }
        if (sel == 1) {
            if (!accept_cond(info, u, cost_max)) { 
                u->armor[i] = -1; return; 
            }
        }
        if (sel == 2) {
            if (!accept_cond(info, u, cost_max)) {
                u->augs[i] = -1; return; 
            }
        }
    }
}

// selects the max cost generated unit
int generate_unit (infos *info, info_unit *u, float cost_max) {
    info_unit cand;
    info_unit candmax; info_unit_init(&candmax);
    float cost;
    for (int i=0; i<GENERATE_UNIT_MAX_ATTEMPTS; i++) {
        info_unit_init(&cand);
        gen_unit_attempt(info, &cand, cost_max);
        if (accept_cond(info, &cand, cost_max)) { 
            if (calc_unit_cost(info, &cand) 
            > calc_unit_cost(info, &candmax)) {
                candmax = cand;
                printf("found: %f\n", calc_unit_cost(info, &cand));
            }
        }
    }
    if (candmax.chassis != -1) {
        printf("i generated!\n");
        *u = candmax;
        return 0;
    }
    else return 1;
}