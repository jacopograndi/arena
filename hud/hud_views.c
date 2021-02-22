#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hud_views.h>

#define LABEL(x, y, s, d) {\
    float p[2]={x,y};\
    render_text_scaled(rend, s, p, t, d);\
}

#define LABEL_F3(x, y, s, q, v, d) {\
    float p[2]={x,y}; char ss[64]; \
    sprintf(ss, "%s: %."#q"f %."#q"f %."#q"f", s, v[0], v[1], v[2]);\
    render_text_scaled(rend, ss, p, t, d);\
}

#define LABEL_I3(x, y, s, v, d) {\
    float p[2]={x,y}; char ss[64]; \
    sprintf(ss, "%s: %d %d %d", s, v[0], v[1], v[2]);\
    render_text_scaled(rend, ss, p, t, 1);\
}

void render_view_stats (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, info_unit *tm) 
{
    float h = 10;
    LABEL(px+10, py+h, "STATS", 2); h += 35;
    
    {   float val = info_unit_get_cost(info, tm);
        char s[64]; sprintf(s, "COST: %.2f", val);
        LABEL(px+10, py+h, s, 1); 
    } h += 20;
    
    float calcweight = info_unit_get_weight(info, tm);
    float maxweight = info->chassis[tm->chassis].weight_max[
        tm->levels[LEVEL_CHASSIS]];
    float pw[2] = { px+10, py+h };
    char sw[64]; sprintf(sw, "WEIGHT: %.0f/%.0f", 
        calcweight, maxweight);
    render_text_scaled(rend, sw, pw, t, 1);
    h += 15;
    
    float hp = info_unit_get_health(info, tm);
    float php[2] = { px+10, py+h };
    char shp[64]; sprintf(shp, "HP: %.2f", hp);
    render_text_scaled(rend, shp, php, t, 1);
    h += 15;
    
    float speed = info_unit_get_speed(info, tm);
    float pspeed[2] = { px+10, py+h };
    char sspeed[64]; sprintf(sspeed, "SPEED: %.2f", speed);
    render_text_scaled(rend, sspeed, pspeed, t, 1);
    h += 15;
    
    float dps = info_unit_get_dps(info, tm);
    float pdps[2] = { px+10, py+h };
    char sdps[64]; sprintf(sdps, "DAMAGE PER TURN: %.2f", dps);
    render_text_scaled(rend, sdps, pdps, t, 1);
    h += 20;
    
    float part[2] = { px+10, py+h };
    render_text_scaled(rend, "ARMOR:", part, t, 1);
    h += 15;
    
    for (int i=0; i<7; i++) {
        float ar = info_unit_get_armor(info, tm, i);
        float par[2] = { px+10, py+h };
        char sar[64]; sprintf(sar, "%s: %.1f%", info->damage_types[i], ar);
        render_text_scaled(rend, sar, par, t, 1);
        h += 15;
    }
    h += 5;
}

void render_view_chassis (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int chassis, int lvl, SDL_Texture *sprites) 
{
    if (chassis != -1) {
        info_chassis *ch = info->chassis+chassis;
        int h = 10;
        
        LABEL(px+10, py+h, info->chassis[chassis].name, 1); h += 30;
        
        LABEL_F3(px+10, py+h, "MAX WEIGHT", 0, ch->weight_max, 1); h += 15;
        LABEL_F3(px+10, py+h, "HP", 0, ch->hp, 1); h += 15;
        LABEL_F3(px+10, py+h, "SPEED", 2, ch->speed, 1); h += 15;
        LABEL_F3(px+10, py+h, "UPKEEP", 2, ch->upkeep, 1); h += 15;
        
        SDL_Rect srcRect = { chassis*32, 32, 32, 32 };
        SDL_Rect dstRect = { px+300-32-10, py+10, 32, 32 };
        SDL_RenderCopy(rend, sprites, &srcRect, &dstRect);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a chassis...", pname, t, 1);
    }
}

void render_view_battery (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int batt, int lvl) 
{
    if (batt != -1) {
        info_battery *battery = info->batteries+batt;
        int h = 10;
        LABEL(px+10, py+h, info->batteries[batt].name, 1); h += 30;
        
        LABEL_F3(px+10, py+h, "MAX WEIGHT", 0, battery->weight, 1); h += 15;
        LABEL_F3(px+10, py+h, "CAPACITY", 0, battery->capacity, 1); h += 15;
        
        float prech[2] = { px+10, py+h }; char srech[64]; 
        if (info->batteries[batt].recharge[lvl] == 0) {
            strcpy(srech, "NOT RECHARGEABLE");
        } else { strcpy(srech, "RECHARGEABLE"); }
        render_text_scaled(rend, srech, prech, t, 1);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a battery...", pname, t, 1);
    }
}


