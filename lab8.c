#include <FPT.h>
#include <D3d_matrix.h>

double ambient; //amount of ambient light
double diffuse_max;
double halfangle = 40;
double radians = 3 * (M_PI / 180);
int hither = 1;
int LX, LY, LZ; //location of light source
int spec_power;
int WIDTH = 600; int HEIGHT = 600; int DEPTH = 600;


typedef struct {
  double centerx, centery, centerz;
  double x[10000], y[10000], z[10000];
  FILE *file;
  int numpolys; //the number of subobjects that make up the object
  int points; //number of points in whatever
  int shapeorder[1000][8]; //Where things connect to
  int shapes[10000]; //the shapes [which shape]
  int xcounter, ycounter, zcounter;
} Object;

typedef struct {
  double avg_depth;
  double color[3];
  double x[100];
  double y[100];
  double z[100];
  double x2d[100];
  double y2d[100];
  int size;
} Plane;

typedef struct {
  int counter;
  Plane *plane;
} Final_plane;

Final_plane total; //Global collection of planes

void printarray(double *a, int size) {
  int i;
  for (i = 0; i < size; i++) {
    printf("%.2lf\n", a[i]);
  }
  printf("\n");
}

//reads in file
//Modified from lab2; does not read in colors
void readobject(FILE *g, Object* poly) {
  int j, k, p, np, shp[10000];
  // int temp_numpolys, temp_shapes[1000], temp_shapeorder[100][100];
  fscanf(g, "%d", &p);
  poly->points = p;
  // printf("there are %d points\n", poly->points);
  for (k = 0; k < poly->points; k++) {
    fscanf(g, "%lf  %lf  %lf", &poly->x[k], &poly->y[k],
           &poly->z[k]);
    // printf("(%lf,%lf,%lf)\n", poly->x[k], poly->y[k], poly->z[k]);
  }
  // printf("%d\n", sizeof(poly->x) / sizeof(poly->x[0]));
  fscanf(g, "%d", &np);
  poly->numpolys = np;
  // printf("there are %d objects in this whatever\n", poly->numpolys); //works up til here
  for (k = 0; k < np; k++) {
    fscanf(g, "%d", &shp[k]);
    poly->shapes[k] = shp[k];
    // printf("%d: ", poly->shapes[k]);
    for (j = 0; j < shp[k]; j++) {
      fscanf(g, "%d", &poly->shapeorder[k][j]);
      // printf("%d ", poly->shapeorder[k][j]);
    }
    // printf("\n");
  }
}

int Clip_Polygon_Against_Plane(
  double a, double b, double c, double d,
  double *polyx, double *polyy, double *polyz, int size,
  double *resx, double *resy, double *resz)

// Clip polygon against the plane ax + by + cz + d = 0,
// where ax + by + cz + d < 0 is considered IN.
// Incoming poly defined in arrays polyx, polyy, polyz with numverts = size.
// Clipped result values are stored in arrays resx, resy, resz
// The numverts of the clipped result is returned as value of the function.

{
  int num, i, j ;
  double x1, y1, z1, x2, y2, z2, x21, y21, z21, den, t,
         xintsct, yintsct, zintsct ;
  double s1, s2 ;
  num = 0 ;
  for (i = 0 ; i < size ; i++) {
    j = (i + 1) % size ;

    // load up segment to be clipped
    x1 = polyx[i] ; y1 = polyy[i] ; z1 = polyz[i] ;
    x2 = polyx[j] ; y2 = polyy[j] ; z2 = polyz[j] ;

    // clip line segment (x1,y1)-(x2,y2) against line
    s1 = (a * x1 + b * y1 + c * z1 + d) ;
    s2 = (a * x2 + b * y2 + c * z2 + d) ;

    if ((s1 >= 0) && (s2 >= 0)) {
      // out to out, do nothing
    } else if ((s1 < 0) && (s2 < 0)) {
      // in to in
      resx[num] = x2 ;
      resy[num] = y2 ;
      resz[num] = z2;
      num++ ;
    } else {
      // one is in, the other out, so find the intersection
      x21 = x2 - x1 ;
      y21 = y2 - y1 ;
      z21 = z2 - z1 ;
      den = a * x21 + b * y21 + c * z21;
      if (den == 0) continue ; // do nothing-should never happen
      t = -(a * x1 + b * y1 + c * z1 + d) / den ;
      xintsct = x1 + t * x21 ;
      yintsct = y1 + t * y21 ;
      zintsct = z1 + t * z21 ;

      if (s1 < 0) {
        // in to out
        resx[num] = xintsct ;
        resy[num] = yintsct ;
        resz[num] = zintsct ;
        num++ ;
      } else  {
        // out to in
        resx[num] = xintsct ;
        resy[num] = yintsct ;
        resz[num] = zintsct;
        num++ ;

        resx[num] = x2 ;
        resy[num] = y2 ;
        resz[num] = z2 ;
        num++ ;
      }
    }
  } // end for i
  return num ;  // return size of the result poly
}

