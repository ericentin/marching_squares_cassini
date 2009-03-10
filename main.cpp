#define MULTI_PLOT true //Determine whether or not to plot multiple iterations.

#define X_MAX 1.0   // Define extent of reference plane, used in call to gluOrtho2D(...)
#define Y_MAX 1.0 
#define X_MIN -1.0 
#define Y_MIN -1.0 
#define N_X 50      // Number of cells in x and y dimensions
#define N_Y 50
#define OX (-1 + i * dx)
#define OY (1 - j * dy)

#include <stdio.h>
#include <glut.h>

double h = 0.0;    // Height of reference plane in which contour is to be plotted --  f(x,y)=h
double dx = 2/(double)(N_X - 1);
double dy = 2/(double)(N_Y - 1);

double data[N_X][N_Y];   // Array to hold function values (a,b,c,d) at corners of each cell 
double f(double x, double y);    // Function whose contour is to be plotted
int cell(double a, double b, double c, double d);  // Helper ftn to determine cell type from f corner values
void lines(int , int, int, double, double, double, double);  // Helper ftn to draw correct line in a cell
void display();

void multidisplay(){
	glClear(GL_COLOR_BUFFER_BIT);
   	
	h = 0;
	for(; h < 1; h += .05){
		display();
	}
}
void display()   // Display callback function - called when window appears; drawing done here
{
	if (!MULTI_PLOT) glClear(GL_COLOR_BUFFER_BIT);
	
	// form data array from function
	for(int i = 0; i < N_X; i++)
		for(int j = 0; j < N_Y; j++)
			data[i][j] = f(OX, OY); 

	// double loop to process each cell (loop through each cell i,j; determine cell type, draw line in cell)
	for(int i = 0; i < N_X - 1; i++){
		for(int j = 0; j < N_Y - 1; j++){
			int c = cell(data[i][j], data[i+1][j], data[i][j+1], data[i+1][j+1]);  // return cell type c (0-15)
			lines(c,i,j, data[i][j], data[i+1][j], data[i][j+1], data[i+1][j+1]);  // draw correct line
		}
	}
	
	glFlush();
}

double f(double x, double y)   // Function to be plotted, example: Ovals of Cassini
{
   double a=0.48, b=0.5;
   return (x*x+y*y+a*a)*(x*x+y*y+a*a)-4*a*a*x*x-b*b*b*b;
}       

void init() 
{ 
   glMatrixMode(GL_PROJECTION); 
   glLoadIdentity(); 
   gluOrtho2D(X_MIN, X_MAX, Y_MIN, Y_MAX); 
   glMatrixMode(GL_MODELVIEW); 
} 

int main(int argc, char **argv) 
{ 
	glutInit(&argc, argv); 
	glutInitWindowPosition(50,50);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Oval of Cassini Contour plot"); 
	init(); 
	glutDisplayFunc(MULTI_PLOT ? multidisplay : display);           // White drawing color
	glClearColor(0.0,0.0,0.0,1.0);   // Black background 
	glColor3f(1.0,1.0,1.0);
	glutMainLoop(); 
}

int cell(double a, double b, double c, double d)  // Determine and return cell type
{
	int n=0; 
	if (a > h) n+=1;  
	if (b > h) n+=8;
	if (c > h) n+=4;
	if (d > h) n+=2;
	return n;
}

void draw_one(int n, int i, int j, double a, double b, double c, double d){
	double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	switch(n){
		case 1:
		case 14:	x1 = OX;
					y1 = OY - (dy * ((h - a) / (c - a)));
					x2 = OX + (dx * ((h - a) / (b - a)));
					y2 = OY;
					//printf("1: (%f, %f)\n", (h - a) / (d - a), (h - a) / (b - a));
					break;
		case 4:
		case 11: 	x1 = OX;
					y1 = OY - (dy * ((h - a) / (c - a)));
					x2 = OX + (dx * ((h - c) / (d - c)));
					y2 = OY - dy;
					//printf("2: (%f, %f)\n", (h - c)/(b - c), (h - c)/(a - c));
					break;
		case 2:
		case 13: 	x1 = OX + dx;
					y1 = OY - (dy * ((h - b) / (d - b)));
					x2 = OX + (dx * ((h - c) / (d - c)));
					y2 = OY - dy;
					//printf("3: (%f, %f)\n", (h - d) / (a - d), (h - d) / (c - d));
					break;
		case 7:
		case 8: 	x1 = OX + dx;
					y1 = OY - (dy * ((h - b) / (d - b)));
					x2 = OX + (dx * ((h - a) / (b - a)));
					y2 = OY;
					//printf("4: (%f, %f)\n", (h - b) / (c - b), (h - b) / (d - b));
					break;
	}
	glBegin(GL_LINES);  
		glVertex2d(x1,y1); 
		glVertex2d(x2,y2); 
	glEnd();
}

void draw_opposite(int n, int i, int j, double a, double b, double c, double d){
	double x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	switch(n){
		case 5:
		case 10:	y1 = OY;
					x1 = OX + (dx * ((h - a) / (b - a)));
					y2 = OY - dy;
					x2 = OX + (dx * ((h - c) / (d - c)));
					break;
		case 6:
		case 9:		x2 = OX;
					y1 = OY - (dy * ((h - b) / (d - b)));
					x1 = OX + dx;
					y2 = OY - (dy * ((h - a) / (c - a)));
					break;
	}
	glBegin(GL_LINES);  
		glVertex2d(x1,y1); 
		glVertex2d(x2,y2); 
	glEnd();

}

void lines(int n, int i, int j, double a, double b, double c, double d) // Draw correct line
{
   // Your code goes here
   // n is cell type (0-15)
   // i,j specifes which cell
   // a,b,c,d are function values at cell corners (from data array)
	switch(n){
		case 1:
		case 2:
		case 4:
		case 7:
		case 8:
		case 11:
		case 13:
		case 14:	
					draw_one(n, i, j, a, b, c, d);
					break;
		case 5:
		case 6:
		case 9:
		case 10:	draw_opposite(n, i, j, a, b, c, d);
					break;
		case 0:
		case 15:	break;
		default:
			//exit(2);
		break;
	}
}

