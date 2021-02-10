// Jonathon Vasilak
// project.c
// This program is a game where the user must navigate through a maze without touching the walls of the maze, the game includes three levels which increase in difficulty and can be played at three different speeds
// The controls of the game can be found in the menu screen, which is displayed as soon as the program is started

// Include Libraries
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "gfx.h"

// Define structures
// Player structure stores information regarding the location of the player
typedef struct{
	int x;
	int y;
	int dx;
	int dy;
} Player;

// Level type contains information regarding the setup of the current level
typedef struct{
	int x;
	int y;
} Level;

// EndPoint contains the coordinates of the end line
typedef struct{
	int x;
	int y;
} EndPoint;

// Function Protoytpes
void printmenu();
void levelone(int, float *);
void leveltwo(int, float *);
void levelthree(int, float *);
void victoryscreen();
void failurescreen();
void perimeter(int);
void place_end1(EndPoint []);
void place_end2(EndPoint []);
void place_end3(EndPoint []);
void place1(int, Level []);
void place2(int, Level []);
void place3(int, Level []);

// Main function
int main()
{
	char *font = "-misc-fixed-bold-r-normal--13-120-75-75-c-70-iso8859-1"; // Sets the font on the graphics window
	int window_size = 600; // determines the size of the graphics window
	gfx_open(window_size, window_size, "My Window"); // opens window
	gfx_changefont(font); // changes the font of text that will be used in the window
	char input;
	float speed = 1;
	float *p;
	p = &speed;
	bool end = false;
	while(end!=true) // the program will continue running until the user enters the input which causes it to quit
	{
			printmenu(); // displays the menu for the user to see
			input = gfx_wait(); // gets input from the user
			switch(input)
			{
				case 'm':
					printmenu(); // displays the menu if the user enters 'm'
					break;
				case 'q':
					end = true; // allows the program to end
					break;
				case '1': // plays the first level
					levelone(window_size, p);
					break;
				case '2': // plays the second level
					leveltwo(window_size, p);
					break;
				case '3': // plays the third level
					levelthree(window_size, p);
					break;
				case '4': // lowers speed
					*p = 0.5;
					break;
				case '5': // default speed
					*p = 1;
					break;
				case '6': // fastest speed
					*p = 2;
					break;
				default: // nothing happens if the user enters an invalid input
					break;
			}
		gfx_flush();
	}
	return 0;
}

void printmenu()
{
	gfx_clear(); // clears the screen before displaying the rules of the game to the user
	gfx_text(75, 50, "Welcome to the Maze Game");
	gfx_text(75, 100, "Press 'w' 'a' 's' 'd' to control the direction you move");
	gfx_text(75, 150, "Press '1', '2', or '3' to play a level.");
	gfx_text(75, 200, "Press 'q' to quit");
	gfx_text(75, 250, "Press 'm' to display the menu");
	gfx_text(75, 300, "Press '4', '5', or '6' to change the speed of the game");
	gfx_text(75, 350, "Press 'p' to pause the game");
	gfx_text(75, 400, "Rules:");
	gfx_text(75, 450, "Try to navigate the maze and reach the blue line to win");
	gfx_text(75, 500, "Touching a wall or the sides of the maze will result in failure");
	gfx_text(75, 550, "Goodluck!");
}

void failscreen()
{
	char input; 
	gfx_clear();
	gfx_text(75, 200, "Level failed: Please try again."); // Informs the user they failed and tells them how to go back to the main menu
	gfx_text(75, 250, "Press any button to return to the menu"); 
	input = gfx_wait(); // the user is only taken back to the main menu if they enter some type of input
}

void winscreen()
{
	char input;
	gfx_clear();
	gfx_text(75, 200, "Great Job! You beat this level!"); // Informs the user they completed the level
	gfx_text(75, 250, "Press any button to return to the menu");
	input = gfx_wait();
}

