
// read the data from multiple xy files and display all of the objects

#include <FPT.h>
#include <D3d_matrix.h>
     // problems if forget to include D3d_matrix.h





int main (int argc, char **argv)
{

  // lots of stuff here 

  //  Half_angle_degrees ... I think you have 40 degrees as the default
  h = 0 ; // h marks the current object
  sign = 1 ;
  action = 't' ;

  while (1) {

    D3d_make_identity (V) ;
    D3d_make_identity (Vi) ;

    q = G_wait_key() ;
    
    if (q == 'q') {
      exit(0) ;

    } else if (q == 'c') {
      sign = -sign ;

    } else if (q == 't') {
      action = q ;

    } else if (q == 'S') {
      action = q ;

    } else if (q == 'r') {
      action = q ;

    } else if (('0' <= q) && (q <= '9')) {
      k = q - '0' ;  if (h < numobjects) h = k ;

    } else if ((q == 'x') && (action == 't')) {

    } else if ((q == 'y') && (action == 't')) {

    } else if ((q == 'z') && (action == 't')) {

    } else if ((q == 'x') && (action == 'r')) {

    } else if ((q == 'y') && (action == 'r')) {

    } else if ((q == 'z') && (action == 'r')) {

    } else {
      printf("no action\n") ;
    }


    D3d_mat_mult_points (x[h],y[h],z[h],  V,  x[h],y[h],z[h],numpoints[h]+1) ;
      // the numpoints[h]+1 is because we have stored the center of the object
      // at the arrays' end

    G_rgb(0,0,0) ; 
    G_clear() ;
    G_rgb(0,0,1) ;
    draw_all_objects() ;

  }


}

