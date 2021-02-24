#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <units.h>
#include <info.h>

#include <vec.h>
#include <jsmn.h>
#include <jsonparse.h>

int type_damage_map (char strdmg[][32]) {
    strcpy(strdmg[0], "pierce");
    strcpy(strdmg[1], "laser");
    strcpy(strdmg[2], "impact");
    strcpy(strdmg[3], "fusion");
    strcpy(strdmg[4], "explosive");
    strcpy(strdmg[5], "emp");
    strcpy(strdmg[6], "spread");
}

int damage_type_map (char *strdmg) {
    if (strcmp(strdmg, "pierce") == 0) return 0;
    if (strcmp(strdmg, "laser") == 0) return 1;
    if (strcmp(strdmg, "impact") == 0) return 2;
    if (strcmp(strdmg, "fusion") == 0) return 3;
    if (strcmp(strdmg, "explosive") == 0) return 4;
    if (strcmp(strdmg, "emp") == 0) return 5;
    if (strcmp(strdmg, "spread") == 0) return 6;
    printf("info: damage type unknown\n"); return -1; 
}


void info_unit_init (info_unit *u) {
    strcpy(u->name, "nameless");
    u->chassis = -1;
    u->battery = -1;
    u->brain = -1;
    for(int i=0; i<8; u->weapons[i] = -1, u->armor[i] = -1, i++);
    for(int i=0; i<16; u->augs[i] = -1, i++);
    for(int i=0; i<34; u->levels[i] = 0, i++);
}

float info_unit_get_capacity(infos *info, info_unit *u) {
    float sum = 0;
    int lc = u->levels[LEVEL_CHASSIS];
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].add_capacity[lvl];
        }
    }
    int lvl = u->levels[LEVEL_BATTERY];
    sum += info->batteries[u->battery].capacity[lvl];
    return sum;
}

float info_unit_get_upkeep(infos *info, info_unit *u) {
    float sum = 0;
    int lc = u->levels[LEVEL_CHASSIS];
    int lbr = u->levels[LEVEL_BRAIN];
    for(int i=0; i<8; i++) {
        if (u->weapons[i] != -1 && info->chassis[u->chassis].slot_weapon[lc]) {
            int lvl = u->levels[LEVEL_WEAPONS+i];
            sum += info->weapons[u->weapons[i]].upkeep[lvl];
        }
        if (u->armor[i] != -1 && info->chassis[u->chassis].slot_armor[lc]) {
            int lvl = u->levels[LEVEL_ARMOR+i];
            sum += info->armors[u->armor[i]].upkeep[lvl];
        }
    }
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].upkeep[lvl];
        }
    }
    sum += info->chassis[u->chassis].upkeep[lc];
    sum += info->brains[u->brain].upkeep[lbr];
    return sum;
}

float info_unit_get_weight (infos *info, info_unit *u) {
    float sum = 0;
    int lc = u->levels[LEVEL_CHASSIS];
    if (u->battery != -1) {
        int lb = u->levels[LEVEL_BATTERY];
        sum += info->batteries[u->battery].weight[lb];
    }
    for(int i=0; i<8; i++) {
        if (u->weapons[i] != -1 && i<info->chassis[u->chassis].slot_weapon[lc]) {
            int lvl = u->levels[LEVEL_WEAPONS+i];
            sum += info->weapons[u->weapons[i]].weight[lvl];
        }
        if (u->armor[i] != -1 && i<info->chassis[u->chassis].slot_armor[lc]) {
            int lvl = u->levels[LEVEL_ARMOR+i];
            sum += info->armors[u->armor[i]].weight[lvl];
        }
    }
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].weight[lvl];
        }
    }
    return sum;
}

float info_unit_get_dps (infos *info, info_unit *u) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<8; i++) {
        if (u->weapons[i] != -1 && i<info->chassis[u->chassis].slot_weapon[lc]) {
            info_weapon *w = info->weapons+u->weapons[i];
            int lw = u->levels[LEVEL_WEAPONS+i];
            float damage = info_unit_get_damage(info, u, i);
            sum += damage/w->cooldown[lw];
        }
    }
    return sum;
}