void levelone(int size, float *q)
{
	gfx_clear();
	perimeter(size); // creates a line along the perimeter
	Player p1;
	int points = 1000; // the number of pixels that will make up the walls of the maze
	Level n[points]; // creates an array of x and y coordinates
	EndPoint e[30]; // creates an array of coordinates for the end goal
	place_end1(e); // puts the end line in the window
	place1(size, n); // places the walls of the level
	p1.x = 20;
	p1.y = 20;
	p1.dx = 1;
	p1.dy = 0;
	gfx_point(p1.x, p1.y); // places a point at the location of the player
	bool paused = false;
	bool win = false;
	bool failure = false;
	bool end = false;
	char input;
	int i;
	while(end != true) // runs until the end condition is met
	{
		if(gfx_event_waiting()) // allows the program to run even when the user is not inputting anything
		{
			input = gfx_wait(); // obtains the user input
			switch(input)
			{
				case 'p':
					if(paused == false) paused = true; // if the game is not paused then it will become paused
					else if(paused == true) paused = false; // if the game is paused then the game is unpaused
					break;
				case 'w':
					p1.dx = 0; // changes the direction in which the player moves
					p1.dy = -1;
					break;
				case 'a':
					p1.dx = -1; // changes the direction in which the player moves
					p1.dy = 0;
					break;
				case 's':
					p1.dx = 0; // changes the direction in which the player moves
					p1.dy = 1;
					break;
				case 'd':
					p1.dx = 1; // changes the direction in which the player moves
					p1.dy = 0;
					break;
				case 'q':
					return; // returns from the function
					break;
				default:
					break;
			}
		}
		if(paused!=true) // if the game is paused the player will not move
		{
			p1.x = p1.x + p1.dx; // updates the players coordinates
			p1.y = p1.y + p1.dy;
			gfx_point(p1.x, p1.y); // places a pixel where the player has moved to
			if(p1.x == 2 || p1.x == size-2) // this if loop determines if the player has hit a wall on the end of the window
			{
				failure = true;
				end = true;
			}
			else if(p1.y == 2 || p1.y == size-2)
			{
				failure = true;
				end = true;
			}
			for(i = 0; i < 500; i++)
			{
				if(p1.x == n[i].x && p1.y == n[i].y) // checks if the player has hit one the walls placed inside of the window
				{
					failure = true;
					end = true;
				}
			}
			for(i = 0; i < 30; i++)
			{
				if(p1.x == e[i].x && p1.y == e[i].y) // if the user has reached a point on the goal line
				{
					win = true;
					end = true;
				}
			}
			usleep(6000/(*q)); // makes the program pause for a short while, deterined by the user
		}
	}
	if(failure == true) failscreen(); // if the player loses the get the failure screen
	if(win == true) winscreen(); // if the player wins they get the win screen
}

void leveltwo(int size, float *q) // the code used in the leveltwo function is very similar to the levelone function, refer to that function for comments
{
	gfx_clear();
	perimeter(size);
	Player p1;
	int points = 1350;
	Level n[points];
	EndPoint e[30];
	place_end2(e);
	place2(size, n);
	p1.x = 20;
	p1.y = 20;
	p1.dx = 1;
	p1.dy = 0;
	gfx_point(p1.x, p1.y);
	bool paused = false;
	bool win = false;
	bool failure = false;
	bool end = false;
	char input;
	int i;
	while(end != true)
	{
		if(gfx_event_waiting())
		{
			input = gfx_wait();
			switch(input)
			{
				case 'p':
					if(paused == false) paused = true;
					else if(paused == true) paused = false;
					break;
				case 'w':
					p1.dx = 0;
					p1.dy = -1;
					break;
				case 'a':
					p1.dx = -1;
					p1.dy = 0;
					break;
				case 's':
					p1.dx = 0;
					p1.dy = 1;
					break;
				case 'd':
					p1.dx = 1;
					p1.dy = 0;
					break;
				case 'q':
					return;
					break;
				default:
					break;
			}
		}
		if(paused!=true)
		{
			p1.x = p1.x + p1.dx;
			p1.y = p1.y + p1.dy;
			gfx_point(p1.x, p1.y);
			if(p1.x == 2 || p1.x == size-2)
			{
				failure = true;
				end = true;
			}
			else if(p1.y == 2 || p1.y == size-2)
			{
				failure = true;
				end = true;
			}
			for(i = 0; i < 1350; i++)
			{
				if(p1.x == n[i].x && p1.y == n[i].y)
				{
					failure = true;
					end = true;
				}
			}
			for(i = 0; i < 30; i++)
			{
				if(p1.x == e[i].x && p1.y == e[i].y)
				{
					win = true;
					end = true;
				}
			}
			usleep(6000/(*q));
		}
	}
	if(failure == true) failscreen();
	if(win == true) winscreen();
}

