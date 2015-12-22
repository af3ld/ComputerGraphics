#include <FPT.h>
#include <D3d_matrix.h>

#define WINDOW_WIDTH 600
#define ROTATE_SPEED .05 

typedef
struct {
  int objnum;
  int polynum;
  double dist;
}
POLYGON;

double x[20][10000], y[20][10000], z[20][10000], colors[20][10000][3];
int numpoints[20], numpolys[20], poly_sizes[20][10000], polygons[20][10000][100];
POLYGON polyLoc[200000];
int plsize;
double red[10], green[10], blue[10], hither = 1;




int compare (const void *p, const void *q)
{
  POLYGON *a, *b ;

  a = (POLYGON*)p ;
  b = (POLYGON*)q ;

  if  (((*a).dist) < ((*b).dist)) return -1 ;
  else if (((*a).dist) > ((*b).dist)) return 1 ;
  else return 0 ;
}

void setUp(){
  G_init_graphics(WINDOW_WIDTH,WINDOW_WIDTH + 30);
  G_rgb(0,0,0);
  G_clear();
  G_clear(1,1,1);
}

double dot_product( double a[3], double b[3]){
 return((a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]) );
}


void loadFiles(int argc, char ** argv){
int i,j,k, m;
FILE * f;
m=0;
for(i=1; i<argc; i++){
	f = fopen(argv[i], "r");
		if(f == NULL){
			printf("Cannot open file %s \n", argv[i]);
		}
	fscanf(f , "%d", &numpoints[i -1]);
	for(j=0; j< numpoints[i-1]; j++){
		fscanf(f, "%lf %lf %lf", &x[i - 1][j], &y[i - 1][j], &z[i-1][j]);
	}
	fscanf( f, "%d", &numpolys[i -1]);
		for(k = 0; k < numpolys[i -1]; k++){
			fscanf( f, "%d", &poly_sizes[i -1][k]);
			for(j=0; j< poly_sizes[i -1][k];j++){
				fscanf(f, "%d", &polygons[i -1][k][j]);
			}
				polyLoc[m].objnum=i-1;
				polyLoc[m].polynum=k;
				polyLoc[m].dist=z[i-1][polygons[i -1][k][0]];
				m++;
		}
		for(k=0; k<numpolys[i -1]; k++){
      		fscanf(f,"%lf %lf %lf", &colors[i -1][k][0], &colors[i -1][k][1],  &colors[i -1][k][2]);
    	}
	}
	qsort(polyLoc, m,sizeof(POLYGON),compare);
	plsize = m;
	fclose(f);
}


void scaleNfit(int argc){
int i,j;
double scale[4][4] ,iscale[4][4];
for(i=0; i<argc-1;i++){
	double xmax, xmin, ymax, ymin, xrad, yrad, xcenter, ycenter, zmax,zmin,zrad,zcenter;
			xmin = x[i][0];
			xmax = x[i][0];
			ymin = y[i][0];
			ymax = y[i][0];
			zmin = z[i][0];
			zmax = z[i][0];
	for(j = 0; j<numpoints[i];j++){
		if(x[i][j] < xmin){
			xmin = x[i][j];
		}
		if(x[i][j] > xmax){
			xmax = x[i][j];
		}
		if(y[i][j] < ymin){
			ymin = y[i][j];
		}
		if(y[i][j] > ymax){
			ymax = y[i][j];
		}
		if(z[i][j] < zmin){
			zmin = z[i][j];
		}
		if(z[i][j] > zmax){
			zmax = z[i][j];
		}
	}
	xrad = (xmax - xmin)/2;
	yrad = (ymax - ymin)/2;
	zrad = (zmax - zmin)/2;
	x[i][numpoints[i]+1] = xmax - xrad;
	y[i][numpoints[i]+1] = ymax - yrad;
	z[i][numpoints[i]+1] = zmax - zrad;
	double s;
	
	if(xrad > yrad){
		s = xrad;
	}else{
		s = yrad;
	}
	if(zrad > s){
		s = zrad;
	}
	s = (WINDOW_WIDTH/2)/s;
	D3d_make_identity(scale);
	D3d_make_identity(iscale);
	D3d_translate(scale, iscale, -xcenter, -ycenter, -zcenter);
	D3d_mat_mult_points(x[i],y[i],z[i],scale,x[i],y[i],z[i], numpoints[i]);
}
}