void render_view_armor (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int armor, int lvl) 
{
    if (armor != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", info->armors[armor].name);
        render_text_scaled(rend, sname, pname, t, 1);        
        char sa[64]; int j=0;
        char temp[16] = "red: ";
        strcpy(sa+j, temp);
        j += strlen(temp);
        for (int i=0; i<7; i++) {
            if (i<7-1) {
                sprintf(temp, "%.0f, ", info->armors[armor].armor[i][lvl]);
            } else {
                sprintf(temp, "%.0f", info->armors[armor].armor[i][lvl]);
            }
            strcpy(sa+j, temp);
            j += strlen(temp);
        }
        sa[j] = '\0';
        float pa[2] = { px+10, py+25 };
        render_text_scaled(rend, sa, pa, t, 1);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select an armor...", pname, t, 1);
    }
}

void render_view_armor_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int armor, int lvl) 
{
    if (armor != -1) {
        info_armor *arm = info->armors+armor;
        int h = 10;
        
        LABEL(px+10, py+h, arm->name, 1); h += 30;
        LABEL_F3(px+10, py+h, "WEIGHT", 0, arm->weight, 1); h += 20;
        LABEL(px+10, py+h, "DAMAGE REDUCTION", 1); h += 15;
        
        for (int i=0; i<7; i++) {
            float sum = 0; for(int j=0;j<MAXLEVEL;j++) 
                sum+=fabs(arm->armor[i][j]);
            if (sum > 0) {
                LABEL_F3(px+10, py+h, info->damage_types[i], 
                    1, arm->armor[i], 1);
            }
            h += 15;
        }
    } else {
        render_view_weapon(rend, t, px, py, info, armor, lvl);
    }
}


void render_view_weapon (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int weapon, int lvl) 
{
    if (weapon != -1) {
        info_weapon *weap = info->weapons+weapon;
        int h = 10;
        
        char dt[32]; 
        sprintf(dt, "DAMAGE TYPE: %s", 
            info->damage_types[info->weapons[weapon].damage_type]);
        
        LABEL(px+10, py+h, weap->name, 1); h += 20;
        LABEL_F3(px+10, py+h, "WEIGHT", 0, weap->weight, 1); h += 15;
        LABEL(px+10, py+h, dt, 1); h += 15;
        LABEL_F3(px+10, py+h, "DAMAGE", 0, weap->damage, 1); h += 15;
        LABEL_F3(px+10, py+h, "RANGE", 0, weap->range, 1); h += 15;
        LABEL_F3(px+10, py+h, "COOLDOWN", 0, weap->cooldown, 1); h += 15;
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a weapon...", pname, t, 1);
    }
}

void render_view_weapon_detail (SDL_Renderer* rend, txtd *t, 
    int px, int py, infos *info, int weapon, int lvl)
{
    if (weapon != -1) {
        info_weapon *weap = info->weapons+weapon;
        int h = 10;
        
        char dt[32]; 
        sprintf(dt, "DAMAGE TYPE: %s", 
            info->damage_types[info->weapons[weapon].damage_type]);
        
        LABEL(px+10, py+h, weap->name, 1); h += 20;
        LABEL_F3(px+10, py+h, "WEIGHT", 0, weap->weight, 1); h += 15;
        LABEL(px+10, py+h, dt, 1); h += 15;
        LABEL_F3(px+10, py+h, "DAMAGE", 0, weap->damage, 1); h += 15;
        LABEL_F3(px+10, py+h, "RANGE", 0, weap->range, 1); h += 15;
        LABEL_F3(px+10, py+h, "COOLDOWN", 0, weap->cooldown, 1); h += 15;
        LABEL_F3(px+10, py+h, "UPKEEP", 0, weap->upkeep, 1); h += 15;
        
        float kb = 0; for(int i=0;i<MAXLEVEL;i++) 
            kb += fabs(weap->knockback[i]);
        if (kb > 0) {
            LABEL_I3(px+10, py+h, "KNOCKBACK", weap->knockback, 1);
            h += 15;
        }
        
        float cps = 0; for(int i=0;i<MAXLEVEL;i++) 
            cps += fabs(weap->charge_per_shot[i]);
        if (cps > 0) {
            LABEL_F3(px+10, py+h, "CHARGE PER SHOT",0,weap->charge_per_shot, 1);
            h += 15;
        }
    } else {
        render_view_weapon(rend, t, px, py, info, weapon, lvl);
    }
}