void levelthree(int size, float *q) // the code in the function levelthree is very similar to levelone, refer to the comments in that function
{
	gfx_clear();
	perimeter(size);
	Player p1;
	int points = 3250;
	Level n[points];
	EndPoint e[30];
	place_end3(e);
	place3(size, n);
	p1.x = 20;
	p1.y = 20;
	p1.dx = 1;
	p1.dy = 0;
	gfx_point(p1.x, p1.y);
	bool paused = false;
	bool win = false;
	bool failure = false;
	bool end = false;
	char input;
	int i;
	while(end != true)
	{
		if(gfx_event_waiting())
		{
			input = gfx_wait();
			switch(input)
			{
				case 'p':
					if(paused == false) paused = true;
					else if(paused == true) paused = false;
					break;
				case 'w':
					p1.dx = 0;
					p1.dy = -1;
					break;
				case 'a':
					p1.dx = -1;
					p1.dy = 0;
					break;
				case 's':
					p1.dx = 0;
					p1.dy = 1;
					break;
				case 'd':
					p1.dx = 1;
					p1.dy = 0;
					break;
				case 'q':
					return;
					break;
				default:
					break;
			}
		}
		if(paused!=true)
		{
			p1.x = p1.x + p1.dx;
			p1.y = p1.y + p1.dy;
			gfx_point(p1.x, p1.y);
			if(p1.x == 2 || p1.x == size-2)
			{
				failure = true;
				end = true;
			}
			else if(p1.y == 2 || p1.y == size-2)
			{
				failure = true;
				end = true;
			}
			for(i = 0; i < 3250; i++)
			{
				if(p1.x == n[i].x && p1.y == n[i].y)
				{
					failure = true;
					end = true;
				}
			}
			for(i = 0; i < 30; i++)
			{
				if(p1.x == e[i].x && p1.y == e[i].y)
				{
					win = true;
					end = true;
				}
			}
			usleep(6000/(*q));
		}
	}
	if(failure == true) failscreen();
	if(win == true) winscreen();
}

void perimeter(int sz)
{
	gfx_color(255,0,0); // changes the color to red
	gfx_line(0,0,sz-1,0); // places the lines along the borders of the window
	gfx_line(sz-1,0,sz-1,sz-1);
	gfx_line(sz-1,sz-1,0,sz-1);
	gfx_line(0,sz-1,0,0);
	gfx_color(255,255,255); // changes the color to white
}

void place_end1(EndPoint end[])
{
	gfx_color(0,255,255); // makes the color blue
	int i;
	int length = 30;
	int start = 535;
	int y = 550;
	for(i = 0; i < length; i++)
	{
		gfx_point(start+i,y); // places a point on the board
		end[i].x = start+i; // records the location of the point
		end[i].y = y;
	}
	gfx_color(255,255,255);	// makes the color white
}

void place_end2(EndPoint end[])
{
	gfx_color(0,255,255); // makes the color blue
	int i;
	int length = 30;
	int start = 360;
	int y = 200;
	for(i = 0; i < length; i++)
	{
		gfx_point(start+i,y);
		end[i].x = start+i;
		end[i].y = y;
	}
	gfx_color(255,255,255);	// makes the color white
}

void place_end3(EndPoint end[])
{
	gfx_color(0,255,255); // makes the color blue
	int i;
	int length = 30;
	int start = 285;
	int y = 300;
	for(i = 0; i < length; i++)
	{
		gfx_point(start+i,y);
		end[i].x = start+i;
		end[i].y = y;
	}
	gfx_color(255,255,255);	// makes the color white
}

void place1(int sz, Level lev[])
{
	gfx_color(255,0,0); // makes the color red
	int i;
	for(i = 0; i < 205; i++)
	{
		gfx_point(250,i); // places a point on the board
		lev[i].x = 250; // records the coordinates of the point just placed
		lev[i].y = i;
	}
	for(i = 205; i < 600; i++)
	{
		gfx_point(450, i);
		lev[i].x = 450;
		lev[i].y = i;
	}
	gfx_color(255,255,255); // makes the color white
}

