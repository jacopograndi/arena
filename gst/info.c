#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "info.h"
#include "units.h"

#include "../umath/vec.h"
#include "../json/jsmn.h"
#include "../json/jsonparse.h"

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

void info_unit_printf (info_unit *u) {
    printf("name: %s\n", u->name);
    printf("chassis: %d; ", u->chassis);
    printf("battery: %d; ", u->battery);
    printf("brain: %d\n", u->brain);
    printf("weapon: ");
    for(int i=0; i<8; printf("%d, ", u->weapons[i]), i++);
    printf("\narmor: ");
    for(int i=0; i<8; printf("%d, ", u->armor[i]), i++);
    printf("\naugs: ");
    for(int i=0; i<16; printf("%d, ", u->augs[i]), i++);
    printf("\nlevels: ");
    for(int i=0; i<34; printf("%d, ", u->levels[i]), i++);
    printf("\n");
}

/*
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
*/

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
                info->templateslen ++;
            }
            index ++;
            i += rt-1;
        }
    }
}

#define LOOP(x) for (int z=0; z<x; z++)

// weapon = 0
void stats_weapon_init (stats_weapon *weap) {
    LOOP(7) weap->damage[z] = 0;
    weap->cooldown = 0;
    weap->range = 0;
    weap->aoe = 0;
    weap->knockback = 0;
    weap->damage_battery = 0;
    weap->stun = 0;
    LOOP(7) weap->armor_reduce[z] = 0;
    weap->charge_per_shot = 0;
}

// frame = 0
void stats_frame_init (stats_frame *frame) {
    frame->hp = 0;
    frame->weight = 0;
    frame->weight_max = 0;
    frame->slot_weapon = 0;
    frame->slot_armor = 0;
    frame->slot_aug = 0;
    frame->speed = 0;
    frame->upkeep = 0;
    frame->capacity = 0;
    frame->recharge = 0;
    LOOP(7) frame->armor[z] = 0;
}

// stats = 0
void stats_comp_init (stats_comp *comp) {
    strcpy(comp->name, "-");
    strcpy(comp->description, "-");
    LOOP(MAXLEVEL) {
        stats_frame_init(comp->base +z);
        stats_frame_init(comp->perc +z);
        stats_weapon_init(comp->base_weapon +z);
        stats_weapon_init(comp->perc_weapon +z);
    }
}


#define PRINT_STRUCT(obj, text, attr, type) { \
    if (obj->attr != 0) {\
        sprintf(arr[i], "%s: %"#type, text, obj->attr); \
        i ++;\
    }\
}

void printf_arr (char arr[][64], int n) {
    for (int i=0; i<n; i++) printf("%s\n", arr[i]);
}

int stats_frame_sprintf (infos *info, stats_frame *frame, char arr[][64]) {
    int i = 0;
    PRINT_STRUCT(frame, "hp", hp, .2f);
    PRINT_STRUCT(frame, "weight", weight, .0f);
    PRINT_STRUCT(frame, "weight max", weight_max, .0f);
    PRINT_STRUCT(frame, "weapon slots", slot_weapon, d);
    PRINT_STRUCT(frame, "armor slots", slot_armor, d);
    PRINT_STRUCT(frame, "augment slots", slot_aug, d);
    PRINT_STRUCT(frame, "speed", speed, .2f);
    PRINT_STRUCT(frame, "upkeep", upkeep, .2f);
    PRINT_STRUCT(frame, "capacity", capacity, .0f);
    PRINT_STRUCT(frame, "recharge", recharge, .1f);
    LOOP(7) {
        char str[64]; sprintf(str, "%s armor", info->damage_types[i]);
        PRINT_STRUCT(frame, str, armor[z], f);
    }
    return i;
}

int stats_weapon_sprintf (infos *info, stats_weapon *weap, char arr[][64]) {
    int i = 0;
    LOOP(7)  {
        char str[64]; sprintf(str, "%s damage", info->damage_types[i]);
        PRINT_STRUCT(weap, str, damage[z], .1f);
    }
    PRINT_STRUCT(weap, "cooldown", cooldown, .2f);
    PRINT_STRUCT(weap, "aoe", aoe, .0f);
    PRINT_STRUCT(weap, "range", range, .0f);
    PRINT_STRUCT(weap, "knockback", knockback, .0f);
    PRINT_STRUCT(weap, "drain charge", damage_battery, .1f);
    PRINT_STRUCT(weap, "stun", stun, .0f);
    PRINT_STRUCT(weap, "charge per shot", charge_per_shot, .2f);
    LOOP(7) {
        char str[64]; sprintf(str, "%s armor reduce", info->damage_types[i]);
        PRINT_STRUCT(weap, str, armor_reduce[z], .1f);
    }
    return i;
}

