#include <FPT.h>
int HEIGHT = 600; int WIDTH = 600;


//click a point and it saves it into an array
int clickAndSave(double x[100], double y[100]){
  double ca[2];
  int i = 0;
  int going = 1;
  while (going == 1){
    G_wait_click(ca);
    //printf("%lf, %lf\n", ca[0], ca[1]);
    if (ca[0] < 75 & ca[1] < 75){
      going = 0;
      x[i] = x[0];
      y[i] = y[0];
      //printf("done\n");
    }else{ 
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
void myPolygon(double x[100], double y[100], int n){
  int i = 0;
  while (i <= n){
    if (i != n){
      G_line(x[i], y[i], x[i+1], y[i+1]);
    } 
    i++;
  }
  }


//sorts the values of an array
void sort(double *x, int m){
  int k, s, i;
  double temp;
  for (i = 0; i < m; i++){
    s = i;
    for (k = i + 1; k < m; k++){
      if (x[k] < x[s]){
	s = k;
      }
      temp = x[i];
      x[i] = x[s];
      x[s] = temp;
    }
  }
}


//prints the x and y coordinates of a point
void printarray(double *w, int z){
  int i;
  for (i = 0; i < z; i++){
    printf("x%d: %lf\n", i, w[i]);
  }
}

double findlargest_y(double *y, int z){
  double tempy = 0;
  for(int i = 0; i < z; i++){
    if (y[i] > tempy){
      tempy = y[i];
    }
  }
  return tempy;
}


double getslope(double x1, double y1, double x2, double y2){
    return (y2 - y1) / (x2 - x1);
}

<<<<<<< HEAD
void fillgon(double *x, double *y, int z){
  double newx, newy;
  double largesty = findlargest_y(y, z);
  
  double m = getslope(x[0], y[0], x[1], y[1]);
  printf("%lf\n", m);
  //printf("x:%lf, y:%lf, x:% lf, y:%lf\n", x[0], y[0], x[1], y[1]);
  newx = (200 - 300) /m - x[0];
  printf("%lf", newx);
  
=======
//takes the x's and y's and returns an array of the slopes
int getslope(double *x, double *y, int z, double *slope){
  int i;
  for (i = 0; i < z; i++){
    slope[i] = (y[i+1] - y[i]) / (x[i + 1] - x[i]);
    //printf("(%lf,%lf) to (%lf,%lf) : %lf\n", x[i], y[i], x[i+1], y[i+1], slope[i]);
  }
  slope[i] = (y[0] - y[i]) / (x[0] - x[i]);
  return  i;
>>>>>>> 5a428e3f44fd862591f342ef4b407a97d92853b1
}







 
int main()
{
  double ax[100], ay[100], bx[100], by[100];
<<<<<<< HEAD
  double slope;
=======
  double slope[100];
>>>>>>> 5a428e3f44fd862591f342ef4b407a97d92853b1
  int anom, bnom, i;


  G_init_graphics(WIDTH, HEIGHT);
  G_rgb(0,0,0);
  G_clear();
  
  G_rgb(1,1,1);
  G_fill_rectangle(0,0,77,77);
  G_rgb(.2,.6,.82);
  G_fill_rectangle(0,0,75,75);
  G_rgb(1,1,1);

  anom = clickAndSave(ax, ay);
  myPolygon(ax,ay,anom); 
  fillgon(ax,ay, anom);

 



  //bnom = clickAndSave(bx, by);
  //myPolygon(bx,by,bnom);

  G_wait_key();

}