float info_unit_get_health(infos *info, info_unit *u) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].add_hp[lvl];
        }
    }
    float mult = (1 + sum/100.0f);
    if (mult < 0) mult = 0;
    float ret = info->chassis[u->chassis].hp[lc] * mult;
    return ret;
}

float info_unit_get_speed(infos *info, info_unit *u) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].add_speed[lvl];
        }
    }
    sum += info->chassis[u->chassis].speed[lc];
    return sum;
}

float info_unit_get_damage (infos *info, info_unit *u, int w) {
    int lc = u->levels[LEVEL_CHASSIS];
    int damage_type = info->weapons[u->weapons[w]].damage_type;
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].add_damage[damage_type][lvl];
        }
    }
    float mult = (1 + sum/100.0f);
    if (mult < 0) mult = 0;
    int lw = u->levels[LEVEL_WEAPONS+w];
    float dam = info->weapons[u->weapons[w]].damage[lw] * mult;
    return dam;
}

float info_unit_get_damage_target (infos *info, info_unit *u, int w, 
    info_unit *t) 
{
    // u shoots at t with w, which is lol
    int damage_type = info->weapons[u->weapons[w]].damage_type;
    float reduction_perc = info_unit_get_armor(info, t, damage_type);
    float damage = info_unit_get_damage(info, u, w);
    float mult = (1 - reduction_perc / 100.0f);
    return damage * mult;
}

float info_unit_get_aoe (infos *info, info_unit *u, int w) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            //sum += info->augs[u->augs[i]].add_aoe[lvl];
        }
    }
    int lw = u->levels[LEVEL_WEAPONS+w];
    float dam = info->weapons[u->weapons[w]].aoe[lw] + sum;
    return dam;
}

float info_unit_get_knockback (infos *info, info_unit *u, int w) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            //sum += info->augs[u->augs[i]].add_knockback[lvl];
        }
    }
    int lw = u->levels[LEVEL_WEAPONS+w];
    float dam = info->weapons[u->weapons[w]].knockback[lw] + sum;
    return dam;
}

float info_unit_get_stun (infos *info, info_unit *u, int w) {
    // TODO
    return 0;
}

float info_unit_get_cooldown(infos *info, info_unit *u, int w) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].add_cooldown[lvl];
        }
    }
    int lw = u->levels[LEVEL_WEAPONS+w];
    sum += info->weapons[u->weapons[w]].cooldown[lw];
    return sum;
}

float info_unit_get_range(infos *info, info_unit *u, int w) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].add_range[lvl];
        }
    }
    int lw = u->levels[LEVEL_WEAPONS+w];
    sum += info->weapons[u->weapons[w]].range[lw];
    return sum;
}

float info_unit_get_maxrange(infos *info, info_unit *u) {
    int lc = u->levels[LEVEL_CHASSIS];
    float max = 0;
    for(int i=0; i<info->chassis[u->chassis].slot_weapon[lc]; i++) {
        if (u->weapons[i] != -1) {
            float range = info_unit_get_range(info, u, i);
            if (range > max) { max = range; }
        }
    }
    return max;
}

float info_unit_get_charge_per_shot (infos *info, info_unit *u, int w) {
    float sum = 0;
    int lw = u->levels[LEVEL_WEAPONS+w];
    sum += info->weapons[u->weapons[w]].charge_per_shot[lw];
    return sum;
}

float info_unit_get_armor(infos *info, info_unit *u, int d) {
    int lc = u->levels[LEVEL_CHASSIS];
    float sum = 0;
    for(int i=0; i<8; i++) {
        if (u->armor[i] != -1 && i<info->chassis[u->chassis].slot_armor[lc]) {
            int lvl = u->levels[LEVEL_ARMOR+i];
            sum += info->armors[u->armor[i]].armor[d][lvl];
        }
    }
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug[lc]) {
            int lvl = u->levels[LEVEL_AUGS+i];
            sum += info->augs[u->augs[i]].add_armor[d][lvl];
        }
    }
    return sum;
}