void rotate(int pnum, int axis, int direction){
	double a[4][4], b[4][4];
 	D3d_make_identity(a);
 	D3d_make_identity(b);
 	int sign = 1;
 	if(direction == 0){
 		sign = -1;
 	}
	D3d_translate(a,b,-x[pnum][numpoints[pnum]+1],-y[pnum][numpoints[pnum]+1],-z[pnum][numpoints[pnum]+1]);
 	if(axis == 0){
		D3d_rotate_x(a,b, sign* ROTATE_SPEED);
 	}else if(axis == 1){
		D3d_rotate_y(a,b, sign*ROTATE_SPEED);
 	}else if(axis == 2){
 		D3d_rotate_z(a,b,sign*ROTATE_SPEED);
 	}
 	D3d_translate(a,b,x[pnum][numpoints[pnum]+1],y[pnum][numpoints[pnum]+1],z[pnum][numpoints[pnum]+1]);
 	D3d_mat_mult_points(x[pnum], y[pnum],z[pnum], a, x[pnum], y[pnum],z[pnum], numpoints[pnum]);
}

void translate(int pnum, int axis, int direction){
	double a[4][4], b[4][4];
 	D3d_make_identity(a);
 	D3d_make_identity(b);
 	int sign = 1;
 	if(direction == 0){
 		sign = -1;
 	}
 	if(axis == 0){
 		x[pnum][numpoints[pnum]+1] = x[pnum][numpoints[pnum]+1] + (sign * 1);
		D3d_translate(a , b, 1 *sign, 0, 0);
 	}else if(axis == 1){

 	y[pnum][numpoints[pnum]+1] = y[pnum][numpoints[pnum]+1] + (sign * 1);
		D3d_translate(a , b, 0,1*sign ,0);
 	}else if(axis == 2){
 		z[pnum][numpoints[pnum]+1] = z[pnum][numpoints[pnum]+1] + (sign * 1);
 		D3d_translate(a , b, 0,0,1*sign );
 	}
 	
 	
 	D3d_mat_mult_points(x[pnum], y[pnum],z[pnum], a, x[pnum], y[pnum],z[pnum], numpoints[pnum]);
}

int  Clip_Polygon_Against_Plane(
                  double a, double b, double c, double d, 
                  double *polyx, double *polyy, double *polyz, int size,
                  double *resx, double *resy, double *resz)

// Clip polygon against the plane ax + by + cz + d = 0,
// where ax + by + cz + d < 0 is considered IN.
// Incoming poly defined in arrays  polyx, polyy, polyz  with numverts = size.
// Clipped result values are stored in arrays  resx, resy, resz
// The numverts of the clipped result is returned as value of the function.

{
  int num,i,j ;
  double x1,y1,z1,x2,y2,z2,x21,y21,z21,den,t,xintsct,yintsct, zintsct ;
  double s1,s2 ;

  num = 0 ;
  for (i = 0 ; i < size ; i++) {
     j = (i + 1) % size ;

     // load up segment to be clipped
     x1 = polyx[i] ; y1 = polyy[i] ; z1 = polyz[i];
     x2 = polyx[j] ; y2 = polyy[j] ; z2 = polyz[j];

     // clip line segment (x1,y1)-(x2,y2) against line
     s1 = (a * x1 + b * y1 + c * z1 + d) ;
     s2 = (a * x2 + b * y2 + c * z2 + d) ;

     if ((s1 >= 0) && (s2 >= 0)) {
        // out to out, do nothing
     } else if ((s1 < 0) && (s2 < 0)) {
        // in to in
        resx[num] = x2 ; resy[num] = y2 ; resz[num] = z2; num++ ;
     } else {
        // one is in, the other out, so find the intersection

        x21 = x2 - x1 ; y21 = y2 - y1 ; z21 = z2 - z1;
        den = a*x21 + b*y21 + c*z21;
        if (den == 0) continue ; // do nothing-should never happen
        t = -(a*x1 + b*y1 + c*z1 + d)/den ;
        xintsct = x1 + t*x21 ;
        yintsct = y1 + t*y21 ;
        zintsct = z1 + t*z21 ; 
        if (s1 < 0) { 
          // in to out
          resx[num] = xintsct ; resy[num] = yintsct ; resz[num] = zintsct; num++ ;
        } else  {
          // out to in
          resx[num] = xintsct ; resy[num] = yintsct ; resz[num] = zintsct; num++ ;
          resx[num] = x2      ; resy[num] = y2      ; resz[num] = z2; num++ ;
        }

     }


  } // end for i

  return num ;  // return size of the result poly
}

