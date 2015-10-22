
#include <FPT.h>
  //http://www.softwareandfinance.com/Turbo_C/Intersection_Two_lines_EndPoints.html



void f_intersect(double px_start, double py_start,
		 double px_end, double py_end,
		 double wx_start, double wy_start,
		 double wx_end, double wy_end, double *c){

  
  double yprime, xprime;
  
  double pslope = (py_start - py_end) / (px_start - px_end);
  double wslope = (wy_start - wy_end) / (wx_start - wx_end);
  double c1 = py_end - pslope * px_end;
  double c2 = wy_end - wslope * wx_end;
  
  if( (pslope - wslope) == 0){
        printf("No Intersection between the lines\n");
  } else {
    xprime = (c2 - c1) / (pslope - wslope);
    yprime = pslope * xprime + c1;
    c[0] = xprime; c[1] = yprime;
  }
}


int Clip_Polygon_Against_Convex_Window(double *px, double *py, int pn,
					double *wx, double *wy, int wn)
{
  int j;

  double coord[2];
  for (int i = 0; i < pn - 1; i++){
    for (j = 0; j < wn - 1; j++){
      f_intersect(px[i], py[i], px[i + 1], py[i + 1], wx[j]
		  , wy[j], wx[j + 1], wy[j + 1], coord);
      if (coord[0] > 1 && coord[1] > 1 && coord[0] < 700 && coord[1] < 700){
	printf("Intersection: = (%lf, %lf)\n", coord[0], coord[1]);
	G_rgb(1,1,1);
	G_fill_circle(coord[0], coord[1], 2);
	G_wait_key();
      }
    }
  }
  
  
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