float info_unit_get_cost (infos *info, info_unit *u) {
    // see design/notes.txt:implement cost function
    float sum = 0;
    int lc = u->levels[LEVEL_CHASSIS];
    info_chassis *chassis = info->chassis+u->chassis;
    float sumchassis = 0;
    sumchassis += powf(2, chassis->slot_weapon[lc])*20;
    sumchassis += powf(2, chassis->slot_armor[lc])*10;
    sumchassis += powf(2, chassis->slot_aug[lc])*5;
    sumchassis += chassis->weight_max[lc]/5;
    sumchassis += chassis->hp[lc]/20;
    sumchassis += chassis->speed[lc]*32;
    sum += sumchassis;
    if (u->battery != -1) {
        int lb = u->levels[LEVEL_BATTERY];
        info_battery *battery = info->batteries+u->battery;
        float sumbattery = 0;
        sumbattery += battery->capacity[lb];
        sum += sumbattery;
    }
    for(int i=0; i<8; i++) {
        int lw = u->levels[LEVEL_WEAPONS+i];
        int la = u->levels[LEVEL_ARMOR+i];
        if (u->weapons[i] != -1 && i<chassis->slot_weapon[lc]) {
            info_weapon *weapon = info->weapons+u->weapons[i];
            float sumweap = 0;
            float dam = info_unit_get_damage(info, u, i);
            float cool = info_unit_get_cooldown(info, u, i);
            float damtot = dam / cool;
            float aoe = info_unit_get_aoe(info, u, i);
            float knockback = info_unit_get_knockback(info, u, i);
            float stun = info_unit_get_stun(info, u, i);
            float mult = 1+aoe*5 + 10;
            damtot = damtot*mult;
            damtot += knockback * 20 + stun * 50;
            sumweap = damtot;
            sum += sumweap;
        }
    }
    for(int t=0; t<7; t++) {
        float armortot = info_unit_get_armor(info, u, t) * 3;
        sum += armortot;
    }
    
    return sum;
}



void weapon_init (info_weapon *w) {
    strcpy(w->name, "nameless");
    for (int l=0; l<MAXLEVEL; l++) { 
        w->damage_type = 0;
        w->weight[l] = 0; 
        w->cooldown[l] = 1;
        w->damage[l] = 0;
        w->range[l] = 0; 
        w->aoe[l] = 0;
        w->knockback[l] = 0;
        w->stun[l] = 0;
        for (int i=0; i<7; i++) { w->reduce_armor[i][l]=0; }
        w->upkeep[l] = 0;
        w->charge_per_shot[l] = 0;
    }
}

void chassis_init (info_chassis *c) {
    strcpy(c->name, "nameless");
    for (int l=0; l<MAXLEVEL; l++) { 
        c->slot_weapon[l] = 0; 
        c->slot_armor[l] = 0;
        c->slot_aug[l] = 0;
        c->weight_max[l] = 0; 
        c->hp[l] = 0;
        c->upkeep[l] = 0;
    }
}

void battery_init (info_battery *b) {
    strcpy(b->name, "nameless");
    for (int l=0; l<MAXLEVEL; l++) { 
        b->weight[l] = 0; 
        b->capacity[l] = 0;
        b->recharge[l] = 0;
    }
}

void armor_init (info_armor *a) {
    strcpy(a->name, "nameless");
    for (int l=0; l<MAXLEVEL; l++) { 
        a->weight[l] = 0;
        for (int i=0; i<7; i++) { a->armor[i][l]=0; }
        a->upkeep[l] = 0;
    }
}

void aug_init (info_aug *a) {
    strcpy(a->name, "nameless");
        for (int l=0; l<MAXLEVEL; l++) { 
        a->weight[l] = 0;
        for (int i=0; i<7; i++) { a->add_damage[i][l]=0; }
        for (int i=0; i<7; i++) { a->add_armor[i][l]=0; }
        a->add_range[l] = 0;
        a->add_cooldown [l]= 0;
        a->add_speed[l] = 0;
        a->add_hp[l] = 0;
        a->add_capacity[l] = 0;
        a->upkeep[l] = 0;
    }
}

void brain_init (info_brain *a) {
    strcpy(a->name, "nameless");
    for (int l=0; l<MAXLEVEL; l++) { 
        a->upkeep[l] = 0;
    }
}


