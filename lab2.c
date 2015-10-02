#include <FPT.h>
#include <D2d_matrix.h>

int points[100]; //number of points in whatever
double x[100][100], y[100][100]; //x and y coordinates
int polys[10];
int shapes[100][100]; //the shapes[what argument][which shape]
int shapeorder[100][100][100]; //where things connect to
double red[100][100], green[100][100], blue[100][100]; //individual colors of each[what arg][which shape]
int boxheight, boxwidth, centerx, centery;


//Draws bounding box around figure
// only availble from draw it method
void boundingbox(int i){
  int k;
  double smallx, smally, bigx, bigy;
  smallx = smally = 500;
  bigx = bigy = 0;
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
  //printf("smallest x: %lf, biggest x: %lf\n", smallx, bigx);
  //printf("smallest y: %lf, biggest y: %lf\n", smally, bigy);
  G_rgb(1,0,0);
  
  boxheight = bigy - smally;
  boxwidth =  bigx - smallx;
  centerx = (bigx + smallx) /2;
  centery = (smally + bigy) / 2;
  
  G_rectangle(smallx, smally, boxwidth, boxheight);
  G_line(smallx, smally, bigx, bigy);
  G_line(smallx, bigy, bigx, smally);
}



//reads the file and attaches the correct variable to each input
void readobject(FILE g, int i){
  int k, j;
  
  fscanf(&g, "%d", &points[i]);
  //printf("there are %d points\n", points[i]);
  for (k=0; k < points[i]; k++){
    fscanf(&g, "%lf  %lf", &x[i][k], &y[i][k]);
    //printf("(%lf,%lf)\n", x[i][k], y[i][k]);
  }
  fscanf(&g, "%d", &polys[i]);
  //printf("there are %d polygons in this whatever\n", polys[i]);
 
  for(k = 0; k < polys[i]; k++){
    fscanf(&g, "%d", &shapes[i][k]);
    for(j = 0; j < shapes[i][k]; j++){
      fscanf(&g, "%d", &shapeorder[i][k][j]);
      //printf("%d\n", shapeorder[i][k][j]);
    }
  }
  for(k=0; k < polys[i]; k++){
    fscanf(&g, "%lf %lf %lf", &red[i][k], &green[i][k], &blue[i][k]); //the rgb
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
    boundingbox(i);
  }
  G_wait_key();
}


 



int main(int argc, char **argv)
{
  int k, chars, i, numpoly; //i = each argument in command line
  char l;
  int tru = 1;
  
  FILE *g;
  
  G_init_graphics(500,500);
  for (i = 0; i < argc; i++){
    g = fopen(argv[i], "r"); //opens a file; r = read only
    if (g == NULL){    //if the file is empty; it will let me know
      printf("can't open\n");
      exit(1);
    } else if (i > 0) {
      readobject(*g, i);
    }
  }
}
