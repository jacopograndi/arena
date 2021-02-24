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
                int *p = (int*)(intptr_t)temp+i;
                *p = atoi(val); 
            }
            if (type == 'c') {
                int8_t *p = (int8_t*)(intptr_t)temp+i;
                *p = atoi(val); 
            }
            if (type == 'f') {
                float *p = (float*)(intptr_t)temp+i;
                *p = atof(val); 
            }
        }
    }
}

void json_dump_array (char *str, void *arr, int len, char type) {
    int cur = 0;
    sprintf(str+cur, "[ "); cur = strlen(str);
    for (int i=0; i<len; i++) { 
        if (type == 'i') {
            sprintf(str+cur, "%d", ((int)(intptr_t)(arr))[i]); 
            cur = strlen(str);
        }
        if (type == 'c') {
            sprintf(str+cur, "%d", ((int8_t*)(intptr_t)(arr))[i]); 
            cur = strlen(str);
        }
        if (type == 'f') {
            sprintf(str+cur, "%f", ((float*)(intptr_t)(arr))[i]); 
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