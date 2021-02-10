// Jonathon Vasilak
// fractals.c
// includes
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// define global variables
#define SIZE 700
#define CENTER 350

// function prototypes
void sierpinski(int, int, int, int, int, int);
void create_triangle(int, int, int, int, int, int);
void shrinkingsquares(int, int, int, int, int, int, int, int);
void create_square(int, int, int, int, int, int, int, int);
void spiral_square(int, int, float, float, int);
void circle_lace(int, int, int);
void snowflake(int, int, float, float);
void tree(int, int, int, float);
void fern(int, int, int, float);

// main function
int main() {
	// defining variables
  bool loop = true;
	int triangleMargin = 20;
	int square_margin = 200;
	int window_center = SIZE/2;
	int circle_diameter = 150;
	int square_size = 4;
	float spiral_angle = 0;
	int spiral_radius = 8;
	float snowflake_length = 250;
	float snowflake_angle = 2*M_PI/5;
	int tree_length = 300;
	float tree_angle = (M_PI)/2;
	int fern_length = 150;
	float fern_angle = M_PI/2;
  gfx_open(SIZE, SIZE, "My Window"); // opens the graphics window
	char c;
  while (loop) {
    c = gfx_wait(); // the program waits for user input
    gfx_clear(); // the program clears the graphics window
    switch (c) {
      case '1': // Sierpinski Triangle   
				sierpinski(triangleMargin, triangleMargin, SIZE-triangleMargin, triangleMargin, SIZE/2, SIZE-triangleMargin);
        break;
      case '2': // Shrinking Squares  
				shrinkingsquares(square_margin, square_margin, SIZE-square_margin, square_margin, SIZE-square_margin, SIZE-square_margin, square_margin, SIZE-square_margin);
        break;
			case '3': // Spiral of Squares
				spiral_square(window_center, window_center, square_size, spiral_angle, spiral_radius);
				break;
			case '4': // Circluar Lace
				circle_lace(window_center, window_center, circle_diameter);
				break;
			case '5': // Snowflake
				snowflake(window_center, window_center, snowflake_length, snowflake_angle);
				break;
			case '6': // Tree
				tree(tree_length, CENTER, SIZE, tree_angle);
				break;
			case '7': // Fern
				fern(fern_length,CENTER,SIZE, fern_angle);
				break;
      case '8': // Spiral of Spirals 
       //	 set up some variables
       //	 call a recursive function  
        break;
      case 'q':
        loop = false; // ends the program
      default:
        break;
    }
  }

  return 0;
}

// all functions go here
void sierpinski(int x1, int y1, int x2, int y2, int x3, int y3)
{
	if(abs(x2-x1) < 5) return; // checks if the length is smaller than a certain value, ends recrusion if this is the case
	create_triangle(x1, y1, x2, y2, x3, y3); // draws a triangle in the window
	sierpinski(x1, y1, (x1+x2)/2, (y1+y2)/2, (x1+x3)/2, (y1+y3)/2); // calls this function again three times, making the sides smaller each time
	sierpinski((x1+x2)/2, (y1+y2)/2, x2, y2, (x2+x3)/2, (y2+y3)/2);
	sierpinski((x1+x3)/2, (y1+y3)/2, (x2+x3)/2, (y2+y3)/2, x3, y3);
}

void create_triangle(int X1, int Y1, int X2, int Y2, int X3, int Y3)
{
	gfx_line(X1, Y1, X2, Y2); // draws the three lines that make a triangle
	gfx_line(X2, Y2, X3, Y3);
	gfx_line(X3, Y3, X1, Y1);
}

