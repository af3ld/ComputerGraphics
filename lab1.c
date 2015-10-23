#include <FPT.h>

int HEIGHT = 600; int WIDTH = 600;

void container(double x1, double y1, double x2, double y2, double *direction){
  double left, top, right, bottom;
    if(x1 <= x2){
        left = x1;
        right = x2;
    } else {
        left = x2;
        right = x1;
    }
    if(y1 >= y2){
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
int clickAndSave(double x[100], double y[100]){
  double ca[2];
  int i = 0;
  int going = 1;
  while (going){
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
void sort(double *x, double *y, int m){
  int k, s, i;
  double tempx, tempy;
  for (i = 0; i < m; i++){
    s = i;
    for (k = i + 1; k < m; k++){
      if (x[k] < x[s]){
	s = k;
      }
      tempx = x[i];
      tempy = y[i];
      x[i] = x[s];
      y[i] = y[s];
      x[s] = tempx;
      y[s] = tempy;
    }
  }
}


//prints the x and y coordinates of a point
void printarray(double *x, double *y, int z){
  int i;
  for (i = 0; i < z; i++){
    printf("x%d: %.2lf, y%d: %.2lf\n", i, x[i],i, y[i]);
  }
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

//helper to get back to 0;
int looper(int i, int z){
  if (i == z - 1){
    return 0;
  } else {
    return i + 1;
  }
}


//fills the polygon
//currently not complete
void fillgon(double *x, double *y, int z, double *slope){
  double tempx, direction[4]; //0=top; 1=bottom; 2=left; 3=right
  double newx[10000], newy[10000];
  int i, j, k, counter;

  for (i = 0; i < z; i++){
    container(x[i],y[i], x[looper(i, z)], y[looper(i, z)], direction);
    
    for (j = (int) direction[1]; j <= direction[0]; j++){
      //G_rgb(1.0/i, .01/i, 1.0/i);
      tempx = find_x(x[i], y[i], slope[i], j);
      if (tempx < direction[3] && tempx > direction[2]){
	newx[i*j] = tempx;
	newy[i*j] = j;
	//printf("x%d: %.2lf\n",i, newx[i*j]);
	G_circle(newx[i*j], j, 2);
      }
    }
    //G_wait_key();
  }
  k = i * j;
  sort(newy, newx, k);
  for (i = 0; i < k; i++){
    if (newx[i] > 0 && newy > 0){
      printf("%d\n", i);
      G_line(newx[i], newy[i], newx[i +1], newy[i +1]);
    }
  }
  
  printarray(newx, newy, k);

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
