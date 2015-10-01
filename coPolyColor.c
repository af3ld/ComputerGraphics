#include <FPT.h>
int main(){

  int numCo, t1, t2, t3, t4, numPoly, c[100], tp[100];
  int scalar, t5, t6;
  double b[100], rgb[100];

  scanf("%d", &numCo);
  t1 = numCo * 2;
  t2 = 0;
  scalar = 50;
  while (t1 > 0){
    scanf("%lf", &b[t2]);
    b[t2] = (b[t2]* scalar);
    // printf("%lf\n", b[t2]);
    t2++;
    t1--;
  }
  //b has the coordinates

  scanf("%d", &numPoly);
  //printf("%d\n", numPoly);
  t1 = numPoly;
  t3 = 0;
  t4 = 0;
  while (t1 > 0){
    scanf("%d", &tp[t4]);
    // printf("%d", tp[t4]);
    t2 = tp[t4];
    t4++;
    while(t2 > 0){
      scanf("%d", &c[t3]);
      // printf("%d\n", c[t3]);
      t3++;
      t2--;
    }
    t1--;
  }
  //c has the polygon corners
  //tp has the number of coordinates per polygon

  t1 = numPoly * 3;
  t2 = 0;
  while (t1 > 0){
    scanf("%lf", &rgb[t2]);
    // printf("%lf", rgb[t2]);
    t2++;
    t1--;
  }
  //rgb has the colors


  G_init_graphics(600,600);
  G_rgb(1,1,1);
  G_clear();

  t2 = 0;
  t3 = 0;
  t5 = numPoly;
  t6 = 0;

  double x[numPoly], y[numPoly];

  while (t5 > 0){
    t1 = tp[t2];
    //printf("%d\n", t1);
    while (t1 > 0){
      y[t1] = b[(c[t3] * 2)+ 1];
      x[t1] = b[c[t3]*2];
      //printf(" %d", c[t3]);
      printf("x: %lf, y: %lf\n", x[t1], y[t1]);
      t3++;
      t1--;
    }
    //printf("\n");
    G_rgb(rgb[t6], rgb[t6 + 1], rgb[t6 + 2]);

    G_fill_polygon(x,y, tp[t2]);
    t5--;
    t2++;
    t6 = t6 + 3;

  }

  G_wait_key();

}
