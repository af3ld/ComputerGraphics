#include <FPT.h>
#include <D2d_matrix.h>

int WIDTH = 500; int HEIGHT = 500;

int points[100]; //number of points in whatever
double x[100][100], y[100][100]; //x and y coordinates
int polys[10];
int shapes[100][100]; //the shapes[what argument][which shape]
int shapeorder[100][100][100]; //where things connect to
double red[100][100], green[100][100], blue[100][100]; //individual colors of each[what arg][which shape]
int boxheight, boxwidth, centerx, centery; 
double smallx, smally, bigx, bigy; //specific smallest & largest (X,Y)'s



//Draws bounding box around figure
// seems only availble from drawit method, but it
//sometimes works from other spots,
//but not from all spots
double boundingbox(int i){
  int k;
  
  smallx = WIDTH * 2; smally = HEIGHT * 2;
  bigx = WIDTH * -2; bigy = HEIGHT * -2;
  for (k = 0; k < points[i]; k++){
    if (x[i][k] < smallx){
      smallx = x[i][k];
    }
    else if(x[i][k] > bigx){
      bigx = x[i][k];
    }
    if (y[i][k] < smally){
      smally = y[i][k];
    }
    else if(y[i][k] > bigy){
      bigy = y[i][k];
    }
  }
  
  boxheight = bigy - smally;
  boxwidth =  bigx - smallx;
  centerx = (bigx + smallx) /2;
  centery = (smally + bigy) / 2;

  if (WIDTH / boxwidth < HEIGHT / boxheight){
    return WIDTH / boxwidth;
  } else {
    return HEIGHT / boxheight;
  }
}


//draws the bounding box
void draw_boundingbox(int i){
  G_rgb(1,0,0);
  //printf("small: (%lf,%lf), big: (%lf,%lf)\n", smallx, smally, bigx, bigy);
  G_rectangle(smallx, smally, boxwidth, boxheight);
  G_line(smallx, smally, bigx, bigy);
  G_line(smallx, bigy, bigx, smally);
}


//reads the file and attaches the correct variable to each input
void readobject(FILE *g, int i){
  int k, j;
  
  fscanf(g, "%d", &points[i]);
  //printf("there are %d points\n", points[i]);
  for (k=0; k < points[i]; k++){
    fscanf(g, "%lf  %lf", &x[i][k], &y[i][k]);
    //printf("(%lf,%lf)\n", x[i][k], y[i][k]);
  }
  fscanf(g, "%d", &polys[i]);
  //printf("there are %d polygons in this whatever\n", polys[i]);
 
  for(k = 0; k < polys[i]; k++){
    fscanf(g, "%d", &shapes[i][k]);
    for(j = 0; j < shapes[i][k]; j++){
      fscanf(g, "%d", &shapeorder[i][k][j]);
      //printf("%d\n", shapeorder[i][k][j]);
    }
  }
  for(k=0; k < polys[i]; k++){
    fscanf(g, "%lf %lf %lf", &red[i][k], &green[i][k], &blue[i][k]); //the rgb
    //printf("(%lf, %lf, %lf)\n", red[i][k], green[i][k], blue[i][k]);
  }
}



//draws the image
void drawit(int i){
  int k, j;
  double tempx[points[i]], tempy[points[i]];

  for (k = 0; k < polys[i]; k++){
    for(j= 0; j < shapes[i][k]; j++){
      //printf("%d", shapeorder[i][k][j]);
      tempx[j] = x[i][shapeorder[i][k][j]];
      tempy[j] = y[i][shapeorder[i][k][j]];
      //printf("x%d:%lf, y%d:%lf\n", shapeorder[i][k][j], tempx[j],
      //shapeorder[i][k][j], tempy[j]);
    }
    
    G_rgb(red[i][k], green[i][k], blue[i][k]);
    G_fill_polygon(tempx, tempy, j);
  }
}

//controls the movement of the object
//Swatch == 0, sets up moving the object to the center, scale, and back
//to the center of the window
//Swatch == 0, sets up the rotate function
void movement(int i, double sf, double m[3][3], double minv[3][3], int swatch){
  if (swatch == 0){
    
    D2d_translate(m, minv, -centerx, -centery);
    D2d_scale(m, minv, sf, sf);
    D2d_translate(m, minv, WIDTH / 2, HEIGHT / 2);
    D2d_mat_mult_points(x[i],y[i], m, x[i],y[i], points[i]);
  } else if (swatch == 1) {
    
    D2d_translate(m, minv, -centerx, -centery);
    D2d_rotate(m, minv, M_PI/45);
    D2d_translate(m, minv, WIDTH / 2, HEIGHT / 2);
    D2d_mat_mult_points(x[i],y[i], m, x[i],y[i], points[i]);
  }

}


//writes the opening instructions
void welcome(int i){
  printf("Press");
  if (i == 1){
    printf(" 1 to start: ");
  } else if (i == 2){
    printf(" 1 or 2 to start: ");
  } else {
    printf(" 1 through %d to begin: ", i);
  }
}



int main(int argc, char **argv){
  char key, c;  FILE *g;
  double m[3][3], minv[3][3], sf;
  int cc;
  int inner_cycle = 1;

  for (cc = 1; cc < argc; cc++){
      g = fopen(argv[cc], "r"); //opens a file; r = read only
      if (g == NULL){	//if the file is empty, it will let me know
	printf("can't open (1)\n");
	exit(1);
      } else {
	readobject(g, cc);
	
	D2d_make_identity(m); D2d_make_identity(minv);
	sf = boundingbox(cc); //Remember, sf == scale factor
        movement(cc, sf, m, minv, 0); //(arg, scale, m, m inverse)
      }
    }
  
  welcome(argc - 1);
  scanf("%c", &key);
  int i = key - '0';
  
  if (i < argc && i > 0){
  G_init_graphics(WIDTH,HEIGHT);

    while (inner_cycle){
      G_rgb(1,1,1);
      G_clear();
	
      D2d_make_identity(m); D2d_make_identity(minv);

      sf = boundingbox(i); //Remember, sf == scale factor
      draw_boundingbox(i);
      movement(i, sf, m, minv, 1);
      //draw_boundingbox(i);
      drawit(i);
      
      G_rgb(0,0,0);
      G_fill_circle(WIDTH/2, HEIGHT/2, 3);
      
      c = G_wait_key();
      cc = c - '0';
      if (cc > 0 && cc < argc){
	if (cc != i){
	  i = cc;
	}
      } else{
	printf("can't open (2)\n");
	exit(1);
      }
    }
  }
}
    
