#include <FPT.h>
#include <D3d_matrix.h>


double centerx[10], centery[10], centerz[10];
double halfangle = 40;
double radians = 3 * (M_PI / 180);
double x[10][10000], y[10][10000], z[10][10000];
int numpolys[10]; //the number of subpolygons that make up the object
int points[10];
int shapeorder[10][10000][10];
int shapes[10][10000];
double xcounter[10], ycounter[10], zcounter[10];

int WIDTH = 600; int HEIGHT = 600; int DEPTH = 600;

typedef struct {
  double avg_depth;
  double x[100];
  double y[100];
  double z[100];
  int size;
} Plane;

typedef struct {
  int counter;
  Plane *plane;
} Final_plane;

Final_plane total;

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
  int j, k;
  // int temp_numpolys, temp_shapes[1000], temp_shapeorder[100][100];
  fscanf(g, "%d", &points[i]);
  // printf("there are %d points\n", points[i]);
  for (k = 0; k < points[i]; k++) {
    fscanf(g, "%lf  %lf  %lf", &x[i][k], &y[i][k], &z[i][k]);
    // printf("(%lf,%lf,%lf)\n", x[i][k], y[i][k], z[i][k]);
  }
  fscanf(g, "%d", &numpolys[i]);
  // printf("there are %d faces in this object\n", numpolys[i]);
  for (k = 0; k < numpolys[i]; k++) {
    fscanf(g, "%d", &shapes[i][k]);
    // printf("%d: ", shapes[i][k]);
    for (j = 0; j < shapes[i][k]; j++) {
      fscanf(g, "%d", &shapeorder[i][k][j]);
      // printf("%d ", shapeorder[i][k][j]);
    }
    // printf("\n");
  }
}

int compare (const void *p, const void *q) {
  Plane *a, *b ;

  a = (Plane*)p ; b = (Plane*)q ;

  return ((*a).avg_depth) < ((*b).avg_depth) ? 1 :
         ((*a).avg_depth) > ((*b).avg_depth) ? -1 : 0;
}

void predraw(int i) {
  double mod = (HEIGHT / 2) / tan(halfangle * (M_PI / 180));
  int size = 0;
  int k, j;
  Plane plane[numpolys[i]];

  for (k = 0; k < numpolys[i]; k++) {
    j = 0;
    plane[k].avg_depth = 0.0;
    while (j < shapes[i][k]) {
      if (fabs(z[i][shapeorder[i][k][j]]) > 10e-7) { //Checks if z == 0

        plane[k].x[j] = mod * (x[i][shapeorder[i][k][j]]
                               / z[i][shapeorder[i][k][j]])
                        + (WIDTH / 2);
        plane[k].y[j] = mod * (y[i][shapeorder[i][k][j]]
                               / z[i][shapeorder[i][k][j]])
                        + (WIDTH / 2);
        // printf("%lf, %lf\n", plane[k].x[j], plane[k].y[j]);
        plane[k].z[j] = z[i][shapeorder[i][k][j]];
        plane[k].avg_depth += z[i][shapeorder[i][k][j]];
        j++;
      } else {
        j++;
      }
    }
    plane[k].size = j;
    plane[k].avg_depth = plane[k].avg_depth / j;
  }

  total.plane = malloc((numpolys[i] + total.counter)  * sizeof(Plane));
  for (k = 0; k < numpolys[i]; k++) {
    total.plane[k + total.counter] = plane[k];
  }
  total.counter += numpolys[i];
}

void draw() {
  int k;
  qsort (total.plane, total.counter, sizeof(Plane), compare);
  // printarray(total.planes, total.counter);
  for (k = 0; k < total.counter; k++) {
    G_rgb(.2, .6, .4);
    G_fill_polygon(total.plane[k].x,
                   total.plane[k].y, total.plane[k].size);
    G_rgb(1, 1, 1);
    G_polygon(total.plane[k].x,
              total.plane[k].y, total.plane[k].size);
  }
}


//finds the largest/smallest value in array
int findextrema(double * y, int z, int swatch) {
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

  centerx[i] = (bigx + smallx) / 2;
  centery[i] = (bigy + smally) / 2;
  centerz[i] = (bigz + smallz) / 2;
  double centers[3] = {centerx[i], centery[i], centerz[i]};
  return (findextrema(centers, 3, 1) == 1) ? WIDTH / boxwidth :
         (findextrema(centers, 3, 1) == 2) ? HEIGHT / boxheight :
         DEPTH / boxdepth;
}

