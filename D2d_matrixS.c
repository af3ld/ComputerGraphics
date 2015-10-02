#include <D2d_matrix.h>



/*

 ( x')          (x)
 ( y')  =   M * (y)  
 ( 1 )          (1)

instead of (x',y',1) = (x,y,1) * M  

*/



int D2d_print_mat (double a[3][3])
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           printf(" %12.4lf ",a[r][c]) ;
      }
      printf("\n") ;
  }

  return 1 ;
} 




int D2d_copy_mat (double a[3][3], double b[3][3])
// a = b
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           a[r][c] = b[r][c] ;
      }
  }

  return 1 ;
} 

int D2d_mat_mult(double res[3][3], double a[3][3], double b[3][3]){
  int i, j, k;
  double temp1, temp2, temp3[3][3];
 
  for (i =0; i < 3; i++){
    for (j = 0; j < 3; j++){
      temp1 = temp2 = 0;
      for (k = 0; k < 3; k++){
      temp1 = a[i][k] * b[k][j];
      temp2 += temp1;
      
      }
    temp3[i][j] = temp2;
    }
  }

  for(i = 0; i < 3; i++){
    for(j= 0; j < 3; j++){
      res[i][j] = temp3[i][j];
    }
  }
  return 1;
}



int D2d_make_identity (double a[3][3])
// a = I
{
  int r,c ;
  for (r = 0 ; r < 3 ; r++ ) {
      for (c = 0 ; c < 3 ; c++ ) {
           if (r == c) a[r][c] = 1.0 ;
               else    a[r][c] = 0.0 ;
      }
  }
    return 1 ;
} 






/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


int D2d_translate (double a[3][3], double b[3][3], double dx, double dy)
// a = translation*a  
// b = b*translation_inverse  
{
  double t[3][3] ;

  D2d_make_identity(t) ;

  t[0][2] =  dx ;  t[1][2] = dy ;  
  D2d_mat_mult(a,  t,a) ;


  t[0][2] = -dx ;  t[1][2] = -dy ;
  D2d_mat_mult(b,  b,t) ;

  return 1 ;
}

int D2d_scale(double a[3][3], double b[3][3], double sx, double sy){

  double t[3][3];

  D2d_make_identity(t);

  t[0][0] = sx; t[1][1] = sy;
  D2d_mat_mult(a,  t,a);

  t[0][0] = 1/sx; t[1][1] = 1/sy;
  D2d_mat_mult(b,  b,t);
  
  return 1;
}


int D2d_rotate(double a[3][3], double b[3][3], double rads){

  double t[3][3] = {  
    {cos(rads), -sin(rads), 0} , 
    {sin(rads), cos(rads), 0},
    {0,0,1}
  };

  D2d_mat_mult(a,  t,a);

  double q[3][3] =  { 
    {cos(rads), sin(rads), 0} , 
    {-sin(rads), cos(rads), 0},
    {0,0,1}
  };
  
  D2d_mat_mult(b,  b,q);
  

  return 1;
}

int D2d_negate_x(double a[3][3], double b[3][3]){

  double t[3][3];
  D2d_make_identity(t);
  t[0][0] = -1;

  D2d_mat_mult(a, t,a);
  D2d_mat_mult(b, b,t);

  return 1;
  
}

int D2d_negate_y(double a[3][3], double b[3][3]){

  double t[3][3];
  D2d_make_identity(t);
  t[1][1] = -1;

  D2d_mat_mult(a, t,a);
  D2d_mat_mult(b, b,t);

  return 1;
  
}


int D2d_mat_mult_points (double *X, double *Y,
                         double m[3][3],
                         double *x, double *y, int numpoints) {
  int i, j, k;
  double tempX[numpoints], tempY[numpoints];
  
  for (i = 0; i < 3; i++){
    for (j = 0; j < numpoints; j++){
      if (i == 0){
	tempX[j] = m[i][0] * x[j] + m[i][1] * y[j]+ m[i][2];  
	} else if (i == 1){
	tempY[j] = m[i][0] * x[j] + m[i][1] * y[j]+ m[i][2];
      }
    }
  }

  for (i = 0; i < numpoints; i++){
    X[i] = tempX[i];
    Y[i] = tempY[i];
  }

  return 1;
}
