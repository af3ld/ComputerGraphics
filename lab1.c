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
  //printf("l:%.2lf, r:%.2lf, t:%.2lf, b:%.2lf\n", left, right, top, bottom);
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
    //printf("%lf, %lf\n", ca[0], ca[1]);
    if (ca[0] < 75 & ca[1] < 75) {
      going = 0;
      x[i] = x[0];
      y[i] = y[0];
      //printf("done\n");
    } else {
      x[i] = ca[0];
      y[i] = ca[1];
      G_circle(x[i], y[i], 2);
      i++;
      // printf(" %d\n", i);
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

void drawPolygonFill(double *x_intersect, int i, int total) {
  int x_pos1, x_pos2;
  for (x_pos1 = 0; x_pos1 < total; x_pos1 += 2) {
    x_pos2 = x_pos1 + 1;
    if (x_pos2 < total) {
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

//Points slope form; returns the new x coordinate
double find_x(double xknown, double yknown, double slope, double newy) {
  double newx = newy - yknown;
  newx = newx / slope;
  return newx + xknown;
}

//helper gets the loop back to 0
int looper(int i, int n) {
  if (i == n - 1) {
    return 0;
  } else {
    return i + 1;
  }
}

//returns the slope
double getslope(double xmin, double ymin, double xmax, double ymax) {
  return  (ymax - ymin) / (xmax - xmin);
}

//finds the largest value in array
int findlargest(double *y, int z) {
  int position = 0;
  double temp = 0;
  for (int i = 0; i < z; i++) {
    if (y[i] > temp) {
      position = i;
      temp = y[i];
    }
  }
  return position;
}


//finds the smallest in array
int findsmallest(double *y, int z) {
  int position = 0;
  double temp = HEIGHT;
  for (int i = 0; i < z; i++) {
    if (y[i] < temp) {
      position = i;
      temp = y[i];
    }
  }
  return position;
}



//fills the polygon
//currently not complete
void fillgon(double *x, double *y, int length) {
  double x_intersect[length * 2]; //Only l*2 intersections can exist
  double bounds[4]; //0=top; 1=bottom; 2=left; 3=right
  double newx[10000], newy[10000], slope;
  int i, xstart, xend, counter, ytop, ybottom, total;

  ytop = findlargest(y, length);
  ybottom = findsmallest(y, length);


  for (i = (int) y[ybottom] - 1;  i <= y[ytop]; i++) {
    counter = 0;
    for (xstart = 0; xstart < length; xstart++) {
      xend = looper(xstart, length);

      semgment_bounds(x[xstart], y[xstart], x[xend],
                      y[looper(xstart, length)], bounds);
      //printf("top: %.2lf, bottom: %.2lf, left: %.2lf, right: %.2lf\n", bounds[0], bounds[1], bounds[2], bounds[3]);

      if (i <= bounds[0] && i >= bounds[1]) {
        slope = getslope(x[xstart], y[xstart], x[xend], y[xend]);
        x_intersect[counter] = (i - y[xstart]) / slope + x[xstart];
        counter++;

      } else if (bounds[2] == bounds[3]) { //horizontal base case
        x_intersect[counter] = x[xstart];
        counter++;
        x_intersect[counter] = x[xend];
      }
    }
    total = counter;

    sort(x_intersect, total);
    drawPolygonFill(x_intersect, i, total);
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



  //bnom = clickAndSave(bx, by);
  //drawPolygon(bx,by,bnom);

  G_wait_key();

}