int clippers(double *x, double *y, double *z, int size) { //make temp
  double yonder = 100;
  size = Clip_Polygon_Against_Plane(0, 1, .57, 0, x, y, z, size, x, y, z);
  size = Clip_Polygon_Against_Plane(0, -1, .57, 0, x, y, z, size, x, y, z);
  size = Clip_Polygon_Against_Plane(1, 0, .57, 0, x, y, z, size, x, y, z);
  size = Clip_Polygon_Against_Plane(-1, 0, .57, 0, x, y, z, size, x, y, z);
  size = Clip_Polygon_Against_Plane(0, 0, 1, -yonder, x, y, z, size, x, y, z);
  size = Clip_Polygon_Against_Plane(0, 0, 1, hither, x, y, z, size, x, y, z);

  return size;
}

//Compare method; auxillary for qsort
int compare (const void *p, const void *q) {
  Plane *a, *b ;

  a = (Plane*)p ; b = (Plane*)q ;

  return ((*a).avg_depth) < ((*b).avg_depth) ? 1 :
         ((*a).avg_depth) > ((*b).avg_depth) ? -1 : 0;
}

//converts vector to unit vector
void to_unit_vect(double *vect) {
  int i;
  double mag = sqrt(pow(vect[0], 2) + pow(vect[1], 2) + pow(vect[2], 2));
  for (i = 0; i < 3; i++) {
    // if (fabs(vect[i]) > 10e-7) {
    vect[i] /= mag;
    // }
  }
}

//returns the dot product of the n & l vectors
double vector_setup(double *x, double *y, double *z, double *n_vect,
                    double *l_vect, double *r_vect, double *e_vect)
{
  double vect1[3] = { x[0] - x[1], y[0] - y[1], z[0] - z[1] };
  double vect2[3] = { x[0] - x[2], y[0] - y[2], z[0] - z[2] };
  D3d_x_product(n_vect, vect1, vect2); //this creates the orthagonal vec
  to_unit_vect(n_vect);

  l_vect[0] = LX - x[0];
  l_vect[1] = LY - y[0];
  l_vect[2] = LZ - z[0];
  to_unit_vect(l_vect);

  e_vect[0] = 0 - x[0]; //Eye at 0,0,0
  e_vect[1] = 0 - y[0];
  e_vect[2] = 0 - z[0];
  to_unit_vect(e_vect);

  if (D3d_dot_product(l_vect, n_vect) < 0) {
    n_vect[0] *= -1;
    n_vect[1] *= -1;
    n_vect[2] *= -1;
  }

  double nl = D3d_dot_product(l_vect, n_vect);

  r_vect[0] = 2 * nl * n_vect[0] - l_vect[0];
  r_vect[1] = 2 * nl * n_vect[1] - l_vect[1];
  r_vect[2] = 2 * nl * n_vect[2] - l_vect[2];
  to_unit_vect(r_vect);

  return nl;
}

void color_setup(double intensity, double i_vect[3]) {
  double tippingpoint = ambient + diffuse_max;
  double temp1 = (intensity - tippingpoint) / (1 - tippingpoint);
  double temp2 = intensity / tippingpoint;
  int i;
  for (i = 0; i < 3; i++) {
    i_vect[i] = (intensity >= tippingpoint) ? temp1 * (1 - i_vect[i]) +
                i_vect[i] : temp2 * i_vect[i];
  }
}

void light_n_color(Plane* plane) {
  double n_vect[3] = {0.0};
  double l_vect[3] = {0.0};
  double e_vect[3] = {0.0};
  double r_vect[3] = {0.0};
  double i_vect[3] = {.9, 0, .2};
  double nl = vector_setup(plane->x, plane->y, plane->z,
                           n_vect, l_vect, r_vect, e_vect);
  double specular = 1 - ambient - diffuse_max;
  double er = D3d_dot_product(e_vect, r_vect);
  er = (er < 0) ? 0 : er;
  double intensity = (D3d_dot_product(e_vect, n_vect) < 0) ? ambient :
                     ambient + (diffuse_max * nl) + specular *
                     pow(er, spec_power);

  color_setup(intensity, i_vect);
  plane->color[0] = i_vect[0];
  plane->color[1] = i_vect[1];
  plane->color[2] = i_vect[2];
}

