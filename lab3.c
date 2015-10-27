#include <FPT.h>
#include <D2d_matrix.h>

int WIDTH = 500; int HEIGHT = 500;
int counter = 0;
int points[100]; //number of points in whatever
double x[100][100], y[100][100]; //x and y coordinates
int polys[10];
int shapes[100][100]; //the shapes[what argument][which shape]
int shapeorder[100][100][100]; //where things connect to
double red[100][100], green[100][100], blue[100][100]; //individual colors of each[what arg][which shape]
int boxheight, boxwidth, centerx, centery;
double smallx, smally, bigx, bigy; //specific smallest & largest (X,Y)'s

//determines if a point is on one side of the line or the other
double whatside(double ax, double ay, double bx,
                double by, double x1, double y1,
                double x2, double y2) {

  return ((y1 - y2) * (ax - x1) + (x2 - x1) * (ay - y1)) *
         ((y1 - y2) * (bx - x1) + (x2 - x1) * (by - y1));
}



//Bounding Box For Line Segment
//(x1,y1) = start of the line
//(x2, y2) = the end of the line
int pointincontainer(double x1, double y1, double x2, double y2,
                     double xprime, double yprime) {
  double left, top, right, bottom;
  if (x1 < x2) {
    left = x1;
    right = x2;
  } else {
    left = x2;
    right = x1;
  }
  if (y1 > y2) {
    top = y1;
    bottom = y2;
  } else {
    top = y2;
    bottom = y1;
  }
  //printf("l:%lf, r:%lf, t:%lf, b:%lf\n", left, right, top, bottom);
  //printf("x': %lf, y': %lf\n", xprime, yprime);
  if ((xprime + 0.01) >= left && (xprime - 0.01) <= right &&
      (yprime + 0.01) <= top && (yprime - 0.01) >= bottom) {
    return 1;
  } else {
    return 0;
  }
}

int looper(int i, int n) {
  if (i == n - 1) {
    return 0;
  } else {
    return i + 1;
  }
}

//gets the slope of all the lines; returns them via modified array
void getslope(double *x, double *y, int z, double *slope) {
  int i;
  for (i = 0; i < z; i++) {
    slope[i] = (y[looper(i, z)] - y[i]) / (x[looper(i, z)] - x[i]);
  }
}



//finds the intersection of the all lines
//returns intersection coordinates via array
void f_intersect(double *px, double *py, int pn,
                 double *wx, double *wy, int wn, double *c) {
  int i, j;
  double yprime, xprime, wslope[100], pslope[100], c1, c2;
  getslope(px, py, pn, pslope);
  getslope(wx, wy, wn, wslope);

  for (i = 0; i < wn; i++) {
    for (j = 0; j < pn; j++) {
      c1 = py[j] - pslope[j] * px[j]; //y = mx + c - > c = y -mx
      c2 = wy[i] - wslope[i] * wx[i];

      if ((pslope[j] - wslope[i]) == 0) { //prevents divison by 0
        printf("Slope division by 0; ERROR!\n");
      } else {
        xprime = (c2 - c1) / (pslope[j] - wslope[i]); //intersection of x
        yprime = pslope[j] * xprime + c1; //intersection of y

        if (pointincontainer(px[j], py[j], px[looper(j, pn)], py[looper(j, pn)],
                             xprime, yprime) == 1 &&
            pointincontainer(wx[i], wy[i], wx[looper(i, wn)], wy[looper(i, wn)],
                             xprime, yprime ) == 1) {
          c[counter] = xprime; c[counter + 1] = yprime;
        G_rgb(0,0,0);
        G_fill_circle(xprime,yprime, 2);
          counter += 2;
        }
      }
    }
  }
}


//click a point and it saves it into an array
int clickAndSave(double *x, double *y) {
  double ca[2];
  int i = 0;
  int going = 1;
  G_rgb(0, 0, 0);;
  G_draw_string("Click here to end clipping", 0, 0);
  while (going) {
    G_wait_click(ca);
    //printf("%lf, %lf\n", ca[0], ca[1]);
    if (ca[0] < 260 & ca[1] < 15) {
      going = 0;
      x[i] = x[0];
      y[i] = y[0];
      //printf("done\n");
    } else {
      x[i] = ca[0];
      y[i] = ca[1];
      G_rgb(0, 1, 1);
      G_circle(x[i], y[i], 1);
      G_rgb(0, 0, 0);
      G_circle(x[i], y[i], 2);
      i++;
      //printf(" %d\n", i);
    }

  }
  return i;
}

//draws the polygon; unfilled
void myPolygon(double *x, double *y, int n) {
  int i = 0;
  while (i <= n) {
    if (i != n) {
      G_line(x[i], y[i], x[i + 1], y[i + 1]);
    }
    i++;
  }
}


