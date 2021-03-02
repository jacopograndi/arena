#include <stdio.h>

#include "test.h"
#include "test_gst.h"


int test_run (infos *info) {
    int result = 0;
    result += test_gst_run(info);
    return result;
}