void to_3d(Plane* plane, double *tempx, double *tempy, double *tempz,
           int size) {
  int j;
  double mod = (HEIGHT / 2) / tan(halfangle * (M_PI / 180));
  while (j < size) {
    if (fabs(tempz[j]) < 10e-7) {
      plane->x2d[j] = mod * tempx[j] / tempz[j] + (WIDTH / 2);
      plane->y2d[j] = mod * tempy[j] / tempz[j] + (WIDTH / 2);
      plane->x[j] = tempx[j];
      plane->y[j] = tempy[j];
      plane->z[j] = tempz[j];
      j++;
    } else {
      j++;
    }
  }
}

//puts all the planes into collection
void predraw(Object poly, int in) {
  double tempx[100], tempy[100], tempz[100];
  int size = 0;
  int i, k, j;
  Plane plane[poly.numpolys];
  for (k = 0; k < poly.numpolys; k++) {
    j = 0;
    plane[k].avg_depth = 0.0;
    while (j < poly.shapes[k]) {
      tempx[j] = poly.x[poly.shapeorder[k][j]];
      tempy[j] = poly.y[poly.shapeorder[k][j]];
      tempz[j] = poly.z[poly.shapeorder[k][j]];
      plane[k].avg_depth += poly.z[poly.shapeorder[k][j]];
      j++;
    }
    plane[k].size = clippers(tempx, tempy, tempz, j);
    printf("%d\n", plane[k].size);
    to_3d(&plane[k], tempx, tempy, tempz, plane[k].size);
    light_n_color(&plane[k]);
    plane[k].avg_depth = plane[k].avg_depth / j;
  }

  for (k = 0; k < poly.numpolys; k++) {
    total.plane[k + total.counter] = plane[k];
  }
  total.counter += poly.numpolys;
}

//draws the all the planes based on average depth
void draw() {
  int i;
  qsort (total.plane, total.counter, sizeof(Plane), compare);
  for (i = 0; i < total.counter; i++) {
    G_rgb(total.plane[i].color[0], total.plane[i].color[1],
          total.plane[i].color[2]);
    G_fill_polygon(total.plane[i].x2d, total.plane[i].y2d,
                   total.plane[i].size);
  }
}

//finds the largest/smallest value in array
int findextrema(double *y, int z, int swatch) {
  int position = 0; int i = 0;
  double temp = 0 + (swatch * HEIGHT);
  for (i = 0; i < z; i++) {
    if (swatch == 0 && y[i] > temp) { //finds largest
      position = i;
      temp = y[i];
    } else if (swatch == 1 && y[i] < temp) { //finds smallest
      position = i;
      temp = y[i];
    }
  }
  return position;
}

//finds the scale factor of the object
//also reveals the center of the object
double scale_n_fit(Object* poly) {
  int k;
  double smallx, bigx, smally, bigy, smallz, bigz;
  bigx = poly->x[findextrema(poly->x, poly->points, 0)];
  smallx = poly->x[findextrema(poly->x, poly->points, 1)];
  bigy = poly->y[findextrema(poly->y, poly->points, 0)];
  smally = poly->y[findextrema(poly->y, poly->points, 1)];
  bigz = poly->z[findextrema(poly->z, poly->points, 0)];
  smallz = poly->z[findextrema(poly->z, poly->points, 1)];

  double boxheight = bigy - smally;
  double boxwidth =  bigx - smallx;
  double boxdepth = bigz - smallz;

  poly->centerx = (bigx + smallx) / 2;
  poly->centery = (bigy + smally) / 2;
  poly->centerz = (bigz + smallz) / 2;
  double centers[3] = {poly->centerx, poly->centery,
                       poly->centerz
                      };
  return (findextrema(centers, 3, 1) == 1) ? WIDTH / boxwidth :
         (findextrema(centers, 3, 1) == 2) ? HEIGHT / boxheight :
         DEPTH / boxdepth;
}

//changes the message based on how many objects are inputted via command line
void welcome() {
  char q;
  printf("Please input the location of the light: ");
  scanf("%d %d %d", &LX, &LY, &LZ);
  printf("Please input the ambient light: ");
  scanf("%lf", &ambient);
  printf("Please input the diffuse max: ");
  scanf("%lf", &diffuse_max);
  printf("Please input the specularity exponent: ");
  scanf("%d", &spec_power);
  printf("\n");
}


