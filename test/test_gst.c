#include <stdio.h>

#include "test.h"

#include "../gst/fxs.h"
#include "../gst/gst.h"

int test_gst_mirror_match (infos *info, int n);
    
    
int test_gst_run (infos *info) {
    int result = 0;
    for (int i=0; i<1; i++) {
        result += test_gst_mirror_match(info, i);
    }
    return result;
}

int test_gst_mirror_match (infos *info, int n) {
    if(TEST_VERBOSE) printf("test_gst: mirror match __test%d\n", n);
    fxs fx;
    fx_init(&fx);
    gamestate gst;
    gst_init(&gst);
    
    char name[64]; sprintf(name, "hidden/__test%d", n);
    info_load_army(gst.army_bp+0, name);
    info_load_army(gst.army_bp+1, name);
    gst.playernum = 2;
    gst_tobattle(&gst, info);
    
    float t = 0; int i=0;
    for (; i < 2000; i++) {
        gst_process(&gst, info, &fx, t);
        t += gst.turnspeed;
        if (gst.over == 1) {
            break;
        }
    }
    if (i < 2000) {
        if(TEST_VERBOSE) printf("test_gst: %d turns\n", i);
        int winner = gst_check_victory(&gst);
        if (winner == -1) {
            if(TEST_VERBOSE) printf("test_gst success: draw\n");
        } else {
            printf("test_gst failed: not draw (%d)\n", winner);
            return 1;
        }
        gst_destroy(&gst);
        return 0;
    }
    return 1;
}