void stats_comp_printf (infos *info, stats_comp *comp) {
    printf("name: %s\n", comp->name);
    printf("description: %s\n", comp->description);
    char arr[32][64];
    LOOP(MAXLEVEL) {
        printf("  level %d ->\n", z);
        { int n = stats_frame_sprintf(info, comp->base +z, arr); 
            printf_arr(arr, n);
        }
        { int n = stats_frame_sprintf(info, comp->perc +z, arr); 
            printf_arr(arr, n); 
        }
        { int n = stats_weapon_sprintf(info, comp->base_weapon +z, arr);
            printf_arr(arr, n); 
        }
        { int n = stats_weapon_sprintf(info, comp->perc_weapon +z, arr);
            printf_arr(arr, n); 
        }
    }
}

void stats_unit_printf (infos *info, stats_unit *u) {
    char arr[32][64];
    { int n = stats_frame_sprintf (info, &u->frame, arr); printf_arr(arr, n); }
    LOOP(u->weaponlen) { 
        int n = stats_weapon_sprintf(info, u->weapon +z, arr); 
        if (n>0) printf("  weapon: \n");
        printf_arr(arr, n); 
    }
}


// stats_unit = 0
void stats_unit_init (stats_unit *s) {
    stats_frame_init(&s->frame);
    LOOP(8) stats_weapon_init(&s->weapon[z]);
    s->weaponlen = 0;
}

// frame a += b
void stats_frame_sum (stats_frame *a, stats_frame *b) {
    a->hp += b->hp;
    a->weight += b->weight;
    a->weight_max += b->weight_max;
    a->slot_weapon += b->slot_weapon;
    a->slot_armor += b->slot_armor;
    a->slot_aug += b->slot_aug;
    a->speed += b->speed;
    a->upkeep += b->upkeep;
    a->capacity += b->capacity;
    a->recharge += b->recharge;
    LOOP(7) a->armor[z] += b->armor[z];
}

// weapon a += b
void stats_weapon_sum (stats_weapon *a, stats_weapon *b) {
    LOOP(7) a->damage[z] += b->damage[z];
    a->cooldown += b->cooldown;
    a->range += b->range;
    a->aoe += b->aoe;
    a->knockback += b->knockback;
    a->damage_battery += b->damage_battery;
    a->stun += b->stun;
    LOOP(7) a->armor_reduce[z] += b->armor_reduce[z];
    a->charge_per_shot += b->charge_per_shot;
}

// stats_unit a += b
void stats_unit_sum (stats_unit *a, stats_unit *b) {
    stats_frame_sum(&a->frame, &b->frame);
    LOOP(8) stats_weapon_sum(&a->weapon[z], &b->weapon[z]);
}

// base, perc += comp.base, comp.perc
void stats_unit_comp_sum (stats_unit *base, stats_unit *perc, 
    stats_comp *comp, int lvl) 
{
    stats_frame_sum (&base->frame, &comp->base[lvl]);
    stats_frame_sum (&perc->frame, &comp->perc[lvl]);
    for (int i=0; i<8; i++) {
        stats_weapon_sum (&base->weapon[i], &comp->base_weapon[lvl]);
        stats_weapon_sum (&perc->weapon[i], &comp->perc_weapon[lvl]);
    }
}

float f_perc_norm (float x) { return x*0.01+1; }

// frame a *= b
void stats_frame_mul (stats_frame *a, stats_frame *b) {
    a->hp *= b->hp;
    a->weight *= b->weight;
    a->weight_max *= b->weight_max;
    a->slot_weapon *= b->slot_weapon;
    a->slot_armor *= b->slot_armor;
    a->slot_aug *= b->slot_aug;
    a->speed *= b->speed;
    a->upkeep *= b->upkeep;
    a->capacity *= b->capacity;
    a->recharge *= b->recharge;
    LOOP(7) a->armor[z] *= b->armor[z];
}

