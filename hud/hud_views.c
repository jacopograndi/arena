#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hud_views.h>

void render_view_stats (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, info_unit *tm) 
{
    float h = 10;
    float pname[2] = { px+10, py+h };
    char sname[64]; sprintf(sname, "STATS");
    render_text_scaled(rend, sname, pname, t, 2);
    h += 35;
    
    float calcweight = info_unit_get_weight(info, tm);
    float maxweight = info->chassis[tm->chassis].weight_max;
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
    infos *info, int chassis, SDL_Texture *sprites) 
{
    if (chassis != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", 
            info->chassis[chassis].name);
        render_text_scaled(rend, sname, pname, t, 2);
        float pweight[2] = { px+10, py+40 };
        char sweight[64]; sprintf(sweight, "MAX WEIGHT: %0.0f", 
            info->chassis[chassis].weight_max);
        render_text_scaled(rend, sweight, pweight, t, 1);
        float php[2] = { px+10, py+55 };
        char shp[64]; sprintf(shp, "HP: %0.1f", 
            info->chassis[chassis].hp);
        render_text_scaled(rend, shp, php, t, 1);
        float pspeed[2] = { px+10, py+70 };
        char sspeed[64]; sprintf(sspeed, "SPEED: %0.2f tiles/turn", 
            info->chassis[chassis].speed);
        render_text_scaled(rend, sspeed, pspeed, t, 1);
        
        SDL_Rect srcRect = { chassis*32, 32, 32, 32 };
        SDL_Rect dstRect = { px+300-32-10, py+10, 32, 32 };
        SDL_RenderCopy(rend, sprites, &srcRect, &dstRect);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a chassis...", pname, t, 1);
    }
}

void render_view_battery (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int batt) 
{
    if (batt != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", 
            info->batteries[batt].name);
        render_text_scaled(rend, sname, pname, t, 1);
        float pweight[2] = { px+10, py+40 };
        char sweight[64]; sprintf(sweight, "WEIGHT: %0.0f", 
            info->batteries[batt].weight);
        render_text_scaled(rend, sweight, pweight, t, 1);
        float pcapacity[2] = { px+10, py+55 };
        char scapacity[64]; sprintf(scapacity, "CAPACITY: %0.1f", 
            info->batteries[batt].capacity);
        render_text_scaled(rend, scapacity, pcapacity, t, 1);
        float prech[2] = { px+10, py+70 };
        char srech[64]; 
        if (info->batteries[batt].recharge == 0) {
            strcpy(srech, "NOT RECHARGEABLE");
        } else {
            strcpy(srech, "RECHARGEABLE");
        }
        render_text_scaled(rend, srech, prech, t, 1);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a battery...", pname, t, 1);
    }
}


void render_view_armor (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int armor) 
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
                sprintf(temp, "%.0f, ", info->armors[armor].armor[i]);
            } else {
                sprintf(temp, "%.0f", info->armors[armor].armor[i]);
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
    infos *info, int armor) 
{
    if (armor != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", info->armors[armor].name);
        render_text_scaled(rend, sname, pname, t, 1);
        float pweight[2] = { px+10, py+40 };
        char sweight[64]; sprintf(sweight, "WEIGHT: %0.0f", 
            info->armors[armor].weight);
        render_text_scaled(rend, sweight, pweight, t, 1);
        
        float pred[2] = { px+10, py+60 };
        render_text_scaled(rend, "DAMAGE REDUCTION", pred, t, 1);
        
        int h=0;
        for (int i=0; i<7; i++) {
            if (info->armors[armor].armor[i] < 0.001) continue;
            float pa[2] = { px+10, py+75+h*15 };
            char sa[64]; sprintf(sa, "%s: %.1f%", 
                info->damage_types[i], info->armors[armor].armor[i]);
            render_text_scaled(rend, sa, pa, t, 1);
            h++;
        }
    } else {
        render_view_weapon(rend, t, px, py, info, armor);
    }
}


void render_view_weapon (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int weapon) 
{
    if (weapon != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", 
            info->weapons[weapon].name);
        render_text_scaled(rend, sname, pname, t, 1);
        float pweight[2] = { px+10, py+30 };
        char sweight[64]; sprintf(sweight, "WEIGHT: %.0f", 
            info->weapons[weapon].weight);
        render_text_scaled(rend, sweight, pweight, t, 1);
        float ptype[2] = { px+10, py+45 };
        char stype[64]; sprintf(stype, "DAMAGE TYPE: %s", 
            info->damage_types[info->weapons[weapon].damage_type]);
        render_text_scaled(rend, stype, ptype, t, 1);
        float pdamage[2] = { px+10, py+60 };
        char sdamage[64]; sprintf(sdamage, "DAMAGE: %.0f", 
            info->weapons[weapon].damage);
        render_text_scaled(rend, sdamage, pdamage, t, 1);
        float prange[2] = { px+10, py+75 };
        char srange[64]; sprintf(srange, "RANGE: %.0f", 
            info->weapons[weapon].range);
        render_text_scaled(rend, srange, prange, t, 1);
        float pcool[2] = { px+10, py+90 };
        char scool[64]; sprintf(scool, "COOLDOWN: %.0f", 
            info->weapons[weapon].cooldown);
        render_text_scaled(rend, scool, pcool, t, 1);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select a weapon...", pname, t, 1);
    }
}