int main (int argc, char **argv) {
  char q, action;
  double mat[4][4], minv[4][4], scaleFactor;
  int cc, sign, curObj, k, h;
  int increment = 20; int temp = 0;
  Object object[argc];

  for (cc = 1; cc < argc; cc++) {
    object[cc].file = fopen(argv[cc], "r"); //opens a file; r = read only
    if (object[cc].file == NULL) { //if the file is empty
      printf("can't open (1)\n");
      exit(1);
    } else {
      readobject(object[cc].file, &object[cc]);

      D3d_make_identity(mat); D3d_make_identity(minv);
      scaleFactor = scale_n_fit(&object[cc]);
      D3d_translate(mat, minv, -object[cc].centerx, -object[cc].centery,
                    -object[cc].centerz);
      D3d_scale(mat, minv, scaleFactor, scaleFactor, scaleFactor);
      D3d_mat_mult_points(object[cc].x, object[cc].y, object[cc].z,
                          mat, object[cc].x, object[cc].y,
                          object[cc].z, object[cc].points);
      temp += object[cc].numpolys;
    }
  }

  total.plane = malloc(temp  * sizeof(Plane));
  welcome();
  curObj = 1;
  sign = 1 ;
  action = 't' ;


  if (curObj < argc && curObj > 0) {
    G_init_graphics(WIDTH, HEIGHT);
    object[curObj].xcounter = 0;
    object[curObj].ycounter = 0;
    object[curObj].zcounter = 0;

    while (1) {
      G_rgb(0, 0, 0);
      G_clear();
      total.counter = 0;

      for (cc = 1; cc < argc; cc++) {
        predraw(object[cc], cc);
      }
      draw();


      D3d_make_identity (mat) ;
      D3d_make_identity (minv) ;

      q = G_wait_key() ;

      if (q == 'q') {
        exit(0) ;
      } else if (q == 'c') {
        sign = -sign ;
      } else if (q == 't') {
        action = q ;
      } else if (q == 'S') {
        action = q ;
      } else if (q == 'r') {
        action = q ;
      } else if (('0' <= q) && (q <= '9')) {
        k = q - '0' ;
        if (h != curObj) {
          curObj = k;
        }
      } else if ((q == 'x') && (action == 't')) {
        D3d_translate (mat, minv, sign * increment, 0, 0);
        object[curObj].xcounter = object[curObj].xcounter + (sign * increment);

      } else if ((q == 'y') && (action == 't')) {
        D3d_translate (mat, minv, 0, sign * increment, 0);
        object[curObj].ycounter = object[curObj].ycounter + (sign * increment);

      } else if ((q == 'z') && (action == 't')) {
        D3d_translate(mat, minv, 0, 0, sign * increment);
        object[curObj].zcounter = object[curObj].zcounter + (sign * increment);

      } else if ((q == 'x') && (action == 'r')) {
        D3d_translate(mat, minv, -object[curObj].xcounter, -object[curObj].ycounter, -object[curObj].zcounter);
        D3d_rotate_x(mat, minv, sign * radians);
        D3d_translate(mat, minv, object[curObj].xcounter, object[curObj].ycounter, object[curObj].zcounter);

      } else if ((q == 'y') && (action == 'r')) {
        D3d_translate(mat, minv, -object[curObj].xcounter, -object[curObj].ycounter, -object[curObj].zcounter);
        D3d_rotate_y(mat, minv, sign * radians);
        D3d_translate(mat, minv, object[curObj].xcounter, object[curObj].ycounter, object[curObj].zcounter);

      } else if ((q == 'z') && (action == 'r')) {
        D3d_translate(mat, minv, -object[curObj].xcounter, -object[curObj].ycounter, -object[curObj].zcounter);
        D3d_rotate_z(mat, minv, sign * radians);
        D3d_translate(mat, minv, object[curObj].xcounter, object[curObj].ycounter, object[curObj].zcounter);

      } else {
        printf("no action\n") ;
      }

      D3d_mat_mult_points(object[curObj].x, object[curObj].y,
                          object[curObj].z, mat,
                          object[curObj].x, object[curObj].y,
                          object[curObj].z,
                          object[curObj].points + 1) ;
      //the numpoints[curObj]+1 is because we have stored
      //the center of the object at the arrays' end
    }
  }
}

