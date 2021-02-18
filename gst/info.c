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

float info_unit_get_weight (infos *info, info_unit *u) {
    float sum = 0;
    if (u->battery != -1) {
        sum += info->batteries[u->battery].weight;
    }
    for(int i=0; i<8; i++) {
        if (u->weapons[i] != -1 && i<info->chassis[u->chassis].slot_weapon) {
            sum += info->weapons[u->weapons[i]].weight;
        }
        if (u->armor[i] != -1 && i<info->chassis[u->chassis].slot_armor) {
            sum += info->armors[u->armor[i]].weight;
        }
    }
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            sum += info->augs[u->augs[i]].weight;
        }
    }
    return sum;
}

float info_unit_get_dps (infos *info, info_unit *u) {
    float sum = 0;
    for(int i=0; i<8; i++) {
        if (u->weapons[i] != -1 && i<info->chassis[u->chassis].slot_weapon) {
            info_weapon *w = info->weapons+u->weapons[i];
            float damage = info_unit_get_damage(info, u, i);
            sum += damage/w->cooldown;
        }
    }
    return sum;
}

float info_unit_get_health(infos *info, info_unit *u) {
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            sum += info->augs[u->augs[i]].add_hp;
        }
    }
    sum += info->chassis[u->chassis].hp;
    return sum;
}

float info_unit_get_speed(infos *info, info_unit *u) {
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            sum += info->augs[u->augs[i]].add_speed;
        }
    }
    sum += info->chassis[u->chassis].speed;
    return sum;
}

float info_unit_get_damage (infos *info, info_unit *u, int w) {
    int damage_type = info->weapons[u->weapons[w]].damage_type;
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            sum += info->augs[u->augs[i]].add_damage[damage_type];
        }
    }
    float mult = (1 + sum/100.0f);
    if (mult < 0) mult = 0;
    float dam = info->weapons[u->weapons[w]].damage * mult;
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

float info_unit_get_cooldown(infos *info, info_unit *u, int w) {
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            sum += info->augs[u->augs[i]].add_cooldown;
        }
    }
    sum += info->weapons[u->weapons[w]].cooldown;
    return sum;
}

float info_unit_get_range(infos *info, info_unit *u, int w) {
    float sum = 0;
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            sum += info->augs[u->augs[i]].add_range;
        }
    }
    sum += info->weapons[u->weapons[w]].range;
    return sum;
}

float info_unit_get_armor(infos *info, info_unit *u, int d) {
    float sum = 0;
    for(int i=0; i<8; i++) {
        if (u->armor[i] != -1 && i<info->chassis[u->chassis].slot_armor) {
            sum += info->armors[u->armor[i]].armor[d];
        }
    }
    for(int i=0; i<16; i++) {
        if (u->augs[i] != -1 && info->chassis[u->chassis].slot_aug) {
            sum += info->augs[u->augs[i]].add_armor[d];
        }
    }
    return sum;
}


void weapon_init (info_weapon *w) {
    strcpy(w->name, "nameless");
    w->damage_type = 0;
    w->weight = 0; w->cooldown = 1;
    w->damage = 0; w->range = 0; w->aoe = 0;
    w->knockback = 0; w->stun = 0;
    for (int i=0; i<7; i++) { w->reduce_armor[i]=0; }
}

void chassis_init (info_chassis *c) {
    strcpy(c->name, "nameless");
    c->slot_weapon = 0; c->slot_armor = 0; c->slot_aug = 0;
    c->weight_max = 0; c->hp = 0;
}

void battery_init (info_battery *b) {
    strcpy(b->name, "nameless");
    b->weight = 0; b->capacity = 0; b->recharge = 0;
}

void armor_init (info_armor *a) {
    strcpy(a->name, "nameless");
    a->weight = 0;
    for (int i=0; i<7; i++) { a->armor[i]=0; }
}

void aug_init (info_aug *a) {
    strcpy(a->name, "nameless");
    a->weight = 0;
    for (int i=0; i<7; i++) { a->add_damage[i]=0; }
    for (int i=0; i<7; i++) { a->add_armor[i]=0; }
    a->add_range = 0;
    a->add_cooldown = 0;
    a->add_speed = 0;
    a->add_hp = 0;
}

