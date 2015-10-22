
#include <FPT.h>


//Points slope form; returns the new x coordinate
double f_intersect(double x1, double y1, double x2, double y2
		   , double *c){
  //(y1 – y2)x + (x2 – x1)y + (x1*y2 – x2*y1) = 0
  //http://math.stackexchange.com/questions/388873/to-find-the-x-and-y-intercepts-of-the-line-axbyc-0
  
  double yprime, xprime;
  yprime = xprime = ((x1*y2) - (x2*y1)) * -1;
  xprime = xprime / (y1 - y2);
  yprime = yprime / (x2 - x1);
  printf("(x',y'): (%lf,%lf)", xprime, yprime);

  G_rgb(0,0,0);
  G_fill_circle(xprime, yprime, 2);
  return 3.00;
  
}


int Clip_Polygon_Against_Convex_Window(double *px, double *py, int pn,
					double *wx, double *wy, int wn)
{

  double coords[2];
  f_intersect(px[1], py[1], wx[1], wy[1], coords);

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