void render_view_aug (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug, int lvl) 
{
    if (aug != -1) {
        info_aug *augm = info->augs+aug;
        int h = 10;
        
        LABEL(px+10, py+h, augm->name, 1); h += 20;
        LABEL_F3(px+10, py+h, "WEIGHT", 0, augm->weight, 1); h += 15;
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select an augment...", pname, t, 1);
    }
}

void render_view_aug_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug, int lvl) 
{
    if (aug != -1) {
        info_aug *augm = info->augs+aug;
        int h = 10;
        
        LABEL(px+10, py+h, augm->name, 1); h += 20;
        LABEL_F3(px+10, py+h, "WEIGHT", 0, augm->weight, 1); h += 15;
        
        float range = 0; for(int i=0;i<MAXLEVEL;i++) 
            range += fabs(augm->add_range[i]);
        if (range > 0) {
            LABEL_F3(px+10, py+h, "RANGE", 0, augm->add_range, 1); h += 15;
        }
        
        float cooldown = 0; for(int i=0;i<MAXLEVEL;i++) 
            cooldown += fabs(augm->add_cooldown[i]);
        if (cooldown != 0) {
            LABEL_F3(px+10, py+h, "COOLDOWN", 2, augm->add_cooldown, 1); 
            h += 15;
        }
        
        float speed = 0; for(int i=0;i<MAXLEVEL;i++) 
            speed += fabs(augm->add_speed[i]);
        if (speed != 0) {
            LABEL_F3(px+10, py+h, "SPEED", 2, augm->add_speed, 1); h += 15;
        }
        
        float hp = 0; for(int i=0;i<MAXLEVEL;i++) 
            hp += fabs(augm->add_hp[i]);
        if (hp != 0) {
            LABEL_F3(px+10, py+h, "HP", 1, augm->add_hp, 1); h += 15;
        }
        
        h += 5;
        
        float sum = 0;
        for (int i=0; i<7; i++) 
            for(int j=0;j<MAXLEVEL;j++) 
                sum += fabs(augm->add_armor[i][j]);
        if (sum != 0) {
            for (int i=0; i<7; i++) {
                float suml = 0; for(int j=0;j<MAXLEVEL;j++) 
                    suml+=fabs(augm->add_armor[i][j]);
                if (suml > 0) {
                    LABEL_F3(px+10, py+h, info->damage_types[i], 
                        1, augm->add_armor[i], 1);
                    h += 15;
                }
            }
        }
        
        sum = 0;
        for (int i=0; i<7; i++) 
            for(int j=0;j<MAXLEVEL;j++) 
                sum += fabs(augm->add_damage[i][j]);
        if (sum != 0) {
            for (int i=0; i<7; i++) {
                float suml = 0; for(int j=0;j<MAXLEVEL;j++) 
                    suml+=fabs(augm->add_damage[i][j]);
                if (suml > 0) {
                    LABEL_F3(px+10, py+h, info->damage_types[i], 
                        1, augm->add_damage[i], 1);
                    h += 15;
                }
            }
        }
    } else {
        render_view_aug(rend, t, px, py, info, aug, lvl);
    }
}


void render_view_brain (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int brain, int lvl) 
{
    if (brain != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", 
            info->brains[brain].name);
        render_text_scaled(rend, sname, pname, t, 1);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a controller...", pname, t, 1);
    }
}


void render_view_template (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int temp, int lvl) 
{
    { button b = { "edit", 4, { px, py } };
    render_button(rend, t, &b); }
    float wedit = get_text_width("edit", t); 
    
    { button b = { "place", 4, { px+wedit+2*4+5, py } };
    render_button(rend, t, &b); }
    float wplace = get_text_width("place", t); 
    
    float pname[2] = { wplace+wedit+4*4+10+px, py+4 };
    char *sname = info->templates[temp].name;
    render_text_scaled(rend, sname, pname, t, 1);
}