void info_unit_parse (char *json, info_unit *u, jsmntok_t *t, int r, 
    infos *info) 
{ 
    info_unit_init(u);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, u->name, t+i+1); 
                i++;
            }
            if (strcmp(key, "chassis") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                u->chassis = atoi(val); 
            }
            if (strcmp(key, "brain") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                u->brain = atoi(val); 
            }
            if (strcmp(key, "battery") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                u->battery = atoi(val); 
            }
            if (strcmp(key, "weapons") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, u->weapons, t+i+2, rt-1, 'c');
                i++;
            }
            if (strcmp(key, "armor") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, u->armor, t+i+2, rt-1, 'c');
                i++;
            }
            if (strcmp(key, "augs") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, u->augs, t+i+2, rt-1, 'c');
                i++;
            }
            if (strcmp(key, "levels") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, u->levels, t+i+2, rt-1, 'c');
                i++;
            }
        } 
    }
}

void info_weapon_parse (char *json, info_weapon *w, jsmntok_t *t, int r, 
    infos *info) 
{ 
    weapon_init(w);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, w->name, t+i+1); 
                i++;
            }
            if (strcmp(key, "damage_type") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->damage_type = damage_type_map(val);
                i++;
            }
            if (strcmp(key, "weight") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->weight, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "cooldown") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->cooldown, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "damage") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->damage, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "range") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->range, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "aoe") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->aoe, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "knockback") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->knockback, t+i+2, rt-1, 'i');
                i += rt;
            }
            if (strcmp(key, "damage_battery") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->damage_battery, t+i+2, rt-1, 'f'); 
                i += rt;
            }
            if (strcmp(key, "stun") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->stun, t+i+2, rt-1, 'i');
                i += rt;
            }
            for (int h=0; h<7; h++) {
                char strkey[32]; sprintf(strkey, "reduce_%s", 
                    info->damage_types[h]);
                if (strcmp(key, strkey) == 0) {
                    int rt = json_parse_subtokens(json, t, r, i+1);
                    json_parse_array(json, w->reduce_armor[h], t+i+2, rt-1, 'f');
                    i += rt;
                    break;
                }
            }
            if (strcmp(key, "upkeep") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, w->upkeep, t+i+2, rt-1, 'f');
                i += rt;
            }
        } 
    } 
}

void info_chassis_parse (char *json, info_chassis *c, jsmntok_t *t, int r, 
    infos *info) 
{ 
    chassis_init(c);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, c->name, t+i+1); 
                i++;
            }
            if (strcmp(key, "slot_weapon") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, c->slot_weapon, t+i+2, rt-1, 'i');
                printf("%d %d %d\n", c->slot_weapon[0], c->slot_weapon[1], c->slot_weapon[2]);
                i += rt;
            }
            if (strcmp(key, "slot_armor") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, c->slot_armor, t+i+2, rt-1, 'i');
                i += rt;
            }
            if (strcmp(key, "slot_aug") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, c->slot_aug, t+i+2, rt-1, 'i');
                i += rt;
            }
            if (strcmp(key, "weight_max") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, c->weight_max, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "hp") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, c->hp, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "speed") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, c->speed, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "upkeep") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, c->upkeep, t+i+2, rt-1, 'f');
                i += rt;
            }
        } 
    } 
}

void info_battery_parse (char *json, info_battery *b, jsmntok_t *t, int r, 
    infos *info) 
{ 
    battery_init(b);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, b->name, t+i+1); 
                i++;
            }
            if (strcmp(key, "weight") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, b->weight, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "capacity") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, b->capacity, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "recharge") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, b->recharge, t+i+2, rt-1, 'i');
                i += rt;
            }
        } 
    } 
}

void info_armor_parse (char *json, info_armor *a, jsmntok_t *t, int r, 
    infos *info) 
{ 
    armor_init(a);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, a->name, t+i+1); 
                i++;
            }
            if (strcmp(key, "weight") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->weight, t+i+2, rt-1, 'f');
                i += rt;
            }
            for (int h=0; h<7; h++) {
                if (strcmp(key, info->damage_types[h]) == 0) {
                    int rt = json_parse_subtokens(json, t, r, i+1);
                    json_parse_array(json, a->armor[h], t+i+2, rt-1, 'f');
                    i += rt;
                }
            }
            if (strcmp(key, "upkeep") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->upkeep, t+i+2, rt-1, 'f');
                i += rt;
            }
        } 
    } 
}

