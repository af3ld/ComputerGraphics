#include <FPT.h>

int HEIGHT = 600; int WIDTH = 600;

void semgment_bounds(double x1, double y1,
                     double x2, double y2, double *direction) {
  double left, top, right, bottom;
  if (x1 <= x2) {
    left = x1;
    right = x2;
  } else {
    left = x2;
    right = x1;
  }
  if (y1 >= y2) {
    top = y1;
    bottom = y2;
  } else {
    top = y2;
    bottom = y1;
  }
  direction[0] = top; direction[1] = bottom;
  direction[2] = left; direction[3] = right;
}

//click a point and it saves it into an array
int clickAndSave(double x[100], double y[100]) {
  double ca[2];
  int i = 0;
  int going = 1;
  while (going) {
    G_wait_click(ca);
    if (ca[0] < 75 & ca[1] < 75) {
      going = 0;
      x[i] = x[0];
      y[i] = y[0];
    } else {
      x[i] = ca[0];
      y[i] = ca[1];
      G_circle(x[i], y[i], 2);
      i++;
    }
  }
  return i;
}

//draws the polygon; unfilled
void drawPolygon(double x[100], double y[100], int n) {
  int i = 0;
  while (i <= n) {
    if (i != n) {
      G_line(x[i], y[i], x[i + 1], y[i + 1]);
    }
    i++;
  }
}

//draws the fill in the polygon
void drawPolygonFill(double *x_intersect, int i, int total) {
  int x_pos1, x_pos2;
  for (x_pos1 = 0; x_pos1 < total; x_pos1 += 2) {
    x_pos2 = x_pos1 + 1;
    if (x_pos2 < total) {
      G_wait_key();
      G_line(x_intersect[x_pos1], i, x_intersect[x_pos2], i);
    }
  }

}

//sorts the values of an array
void sort(double *x, int m) {
  int k, s, i;
  double tempx, tempy;
  for (i = 0; i < m; i++) {
    s = i;
    for (k = i + 1; k < m; k++) {
      if (x[k] < x[s]) {
        s = k;
      }
      tempx = x[i];
      x[i] = x[s];
      x[s] = tempx;
    }
  }
}

//prints the x and y coordinates of a point
void printarray(double *x, double y, int z) {
  int i;
  for (i = 0; i < z; i++) {
    printf("x%d: %.2lf, y%d: %.2lf\n", i, x[i], i, y);
  }
}

//helper gets the loop back to 0
int looper(int i, int n) {
  if (i == n - 1) {
    return 0;
  } else {
    return i + 1;
  }
}

//finds the largest/smallest value in array
int findextrema(double *y, int z, int swatch) {
  int position = 0;
  double temp = 0 + (swatch * HEIGHT);
  for (int i = 0; i < z; i++) {
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

//fills the polygon by iterating from the bottom to the top of the polygon
void fillgon(double *x, double *y, int length) {
  double x_intersect[length * 2]; //Only l*2 intersections can exist
  double bounds[4]; //0=top; 1=bottom; 2=left; 3=right
  double newx[10000], newy[10000], slope;
  int i, xstart, xend, counter, ytop, ybottom;

  ytop = findextrema(y, length, 0); //0 returns largest
  ybottom = findextrema(y, length, 1); //1 returns smallest

  for (i = (int) y[ybottom] - 1;  i <= y[ytop] + 1; i++) {
    counter = 0;
    for (xstart = 0; xstart < length; xstart++) {
      xend = looper(xstart, length);

      semgment_bounds(x[xstart], y[xstart], x[xend],
                      y[looper(xstart, length)], bounds);
      //returns the north, south, east, west points of the line segment

      if (i <= bounds[0] && i >= bounds[1]) {
        if (bounds[2] == bounds[3]) { //horizontal base case
          x_intersect[counter] = x[xstart];
          counter++;
          x_intersect[counter] = x[xend];
        } else {
          slope = (y[xend] - y[xstart]) / (x[xend] - x[xstart]);
          x_intersect[counter] = (i - y[xstart]) / slope + x[xstart];
          counter++;
        }
      }
    }

    sort(x_intersect, counter);
    drawPolygonFill(x_intersect, i, counter);
  }
}




int main() {
  double ax[100], ay[100], bx[100], by[100];
  int anom, bnom, i;


  G_init_graphics(WIDTH, HEIGHT);
  G_rgb(0, 0, 0);
  G_clear();

  G_rgb(1, 1, 1);
  G_fill_rectangle(0, 0, 77, 77);
  G_rgb(.2, .6, .82);
  G_fill_rectangle(0, 0, 75, 75);
  G_rgb(1, 1, 1);

  anom = clickAndSave(ax, ay);
  drawPolygon(ax, ay, anom);
  fillgon(ax, ay, anom);


  G_rgb(1, 0, .4);
  bnom = clickAndSave(bx, by);
  drawPolygon(bx, by, bnom);
  fillgon(bx, by, bnom);

  G_wait_key();

}
