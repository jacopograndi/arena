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

#define STATS_CHASSIS 0
#define STATS_BRAIN 1
#define STATS_BATTERY 2
#define STATS_WEAPONS 3
#define STATS_ARMOR 4
#define STATS_AUGS 5

// loaded stats
typedef struct {
    float damage[7];
    float cooldown;
    float range;
    float aoe;
    float knockback;
    float damage_battery;
    float stun;
    float armor_reduce[7];
    float charge_per_shot;
} stats_weapon;

typedef struct {
    float hp;
    float weight;
    float weight_max;
    int slot_weapon;
    int slot_armor;
    int slot_aug;
    float speed;
    float upkeep;
    float capacity;
    float recharge;
    float armor[7];
} stats_frame;

typedef struct {
    char name[32];
    char description[256];
    stats_frame base[MAXLEVEL];
    stats_frame perc[MAXLEVEL];
    stats_weapon base_weapon[MAXLEVEL];
    stats_weapon perc_weapon[MAXLEVEL];
} stats_comp;


// computed stats ready for army use
typedef struct {
    stats_frame frame;
    stats_weapon weapon[8];
    int weaponlen;
} stats_unit;


typedef struct {
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
    char damage_types[7][32];
    info_unit templates[MAXTEMPLATES];
    int templateslen;
    
    stats_comp *stats[6];
    int statslen[6];
} infos;

void info_unit_init (info_unit *u);


int stats_frame_sprintf (infos *info, stats_frame *frame, char arr[][64]);
int stats_weapon_sprintf (infos *info, stats_weapon *weap, char arr[][64]);
void stats_unit_compute (infos *info, info_unit *u, stats_unit *base);

float stats_compute_damage (stats_weapon *weapon, stats_frame *frame, 
    float *red); 

void info_load (infos *info);

void info_save_templates (infos *info, char *filename);
void info_template_add (infos *info, info_unit *temp);


#include "units.h"
void info_load_army(struct army_ *ar, char *filename);
void info_save_army(struct army_ *ar, char *filename);

int info_army_get_list(char l[][32]);
void info_army_rename (char oldn[], char newn[]);
void info_army_remove (char name[]);

void info_load_playername(char n[]);
void info_save_playername(char n[]);

#endif