//Draws bounding box around figure
// seems only availble from drawit method, but it
//sometimes works from other spots,
//but not from all spots
double boundingbox(int i) {
  int k;

  smallx = WIDTH * 2; smally = HEIGHT * 2;
  bigx = WIDTH * -2; bigy = HEIGHT * -2;
  for (k = 0; k < points[i]; k++) {
    if (x[i][k] < smallx) {
      smallx = x[i][k];
    }
    else if (x[i][k] > bigx) {
      bigx = x[i][k];
    }
    if (y[i][k] < smally) {
      smally = y[i][k];
    }
    else if (y[i][k] > bigy) {
      bigy = y[i][k];
    }
  }

  boxheight = bigy - smally;
  boxwidth =  bigx - smallx;
  centerx = (bigx + smallx) / 2;
  centery = (smally + bigy) / 2;

  if (WIDTH / boxwidth < HEIGHT / boxheight) {
    return WIDTH / boxwidth;
  } else {
    return HEIGHT / boxheight;
  }
}


//reads the file and attaches the correct variable to each input
void readobject(FILE *g, int i) {
  int k, j;

  fscanf(g, "%d", &points[i]);
  //printf("there are %d points\n", points[i]);
  for (k = 0; k < points[i]; k++) {
    fscanf(g, "%lf  %lf", &x[i][k], &y[i][k]);
    //printf("(%lf,%lf)\n", x[i][k], y[i][k]);
  }
  fscanf(g, "%d", &polys[i]);
  //printf("there are %d polygons in this whatever\n", polys[i]);

  for (k = 0; k < polys[i]; k++) {
    fscanf(g, "%d", &shapes[i][k]);
    for (j = 0; j < shapes[i][k]; j++) {
      fscanf(g, "%d", &shapeorder[i][k][j]);
      //printf("%d\n", shapeorder[i][k][j]);
    }
  }
  for (k = 0; k < polys[i]; k++) {
    fscanf(g, "%lf %lf %lf", &red[i][k], &green[i][k], &blue[i][k]); //the rgb
    //printf("(%lf, %lf, %lf)\n", red[i][k], green[i][k], blue[i][k]);
  }
}

//draws the image
void drawit(int i) {
  int k, j;
  double tempx[points[i]], tempy[points[i]];

  for (k = 0; k < polys[i]; k++) {
    for (j = 0; j < shapes[i][k]; j++) {
      tempx[j] = x[i][shapeorder[i][k][j]];
      tempy[j] = y[i][shapeorder[i][k][j]];
    }

    G_rgb(red[i][k], green[i][k], blue[i][k]);
    G_fill_polygon(tempx, tempy, j);
  }
}

//controls the movement of the object
void movement(int i, double sf, double m[3][3], double minv[3][3]) {

  D2d_translate(m, minv, -centerx, -centery);
  D2d_scale(m, minv, sf, sf);
  D2d_translate(m, minv, WIDTH / 2, HEIGHT / 2);
  D2d_mat_mult_points(x[i], y[i], m, x[i], y[i], points[i]);
}


//writes the opening instructions
void welcome(int i) {
  printf("Press");
  if (i == 1) {
    printf(" 1 to start,");
  } else if (i == 2) {
    printf(" 1 or 2 to start,");
  } else {
    printf(" 1 through %d to begin,", i);
  }
  printf("\nthen press TAB to begin clipping\n");
}

//the clipping container
void clipping(double *clipx, double *clipy, int i) {
  double c[100];
  int temp_i = clickAndSave(clipx, clipy);
  myPolygon(clipx, clipy, temp_i);
  f_intersect(clipx, clipy, temp_i, x[i], y[i], points[i], c);
  G_wait_key();

}



int main(int argc, char **argv) {
  char key, c;  FILE *g;
  double m[3][3], minv[3][3], sf, clipx[100], clipy[100];
  int cc;
  int inner_cycle = 1;

  for (cc = 1; cc < argc; cc++) {
    g = fopen(argv[cc], "r"); //opens a file; r = read only
    if (g == NULL) { //if the file is empty, it will let me know
      printf("can't open (1)\n");
      exit(1);
    } else {
      readobject(g, cc);

      D2d_make_identity(m); D2d_make_identity(minv);
      sf = boundingbox(cc); //Remember, sf == scale factor
      movement(cc, sf, m, minv); //(arg, scale, m, m inverse)
    }
  }

  welcome(argc - 1);
  scanf("%c", &key);
  int i = key - '0';

  if (i < argc && i > 0) {
    G_init_graphics(WIDTH, HEIGHT);

    D2d_make_identity(m); D2d_make_identity(minv);
    D2d_translate(m, minv, -WIDTH / 2, -HEIGHT / 2);
    D2d_rotate(m, minv, M_PI / 45);
    D2d_translate(m, minv, WIDTH / 2, HEIGHT / 2);

    while (inner_cycle) {
      G_rgb(1, 1, 1);
      G_clear();

      D2d_mat_mult_points(x[i], y[i], m, x[i], y[i], points[i]);

      drawit(i);

      G_rgb(0, 0, 0);
      G_fill_circle(WIDTH / 2, HEIGHT / 2, 3);

      c = G_wait_key();
      cc = c - '0';

      if (cc > 0 && cc < argc) {
        if (cc != i) {
          i = cc;
        }
      } else if (cc == -39) {

        inner_cycle = 0;

        clipping(clipx, clipy, i);
      } else {
        printf("can't open (2)\n");
        exit(1);
      }
    }
  }
}

