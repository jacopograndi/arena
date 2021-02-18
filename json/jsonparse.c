#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <jsonparse.h>

void substr_token (char *json, char *temp, jsmntok_t *t) {
    memcpy(temp, json+t->start, t->end - t->start);
    temp[t->end-t->start] = '\0';
}

void json_parse_array(char *json, void *temp, jsmntok_t *t, int r, char type) {
    for (int i=0; i<r; i++) { 
        if (t[i].type == JSMN_PRIMITIVE) {
            char val[32]; substr_token(json, val, t+i); 
            if (type == 'i') {
                int *p = (int*)(intptr_t)temp+sizeof(int)*i;
                *p = atoi(val); 
            }
            if (type == 'c') {
                int8_t *p = (int8_t*)(intptr_t)temp+sizeof(int8_t)*i;
                *p = atoi(val); 
            }
        }
    }
}

void json_dump_array (char *str, void *arr, int len, char type) {
    int cur = 0;
    sprintf(str+cur, "[ "); cur = strlen(str);
    for (int i=0; i<len; i++) { 
        if (type == 'i') {
            sprintf(str+cur, "%d", (int)(intptr_t)(arr+i*sizeof(int))); 
            cur = strlen(str);
        }
        if (type == 'c') {
            printf("%d\n", ((int8_t*)(intptr_t)(arr))[i]);
            sprintf(str+cur, "%d", ((int8_t*)(intptr_t)(arr))[i]); 
            cur = strlen(str);
        }
        if (i < len-1) {
            sprintf(str+cur, ", "); cur = strlen(str);
        }
    }
    sprintf(str+cur, " ]");
}

int json_parse_subtokens (char *json, jsmntok_t *t, int r, int i) {
    int rt = 0;
    for (int j=i; j<r; j++) {
        if (t[j].start < t[i].end) { rt ++; } 
        else { break; }
    }
    return rt;
}

/* DEPRECATED TRIALS, delete after backup
// parses a <str, int> pair set with a set of already parsed tokens
int json_parse_dict_ci_init (char *json, 
        pair_ci *dict, jsmntok_t *t, int r) { 
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i++) { 
        if (t[i].type == JSMN_OBJECT) { 
            dict_i ++; 
            obj_i = 0; 
        } 
        if (t[i].type == JSMN_STRING) { 
            if (obj_i == 0) { 
                substr_token(json, dict[dict_i].key, t+i); 
            } else if (obj_i == 1) { 
                char ts[13]; substr_token(json, ts, t+i); 
                dict[dict_i].i = atoi(ts); 
            } 
            obj_i++; 
        } 
    } 
    return dict_i+1; 
}

// parses a <str, int> pair set with a set of already parsed tokens
int json_parse_dict_cf_init (char *json, 
        pair_cf *dict, jsmntok_t *t, int r) { 
    int obj_i = 0, dict_i = -1; 
    for (int i=0; i<r; i++) { 
        if (t[i].type == JSMN_OBJECT) { 
            dict_i ++; 
            obj_i = 0; 
        } 
        if (t[i].type == JSMN_STRING) { 
            if (obj_i == 0) { 
                substr_token(json, dict[dict_i].key, t+i); 
            } else if (obj_i == 1) { 
                char ts[13]; substr_token(json, ts, t+i); 
                dict[dict_i].i = atof(ts); 
            } 
            obj_i++; 
        } 
    } 
    return dict_i+1; 
}


// parses a <str, int> pair set
int json_parse_dict_ci (char *json, pair_ci *dict) {
    jsmn_parser p; jsmn_init(&p);
    jsmntok_t t[MAXTOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAXTOKENS);
    json_parse_dict_ci_init(json, dict, t, r);
}

// parses a <str, float> pair set
int json_parse_dict_cf (char *json, pair_cf *dict) {
    jsmn_parser p; jsmn_init(&p);
    jsmntok_t t[MAXTOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAXTOKENS);
    json_parse_dict_cf_init(json, dict, t, r);
}



// parses a list of objects which are lists of pairs <str, int>
int json_parse_list_ci (char *json, list_ci *list) {
    jsmn_parser p; jsmn_init(&p);
    jsmntok_t t[MAXTOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAXTOKENS);
    int list_i = 0;
    for (int i=1; i<r; i++) { // i=1: ignore outer []
        if (t[i].type == JSMN_ARRAY) {
            int rt = 0;
            for (int j=i; j<r; j++) {
                if (t[j].start < t[i].end) { rt ++; } 
                else { break; }
            }
            int len = json_parse_dict_ci_init(
                json, list[list_i].pairs, t+i, rt);
            list[list_i].len = len;
            list_i ++;
            i += rt-1;
        }
    }
    return list_i;
}

// parses a list of objects which are lists of pairs <str, float>
int json_parse_list_cf (char *json, list_cf *list) {
    jsmn_parser p; jsmn_init(&p);
    jsmntok_t t[MAXTOKENS];
    int r = jsmn_parse(&p, json, strlen(json), t, MAXTOKENS);
    int list_i = 0;
    for (int i=1; i<r; i++) { // i=1: ignore outer []
        if (t[i].type == JSMN_ARRAY) {
            int rt = 0;
            for (int j=i; j<r; j++) {
                if (t[j].start < t[i].end) { rt ++; } 
                else { break; }
            }
            int len = json_parse_dict_cf_init(
                json, list[list_i].pairs, t+i, rt);
            list[list_i].len = len;
            list_i ++;
            i += rt-1;
        }
    }
    return list_i;
}
*/