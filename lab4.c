#include <FPT.h>
#include <D3d_matrix.h>


double centerx, centery, centerz;
double halfangle = 40;
double x[100][100], y[100][100], z[100][100]; //x, y, z coordinates
int points[100]; //number of points in whatever
int polys[10];
int shapeorder[100][100][100]; //where things connect to
int shapes[100][100]; //the shapes[what argument][which shape]
int WIDTH = 600; int HEIGHT = 600; int DEPTH = 600;

void printarray(double *a, int size) {
  int i;
  for (i = 0; i < size; i++) {
    printf("%.2lf\n", a[i]);
  }
  printf("\n");
}

//reads in file
//Modified from lab2; does not read in colors
void readobject(FILE *g, int i) {
  int k, j;
  fscanf(g, "%d", &points[i]);
  // printf("there are %d points\n", points[i]);
  for (k = 0; k < points[i]; k++) {
    fscanf(g, "%lf  %lf  %lf", &x[i][k], &y[i][k], &z[i][k]);
    // printf("(%lf,%lf,%lf)\n", x[i][k], y[i][k], z[i][k]);
  }
  fscanf(g, "%d", &polys[i]);
  // printf("there are %d polygons in this whatever\n", polys[i]);
  for (k = 0; k < polys[i]; k++) {
    fscanf(g, "%d", &shapes[i][k]);
    // printf("%d: ", shapes[i][k]);
    for (j = 0; j < shapes[i][k]; j++) {
      fscanf(g, "%d", &shapeorder[i][k][j]);
      // printf("%d ", shapeorder[i][k][j]);
    }
    // printf("\n");
  }
}

void drawit(int i) {
  int k, j;
  double tempx[points[i]], tempy[points[i]];
  double mod = (HEIGHT / 2) / tan(halfangle * (M_PI / 180));
  for (k = 0; k < polys[i]; k++) {
    j = 0;
    while (j < shapes[i][k]) {
      // printf("%d", shapeorder[i][k][j]);
      if (fabs(z[i][shapeorder[i][k][j]]) > 10e-7) { //Checks if z == 0
        tempx[j] = mod * (x[i][shapeorder[i][k][j]] /
                          z[i][shapeorder[i][k][j]]) + (WIDTH / 2);
        tempy[j] = mod * (y[i][shapeorder[i][k][j]] /
                          z[i][shapeorder[i][k][j]]) + (HEIGHT / 2);
        printf("x%d:%.2lf, y%d:%.2lf, z%d:%.2lf\n",
               shapeorder[i][k][j], tempx[j], shapeorder[i][k][j],
               tempy[j], shapeorder[i][k][j],
               z[i][shapeorder[i][k][j]]);
        j++;
      } else {
        j++;
      }
    }
    G_rgb(1, 0, .3);
    G_polygon(tempx, tempy, j);
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
double scale_n_fit(int i) {
  int k;
  double smallx, bigx, smally, bigy, smallz, bigz;
  bigx = x[i][findextrema(x[i], points[i], 0)];
  smallx = x[i][findextrema(x[i], points[i], 1)];
  bigy = y[i][findextrema(y[i], points[i], 0)];
  smally = y[i][findextrema(y[i], points[i], 1)];
  bigz = z[i][findextrema(z[i], points[i], 0)];
  smallz = z[i][findextrema(z[i], points[i], 1)];

  double boxheight = bigy - smally;
  double boxwidth =  bigx - smallx;
  double boxdepth = bigz - smallz;

  centerx = (bigx + smallx) / 2;
  centery = (bigy + smally) / 2;
  centerz = (bigz + smallz) / 2;
  double centers[3] = {centerx, centery, centerz};
  return (findextrema(centers, 3, 1) == 1) ? WIDTH / boxwidth :
         (findextrema(centers, 3, 1) == 2) ? HEIGHT / boxheight :
         DEPTH / boxdepth;
}

//changes the message based on how many objects are inputted via command line
void welcome(int i) {
  printf((i == 1) ? "Press 1 to start: " :
         (i == 2) ? "Press 1 or 2 to start: " :
         "Press 1 through %d to start ", i);
}

int main (int argc, char **argv)
{
  char q, action;
  double mat[4][4], minv[4][4], scaleFactor;
  FILE *g;
  int cc, sign, currentObj, k, h;

  for (cc = 1; cc < argc; cc++) {
    g = fopen(argv[cc], "r"); //opens a file; r = read only
    if (g == NULL) { //if the file is empty, it will let me know
      printf("can't open (1)\n");
      exit(1);
    } else {
      readobject(g, cc);

      D3d_make_identity(mat); D3d_make_identity(minv);
      scaleFactor = scale_n_fit(cc);
      D3d_translate(mat, minv, -centerx, -centery, -centerz);
      D3d_scale(mat, minv, scaleFactor, scaleFactor, scaleFactor);
      D3d_mat_mult_points(x[cc], y[cc], z[cc], mat,
                          x[cc], y[cc], z[cc], points[cc]);
      // printarray(z[cc], points[cc]);
    }
  }

  welcome(argc - 1);
  scanf("%c", &q);
  currentObj = q - '0';
  sign = 1 ;
  action = 't' ;

  if (currentObj < argc && currentObj > 0) {
    G_init_graphics(WIDTH, HEIGHT);
    // printf("This is as far as you got for now\n");
    while (1) {
      G_rgb(0, 0, 0);
      G_clear();
      drawit(currentObj);

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
        k = q - '0' ;  if (h < polys[currentObj]) currentObj = k ;

      } else if ((q == 'x') && (action == 't')) {

      } else if ((q == 'y') && (action == 't')) {

      } else if ((q == 'z') && (action == 't')) {

      } else if ((q == 'x') && (action == 'r')) {

      } else if ((q == 'y') && (action == 'r')) {

      } else if ((q == 'z') && (action == 'r')) {

      } else {
        printf("no action\n") ;
      }


      //   D3d_mat_mult_points (x[currentObj], y[currentObj], z[currentObj],  V,  x[currentObj], y[currentObj], z[currentObj], numpoints[currentObj] + 1) ;
      //   // the numpoints[currentObj]+1 is because we have stored the center of the object
      //   // at the arrays' end

      //   G_rgb(0, 0, 0) ;
      //   G_clear() ;
      //   G_rgb(0, 0, 1) ;
      //   draw_all_objects() ;

    }
  }
}

