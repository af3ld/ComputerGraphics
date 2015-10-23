
#include <FPT.h>
  //http://www.softwareandfinance.com/Turbo_C/Intersection_Two_lines_EndPoints.html
// Bounding Box For Line Segment
int pointincontainer(double x1, double y1, double x2, double y2,
			 double point_x, double point_y){
    double left, top, right, bottom; 
    
    if(x1 < x2){
        left = x1;
        right = x2;
    } else {
        left = x2;
        right = x1;
    }
    if(y1 < y2){
        top = y1;
        bottom = y2;
    } else {
        top = y1;
        bottom = y2;
    }
    if ((point_x+0.01) >= left && (point_x-0.01) <= right && 
	(point_y+0.01) >= top && (point_y-0.01) <= bottom) {
      
        return 1;
    } else {
        return 0;
    }
}


//gets the slope of all the lines; returns them via modified array
void getslope(double *x, double *y, int z, double *slope){
  int i;
  for (i = 0; i < z; i++){
    slope[i] = (y[i+1] - y[i]) / (x[i + 1] - x[i]);
  }
  slope[i] = (y[0] - y[i]) / (x[0] - x[i]);
}


int f_intersect(double *px, double *py,
		 double *wx, double *wy,
		 double *pslope, double *wslope, double *c){

  
  double yprime, xprime;
  
  double c1 = py - pslope * px; //y = mx + c
  double c2 = wy - wslope * wx;
  
  if ((pslope - wslope) == 0){
        printf("Slope division = 0; ERROR!\n");
	return 0;
	
  } else {
    xprime = (c2 - c1) / (pslope - wslope);
    yprime = pslope * xprime + c1;
    
    if(pointincontainer(px, py, x, y, xprime, yprime ) == 1 &&
       pointincontainer(wx, wy, x, y, xprime, yprime ) == 1){
      c[0] = xprime; c[1] = yprime;
      return 1;
    } else {
      return 0:
    }
  }
}


int Clip_Polygon_Against_Convex_Window(double *px, double *py, int pn,
				       double *wx, double *wy, int wn){
  int j, i;
  double cd[2], wslope[100], pslope[100];
  getslope(px, py, pn, pslope); 
  getslope(wx, wy, wn, wslope);

  //for(i = 0; i <= pn; i++){
    //for(j = 0; j <= wn; j++){
  f_intersect(px, py, wx, wy, pslope, wslope, cd);
  G_rgb(1,1,1);
  G_fill_circle(cd[0], cd[1], 2);
  //}
  //}
  return 1;
}


int main()
// this tests clipping of polygon to convex window
//prefix -p == polygon; prefix -w == window
{
  int pn, wn ;
  double pt[2],u,v,q ;

  double px[100] = {  70,460,400} ;
  double py[100] = { 350, 25,550} ;
  pn = 3 ;

  double wx[100] = { 100,600,550,150} ;
  double wy[100] = { 150,200,450,500} ;
  wn = 4 ;

  srand48(100) ;

  G_init_graphics (700, 700) ;
  G_rgb (0,0,0) ;
  G_clear() ;

  G_rgb (1,0,0) ;
  G_polygon(wx,wy,wn) ;

  G_rgb (0,0,1) ;
  G_polygon(px,py,pn) ;


  q = G_wait_key() ;


  pn = Clip_Polygon_Against_Convex_Window (px, py, pn, wx, wy, wn) ;

  G_rgb (1,1,0) ;
  G_fill_polygon(px,py,pn) ;
  q = G_wait_key() ;
}