void render_view_weapon_detail (SDL_Renderer* rend, txtd *t, 
    int px, int py, infos *info, int weapon)
{
    if (weapon != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", 
            info->weapons[weapon].name);
        render_text_scaled(rend, sname, pname, t, 1);
        float pweight[2] = { px+10, py+30 };
        char sweight[64]; sprintf(sweight, "WEIGHT: %.0f", 
            info->weapons[weapon].weight);
        render_text_scaled(rend, sweight, pweight, t, 1);
        float ptype[2] = { px+10, py+45 };
        char stype[64]; sprintf(stype, "DAMAGE TYPE: %s", 
            info->damage_types[info->weapons[weapon].damage_type]);
        render_text_scaled(rend, stype, ptype, t, 1);
        float pdamage[2] = { px+10, py+60 };
        char sdamage[64]; sprintf(sdamage, "DAMAGE: %.0f", 
            info->weapons[weapon].damage);
        render_text_scaled(rend, sdamage, pdamage, t, 1);
        float prange[2] = { px+10, py+75 };
        char srange[64]; sprintf(srange, "RANGE: %.0f", 
            info->weapons[weapon].range);
        render_text_scaled(rend, srange, prange, t, 1);
        float pcool[2] = { px+10, py+90 };
        char scool[64]; sprintf(scool, "COOLDOWN: %.0f", 
            info->weapons[weapon].cooldown);
        render_text_scaled(rend, scool, pcool, t, 1);
    } else {
        render_view_weapon(rend, t, px, py, info, weapon);
    }
}


void render_view_aug (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug) 
{
    if (aug != -1) {
        float pname[2] = { px+10, py+10 };
        char sname[64]; sprintf(sname, "%s", 
            info->augs[aug].name);
        render_text_scaled(rend, sname, pname, t, 1);
        float pweight[2] = { px+10, py+30 };
        char sweight[64]; sprintf(sweight, "WEIGHT: %.0f", 
            info->augs[aug].weight);
        render_text_scaled(rend, sweight, pweight, t, 1);
    } else {
        float pname[2] = { px+10, py+10 };
        render_text_scaled(rend, "select an augment...", pname, t, 1);
    }
}

void render_view_aug_detail (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int aug) 
{
    if (aug != -1) {
        float h = 10;
        float pname[2] = { px+10, py+h };
        char sname[64]; sprintf(sname, "%s", 
            info->augs[aug].name);
        render_text_scaled(rend, sname, pname, t, 1);
        h += 20;
        
        float pweight[2] = { px+10, py+h };
        char sweight[64]; sprintf(sweight, "WEIGHT: %.0f", 
            info->augs[aug].weight);
        render_text_scaled(rend, sweight, pweight, t, 1);
        h += 15;
        
        float range = info->augs[aug].add_range;
        if (range != 0) {
            float p[2] = { px+10, py+h };
            char s[64]; sprintf(s, "RANGE: %.1f", range);
            render_text_scaled(rend, s, p, t, 1);
            h += 15;
        }
        
        float cooldown = info->augs[aug].add_cooldown;
        if (cooldown != 0) {
            float p[2] = { px+10, py+h };
            char s[64]; sprintf(s, "COOLDOWN: %.2f", cooldown);
            render_text_scaled(rend, s, p, t, 1);
            h += 15;
        }
        
        float speed = info->augs[aug].add_speed;
        if (speed != 0) {
            float p[2] = { px+10, py+h };
            char s[64]; sprintf(s, "SPEED: %.2f", speed);
            render_text_scaled(rend, s, p, t, 1);
            h += 15;
        }
        
        float hp = info->augs[aug].add_hp;
        if (hp != 0) {
            float p[2] = { px+10, py+h };
            char s[64]; sprintf(s, "HP: %.1f", hp);
            render_text_scaled(rend, s, p, t, 1);
            h += 15;
        }
        
        h += 5;
        
        float sum = 0;
        for (int i=0; i<7; i++) sum += fabs(info->augs[aug].add_armor[i]);
        if (sum != 0) {
            float p[2] = { px+10, py+h };
            render_text_scaled(rend, "ARMOR: ", p, t, 1);
            h += 15;
            for (int i=0; i<7; i++) {
                if (info->augs[aug].add_armor[i] == 0) continue;
                float pa[2] = { px+10, py+h };
                char sa[64]; sprintf(sa, "%s: %.1f%", 
                    info->damage_types[i], info->augs[aug].add_armor[i]);
                render_text_scaled(rend, sa, pa, t, 1);
                h += 15;
            }
        }
        
        sum = 0;
        for (int i=0; i<7; i++) sum += fabs(info->augs[aug].add_damage[i]);
        if (sum != 0) {
            float p[2] = { px+10, py+h };
            render_text_scaled(rend, "DAMAGE: ", p, t, 1);
            h += 15;
            for (int i=0; i<7; i++) {
                if (info->augs[aug].add_damage[i] == 0) continue;
                float pa[2] = { px+10, py+h };
                char sa[64]; sprintf(sa, "%s: %.1f%", 
                    info->damage_types[i], info->augs[aug].add_damage[i]);
                render_text_scaled(rend, sa, pa, t, 1);
                h += 15;
            }
        }
    } else {
        render_view_aug(rend, t, px, py, info, aug);
    }
}


void render_view_brain (SDL_Renderer* rend, txtd *t, int px, int py, 
    infos *info, int brain) 
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
    infos *info, int temp) 
{
    button b = { "edit", 4, { px, py } };
    render_button(rend, t, &b);
    float wedit = get_text_width("edit", t);
    
    button b1 = { "place", 4, { px+wedit+2*4+5, py } };
    render_button(rend, t, &b1);
    float wplace = get_text_width("place", t);
    
    float pname[2] = { wplace+wedit+4*4+10+px, py+4 };
    char *sname = info->chassis[info->templates[temp].chassis].name;
    render_text_scaled(rend, sname, pname, t, 1);
}