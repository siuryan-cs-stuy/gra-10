#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;

  normalize(light[LOCATION]);
  normalize(normal);
  normalize(view);

  color a = calculate_ambient(alight, areflect);
  color d = calculate_diffuse(light, dreflect, normal);
  color s = calculate_specular(light, sreflect, view, normal);

  i.red = a.red + d.red + s.red;
  i.green = a.green + d.green + s.green;
  i.blue = a.blue + d.blue + s.blue;

  //limit_color(&i);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * *areflect;
  a.green = alight.green * *areflect;
  a.blue = alight.blue * *areflect;
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  d.red = light[COLOR][RED] * *dreflect * dot_product(normal, light[LOCATION]);
  d.green = light[COLOR][GREEN] * *dreflect * dot_product(normal, light[LOCATION]);
  d.blue = light[COLOR][BLUE] * *dreflect * dot_product(normal, light[LOCATION]);
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  int expt = 10;
  color s;
  double *tmp = scalar_mult_vector(2 * dot_product(normal, light[LOCATION]), normal);
  tmp = vector_subtract(tmp, light[LOCATION]);

  s.red = light[COLOR][RED] * *sreflect * pow(dot_product(tmp, view), expt);
  s.green = light[COLOR][GREEN] * *sreflect * pow(dot_product(tmp, view), expt);
  s.blue = light[COLOR][BLUE] * *sreflect * pow(dot_product(tmp, view), expt);

  return s;
}


//limit each component of c to a max of 255
void limit_color( color * c ) {
    c->red = c->red > 255 ? 255 : c->red;
    c->green = c->green > 255 ? 255 : c->green;
    c->blue = c->blue > 255 ? 255 : c->blue;
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
    int i;
    double magnitude = 0;
    for (i = 0; i < 3; i++) {
        magnitude += vector[i] * vector[i];
    }
    magnitude = sqrt(magnitude);
    for (i = 0; i < 3; i++) {
        vector[i] = vector[i] / magnitude;
    }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
    int i;
    double dp = 0;
    for (i = 0; i < 3; i++) {
        dp += a[i] * b[i];
    }
    return dp;
}

double *scalar_mult_vector( double x, double *v ) {
    int i;
    for (i = 0; i < 3; i++) {
        v[i] = x * v[i];
    }
    return v;
}

double *vector_subtract( double *v1, double *v2 ) {
    int i;
    for (i = 0; i < 3; i++) {
        v1[i] = v1[i] - v2[i];
    }
    return v1;
}

double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
