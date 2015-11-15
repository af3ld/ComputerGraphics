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

int check_if_behind(double *plane_x, double *plane_y, double *plane_z,
                    int size) {
  double p_vect[3] = {0.0};
  double vect1[3] = {plane_x[0] - plane_x[1],
                     plane_y[0] - plane_y[1],
                     plane_z[0] - plane_z[1],
                    };
  double vect2[3] = {plane_x[0] - plane_x[2],
                     plane_y[0] - plane_y[2],
                     plane_z[0] - plane_z[2],
                    };
  double e_vect[3] = {-plane_x[0], -plane_y[0], -plane_z[0] };
  D3d_x_product(p_vect, vect1, vect2);
  int dot_product = (p_vect[0] * e_vect[0]) + (p_vect[1] *
                    e_vect[1]) + (p_vect[2] * e_vect[2]);
  
  return (dot_product > 0) ? 1 : 0;
}

void drawit(int i) {
  int k, j;
  double tempx[points[i]], tempy[points[i]];
  double plane_x[points[i]], plane_y[points[i]], plane_z[points[i]];
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

        plane_x[j] = x[i][shapeorder[i][k][j]];
        plane_y[j] = y[i][shapeorder[i][k][j]];
        plane_z[j] = z[i][shapeorder[i][k][j]];
        j++;
      } else {
        j++;
      }
    }
    if (check_if_behind(plane_x, plane_y, plane_z, j) == 1) {
      G_rgb(1, 0, .3);
      G_polygon(tempx, tempy, j);
    }
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
  double radians = 3 * (M_PI / 180);
  FILE *g;
  int cc, sign, currentObj, k, h;
  int increment = 15;
  int xcounter, ycounter, zcounter = 0;

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
        k = q - '0' ;
        if (h != currentObj) {
          currentObj = k;
        }
      } else if ((q == 'x') && (action == 't')) {
        D3d_translate (mat, minv, sign * increment, 0, 0);
        xcounter = xcounter + (sign * increment);

      } else if ((q == 'y') && (action == 't')) {
        D3d_translate (mat, minv, 0, sign * increment, 0);
        ycounter = ycounter + (sign * increment);

      } else if ((q == 'z') && (action == 't')) {
        D3d_translate(mat, minv, 0, 0, sign * increment);
        zcounter = zcounter + (sign * increment);

      } else if ((q == 'x') && (action == 'r')) {
        D3d_translate(mat, minv, -xcounter, -ycounter, -zcounter);
        D3d_rotate_x(mat, minv, sign * radians);
        D3d_translate(mat, minv, xcounter, ycounter, zcounter);

      } else if ((q == 'y') && (action == 'r')) {
        D3d_translate(mat, minv, -xcounter, -ycounter, -zcounter);
        D3d_rotate_y(mat, minv, sign * radians);
        D3d_translate(mat, minv, xcounter, ycounter, zcounter);

      } else if ((q == 'z') && (action == 'r')) {
        D3d_translate(mat, minv, -xcounter, -ycounter, -zcounter);
        D3d_rotate_z(mat, minv, sign * radians);
        D3d_translate(mat, minv, xcounter, ycounter, zcounter);

      } else {
        printf("no action\n") ;
      }


      D3d_mat_mult_points (x[currentObj], y[currentObj], z[currentObj],
                           mat,  x[currentObj], y[currentObj],
                           z[currentObj], points[currentObj] + 1) ;
      //the numpoints[currentObj]+1 is because we have stored
      //the center of the object at the arrays' end

      G_rgb(0, 0, 0);
      G_clear();
      drawit(currentObj);
    }
  }
}