void info_aug_parse (char *json, info_aug *a, jsmntok_t *t, int r, 
    infos *info) 
{ 
    aug_init(a);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, a->name, t+i+1); 
                i++;
            }
            if (strcmp(key, "weight") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->weight, t+i+2, rt-1, 'f');
                i += rt;
            }
            for (int h=0; h<7; h++) {
                char strkey[32]; sprintf(strkey, "damage_%s", 
                    info->damage_types[h]);
                if (strcmp(key, strkey) == 0) {
                    int rt = json_parse_subtokens(json, t, r, i+1);
                    json_parse_array(json, a->add_damage[h], t+i+2, rt-1, 'f');
                    i += rt;
                    break;
                }
            }
            for (int h=0; h<7; h++) {
                char strkey[32]; sprintf(strkey, "armor_%s", 
                    info->damage_types[h]);
                if (strcmp(key, strkey) == 0) {
                    int rt = json_parse_subtokens(json, t, r, i+1);
                    json_parse_array(json, a->add_armor[h], t+i+2, rt-1, 'f');
                    i += rt;
                    break;
                }
            }
            if (strcmp(key, "add_range") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->add_range, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "add_cooldown") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->add_cooldown, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "add_hp") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->add_hp, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "add_capacity") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->add_capacity, t+i+2, rt-1, 'f');
                i += rt;
            }
            if (strcmp(key, "upkeep") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->upkeep, t+i+2, rt-1, 'f');
                i += rt;
            }
        } 
    } 
}

void info_brain_parse (char *json, info_brain *a, jsmntok_t *t, int r, 
    infos *info) 
{ 
    brain_init(a);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, a->name, t+i+1); 
                i++;
            }
            if (strcmp(key, "upkeep") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, a->upkeep, t+i+2, rt-1, 'f');
                i += rt;
            }
        } 
    } 
}

void info_parse_json (infos *info, char *json, char *obj) {
    jsmn_parser p; jsmn_init(&p);
    jsmntok_t t[MAXTOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAXTOKENS);
    int index = 0;
    for (int i=1; i<r; i++) { // i=1: ignore outer []
        if (t[i].type == JSMN_OBJECT) {
            int rt = json_parse_subtokens(json, t, r, i);
            if (strcmp(obj, "template") == 0) {
                info_unit_parse(json, 
                    info->templates+index, t+i+1, rt, info);
                info->templateslen = index+1;
            }
            if (strcmp(obj, "weapon") == 0) {
                info_weapon_parse(json, 
                    info->weapons+index, t+i+1, rt, info);
                info->weaponslen = index+1;
            }
            if (strcmp(obj, "chassis") == 0) {
                info_chassis_parse(json, 
                    info->chassis+index, t+i+1, rt, info);
                info->chassislen = index+1;
            }
            if (strcmp(obj, "battery") == 0) {
                info_battery_parse(json, 
                    info->batteries+index, t+i+1, rt, info);
                info->batterieslen = index+1;
            }
            if (strcmp(obj, "armor") == 0) {
                info_armor_parse(json, 
                    info->armors+index, t+i+1, rt, info);
                info->armorslen = index+1;
            }
            if (strcmp(obj, "aug") == 0) {
                info_aug_parse(json, 
                    info->augs+index, t+i+1, rt, info);
                info->augslen = index+1;
            }
            if (strcmp(obj, "brain") == 0) {
                info_brain_parse(json, 
                    info->brains+index, t+i+1, rt, info);
                info->brainslen = index+1;
            }
            index ++;
            i += rt-1;
        }
    }
}