int Clip_Polygon_Against_Window(double *polyx, double *polyy, double *polyz, int size){
	double resx[100], resy[100], resz[100], yonder = 100; 

	size = Clip_Polygon_Against_Plane(0,1,.57,0,polyx,polyy,polyz,size, resx,resy,resz);
	size = Clip_Polygon_Against_Plane(0,-1,.57,0,resx,resy,resz,size,polyx,polyy,polyz);
	size = Clip_Polygon_Against_Plane(0,0,1,hither,polyx,polyy,polyz,size,resx,resy,resz);
	size = Clip_Polygon_Against_Plane(0,0,1,-yonder,resx,resy,resz,size,polyx,polyy,polyz);
	size = Clip_Polygon_Against_Plane(1,0,.57,0,polyx,polyy,polyz,size,resx,resy,resz);
	size = Clip_Polygon_Against_Plane(-1,0,.57,0,resx,resy,resz,size,polyx,polyy,polyz);
	return size;

}

void display(int pnum, int sign, int specpow, double diffusepro, double ambient, double lightx, double lighty, double lightz){
	int i, j, k, m;
	G_rgb(0,0,0);
	G_clear();
	double halfAngle = 30*M_PI/180;
	G_clear(1,1,1);
	if(sign == 0){
			sign = -1;
		}else{
			sign=1;
		}
int n=0;
for(n=0; n<plsize; n++){
double xtemp[100];
double ytemp[100];
double ztemp[100];
	for(j=0; j<poly_sizes[polyLoc[n].objnum][polyLoc[n].polynum];j++){
 		xtemp[j] = x[polyLoc[n].objnum][polygons[polyLoc[n].objnum][polyLoc[n].polynum][j]];
 		ytemp[j] = y[polyLoc[n].objnum][polygons[polyLoc[n].objnum][polyLoc[n].polynum][j]];
 		ztemp[j] = z[polyLoc[n].objnum][polygons[polyLoc[n].objnum][polyLoc[n].polynum][j]];

 	
	}

		int tempsize = poly_sizes[polyLoc[n].objnum][polyLoc[n].polynum];
		double perpendicular[3], first[3], second[3], eye[3], light[3],reflection[3];

		


	 tempsize = Clip_Polygon_Against_Window(xtemp,ytemp,ztemp, poly_sizes[polyLoc[n].objnum][polyLoc[n].polynum]);
	 if(tempsize < 1)continue;

	 	first[0] = xtemp[0]-xtemp[1];
		first[1] = ytemp[0]-ytemp[1];
		first[2] = ztemp[0]-ztemp[1];

		second[0] = xtemp[0]-xtemp[2];
		second[1] = ytemp[0]-ytemp[2];
		second[2] = ztemp[0]-ztemp[2];

		eye[0] = 0 - xtemp[0] ;
		eye[1] = 0 - ytemp[0] ;
		eye[2] = 0 - ztemp[0] ;

		light[0]= lightx - xtemp[0] ;
		light[1]= lighty - ytemp[0] ;
		light[2]= lightz - ztemp[0] ;

		double xfinal[tempsize], yfinal[tempsize];
for(j=0; j<tempsize;j++){

		xfinal[j] = xtemp[j]/ztemp[j];
		yfinal[j] = ytemp[j]/ztemp[j];
		xfinal[j] = (xfinal[j] * (300/tan(halfAngle))) + 300;
		yfinal[j] = (yfinal[j] * (300/tan(halfAngle))) + 300;
}

		D3d_x_product(perpendicular,first,second);
		double length = sqrt(perpendicular[0]*perpendicular[0] + perpendicular[1]*perpendicular[1] + perpendicular[2]*perpendicular[2]);
		perpendicular[0] = perpendicular[0]/length;
		perpendicular[1] = perpendicular[1]/length;
		perpendicular[2] = perpendicular[2]/length;

		double lightLength =  sqrt(light[0]*light[0] + light[1]*light[1] + light[2]*light[2]);
		light[0] = light[0]/lightLength;
		light[1] = light[1]/lightLength;
		light[2] = light[2]/lightLength;
		double eyelength =  sqrt(eye[0]*eye[0] + eye[1]*eye[1] + eye[2]*eye[2]);
		eye[0] = eye[0]/eyelength;
		eye[1] = eye[1]/eyelength;
		eye[2] = eye[2]/eyelength;


		double pl = dot_product(perpendicular, light);

		if(pl < 0){
			perpendicular[0] = -1 * perpendicular[0];	
			perpendicular[1] = -1 * perpendicular[1];
			perpendicular[2] = -1 * perpendicular[2];
			pl = dot_product(perpendicular, light);
		}


		double pe = dot_product(perpendicular, eye);
		double intensity;
                if (pe < 0) { intensity = ambient ; goto JJJ ; }

		double diffuse = diffusepro * pl;

		reflection[0] = ((2 * pl) * perpendicular[0]) - light[0];
		reflection[1] = ((2 * pl) * perpendicular[1]) - light[1];
		reflection[2] = ((2 * pl) * perpendicular[2]) - light[2];
		double specular;
	
		specular = (1 - ambient - diffusepro) * pow((dot_product(eye, reflection)),specpow);	
		
		 intensity = ambient + diffuse + specular;


 JJJ : ;
		double redt, greent, bluet;

		if(intensity <= ambient + diffusepro){
			double f = intensity/ (ambient + diffusepro);
			redt = f*red[polyLoc[n].objnum];
			greent = f* green[polyLoc[n].objnum];
			bluet = f*blue[polyLoc[n].objnum];
		}
		else{
			double I[3];
			I[0] = 1-red[polyLoc[n].objnum];
			I[1] = 1-green[polyLoc[n].objnum];
			I[2] = 1-blue[polyLoc[n].objnum];
			double f = (intensity - ambient - diffusepro)/ (1 - ambient - diffusepro);
			redt = f*I[0] + red[polyLoc[n].objnum];
			greent = f*I[1] + green[polyLoc[n].objnum];
			bluet = f*I[2] + blue[polyLoc[n].objnum];
		}
	
	


		G_rgb(redt,greent,bluet);
 		G_fill_polygon(xfinal, yfinal, tempsize);
 		G_rgb(0,0,0);
 		G_polygon(xfinal, yfinal, tempsize);
}
}

