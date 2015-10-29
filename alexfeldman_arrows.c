/*

1. Download the following :

     sftp  student@simpson1
     mget  arrows3.exe
     mget  arrows3student.c   (which is an electronic version of this file)
     mget  D2d_matrix.h
     mget  D2d_matrixJeff.c  (if you don't trust your own)
     quit


2. Run the downloaded executable, via :

     ./arrows3.exe

Press and hold the spacebar to see how the program behaves.


3. Your task is to enhance this file, using the matrix tools in
D2d_matrix, with an eye to duplicating the behavior of the executable,
arrows3.exe.  For full credit, aim for run-time efficiency
by judicious use of the matrix tools.


4. In order for Jeff to grade this, before you start working, close
this file (if you are in emacs) and rename it from the command line
to a name that reflects your name :

   mv arrows3student.c     yournameARROWS.c

Make sure the ARROWS is captialized.

5. To compile your code :

   acom -I.  D2d_matrixJeff.c   yournameARROWS.c

(assuming you use Jeff's D2d_matrix file)

*/

#include <FPT.h>
#include <D2d_matrix.h>
int HEIGHT = 700; int WIDTH = 700;

//arrow
double ax[4] = {0,  -20,  20, -20} ;
double ay[4] = {0,  -15,   0,  15} ;


void copy_array(double *a, double *b, int size) {
  int i;
  for (i = 0; i < size; i++) {
    b[i] = a[i];
  }
}



void duplicate_n_arrange(double mat[3][3], double inv[3][3]) {
  int i, j;
  double bx[4]; double by[4];
  double radians = ((360 / 13) * M_PI / 180);
  copy_array(ax, bx, 4);
  copy_array(ay, by, 4);
  D2d_make_identity(mat); D2d_make_identity(inv);
  D2d_translate(mat, inv, WIDTH / -2, HEIGHT / -2);
  D2d_rotate(mat, inv, radians);
  D2d_translate(mat, inv, WIDTH / 2, HEIGHT / 2);
  for (i = 0; i < 13; i++) {
    D2d_mat_mult_points(bx, by,  mat, bx, by, 4) ;
    G_fill_polygon(bx, by, 4);
  }
}

//moves the arrow away from the origin
//and controls the scaling; Currently it increases by 10%
void move_n_scale(double scalefactor, double mat[3][3], double inv[3][3]) {
  D2d_make_identity(mat); D2d_make_identity(inv);
  D2d_translate(mat, inv, -WIDTH / 2, -HEIGHT / 2);
  D2d_scale(mat, inv, scalefactor, scalefactor);
  D2d_translate(mat, inv, WIDTH / 2, HEIGHT / 2);
  D2d_translate(mat, inv, 10, 0);
}


int main()
{
  double mat[3][3], inv[3][3], radians ;
  double r = 0;
  double b = 1;
  int q, spacebar; int running = 1;

  G_init_graphics(WIDTH, HEIGHT) ; //WIDTH x HEIGHT = 700x700

  D2d_make_identity(mat); D2d_make_identity(inv);
  D2d_translate(mat, inv, WIDTH /2, HEIGHT /2);
  D2d_mat_mult_points(ax, ay,  mat, ax, ay, 4) ;


  while (running) { //loops the program
    G_rgb(0, 0, 0) ;
    G_clear() ;
    G_rgb(1, 1, 1) ;
    G_draw_string("Press and hold the spacebar.", 10, 10) ;
    G_rgb(r, 0, b);

    duplicate_n_arrange(mat, inv);
    move_n_scale(1.08, mat, inv);

    spacebar = G_wait_key();
    while (spacebar - '0' == -16) { //ascii char value for spacebar
      D2d_mat_mult_points(ax, ay,  mat, ax, ay, 4) ;
      r += .03;
      b -= .03;
      spacebar = -1;
    }
    if (spacebar == 'q' || ax[3] > WIDTH + 20) {
      // running = 0;
      exit(1);
    }
  }
}