void brain_init (info_brain *a) {
    strcpy(a->name, "nameless");
}


void info_unit_parse (char *json, info_unit *u, jsmntok_t *t, int r) { 
    info_unit_init(u);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=2) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, u->name, t+i+1); 
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
            }
            if (strcmp(key, "armor") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, u->armor, t+i+2, rt-1, 'c');
            }
            if (strcmp(key, "augs") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, u->augs, t+i+2, rt-1, 'c');
            }
            if (strcmp(key, "levels") == 0) {
                int rt = json_parse_subtokens(json, t, r, i+1);
                json_parse_array(json, u->levels, t+i+2, rt-1, 'c');
            }
        } 
    }
}

void info_weapon_parse (char *json, info_weapon *w, jsmntok_t *t, int r) { 
    weapon_init(w);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=2) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, w->name, t+i+1); 
            }
            if (strcmp(key, "damage_type") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->damage_type = damage_type_map(val);
            }
            if (strcmp(key, "weight") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->weight = atof(val); 
            }
            if (strcmp(key, "cooldown") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->cooldown = atof(val); 
            }
            if (strcmp(key, "damage") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->damage = atof(val); 
            }
            if (strcmp(key, "range") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->range = atof(val); 
            }
            if (strcmp(key, "aoe") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->aoe = atof(val); 
            }
            if (strcmp(key, "knockback") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->knockback = atoi(val); 
            }
            if (strcmp(key, "damage_battery") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->damage_battery = atof(val); 
            }
            if (strcmp(key, "stun") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->stun = atoi(val); 
            }
            if (strcmp(key, "reduce_pierce") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->reduce_armor[0] = atof(val); 
            }
            if (strcmp(key, "reduce_laser") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->reduce_armor[1] = atof(val); 
            }
            if (strcmp(key, "reduce_impact") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->reduce_armor[2] = atof(val); 
            }
            if (strcmp(key, "reduce_fusion") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->reduce_armor[3] = atof(val); 
            }
            if (strcmp(key, "reduce_explosive") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->reduce_armor[4] = atof(val); 
            }
            if (strcmp(key, "reduce_emp") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->reduce_armor[5] = atof(val); 
            }
            if (strcmp(key, "reduce_spread") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                w->reduce_armor[6] = atof(val); 
            }
        } 
    } 
}

void info_chassis_parse (char *json, info_chassis *c, jsmntok_t *t, int r) { 
    chassis_init(c);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=2) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, c->name, t+i+1); 
            }
            if (strcmp(key, "slot_weapon") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                c->slot_weapon = atoi(val); 
            }
            if (strcmp(key, "slot_armor") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                c->slot_armor = atoi(val); 
            }
            if (strcmp(key, "slot_aug") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                c->slot_aug = atoi(val); 
            }
            if (strcmp(key, "weight_max") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                c->weight_max = atof(val); 
            }
            if (strcmp(key, "hp") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                c->hp = atof(val); 
            }
            if (strcmp(key, "speed") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                c->speed = atof(val); 
            }
        } 
    } 
}

void info_battery_parse (char *json, info_battery *b, jsmntok_t *t, int r) { 
    battery_init(b);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=2) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, b->name, t+i+1); 
            }
            if (strcmp(key, "weight") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                b->weight = atof(val); 
            }
            if (strcmp(key, "capacity") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                b->capacity = atof(val); 
            }
            if (strcmp(key, "recharge") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                b->recharge = atoi(val); 
            }
        } 
    } 
}

void info_armor_parse (char *json, info_armor *a, jsmntok_t *t, int r) { 
    armor_init(a);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=2) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, a->name, t+i+1); 
            }
            if (strcmp(key, "weight") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->weight = atof(val); 
            }
            if (strcmp(key, "pierce") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->armor[0] = atof(val); 
            }
            if (strcmp(key, "laser") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->armor[1] = atof(val); 
            }
            if (strcmp(key, "impact") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->armor[2] = atof(val); 
            }
            if (strcmp(key, "fusion") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->armor[3] = atof(val); 
            }
            if (strcmp(key, "explosive") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->armor[4] = atof(val); 
            }
            if (strcmp(key, "emp") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->armor[5] = atof(val); 
            }
            if (strcmp(key, "spread") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->armor[6] = atof(val); 
            }
        } 
    } 
}