void shrinkingsquares(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	if(abs(x1-x2) < 5) return; // checks if length is too short
	create_square(x1, y1, x2, y2, x3, y3, x4, y4); // draws a square
	shrinkingsquares(x1-(abs(x1-x2)/4),y1-(abs(x1-x2)/4), x1+(abs(x1-x2)/4), y1-(abs(x1-x2)/4), x1+(abs(x1-x2)/4), y1+(abs(x1-x2)/4), x1-(abs(x1-x2)/4), y1+(abs(x1-x2)/4)); // runs the function four times, placing a square on the corners of the current square
	shrinkingsquares(x2-(abs(x1-x2)/4),y2-(abs(x1-x2)/4), x2+(abs(x1-x2)/4), y2-(abs(x1-x2)/4), x2+(abs(x1-x2)/4), y2+(abs(x1-x2)/4), x2-(abs(x1-x2)/4), y2+(abs(x1-x2)/4));
	shrinkingsquares(x3-(abs(x1-x2)/4),y3-(abs(x1-x2)/4), x3+(abs(x1-x2)/4), y3-(abs(x1-x2)/4), x3+(abs(x1-x2)/4), y3+(abs(x1-x2)/4), x3-(abs(x1-x2)/4), y3+(abs(x1-x2)/4));
	shrinkingsquares(x4-(abs(x1-x2)/4),y4-(abs(x1-x2)/4), x4+(abs(x1-x2)/4), y4-(abs(x1-x2)/4), x4+(abs(x1-x2)/4), y4+(abs(x1-x2)/4), x4-(abs(x1-x2)/4), y4+(abs(x1-x2)/4));
}

void create_square(int X1, int Y1, int X2, int Y2, int X3, int Y3, int X4, int Y4)
{
	gfx_line(X1,Y1,X2,Y2); // draws four lines that make a square
	gfx_line(X2,Y2,X3,Y3);
	gfx_line(X3,Y3,X4,Y4);
	gfx_line(X4,Y4,X1,Y1);
}

void spiral_square(int centerx, int centery, float size, float angle, int radius)
{
	if(radius > SIZE) return; // checks if the square is off screen
	spiral_square(centerx+radius*cos(angle), centery+radius*sin(angle), size*1.2, angle+M_PI/5, radius*1.2); // creates an invisible spiral of points, where the squares will be centered at
	create_square(centerx-size/2, centery-size/2, centerx+size/2, centery-size/2, centerx+size/2, centery+size/2, centerx-size/2, centery+size/2); // places the squares in the graphics window
}

void circle_lace(int centerX, int centerY, int radius)
{
	gfx_circle(centerX, centerY, radius); // draws a circle in the window
	if (radius < 5) return;
	circle_lace(centerX+radius*cos(0), centerY+radius*sin(0), radius*0.4); // runs the function five times, placing circles around the current circle
	circle_lace(centerX+radius*cos(M_PI), centerY+radius*sin(M_PI), radius*.4);
	circle_lace(centerX+radius*cos(M_PI/3), centerY+radius*sin(M_PI/3), radius*.4);
	circle_lace(centerX+radius*cos(2*M_PI/3), centerY+radius*sin(2*M_PI/3), radius*.4);
	circle_lace(centerX+radius*cos(4*M_PI/3), centerY+radius*sin(4*M_PI/3), radius*.4);
	circle_lace(centerX+radius*cos(5*M_PI/3), centerY+radius*sin(5*M_PI/3), radius*.4);
}

void snowflake(int X, int Y, float length, float s_angle)
{
	if (length < 5) return; // checks if the length is too small
	float i = s_angle; // sets an angle that does not change in the following loop
	while(s_angle <= i+2*M_PI) // runs 5 times
	{
		int x = X+length*cos(s_angle); // creates another set of coordinates that marks the end of the line
		int y = Y+length*sin(s_angle);
		gfx_line(X, Y, x, y); // draws a line
		snowflake(x, y, length/3, i); 
		s_angle = s_angle+2*M_PI/5; // increase the angle
	}
}

void tree(int length, int x, int y, float t_angle)
{
	if(length < 5) return;
	int X = x + length*cos(t_angle); // these variables are the coordinates of the end of the line that will be drawn
	int Y = y - length*sin(t_angle);
	gfx_line(x,y,X,Y); // draws a line in the graphics window
	tree(length*0.6, X, Y, t_angle+M_PI/4); // runs the function twice, meaning the two new branches are created for every existing one
	tree(length*0.6, X, Y, t_angle-M_PI/4);
}

void fern(int length, int x, int y, float angle)
{
	if(length < 4) return;
	int i;
	int X = x+length*cos(angle); // the coordinates of the end of the line that is going to be drawn
	int Y = y-length*sin(angle);
	for(i = 0; i < 4; i++)
	{
		gfx_line(x,y,X,Y); // draws a line
		fern(length/3,X,Y,angle-M_PI/6); // runs the function two time, meaning two new branches are drawn
		fern(length/3,X,Y,angle+M_PI/6);
		x = X; // updates the coordinates for the next line that will be drawn
		y = Y;
		X = x+length*cos(angle);
		Y = y-length*sin(angle);
	}
}
