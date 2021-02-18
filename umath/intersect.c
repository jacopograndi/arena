#include <intersect.h>

int pt_rect (float p[], float a[], float b[]) {
    if (p[0]>a[0] && p[0]<a[0]+b[0] && p[1]>a[1] && p[1]<a[1]+b[1]) 
        return 1;
    return 0;
}