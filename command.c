#include <FPT.h>
int main(int argc, char **argv){

  int i,k,numchars;
  numchars = 0;
  for (i=0; i< argc; i++){
    k = 0;
    while (argv[i][k] != '/0') {
    k++;
  }
  numchars += k;
  }

printf("the number of characters is: %d\n", numchars);

}