// weapon a *= b
void stats_weapon_mul (stats_weapon *a, stats_weapon *b) {
    LOOP(7) a->damage[z] *= b->damage[z];
    a->cooldown *= b->cooldown;
    a->range *= b->range;
    a->aoe *= b->aoe;
    a->knockback *= b->knockback;
    a->damage_battery *= b->damage_battery;
    a->stun *= b->stun;
    LOOP(7) a->armor_reduce[z] *= b->armor_reduce[z];
    a->charge_per_shot *= b->charge_per_shot;
}

void stats_unit_mul (stats_unit *base, stats_unit *perc) {
    stats_frame_mul (&base->frame, &perc->frame);
    for (int i=0; i<8; i++) {
        stats_weapon_mul (&base->weapon[i], &perc->weapon[i]);
    }
}

// map
void stats_frame_map (stats_frame *a, float (*f)(float)) {
    a->hp = f(a->hp);
    a->weight = f(a->weight);
    a->weight_max = f(a->weight_max);
    a->slot_weapon = f(a->slot_weapon);
    a->slot_armor = f(a->slot_armor);
    a->slot_aug = f(a->slot_aug);
    a->speed = f(a->speed);
    a->upkeep = f(a->upkeep);
    a->capacity = f(a->capacity);
    a->recharge = f(a->recharge);
    LOOP(7) a->armor[z] = f(a->armor[z]);
}

void stats_weapon_map (stats_weapon *a, float (*f)(float)) {
    LOOP(7) a->damage[z] = f(a->damage[z]);
    a->cooldown = f(a->cooldown);
    a->range = f(a->range);
    a->aoe = f(a->aoe);
    a->knockback = f(a->knockback);
    a->damage_battery = f(a->damage_battery);
    a->stun = f(a->stun);
    LOOP(7) a->armor_reduce[z] = f(a->armor_reduce[z]);
    a->charge_per_shot = f(a->charge_per_shot);
}

void stats_unit_map (stats_unit *base, float (*f)(float)) {
    stats_frame_map (&base->frame, f);
    for (int i=0; i<8; i++) {
        stats_weapon_map (&base->weapon[i], f);
    }
}

// fold
float stats_frame_fold (stats_frame *frame, float (*f)(float, float)) {
    float v = 0;
    v = f(v, frame->hp);
    v = f(v, frame->weight);
    v = f(v, frame->weight_max);
    v = f(v, frame->slot_weapon);
    v = f(v, frame->slot_armor);
    v = f(v, frame->slot_aug);
    v = f(v, frame->speed);
    v = f(v, frame->upkeep);
    v = f(v, frame->capacity);
    v = f(v, frame->recharge);
    LOOP(7) v = f(v, frame->armor[z]);
    return v;
}

float stats_weapon_fold (stats_weapon *weapon, float (*f)(float, float)) {
    float v = 0;
    LOOP(7) v = f(v, weapon->damage[z]);
    v = f(v, weapon->cooldown);
    v = f(v, weapon->range);
    v = f(v, weapon->aoe);
    v = f(v, weapon->knockback);
    v = f(v, weapon->damage_battery);
    v = f(v, weapon->stun);
    LOOP(7) v = f(v, weapon->armor_reduce[z]);
    v = f(v, weapon->charge_per_shot);
    return v;
}

float stats_unit_fold (stats_unit *base, float (*f)(float, float)) {
    float v = 0;
    v = f(v, stats_frame_fold (&base->frame, f));
    for (int i=0; i<8; i++) {
        v = f(v, stats_weapon_fold (&base->weapon[i], f));
    }
    return v;
}

float f_clamp_positive (float x) { if (x < 0) x = 0; }

