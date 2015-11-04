#include <D3d_matrix.h>




int D3d_print_mat (double a[4][4]) {
	int r, c ;
	for (r = 0 ; r < 4 ; r++ ) {
		for (c = 0 ; c < 4 ; c++ ) {
			printf(" %12.4lf ", a[r][c]) ;
		}
		printf("\n") ;
	}

	return 1 ;
}

int D2d_copy_mat (double a[4][4], double b[4][4]) {
	int r, c ;
	for (r = 0 ; r < 4 ; r++ ) {
		for (c = 0 ; c < 4 ; c++ ) {
			a[r][c] = b[r][c] ;
		}
	}

	return 1 ;
}

int D2d_mat_mult(double res[4][4], double a[4][4], double b[4][4]) {
	int i, j, k;
	double temp1, temp2, temp3[4][4];

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			temp1 = temp2 = 0;
			for (k = 0; k < 4; k++) {
				temp1 = a[i][k] * b[k][j];
				temp2 += temp1;

			}
			temp3[i][j] = temp2;
		}
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			res[i][j] = temp3[i][j];
		}
	}
	return 1;
}

int D2d_make_identity (double a[4][4]) {
// a = I
	int r, c ;
	for (r = 0 ; r < 4 ; r++ ) {
		for (c = 0 ; c < 4 ; c++ ) {
			if (r == c) a[r][c] = 1.0 ;
			else    a[r][c] = 0.0 ;
		}
	}
	return 1 ;
}

int D2d_translate (double a[4][4], double b[4][4], double dx, double dy, double dz) {
// a = translation*a
// b = b*translation_inverse
	double t[4][4] ;

	D3d_make_identity(t) ;

	t[0][3] =  dx ;  t[1][3] = dy ;  t[2][3] = dz;
	D3d_mat_mult(a,  t, a) ;


	t[0][2] = -dx ;  t[1][2] = -dy ;
	D3d_mat_mult(b,  b, t) ;

	return 1 ;
}

int D3d_scale (double a[4][4], double b[4][4], double sx, double sy, double sz) {
	double t[4][4];

	D3d_make_identity(t);

	t[0][0] = sx; t[1][1] = sy; t[2][2] = sz;
	D3d_mat_mult(a,  t, a);

	t[0][0] = 1 / sx; t[1][1] = 1 / sy;
	D3d_mat_mult(b,  b, t);

	return 1;
}

int D3d_rotate_x (double a[4][4], double b[4][4], double rads) {

	double t[4][4] = {
		{1, 0, 0, 0},
		{0, cos(rads), -sin(rads), 0},
		{0, sin(rads), cos(rads), 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(a,  t, a);

	double q[4][4] =  {
		{1, 0, 0, 0},
		{0, cos(rads), sin(rads), 0} ,
		{0, -sin(rads), cos(rads), 0},
		{0, 0, 0, 1}
	};

	D2d_mat_mult(b,  b, q);


	return 1;
}

int D3d_rotate_y (double a[4][4], double b[4][4], double rads) {

	double t[4][4] = {
		{cos(rads), 0, sin(rads), 0},
		{0, 1, 0, 0},
		{ -sin(rads), 0, cos(rads), 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(a,  t, a);

	double q[4][4] =  {
		{cos(rads), 0, -sin(rads), 0},
		{0, 1, 0, 0},
		{sin(rads), 0, cos(rads), 0},
		{0, 0, 0, 1}
	};

	D2d_mat_mult(b,  b, q);


	return 1;
}

int D3d_rotate_z (double a[4][4], double b[4][4], double rads) {
	double t[4][4] = {
		{cos(rads), -sin(rads), 0, 0},
		{sin(rads), cos(rads), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(a,  t, a);

	double q[4][4] =  {
		{cos(rads), sin(rads), 0, 0},
		{-sin(rads), cos(rads), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	D2d_mat_mult(b,  b, q);


	return 1;
}
