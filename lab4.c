#include <FPT.h>
#include <D3d_matrix.h>

double smallx, smally, bigx, bigy; //specific smallest & largest (X,Y)'s
double x[100][100], y[100][100], z[100][100]; //x, y, z coordinates
int boxheight, boxwidth, centerx, centery;
int points[100]; //number of points in whatever
int polys[10];
int shapeorder[100][100][100]; //where things connect to
int shapes[100][100]; //the shapes[what argument][which shape]

int WIDTH = 600; int HEIGHT = 600;

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
  int cc, sign, currentObj;

  for (cc = 1; cc < argc; cc++) {
    g = fopen(argv[cc], "r"); //opens a file; r = read only
    if (g == NULL) { //if the file is empty, it will let me know
      printf("can't open (1)\n");
      exit(1);
    } else {
      readobject(g, cc);
    }
  }

  // lots of stuff here

  //Half_angle_degrees ... I think you have 40 degrees as the default
  welcome(argc - 1);
  scanf("%c", &q);
  currentObj = q - '0';
  sign = 1 ;
  action = 't' ;

  if (currentObj < argc && currentObj > 0) {
    // G_init_graphics(WIDTH, HEIGHT);
    printf("This is as far as you got for now\n");
    D3d_make_identity(mat); D3d_make_identity(minv);
    // D2d_translate(m, minv, -WIDTH / 2, -HEIGHT / 2);
    // D2d_rotate(m, minv, M_PI/45);
    // D2d_translate(m, minv, WIDTH / 2, HEIGHT / 2);

    // while (1) {

      //   D3d_make_identity (V) ;
      //   D3d_make_identity (Vi) ;

      //   q = G_wait_key() ;

      //   if (q == 'q') {
      //     exit(0) ;

      //   } else if (q == 'c') {
      //     sign = -sign ;

      //   } else if (q == 't') {
      //     action = q ;

      //   } else if (q == 'S') {
      //     action = q ;

      //   } else if (q == 'r') {
      //     action = q ;

      //   } else if (('0' <= q) && (q <= '9')) {
      //     k = q - '0' ;  if (h < numobjects) currentObj = k ;

      //   } else if ((q == 'x') && (action == 't')) {

      //   } else if ((q == 'y') && (action == 't')) {

      //   } else if ((q == 'z') && (action == 't')) {

      //   } else if ((q == 'x') && (action == 'r')) {

      //   } else if ((q == 'y') && (action == 'r')) {

      //   } else if ((q == 'z') && (action == 'r')) {

      //   } else {
      //     printf("no action\n") ;
      //   }


      //   D3d_mat_mult_points (x[currentObj], y[currentObj], z[currentObj],  V,  x[currentObj], y[currentObj], z[currentObj], numpoints[currentObj] + 1) ;
      //   // the numpoints[currentObj]+1 is because we have stored the center of the object
      //   // at the arrays' end

      //   G_rgb(0, 0, 0) ;
      //   G_clear() ;
      //   G_rgb(0, 0, 1) ;
      //   draw_all_objects() ;

      // }


    
  }
}

