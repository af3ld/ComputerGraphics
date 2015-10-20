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

//finds the largest value in array
double findlargest(double *y, int z){
  int position = 0;
  double temp = 0;
  for(int i = 0; i < z; i++){
    if (y[i] > temp){
       position = i;
       temp = y[i];
    }
  }
  return position;
}


//finds the smallest in array
double findsmallest(double *y, int z){
  int position = 0;
  double temp = HEIGHT;
  for(int i = 0; i < z; i++){
    if (y[i] < temp){
       position = i;
       temp = y[i];
    }
  }
  return position;
}


//Points slope form; returns the new x coordinate
double find_x(double xknown, double yknown, double slope, double newy){
  double newx = newy - yknown;
  newx = newx / slope;
  return newx + xknown;
}

//gets the slope of all the lines; returns them via modified array
void getslope(double *x, double *y, int z, double *slope){
  int i;
  for (i = 0; i < z; i++){
    slope[i] = (y[i+1] - y[i]) / (x[i + 1] - x[i]);
  }
  slope[i] = (y[0] - y[i]) / (x[0] - x[i]);
}


//fills the polygon
//currently not complete
void fillgon(double *x, double *y, int z, double *slope){
  double xintersect[100], yintersect[100];
  int i, j;
  int largest_y_pos = findlargest(y, z); int smallest_y_pos = findsmallest(y,z);
  
  for (j = (int) y[smallest_y_pos] - 1; j < y[largest_y_pos]; j++){
  for (i = 0; i < z; i++){
    double newx = find_x(x[i], y[i], slope[i], j);
    printf("%lf\n", newx);
    G_circle(newx, j, 3);
    }
  printf("\n");
  }
  
  //for(int i = (int) y[smallest_y_pos] - 1; i < y[largest_y_pos]; i++){
    //double start_x = find_x(x[0], y[0], slope[0], i);
  //G_circle(start_x, i, 1);
  //double end_x = find_x(x[2], y[2], slope[1], i);
  //printf("start:%lf, end: %lf\n", start_x, end_x);
  //G_line(start_x, i, end_x, i);  
  //} 
}



 
int main()
{
  double ax[100], ay[100], bx[100], by[100];

  double slope[100];
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
  getslope(ax, ay, anom ,slope);
  fillgon(ax,ay, anom, slope);



  //bnom = clickAndSave(bx, by);
  //myPolygon(bx,by,bnom);

  G_wait_key();

}
