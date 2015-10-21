
#include <FPT.h>


int Clip_Polygon_Against_Convex_Window(double px, double py, int pn,
					double wx, double xy, int wn){
  if(x2-x1){
    m=(y2-y1)/(x2-x1);
  } else {
    m=100000;
  }
  if(x1 >= xmin && x2 >= xmin){
    arr[k]=x2;
    arr[k+1]=y2;
    k+=2;
  }
  if(x1 < xmin && x2 >= xmin){
    
    arr[k]=xmin;
    arr[k+1]=y1+m*(xmin-x1);
    arr[k+2]=x2;
    arr[k+3]=y2;
    k+=4;
  }
  if(x1 >= xmin  && x2 < xmin){

    arr[k]=xmin;
    arr[k+1]=y1+m*(xmin-x1);
    k+=2;
  }
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


  pn = Clip_Polygon_Against_Convex_Window (px, py, pn,
					    wx, wy, wn) ;  

  G_rgb (1,1,0) ;
  G_fill_polygon(px,py,pn) ;
  q = G_wait_key() ;
}




