#include <vec.h>

float vec2_add (float c[], float a[], float b[]) { 
    c[0]=a[0]+b[0]; c[1]=a[1]+b[1]; 
}
float vec2_sub (float c[], float a[], float b[]) { 
    c[0]=a[0]-b[0]; c[1]=a[1]-b[1]; 
}
float vec2_mul (float c[], float a[], float b) { 
    c[0]=a[0]*b; c[1]=a[1]*b; 
}
float vec2_div (float c[], float a[], float b) { 
    c[0]=a[0]/b; c[1]=a[1]/b; 
}

float vec2_mag (float v[]) { return sqrt(v[0]*v[0]+v[1]*v[1]); }
float vec2_mag2 (float v[]) { return v[0]*v[0]+v[1]*v[1]; }

void vec2_norm (float v[]) { 
    float m=vec2_mag(v); v[0]/=m; v[1]/=m;
}


float vec3_add (float c[], float a[], float b[]) { 
    c[0]=a[0]+b[0]; c[1]=a[1]+b[1]; c[2]=a[2]+b[2]; 
}
float vec3_sub (float c[], float a[], float b[]) { 
    c[0]=a[0]-b[0]; c[1]=a[1]-b[1]; c[2]=a[2]-b[2]; 
}
float vec3_mul (float c[], float a[], float b) { 
    c[0]=a[0]*b; c[1]=a[1]*b; c[2]=a[2]*b; 
}
float vec3_div (float c[], float a[], float b) { 
    c[0]=a[0]/b; c[1]=a[1]/b; c[2]=a[2]/b; 
}

float vec3_mag (float v[]) { return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]); }
float vec3_mag2 (float v[]) { return v[0]*v[0]+v[1]*v[1]+v[2]*v[2]; }

void vec3_norm (float v[]) { 
    float m=vec3_mag(v); v[0]/=m; v[1]/=m; v[2]/=m;
}