// compute all necessary components stats of u into base
void stats_unit_compute (infos *info, info_unit *u, stats_unit *base) {
    stats_unit_init(base);
    stats_unit perc; stats_unit_init(&perc);    
    {
        stats_comp *comp = &info->stats[STATS_CHASSIS][u->chassis];
        int lvl = u->levels[LEVEL_CHASSIS];
        stats_unit_comp_sum(base, &perc, comp, lvl);
    };
    
    {
        stats_comp *comp = &info->stats[STATS_BRAIN][u->brain];
        int lvl = u->levels[LEVEL_BRAIN];
        stats_unit_comp_sum(base, &perc, comp, lvl);
    };
    
    {
        stats_comp *comp = &info->stats[STATS_BATTERY][u->battery];
        int lvl = u->levels[LEVEL_BATTERY];
        stats_unit_comp_sum(base, &perc, comp, lvl);
    };
    
    for (int i=0; i<8; i++) {
        if (u->armor[i] != -1 && i<base->frame.slot_armor) {
            stats_comp *comp = &info->stats[STATS_ARMOR][u->armor[i]];
            int lvl = u->levels[LEVEL_ARMOR+i];
            stats_unit_comp_sum(base, &perc, comp, lvl);
        }
    }
    
    for (int i=0; i<16; i++) {
        if (u->augs[i] != -1 && i<base->frame.slot_aug) {
            stats_comp *comp = &info->stats[STATS_AUGS][u->augs[i]];
            int lvl = u->levels[LEVEL_AUGS+i];
            stats_unit_comp_sum(base, &perc, comp, lvl);
        }
    }
    
    int wn = 0;
    for (int i=0; i<8; i++) {
        if (u->weapons[i] != -1 && i<base->frame.slot_weapon) {
            stats_comp *comp = &info->stats[STATS_WEAPONS][u->weapons[i]];
            int lvl = u->levels[LEVEL_WEAPONS+i];
            stats_frame_sum (&base->frame, &comp->base[lvl]);
            stats_frame_sum (&perc.frame, &comp->perc[lvl]);
            stats_weapon_sum (&base->weapon[wn], &comp->base_weapon[lvl]);
            stats_weapon_sum (&perc.weapon[wn], &comp->perc_weapon[lvl]);
            wn ++;
        }
    }
    base->weaponlen = wn;
    
    stats_unit_map(&perc, f_perc_norm);
    for (int w=0; w<wn; w++) {
        stats_weapon_map(perc.weapon +w, f_clamp_positive);
    }
    
    stats_unit_mul(base, &perc);
}

float stats_compute_damage (stats_weapon *weapon, stats_frame *frame, 
    float *red) 
{
    /* TODO: red */
    float damage = 0;
    for (int i=0; i<7; i++) {
        damage += weapon->damage[i] * (1- frame->armor[i]*0.01);
    }
    return damage;
}

float f_add1 (float x) { return x+1; }

void cost_weights_init (stats_unit *w) {
    stats_unit_init(w);
    stats_unit_map(w, f_add1);
}

float f_sum (float a, float b) { return a+b; }

float stats_compute_cost (stats_unit *w, stats_unit *base) {
    stats_unit costed = *base;
    stats_unit_mul(&costed, w);
    float cost = stats_unit_fold(&costed, f_sum);
    return cost;
}


#define MATCH(frame, attr, type) {\
    if (strcmp(key, #frame"_"#attr) == 0) {\
        int rt = json_parse_subtokens(json, t, r, i+1); \
        if (type == 'f') {\
            float v[3]; json_parse_array(json, v, t+i+2, rt-1, type);\
            LOOP(MAXLEVEL) comp->frame[z].attr = v[z];\
        } if (type == 'i') {\
            int v[3]; json_parse_array(json, v, t+i+2, rt-1, type);\
            LOOP(MAXLEVEL) comp->frame[z].attr = v[z];\
        }\
        i += rt;\
    }\
} 

#define MATCH_ARRAY(frame, attr, attr2, type, num) {\
    char str[64]; sprintf(str, "%s%s", #frame"_"#attr"_", attr2);\
    if (strcmp(key, str) == 0) {\
        int rt = json_parse_subtokens(json, t, r, i+1); \
        float v[3]; json_parse_array(json, v, t+i+2, rt-1, type);\
        LOOP(MAXLEVEL) comp->frame[z].attr[num] = v[z];\
        i += rt;\
    }\
} 


