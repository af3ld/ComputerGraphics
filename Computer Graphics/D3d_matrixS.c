#include <D3d_matrix.h>
#include <string.h>


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

int D3d_copy_mat (double a[4][4], double b[4][4]) {
	int r, c ;
	for (r = 0 ; r < 4 ; r++ ) {
		for (c = 0 ; c < 4 ; c++ ) {
			a[r][c] = b[r][c] ;
		}
	}

	return 1 ;
}

int D3d_mat_mult(double res[4][4], double a[4][4], double b[4][4]) {
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

int D3d_make_identity (double a[4][4]) {
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

int D3d_translate (double a[4][4], double b[4][4], double dx, double dy, double dz) {
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

	D3d_mat_mult(b,  b, q);


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

	D3d_mat_mult(b,  b, q);


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
		{ -sin(rads), cos(rads), 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(b,  b, q);


	return 1;
}

int D3d_cs_rotate_x (double a[4][4], double b[4][4], double cs, double sn) {
//rotates by inputting the sin and cos
	double t[4][4] = {
		{1, 0, 0, 0},
		{0, cs, -sn, 0},
		{0, sn, cs, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(a,  t, a);

	double q[4][4] =  {
		{1, 0, 0, 0},
		{0, cs, sn, 0},
		{0, -sn, cs, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(b,  b, q);


	return 1;
}

int D3d_cs_rotate_y (double a[4][4], double b[4][4], double cs, double sn) {
//rotates by inputting the sin and cos

	double t[4][4] = {
		{cs, 0, sn, 0},
		{0, 1, 0, 0},
		{ -sn, 0, cs, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(a,  t, a);

	double q[4][4] =  {
		{cs, 0, -sn, 0},
		{0, 1, 0, 0},
		{sn, 0, cs, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(b,  b, q);


	return 1;
}

int D3d_cs_rotate_z (double a[4][4], double b[4][4], double cs, double sn) {
//rotates by inputting the sin and cos

	double t[4][4] = {
		{cs, -sn, 0, 0},
		{sn, cs, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(a,  t, a);

	double q[4][4] =  {
		{cs, sn, 0, 0},
		{ -sn, cs, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	D3d_mat_mult(b,  b, q);


	return 1;
}

int D3d_negate_x (double a[4][4], double b[4][4]) {
// negate the x....reflects in the yz-plane
// a = reflect*a
// b = b*reflect_inverse

	double t[4][4];
	D3d_make_identity(t);
	t[0][0] = -1;

	D3d_mat_mult(a, t, a);
	D3d_mat_mult(b, b, t);

	return 1;
}

int D3d_negate_y (double a[4][4], double b[4][4]) {
// negate the y....reflects in the xz-plane
// a = reflect*a
// b = b*reflect_inverse

	double t[4][4];
	D3d_make_identity(t);
	t[1][1] = -1;

	D3d_mat_mult(a, t, a);
	D3d_mat_mult(b, b, t);

	return 1;
}


int D3d_negate_z (double a[4][4], double b[4][4]) {
// negate the z....reflects in the xy-plane
// a = reflect*a
// b = b*reflect_inverse

	double t[4][4];
	D3d_make_identity(t);
	t[2][2] = -1;

	D3d_mat_mult(a, t, a);
	D3d_mat_mult(b, b, t);

	return 1;
}

int D3d_mat_mult_points (double *X, double *Y, double *Z,
                         double m[4][4],
                         double *x, double *y, double *z, int numpoints) {
// |X0 X1 X2 ...|       |x0 x1 x2 ...|
// |Y0 Y1 Y2 ...| = m * |y0 y1 y2 ...|
// |Z0 Z1 Z2 ...|       |z0 z1 z2 ...|
// | 1  1  1 ...|       | 1  1  1 ...|

	int i, j;
	double tempX[numpoints], tempY[numpoints], tempZ[numpoints];

	for (i = 0; i < 4; i++) {
		for (j = 0; j < numpoints; j++) {
			if (i == 0) {
				tempX[j] = m[i][0] * x[j] + m[i][1] * y[j] + m[i][2] * z[j] + m[i][3];
			} else if (i == 1) {
				tempY[j] = m[i][0] * x[j] + m[i][1] * y[j] + m[i][2] * z[j] + m[i][3];
			} else if (i == 2) {
				tempZ[j] = m[i][0] * x[j] + m[i][1] * y[j] + m[i][2] * z[j] + m[i][3];
			}
		}
	}

	for (i = 0; i < numpoints; i++) {
		X[i] = tempX[i];
		Y[i] = tempY[i];
		Z[i] = tempZ[i];
	}

	return 1;
}

int D3d_mat_mult_pt (double P[3], double m[4][4], double Q[3]) {
// multiplies a SINGLE point by a matrix
// | P[0] |       | Q[0] |
// | P[1] | = m * | Q[1] |
// | P[2] |       | Q[2] |
// |  1   |       |  1   |

	int i, j;
	double temp[3];

	for (i = 0; i < 4; i++) {
		if (i == 0) {
			temp[0] = m[i][0] * Q[0] + m[i][1] * Q[1] + m[i][2] * Q[2] + m[i][3];
		} else if (i == 1) {
			temp[1] = m[i][0] * Q[0] + m[i][1] * Q[1] + m[i][2] * Q[2] + m[i][3];
		} else if (i == 2) {
			temp[2] = m[i][0] * Q[0] + m[i][1] * Q[1] + m[i][2] * Q[2] + m[i][3];
		}
	}
	memcpy(P, temp, 3);
	return 1;
}

int D3d_x_product (double res[3], double a[3], double b[3]) {
	double temp[3] = {
		(a[1] * b[2]) - (b[1] * a[2]),
		(a[2] * b[0]) - (b[2] * a[0]),
		(a[0] * b[1]) - (b[0] * a[1]),
	};
	res[0] = temp[0];
	res[1] = temp[1];
	res[2] = temp[2];
	return 1;
}


//returns the dot product of two vectors
double D3d_dot_product(double vect1[3], double vect2[3]) {
  return (vect1[0] * vect2[0]) + (vect1[1] *
                                  vect2[1]) + (vect1[2] * vect2[2]);
}


int D3d_make_movement_sequence_matrix (
    double mat[4][4],
    double inv[4][4],
    int num_movements,
    int *movement_type_list,
    double *parameter_list ) {

	D3d_make_identity(mat); D3d_make_identity(inv);
	printf("This has not been created yet\n");
	return 1;
}

int D3d_view (double view[4][4],  double view_inverse[4][4],
              double eye_x, double eye_y, double eye_z ,
              double coi_x, double coi_y, double coi_z ,
              double up_x,  double up_y,  double up_z ) {

	printf("This has not been created yet\n");
	return 1;
}




