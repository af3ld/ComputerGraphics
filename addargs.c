#include <stdio.h>


int error ;


int convert(char *q)
{
  int i,v ;

  error = 0 ;
  i = v = 0 ;
  while (q[i] != '\0') {
    if ((q[i] >= '0') && (q[i] <= '9')) {
      v = v*10 + (q[i] - '0') ;
    }
    else {
      error = 1 ; return 0 ;
    }

    i++ ;
  }

  return v ;
}




int main (int argc, char **argv)
{

  int a,b,c ;
 
  if (argc != 3) {
      printf("usage :pgm_name  unsigned_integer1  unsigned_integer2\n") ;
      exit(1) ;
  }


  a = convert(argv[1]) ;
  if (error) {
        printf("%s is not an unsigned integer\n", argv[1]) ;
        exit(1) ;
  }


  b = convert(argv[2]) ;
  if (error) {
        printf("%s is not an unsigned integer\n", argv[2]) ;
        exit(1) ;
  }

  c = a + b ;

  printf("%d + %d = %d\n",a,b,c) ;

}