//changes the message based on how many objects are inputted via command line
void welcome(int i) {
  printf((i == 1) ? "Press 1 to move the object: " :
         (i == 2) ? "Press 1 or 2 to the first or second object: " :
         "Press 1 through %d to move the respective object: ", i);
}

int main (int argc, char **argv){
  char q, action;
  double mat[4][4], minv[4][4], scaleFactor;
  FILE *g;
  int cc, sign, currentObj, k, h;
  int increment = 20;

  for (cc = 1; cc < argc; cc++) {
    g = fopen(argv[cc], "r"); //opens a file; r = read only
    if (g == NULL) { //if the file is empty
      printf("can't open (1)\n");
      exit(1);
    } else {
      readobject(g, cc);
      // printarray(z[cc], points[cc]);
      D3d_make_identity(mat); D3d_make_identity(minv);
      scaleFactor = scale_n_fit(cc);
      D3d_translate(mat, minv, -centerx[cc], -centery[cc], -centerz[cc]);
      D3d_scale(mat, minv, scaleFactor, scaleFactor, scaleFactor);
      D3d_mat_mult_points(x[cc], y[cc], z[cc], mat, x[cc],
                          y[cc], z[cc], points[cc]);
    }
  }

  printf("Which object (between 1 and %d) would you like to start with? \n", argc - 1);
  scanf("%c", &q);
  currentObj = q - '0';
  sign = 1 ;
  action = 't' ;


  if (currentObj < argc && currentObj > 0) {
    G_init_graphics(WIDTH, HEIGHT);

    while (1) {
      G_rgb(0, 0, 0);
      G_clear();

      total.counter = 0;
      for (cc = 1; cc < argc; cc++) {
        predraw(cc);
      }
      draw();

      // exit(1);
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
      } else if (('0' < q) && (q <= argc + '0' - 1)) {
        k = q - '0' ;
        if (k != currentObj) {
          currentObj = k;
          printf("Which object: %d\n", currentObj);

        }
      } else if ((q == 'x') && (action == 't')) {
        D3d_translate (mat, minv, sign * increment, 0, 0);
        xcounter[currentObj] = xcounter[currentObj] + (sign * increment);

      } else if ((q == 'y') && (action == 't')) {
        D3d_translate (mat, minv, 0, sign * increment, 0);
        ycounter[currentObj] = ycounter[currentObj] + (sign * increment);

      } else if ((q == 'z') && (action == 't')) {
        D3d_translate(mat, minv, 0, 0, sign * increment);
        zcounter[currentObj] = zcounter[currentObj] + (sign * increment);

      } else if ((q == 'x') && (action == 'r')) {
        D3d_translate(mat, minv, -xcounter[currentObj],
                      -ycounter[currentObj], -zcounter[currentObj]);
        D3d_rotate_x(mat, minv, sign * radians);
        D3d_translate(mat, minv, xcounter[currentObj],
                      ycounter[currentObj], zcounter[currentObj]);

      } else if ((q == 'y') && (action == 'r')) {
        D3d_translate(mat, minv, -xcounter[currentObj],
                      -ycounter[currentObj], -zcounter[currentObj]);
        D3d_rotate_y(mat, minv, sign * radians);
        D3d_translate(mat, minv, xcounter[currentObj],
                      ycounter[currentObj], zcounter[currentObj]);

      } else if ((q == 'z') && (action == 'r')) {
        D3d_translate(mat, minv, -xcounter[currentObj],
                      -ycounter[currentObj], -zcounter[currentObj]);
        D3d_rotate_z(mat, minv, sign * radians);
        D3d_translate(mat, minv, xcounter[currentObj],
                      ycounter[currentObj], zcounter[currentObj]);

      } else {
        printf("no action\n") ;
      }

      D3d_mat_mult_points(x[currentObj], y[currentObj],
                          z[currentObj], mat,
                          x[currentObj], y[currentObj],
                          z[currentObj], points[currentObj] + 1);
      //the numpoints[currentObj]+1 is because we have stored
      //the center of the object at the arrays' end
    }
  }
}

