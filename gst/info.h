#ifndef INFO_H
#define INFO_H

#define MAXTEMPLATES 128

#include <stdint.h>

typedef struct  {
    char name[32];
    // indexes of info.*
    int8_t chassis;
    int8_t brain;
    int8_t weapons[8];
    int8_t armor[8];
    int8_t augs[16];
    int8_t battery;
    int8_t levels[34];
} info_unit;

typedef struct {
    char name[32];
    int damage_type;
    float weight;
    float cooldown;
    float damage;
    float range;
    float aoe;
    int knockback;
    float damage_battery;
    int stun;
    float reduce_armor[7];
} info_weapon;

typedef struct {
    char name[32];
    int slot_weapon;
    int slot_armor;
    int slot_aug;
    float weight_max;
    float hp;
    float speed;
} info_chassis;

typedef struct {
    char name[32];
    float weight;
    float capacity;
    int recharge;
} info_battery;

typedef struct {
    char name[32];
    float weight;
    float armor[7];
} info_armor;

typedef struct {
    char name[32];
    float weight;
    float add_damage[7];
    float add_armor[7];
    float add_range;
    float add_cooldown;
    float add_speed;
    float add_hp;
} info_aug;

typedef struct {
    char name[32];
} info_brain;

typedef struct {
    char damage_types[7][32];
    info_unit templates[MAXTEMPLATES];
    int templateslen;
    info_weapon weapons[64];
    int weaponslen;
    info_chassis chassis[32];
    int chassislen;
    info_battery batteries[32];
    int batterieslen;
    info_armor armors[32];
    int armorslen;
    info_aug augs[32];
    int augslen;
    info_brain brains[32];
    int brainslen;
} infos;

void info_unit_init (info_unit *u);
float info_unit_get_weight (infos *info, info_unit *u);
float info_unit_get_dps (infos *info, info_unit *u);
float info_unit_get_health(infos *info, info_unit *u);
float info_unit_get_speed(infos *info, info_unit *u);
float info_unit_get_damage(infos *info, info_unit *u, int w);
float info_unit_get_damage_target(infos *info, info_unit *u, int w, 
    info_unit *t);
float info_unit_get_cooldown(infos *info, info_unit *u, int w);
float info_unit_get_range(infos *info, info_unit *u, int w);
float info_unit_get_armor(infos *info, info_unit *u, int d);

void info_load (infos *info);

void info_save_templates (infos *info, char *filename);
void info_template_add (infos *info, info_unit *temp);


void info_load_army(struct army_ *ar, char *filename);
void info_save_army(struct army_ *ar, char *filename);

int info_army_get_list(char l[][32]);

void info_load_playername(char n[]);
void info_save_playername(char n[]);

#endif