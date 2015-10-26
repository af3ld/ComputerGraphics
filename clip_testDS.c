#include <FPT.h>
int counter;

// Bounding Box For Line Segment
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

  counter = 0;
  for (i = 0; i < wn; i++) {
    for (j = 0; j < pn; j++) {
      c1 = py[j] - pslope[j] * px[j]; //y = mx + c -> c = y -mx
      c2 = wy[i] - wslope[i] * wx[i];

      if ((pslope[j] - wslope[i]) == 0) { //prevents divison by 0
        printf("Slope division = 0; ERROR!\n");
      } else {
        xprime = (c2 - c1) / (pslope[j] - wslope[i]); //intersection of x
        yprime = pslope[j] * xprime + c1; //intersection of y

        if (pointincontainer(px[j], py[j], px[looper(j, pn)], py[looper(j, pn)],
                             xprime, yprime) == 1 &&
            pointincontainer(wx[i], wy[i], wx[looper(i, wn)], wy[looper(i, wn)],
                             xprime, yprime ) == 1) {
          c[counter] = xprime; c[counter + 1] = yprime;
          counter += 2;
          G_rgb(1, 1, 1);
          G_fill_circle(xprime, yprime, 2);
        }
      }
    }
  }
}


int Clip_Polygon_Against_Convex_Window(double *px, double *py, int pn,
                                       double *wx, double *wy, int wn) {
  int j, i;
  double cd[100];

  f_intersect(px, py, pn, wx, wy, wn, cd);
  double cleaned_coords[counter + 1];
  
  for (i = 0; i < 100; i += 2) {
    if (cd[i] > 0.1 && cd[i] < 700 && 0 == isnan(cd[i]) 
      && cd[i + 1] > 0.1 && cd[i + 1] < 700 && isnan(cd[i + 1]) == 0) {
      cleaned_coords[i] = cd[i]; 
      cleaned_coords[i + 1] = cd[i + 1];
    }
  }

  return 1;
}


int main()
// this tests clipping of polygon to convex window
//prefix -p == polygon; prefix -w == window
{
  int pn, wn ;
  double pt[2], u, v, q ;

  double px[100] = {  70, 460, 400} ;
  double py[100] = { 350, 25, 550} ;
  pn = 3 ;

  double wx[100] = { 100, 600, 550, 150} ;
  double wy[100] = { 150, 200, 450, 500} ;
  wn = 4 ;

  srand48(100) ;

  G_init_graphics (700, 700) ;
  G_rgb (0, 0, 0) ;
  G_clear() ;

  G_rgb (1, 0, 0) ;
  G_polygon(wx, wy, wn) ;

  G_rgb (0, 0, 1) ;
  G_polygon(px, py, pn) ;


  q = G_wait_key() ;


  pn = Clip_Polygon_Against_Convex_Window (px, py, pn, wx, wy, wn) ;

  G_rgb (1, 1, 0) ;
  G_fill_polygon(px, py, pn) ;
  q = G_wait_key() ;
}