int resize(){
	int i=0;
	for(i=0;i<plsize;i++){
		polyLoc[i].dist = z[polyLoc[i].objnum][polygons[polyLoc[i].objnum][polyLoc[i].polynum][0]];
	}
	qsort(polyLoc, plsize,sizeof(POLYGON),compare);
}


int main(int argc, char ** argv){

	if(argc == 1){
	printf("usage: pgm_name file1.xy file2.xy ... \n");
	exit(1);
}
int specpow;
double diffusepro, ambient;
 diffusepro = .5;
 ambient = .2;
double x = -200;
double y=-100;
double z=50;
 specpow = 70;
// red = 1;
// green = 0;
// blue = 0;

// printf("enter ambeint  > ");
// scanf("%lf", &ambient);
// printf("enter Diffuse  > ");
// scanf("%lf", &diffusepro);
// printf("enter specular  > ");
// scanf("%d", &specpow);
// printf("enter light coordinates \n");
// scanf("%lf %lf %lf", &x, &y, &z);

int i = 0;
for(i=0;i< argc-1;i++){
	printf("enter object %d color \n", i);
scanf("%lf %lf %lf", &red[i], &green[i], &blue[i]);
}

setUp();
loadFiles(argc, argv);
scaleNfit(argc);
display(0, 1, specpow, diffusepro, ambient, x ,y,z);
int num, file, axis;
file = 0;
axis = 0;
int sign = 0;
int direction = 0;
while(1 == 1){
	num = G_wait_key();

	if(num == 114){
		rotate(file, axis, direction);
		resize();
		display(file, sign, specpow, diffusepro, ambient, x , y, z);
	}
	else if(num == 81 || num == 113){//quit
		break;
	}
	else if(num ==120){//x-axis
		axis = 0;
	}
	else if(num == 121){//y-axis
		axis = 1;
	}
	else if(num == 122){//z-axis 
		axis = 2;
	}
	else if(num == 116){//translate 
		translate(file, axis, direction);
		resize();
		display(file,sign, specpow, diffusepro, ambient, x , y, z);
	}
	else if(num == 99){//change translate direction
		if(direction == 0){
			direction = 1;
		}else{
			direction=0;
		}
	}else if(num == 'h'){//adjust hither
		(sign==0) ? (hither+=.1 ): (hither-=.1);
		display(file, sign, specpow, diffusepro, ambient, x , y, z);
	}
	else if( num >= 48 && num <= 57){//choose file
		file = num - 48;
		if(file < argc){
			display(file, sign, specpow, diffusepro, ambient, x , y, z);
		}
	}else if(num == 115){
		sign = (sign+1)%2;

		display(file, sign, specpow, diffusepro, ambient, x , y, z);
	}else{
		printf("no command");
	}
}
}
