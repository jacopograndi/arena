#ifndef INFO_H
#define INFO_H

#define MAXTEMPLATES 128
#define MAXLEVEL 3

#include <stdint.h>

#define LEVEL_CHASSIS 0
#define LEVEL_BRAIN 1
#define LEVEL_BATTERY 2
#define LEVEL_WEAPONS 3
#define LEVEL_ARMOR 11
#define LEVEL_AUGS 19

typedef struct  {
    char name[32];
    // indexes of info.*
    int8_t chassis;
    int8_t brain;
    int8_t battery;
    int8_t weapons[8];
    int8_t armor[8];
    int8_t augs[16];
    int8_t levels[35];
} info_unit;

typedef struct {
    char name[32];
    int damage_type;
    float weight[MAXLEVEL];
    float cooldown[MAXLEVEL];
    float damage[MAXLEVEL];
    float range[MAXLEVEL];
    float aoe[MAXLEVEL];
    int knockback[MAXLEVEL];
    float damage_battery[MAXLEVEL];
    int stun[MAXLEVEL];
    float reduce_armor[7][MAXLEVEL];
    float upkeep[MAXLEVEL];
    float charge_per_shot[MAXLEVEL];
} info_weapon;

typedef struct {
    char name[32];
    int slot_weapon[MAXLEVEL];
    int slot_armor[MAXLEVEL];
    int slot_aug[MAXLEVEL];
    float weight_max[MAXLEVEL];
    float hp[MAXLEVEL];
    float speed[MAXLEVEL];
    float upkeep[MAXLEVEL];
} info_chassis;

typedef struct {
    char name[32];
    float weight[MAXLEVEL];
    float capacity[MAXLEVEL];
    int recharge[MAXLEVEL];
} info_battery;

typedef struct {
    char name[32];
    float weight[MAXLEVEL];
    float armor[7][MAXLEVEL];
    float upkeep[MAXLEVEL];
} info_armor;

typedef struct {
    char name[32];
    float weight[MAXLEVEL];
    float add_damage[7][MAXLEVEL];
    float add_armor[7][MAXLEVEL];
    float add_range[MAXLEVEL];
    float add_cooldown[MAXLEVEL];
    float add_speed[MAXLEVEL];
    float add_hp[MAXLEVEL];
    float add_capacity[MAXLEVEL];
    float upkeep[MAXLEVEL];
} info_aug;

typedef struct {
    char name[32];
    float upkeep[MAXLEVEL];
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
float info_unit_get_capacity (infos *info, info_unit *u);
float info_unit_get_upkeep (infos *info, info_unit *u);
float info_unit_get_weight (infos *info, info_unit *u);
float info_unit_get_dps (infos *info, info_unit *u);
float info_unit_get_health(infos *info, info_unit *u);
float info_unit_get_speed(infos *info, info_unit *u);
float info_unit_get_damage(infos *info, info_unit *u, int w);
float info_unit_get_charge_per_shot(infos *info, info_unit *u, int w);
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