void info_aug_parse (char *json, info_aug *a, jsmntok_t *t, int r) { 
    aug_init(a);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=2) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, a->name, t+i+1); 
            }
            if (strcmp(key, "weight") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->weight = atof(val); 
            }
            if (strcmp(key, "damage_pierce") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_damage[0] = atof(val); 
            }
            if (strcmp(key, "damage_laser") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_damage[1] = atof(val); 
            }
            if (strcmp(key, "damage_impact") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_damage[2] = atof(val); 
            }
            if (strcmp(key, "damage_fusion") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_damage[3] = atof(val); 
            }
            if (strcmp(key, "damage_explosive") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_damage[4] = atof(val); 
            }
            if (strcmp(key, "damage_emp") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_damage[5] = atof(val); 
            }
            if (strcmp(key, "add_spread") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_damage[6] = atof(val); 
            }
            if (strcmp(key, "armor_pierce") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_armor[0] = atof(val); 
            }
            if (strcmp(key, "armor_laser") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_armor[1] = atof(val); 
            }
            if (strcmp(key, "armor_impact") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_armor[2] = atof(val); 
            }
            if (strcmp(key, "armor_fusion") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_armor[3] = atof(val); 
            }
            if (strcmp(key, "armor_explosive") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_armor[4] = atof(val); 
            }
            if (strcmp(key, "armor_emp") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_armor[5] = atof(val); 
            }
            if (strcmp(key, "armor_spread") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_armor[6] = atof(val); 
            }
            if (strcmp(key, "add_range") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_range = atof(val); 
            }
            if (strcmp(key, "add_cooldown") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_cooldown = atof(val); 
            }
            if (strcmp(key, "add_hp") == 0) {
                char val[32]; substr_token(json, val, t+i+1); 
                a->add_hp = atof(val); 
            }
        } 
    } 
}

void info_brain_parse (char *json, info_brain *a, jsmntok_t *t, int r) { 
    brain_init(a);
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i+=2) { 
        if (t[i].type == JSMN_STRING) { 
            char key[32]; substr_token(json, key, t+i); 
            if (strcmp(key, "name") == 0) {
                substr_token(json, a->name, t+i+1); 
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
                info_unit_parse(json, info->templates+index, t+i+1, rt);
                info->templateslen = index+1;
            }
            if (strcmp(obj, "weapon") == 0) {
                info_weapon_parse(json, info->weapons+index, t+i+1, rt);
                info->weaponslen = index+1;
            }
            if (strcmp(obj, "chassis") == 0) {
                info_chassis_parse(json, info->chassis+index, t+i+1, rt);
                info->chassislen = index+1;
            }
            if (strcmp(obj, "battery") == 0) {
                info_battery_parse(json, info->batteries+index, t+i+1, rt);
                info->batterieslen = index+1;
            }
            if (strcmp(obj, "armor") == 0) {
                info_armor_parse(json, info->armors+index, t+i+1, rt);
                info->armorslen = index+1;
            }
            if (strcmp(obj, "aug") == 0) {
                info_aug_parse(json, info->augs+index, t+i+1, rt);
                info->augslen = index+1;
            }
            if (strcmp(obj, "brain") == 0) {
                info_brain_parse(json, info->brains+index, t+i+1, rt);
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
        char w3[128]; json_dump_array(w3, u->levels, 34, 'c');
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
    char buf[1024*64];
    char pathname[64]; sprintf(pathname, "army/%s.txt", filename);
    int len = info_read_file(buf, pathname, 1024*64);
    memcpy(ar, buf, len-1);
    ar->grid = NULL;
    army_grid_init(ar);
}

void info_save_army(struct army_ *ar, char *filename) {
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