int info_read_file (char *dst, char *name, int size) {
    memset(dst, 0, size*sizeof(char));
    FILE *f = fopen(name, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(dst, 1, fsize, f);
    fclose(f);
    dst[fsize] = '\0';
    return fsize;
}

void info_load (infos *info) {
    int size = 1024*64;
    char json[size];
    type_damage_map(info->damage_types);
    info->augslen = 0;
    info_read_file(json, "content/weapons.txt", size);
    info_parse_json(info, json, "weapon");
    info_read_file(json, "content/chassis.txt", size);
    info_parse_json(info, json, "chassis");
    info_read_file(json, "content/batteries.txt", size);
    info_parse_json(info, json, "battery");
    info_read_file(json, "content/armor.txt", size);
    info_parse_json(info, json, "armor");
    info_read_file(json, "content/augments.txt", size);
    info_parse_json(info, json, "aug");
    info_read_file(json, "content/brains.txt", size);
    info_parse_json(info, json, "brain");
    
    info_read_file(json, "content/templates/default.txt", size);
    info_parse_json(info, json, "template");
    /*
    for (int i=0; i<info->unitslen; i++) {
        printf("name: %s\n", info->units[i].name);
        printf(" hp: %f\n", info->units[i].hp);
        printf(" damage: %f\n", info->units[i].damage);
        printf(" range: %f\n", info->units[i].range);
    }*/
    /*
    for (int i=0; i<info->weaponslen; i++) {
        printf("name: %s\n", info->weapons[i].name);
        printf(" damage_type: %d\n", info->weapons[i].damage_type);
        printf(" weight: %f\n", info->weapons[i].weight);
        printf(" cooldown: %f\n", info->weapons[i].cooldown);
        printf(" damage: %f\n", info->weapons[i].damage);
        printf(" range: %f\n", info->weapons[i].range);
        printf(" aoe: %f\n", info->weapons[i].aoe);
        printf(" knockback: %d\n", info->weapons[i].knockback);
        printf(" damage_battery: %f\n", info->weapons[i].damage_battery);
        printf(" stun: %d\n", info->weapons[i].stun);
        float sum = 0; for (int j=0; j<7; j++) { 
            sum += info->weapons[i].reduce_armor[j];
        }
        if (sum > 0) {
            printf(" red pierce: %f\n", info->weapons[i].reduce_armor[0]);
            printf(" red laser: %f\n", info->weapons[i].reduce_armor[1]);
            printf(" red impact: %f\n", info->weapons[i].reduce_armor[2]);
            printf(" red fusion: %f\n", info->weapons[i].reduce_armor[3]);
            printf(" red explosive: %f\n", info->weapons[i].reduce_armor[4]);
            printf(" red emp: %f\n", info->weapons[i].reduce_armor[5]);
            printf(" red spread: %f\n", info->weapons[i].reduce_armor[6]);
        }
    }*/
    /*
    for (int i=0; i<info->chassislen; i++) {
        printf("name: %s\n", info->chassis[i].name);
        printf(" slot_weapon: %d\n", info->chassis[i].slot_weapon);
        printf(" slot_armor: %d\n", info->chassis[i].slot_armor);
        printf(" slot_aug: %d\n", info->chassis[i].slot_aug);
        printf(" weight_max: %f\n", info->chassis[i].weight_max);
        printf(" hp: %f\n", info->chassis[i].hp);
    }*/
    /*
    for (int i=0; i<info->batterieslen; i++) {
        printf("name: %s\n", info->batteries[i].name);
        printf(" weight: %f\n", info->batteries[i].weight);
        printf(" capacity: %f\n", info->batteries[i].capacity);
        printf(" recharge: %d\n", info->batteries[i].recharge);
    }*/
    /*
    for (int i=0; i<info->armorslen; i++) {
        printf("name: %s\n", info->armors[i].name);
        printf(" weight: %f\n", info->armors[i].weight);
        printf(" pierce: %f\n", info->armors[i].armor[0]);
        printf(" laser: %f\n", info->armors[i].armor[1]);
        printf(" impact: %f\n", info->armors[i].armor[2]);
        printf(" fusion: %f\n", info->armors[i].armor[3]);
        printf(" explosive: %f\n", info->armors[i].armor[4]);
        printf(" emp: %f\n", info->armors[i].armor[5]);
        printf(" spread: %f\n", info->armors[i].armor[6]);
    }*/
}

void info_dump_json_templates (infos *info, char *str) {
    int len = 0;
    sprintf(str+len, "[ \n"); len = strlen(str);
    for (int i=0; i<info->templateslen; i++) {
        info_unit *u = info->templates+i;
        sprintf(str+len, "    { \n        \"name\": \"%s\",\n", u->name); 
        len = strlen(str);
        sprintf(str+len, "        \"chassis\": \"%d\",\n", u->chassis); 
        len = strlen(str);
        sprintf(str+len, "        \"brain\": \"%d\",\n", u->brain); 
        len = strlen(str);
        sprintf(str+len, "        \"battery\": \"%d\",\n", u->battery);
        len = strlen(str);
        char w0[64]; json_dump_array(w0, u->weapons, 8, 'c');
        sprintf(str+len, "        \"weapons\": %s,\n", w0); 
        len = strlen(str);
        char w1[64]; json_dump_array(w1, u->armor, 8, 'c');
        sprintf(str+len, "        \"armor\": %s,\n", w1); 
        len = strlen(str);
        char w2[64]; json_dump_array(w2, u->augs, 8, 'c');
        sprintf(str+len, "        \"augs\": %s,\n", w2); 
        len = strlen(str);
        char w3[128]; json_dump_array(w3, u->levels, 35, 'c');
        sprintf(str+len, "        \"levels\": %s\n", w3); 
        len = strlen(str);
        char comma = ','; if(i==info->templateslen-1) comma = ' ';
        sprintf(str+len, "    }%c\n", comma); 
        len = strlen(str);
    }
    sprintf(str+len, "]"); len = strlen(str);
}

void info_save_templates (infos *info, char *filename) {
    char src[1024*64];
    info_dump_json_templates(info, src);
    char pathname[64]; sprintf(pathname, "content/templates/%s.txt", filename);
    FILE *f = fopen(pathname, "wb");
    fwrite(src, 1, strlen(src)*sizeof(char), f);
    fclose(f);
}

void info_template_add (infos *info, info_unit *temp) {
    if (info->templateslen < MAXTEMPLATES) {
        info->templates[info->templateslen] = *temp;
        info->templateslen++;
    }
}

void info_load_army(struct army_ *ar, char *filename) {
    strcpy(ar->name, filename);
    char buf[1024*64];
    char pathname[64]; sprintf(pathname, "army/%s.txt", filename);
    int len = info_read_file(buf, pathname, 1024*64);
    memcpy(ar, buf, len-1);
    ar->grid = NULL;
    army_grid_init(ar);
}

void info_save_army(struct army_ *ar, char *filename) {
    strcpy(ar->name, filename);
    char pathname[64]; sprintf(pathname, "army/%s.txt", filename);
    FILE *f = fopen(pathname, "wb");
    fwrite(ar, 1, sizeof(army), f);
    fclose(f);
}


void info_load_playername(char n[]) {
    char buf[1024];
    int len = info_read_file(buf, "player.txt", 1024);
    memcpy(n, buf, sizeof(char)*len);
}

void info_save_playername(char n[]) {
    FILE *f = fopen("player.txt", "wb");
    fwrite(n, 1, strlen(n)*sizeof(char), f); fclose(f);
}


#include <plibsys.h>
int info_army_get_list (char l[][32]) {
    int len = 0;
    PDirEntry *entry;
    PDir *dir = p_dir_new ("army/", NULL);
    while ((entry = p_dir_get_next_entry (dir, NULL)) != NULL) {
        if (entry->type == P_DIR_ENTRY_TYPE_FILE) {
            strcpy(l[len], entry->name);
            l[len][strlen(l[len])-4] = '\0'; // del .txt
            len++;
        }
    }
    return len;
}

void info_army_rename (char oldn[], char newn[]) {
    char pathnameold[64]; sprintf(pathnameold, "army/%s.txt", oldn);
    char pathnamenew[64]; sprintf(pathnamenew, "army/%s.txt", newn);
    rename(pathnameold, pathnamenew);
}

void info_army_remove (char name[]) {
    char pathname[64]; sprintf(pathname, "army/%s.txt", name);
    remove(pathname);
}