void stats_comp_parse (char *json, stats_comp *comp, jsmntok_t *t, int r, 
    infos *info) 
{
    stats_comp_init(comp);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=1) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, comp->name, t+i+1); i++;
            }
            if (strcmp(key, "description") == 0) {
                substr_token(json, comp->description, t+i+1); i++;
            }
            
            MATCH(base, hp, 'f');
            MATCH(base, weight, 'f');
            MATCH(base, weight_max, 'f');
            MATCH(base, slot_weapon, 'i');
            MATCH(base, slot_armor, 'i');
            MATCH(base, slot_aug, 'i');
            MATCH(base, speed, 'f');
            MATCH(base, upkeep, 'f');
            MATCH(base, capacity, 'f');
            MATCH(base, recharge, 'f');
            for (int j=0;j<7; j++) {
                MATCH_ARRAY(base, armor, info->damage_types[j], 'f', j);
            }
            
            MATCH(perc, hp, 'f');
            MATCH(perc, weight, 'f');
            MATCH(perc, weight_max, 'f');
            MATCH(perc, slot_weapon, 'i');
            MATCH(perc, slot_armor, 'i');
            MATCH(perc, slot_aug, 'i');
            MATCH(perc, speed, 'f');
            MATCH(perc, upkeep, 'f');
            MATCH(perc, capacity, 'f');
            MATCH(perc, recharge, 'f');
            for (int j=0;j<7; j++) {
                MATCH_ARRAY(perc, armor, info->damage_types[j], 'f', j);
            }
            
            MATCH(base_weapon, cooldown, 'f');
            MATCH(base_weapon, range, 'f');
            MATCH(base_weapon, aoe, 'f');
            MATCH(base_weapon, knockback, 'f');
            MATCH(base_weapon, damage_battery, 'f');
            MATCH(base_weapon, stun, 'f');
            MATCH(base_weapon, charge_per_shot, 'f');
            for (int j=0;j<7; j++) {
                MATCH_ARRAY(base_weapon, damage, 
                    info->damage_types[j], 'f', j);
            }
            for (int j=0;j<7; j++) {
                MATCH_ARRAY(base_weapon, armor_reduce, 
                    info->damage_types[j], 'f', j);
            }
            
            MATCH(perc_weapon, cooldown, 'f');
            MATCH(perc_weapon, range, 'f');
            MATCH(perc_weapon, aoe, 'f');
            MATCH(perc_weapon, knockback, 'f');
            MATCH(perc_weapon, damage_battery, 'f');
            MATCH(perc_weapon, stun, 'f');
            MATCH(perc_weapon, charge_per_shot, 'f');
            for (int j=0;j<7; j++) {
                MATCH_ARRAY(perc_weapon, damage, 
                    info->damage_types[j], 'f', j);
            }
            for (int j=0;j<7; j++) {
                MATCH_ARRAY(perc_weapon, armor_reduce, 
                    info->damage_types[j], 'f', j);
            }
        }
    }
}

void info_stats_parse (infos *info, char *json, int stats_type) { 
    jsmn_parser p; jsmn_init(&p);
    jsmntok_t t[MAXTOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAXTOKENS);
    int index = 0;
    for (int i=1; i<r; i++) { // i=1: ignore outer []
        if (t[i].type == JSMN_OBJECT) {
            int rt = json_parse_subtokens(json, t, r, i);
            stats_comp *comp = info->stats[stats_type]
                +info->statslen[stats_type];
            stats_comp_parse(json, comp, t+i+1, rt, info);
            info->statslen[stats_type] ++;
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
    cost_weights_init(&info->cost_weights);
    
    info->templateslen = 0;
    info_read_file(json, "content/templates/default.txt", size);
    info_parse_json(info, json, "template");
    
    char files[6][32] = { 
        "content/chassis.txt",
        "content/brains.txt", 
        "content/batteries.txt", 
        "content/weapons.txt", 
        "content/armor.txt", 
        "content/augments.txt" 
    };
    for (int i=0; i<6; i++) {
        info->stats[i] = (stats_comp*)malloc(64*sizeof(stats_comp));
        if (info->stats[i] != NULL) {
            info->statslen[i] = 0;
            info_read_file(json, files[i], size);
            info_stats_parse(info, json, i);
        } else { printf("error: out of memory in allocating for stats"); }
    }
    
    /*
    info_unit *u = info->templates+0;
    stats_unit base;
    stats_unit_compute(info, u, &base);
    stats_unit_printf(info, &base);
    */
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

void info_template_rm (infos *info, int n) {
    for (int i=0; i<info->templateslen-1; i++) {
        if (i >= n) {
            info->templates[i] = info->templates[i+1];
        }
    }
    info->templateslen--;
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