void place2(int sz, Level lev[])
{
	gfx_color(255,0,0); // changes the color to red
	int i;
	int count = 0; // how many points have been placed so far
	for(i = 1; i <= 200; i++)
	{
		gfx_point(i,150); // places a point in the graphics window
		lev[count].x = i; // saves the x and y coordinates of the point that was just placed
		lev[count].y = 150;
		count++;
	}
	for(i = 1; i <= 500; i++)
	{
		gfx_point(300, i);
		lev[count].x = 300;
		lev[count].y = i;
		count++;
	}
	for(i = 1; i <= 200; i++)
	{
		gfx_point(100+i,300);
		lev[count].x = 100+i;
		lev[count].y = 300;
		count++;
	}
	for(i = 1; i <= 200; i++)
	{
		gfx_point(sz-i,400);
		lev[count].x = sz-i;
		lev[count].y = 400;
		count++;
	}
	for(i = 1; i <= 150; i++)
	{
		gfx_point(300+i,250);
		lev[count].x = 300+i;
		lev[count].y = 250;
		count++;
	}
	for(i = 1; i <= 100; i++)
	{
		gfx_point(450,250-i);
		lev[count].x = 450;
		lev[count].y = 250-i;
		count++;
	}
	gfx_color(255,255,255); // changes the color back to white
}

void place3(int sz, Level lev[])
{
	gfx_color(255,0,0); // makes the walls red
	int i;
	int count = 0; // keeps track of how many pixels have been placed so far
	for(i = 1; i <= 150; i++) // all of the follow loops place the walls of the maze in this level
	{
		gfx_point(i,100);
		lev[count].x = i;
		lev[count].y = 100;
		count++;
	}
	for(i = 1; i <= 250; i++)
	{
		gfx_point(350,100+i);
		lev[count].x = 350;
		lev[count].y = 100+i;
		count++;
	}
	for(i = 1; i <= 50; i++)
	{
		gfx_point(150, i+50);
		lev[count].x = 150;
		lev[count].y = i+50;
		count++;
	}
	for(i =1; i <= 250; i++)
	{
		gfx_point(i+150, 50);
		lev[count].x = i+150;
		lev[count].y = 50;
		count++;
	}
	for(i = 1; i <= 300; i++)
	{
		gfx_point(400, 50+i);
		lev[count].x = 400;
		lev[count].y = 50+i;
		count++;
	}
	for(i = 1; i <= 100; i++)
	{
		gfx_point(500, 50+i);
		lev[count].x = 500;
		lev[count].y = 50+i;
		count++;
	}
	for(i = 1; i <= 100; i++)
	{
		gfx_point(sz-i, 150);
		lev[count].x = sz-i;
		lev[count].y = 150;
		count++;
	}
	for(i = 1; i <= 100; i++)
	{
		gfx_point(400+i,250);
		lev[count].x = 400+i;
		lev[count].y = 250;
		count++;
	}
	for(i = 1; i <= 250; i++)
	{
		gfx_point(500, 250+i);
		lev[count].x = 500;
		lev[count].y = 250+i;
		count++;
	}
	for(i = 1; i <= 200; i++)
	{
		gfx_point(500-i,450);
		lev[count].x = 500-i;
		lev[count].y = 450;
		count++;
	}
	for(i = 1; i <= 150; i++)
	{
		gfx_point(250,sz-i);
		lev[count].x = 250;
		lev[count].y = sz-i;
		count++;
	}
	for(i = 1; i <= 400; i++)
	{
		gfx_point(i+50,400);
		lev[count].x = i+50;
		lev[count].y = 400;
		count++;
	}
	for(i = 1; i <= 100; i++)
	{
		gfx_point(450,400-i);
		lev[count].x = 450;
		lev[count].y = 400-i;
		count++;
	}
	for(i = 1; i <= 300; i++)
	{
		gfx_point(250,400-i);
		lev[count].x = 250;
		lev[count].y = 400-i;
		count++;
	}
	for(i = 1; i <= 250; i++)
	{
		gfx_point(350,100+i);
		lev[count].x = 350;
		lev[count].y = 100+i;
		count++;
	}
	for(i = 1; i <= 150; i++)
	{
		gfx_point(i,300);
		lev[count].x = i;
		lev[count].y = 300;
		count++;
	}
	for(i = 1; i <= 150; i++)
	{
		gfx_point(250-i,250);
		lev[count].x = 250-i;
		lev[count].y = 250;
		count++;
	}
	gfx_color(255,255,255); // changes the color back to white
}
