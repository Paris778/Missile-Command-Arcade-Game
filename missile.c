#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <curses.h>
#include <unistd.h>


//TITLE:    Missile Command NCURSES VERSION
//AUTHOR:   Paraskevas Solomou (34838805)
//VERSION:  Final Version as of 22/05/2019

#define numEnemy 4
#define numDef 5

//Player Structure

struct Player
{
	int xloc;
	int yloc;
	int lives;
} player;

//Enemy Rocket Structure

struct rocket
{
	int xloc;
	int yloc;
	int speed;
	bool alive;
	int xoriginal;
	int frequency;
} rocket;

//Anti-missile structure

struct defence
{
	int xloc;
	int yloc;
	int xtarget;
	int ytarget;
	int speed;
	bool alive;
	int xoriginal;
	bool cleaned;
} defence;

//////////////////////////
/////Function Prototyping

int getmv(int input, int loc);
int getmvy(int input2, int xloc);
int mvright(int loc);
int mvleft(int loc);
int mvdown(int loc);
int mvup(int loc);
//int kbhit(void);



//////////////////
//MAIN
/////////////////

int main()
{
 

	initscr(); //initialises screen (constructor)
	noecho();
	cbreak();
	start_color();
	curs_set(0);
	

	//Variables

	struct Player ship;
	ship.lives = 1;
	ship.xloc = 20;
	ship.yloc = 34;

	int x, y, begx, begy, xMax, yMax, i = 0, rocketx, j, missileID = 0, k, p, n = 0, level = 0, q = 0,speed=900;
	int score = 0, highScore = 0,targetScore=300;
	bool keepGoing = true;
	bool fire = true;
	srand(time(NULL));

	getmaxyx(stdscr, yMax, xMax);

	//Initializing the Game WIndow
	WINDOW *GameWindow = newwin(40, 80, yMax - 45, 5); //40y 80x Window
	box(GameWindow, 0, 0);
	//User INPUT
	int input;
	keypad(GameWindow, true);

	//Defining Enemy rockets
	struct rocket rocket[numEnemy];
    //Setting up Defence Rockets
	struct defence defence[numDef] = {0};

	for (n = 0; n < numDef; n++)
	{
		defence[n].alive = false;
		defence[n].cleaned = true;
	}

	//COSMETICS & COLOUR PAIRS
	attron(A_STANDOUT);
	mvwprintw(stdscr, 1, 35, "MISSILE COMMAND");
	attroff(A_STANDOUT);
	nodelay(stdscr,TRUE);
	init_pair(1,COLOR_BLUE,COLOR_BLACK);
	init_pair(2,COLOR_GREEN,COLOR_BLACK);
	init_pair(3,COLOR_RED,COLOR_BLACK);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	wattron(GameWindow,COLOR_PAIR(1));
	mvwprintw(GameWindow, 38, 1, "[][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][][]");
	wattroff(GameWindow,COLOR_PAIR(1));

	///////////////
	//GAME LOOPS
	
	
	do
	{
		ship.lives = 7;
		score = 0;
		targetScore=300;

			//Enemy Rockets Setting/Re-setting
		
			for (n = 0; n < numEnemy; n++)
			{
				struct rocket rocket[n];
				rocket[n].yloc = 3;
				rocket[n].speed = 1;
				rocket[n].xloc = (rand() % 77) + 1;
				rocket[n].xoriginal = rocket[n].xloc;
				rocket[n].alive = false;
			}
			for (n = 0; n < numEnemy; n++)
			{
				rocket[n].frequency = (rand() % 9) + 4;
			}
		
		do
		{

			//Player movement

			input = getch();
			ship.xloc = getmv(input, ship.xloc);
			mvwaddch(GameWindow, ship.yloc, ship.xloc - 1, ' ');
			mvwaddch(GameWindow, ship.yloc, ship.xloc + 1, ' ');
			ship.yloc = getmvy(input, ship.yloc);
			mvwaddch(GameWindow, ship.yloc - 1, ship.xloc, ' ');
			mvwaddch(GameWindow, ship.yloc + 1, ship.xloc, ' ');
			wattron(GameWindow,COLOR_PAIR(4));
			mvwaddch(GameWindow, ship.yloc, ship.xloc, '@');
			wattroff(GameWindow,COLOR_PAIR(4));

			////////////////////////////
			//Making defence rockets

			for (n = 0; n < numDef; n++)
			{
				
				if (input == 32 && !defence[n].alive)
				{
					defence[n].alive = true;
					defence[n].xloc = ship.xloc;
					defence[n].yloc = 35;
					defence[n].xtarget = ship.xloc;
					defence[n].ytarget = ship.yloc;
					break;
				}
			}

			//Moving and maintaining defence missiles

			for (n = 0; n < numDef; n++)
			{
				
				if (defence[n].alive)
				{
					wattron(GameWindow,COLOR_PAIR(2));
					mvwaddch(GameWindow, defence[n].yloc, defence[n].xloc, 'o');
					wattroff(GameWindow,COLOR_PAIR(2));
					defence[n].yloc--;
					        if(defence[n].xloc==defence[n].xtarget && defence[n].yloc==defence[n].ytarget)
									{
												defence[n].alive = false;
												mvwprintw(GameWindow,defence[n].ytarget,defence[n].xtarget,"*");
											//Clean the dead missile
												for (j = (defence[n].ytarget-1); j <= 36; j++)
														{
															mvwaddch(GameWindow, j, defence[n].xtarget, ' ');
														}
												
												
		
												//Double for loop to check for enemies in the blast radius 
													for(k=defence[n].xtarget-2;k<=defence[n].xtarget+2;k++)
															{   
																for(p=defence[n].ytarget-2;p<=defence[n].ytarget+2;p++)
																	{
																		for(q=0;q<numEnemy;q++)
																			{
																				if(rocket[q].xloc==k && rocket[q].yloc==p)
																					{
																						rocket[q].alive = false; 
																						score+=10; 
																						score+=(20-rocket[q].frequency);
																					}
		                                       
																			}
																	}
													 }
								
								}
				}
        
				
				

				
				refresh();
				wrefresh(GameWindow);
				
			}

			//Window info
			mvwprintw(GameWindow, 1, 1, "xloc: %d - yloc: %d - Next Level at: %5d", ship.xloc, ship.yloc,targetScore);
			mvwprintw(GameWindow, 2, 2, "Lives Remaining: %1d ", ship.lives);
			mvwprintw(GameWindow, 2, 22, "SCORE: %5d - HIGH SCORE: %6d", score, highScore);

			//////////////////
			//Enemy Rockets

			//Enemy rocket maintenence

			for (n = 0; n < numEnemy; n++)
			{
				if (!rocket[n].alive)
				{ 
					//clean dead enemy rockets
					for (j = 2; j <= 36; j++)
					{
						mvwaddch(GameWindow, j, rocket[n].xoriginal, ' ');
					}
				}

				if (i % 20 == 0 && !rocket[n].alive)
				{
					rocket[n].alive = true;
					rocket[n].xloc = (rand() % 77) + 1;
					rocket[n].yloc = 2;
					rocket[n].xoriginal = rocket[n].xloc;
					wattron(GameWindow,COLOR_PAIR(3));
					mvwaddch(GameWindow, rocket[n].yloc, rocket[n].xloc, 'V');
					wattroff(GameWindow,COLOR_PAIR(3));
				}
				int freq = rocket[n].frequency;
				if ((i % freq == 0) && rocket[n].alive)
				{
					rocket[n].yloc += 1;

					if (score > targetScore)
					{
						rocket[n].speed += 1;
						speed-=200;
						targetScore = targetScore * 2;
					}
				}

				wattron(GameWindow,COLOR_PAIR(3));
				mvwaddch(GameWindow, rocket[n].yloc, rocket[n].xloc, 'V');
				wattroff(GameWindow,COLOR_PAIR(3));

				if (rocket[n].yloc >= 34 && rocket[n].alive)
				{
					rocket[n].alive = false;
					rocket[n].yloc = 2;
					ship.lives -= 1;

					for (j = 2; j <= 36; j++)
					{
						mvwaddch(GameWindow, j, rocket[n].xoriginal, ' ');
					}
				}
				usleep(50*speed);
			}

			//Cities

			//City1
			if (ship.lives <= 4)
			wattron(GameWindow, A_INVIS);
			else wattron(GameWindow,COLOR_PAIR(1));
			mvwprintw(GameWindow, 36, 72, "[][]");
			mvwprintw(GameWindow, 37, 72, "[][]");
			wattroff(GameWindow, A_INVIS);

			//City2
			if (ship.lives <= 2)
			wattron(GameWindow, A_INVIS);
			else wattron(GameWindow,COLOR_PAIR(1));
			mvwprintw(GameWindow, 36, 62, "[][]");
			mvwprintw(GameWindow, 37, 62, "[][]");
			wattroff(GameWindow, A_INVIS);

			//City3
			if (ship.lives <= 5)
			wattron(GameWindow, A_INVIS);
			else wattron(GameWindow,COLOR_PAIR(1));
			mvwprintw(GameWindow, 36, 52, "[][]");
			mvwprintw(GameWindow, 37, 52, "[][]");
			wattroff(GameWindow, A_INVIS);

			//Missile Base
			if (ship.lives <= 0)
			wattron(GameWindow, A_INVIS);
			else wattron(GameWindow,COLOR_PAIR(1));
			mvwprintw(GameWindow, 35, 38, "[][]");
			mvwprintw(GameWindow, 36, 36, "[]]oo[[]");
			mvwprintw(GameWindow, 37, 36, "[]]oo[[]");
			wattroff(GameWindow, A_INVIS);

			//City4
			if (ship.lives <= 1)
			wattron(GameWindow, A_INVIS);
			else wattron(GameWindow,COLOR_PAIR(1));
			mvwprintw(GameWindow, 36, 4, "[][]");
			mvwprintw(GameWindow, 37, 4, "[][]");
			wattroff(GameWindow, A_INVIS);

			//City5
			if (ship.lives <= 3)
			wattron(GameWindow, A_INVIS);
			else wattron(GameWindow,COLOR_PAIR(1));
			mvwprintw(GameWindow, 36, 14, "[][]");
			mvwprintw(GameWindow, 37, 14, "[][]");
			wattroff(GameWindow, A_INVIS);

			//City6
			if (ship.lives <= 6)
			wattron(GameWindow, A_INVIS);
			else wattron(GameWindow,COLOR_PAIR(1));
			mvwprintw(GameWindow, 36, 24, "[][]");
			mvwprintw(GameWindow, 37, 24, "[][]");
			wattroff(GameWindow, A_INVIS);

			attroff(COLOR_PAIR(1));

			box(GameWindow, 0, 0);
			wrefresh(GameWindow);
			refresh();
			i++;
		} while (ship.lives > 0);

		if (score > highScore)
		{
			highScore = score;
			mvwprintw(stdscr, 45, 6, "CONGRATULATIONS !!!  NEW HIGHSCORE");
		}

        wattron(GameWindow,A_STANDOUT);
		mvwprintw(GameWindow, 20, 10, "Press '1' and ENTER to play again. Or any other key to QUIT");
		mvwprintw(GameWindow, 18, 34, "   YOU LOST   ");
		wattroff(GameWindow,A_STANDOUT);
		wrefresh(GameWindow);
		refresh();
		int go = 0;
		scanf("%d", &go);
		if (go == 1)
		{
			keepGoing = true;
			mvwprintw(GameWindow, 20, 10, "                                                                          ");
		    mvwprintw(GameWindow, 18, 25, "                                 ");
		}
		
		else keepGoing = false;
		refresh();
		wrefresh(GameWindow);

	} while (keepGoing);

	refresh();
	wrefresh(GameWindow);

	getch();
	endwin(); //de-constructor
	printf("Your highscore was: %d\n", highScore);
	return 0;
}

/////////////////////////
//Functions

//get move function x-axis
int getmv(int input2, int xloc)
{
	if (input2 == 97)
	{
		xloc = mvleft(xloc);
	}
	if (input2 == 100)
	{
		xloc = mvright(xloc);
	}
	if (input2 == 65)
	{
		xloc = mvleft(xloc);
	}
	if (input2 == 68)
	{
		xloc = mvright(xloc);
	}

	return xloc;
}

//get move function y-axis
int getmvy(int input2, int xloc)
{

	if (input2 == 119)
	{
		xloc = mvup(xloc);
	}
	if (input2 == 87)
	{
		xloc = mvup(xloc);
	}
	if (input2 == 115)
	{
		xloc = mvdown(xloc);
	}
	if (input2 == 83)
	{
		xloc = mvdown(xloc);
	}

	return xloc;
}

//Move right function
int mvright(int xloc)
{
	xloc += 1;
	if (xloc >= 79)
	{
		xloc = 78;
	}
	return xloc;
}

//Move left function
int mvleft(int xloc)
{
	xloc -= 1;
	if (xloc <= 0)
	{
		xloc = 1;
	}
	return xloc;
}

//Move up function
int mvup(int yloc)
{
	yloc -= 1;
	if (yloc <= 3)
	{
		yloc = 3;
	}
	return yloc;
}

//Move down function
int mvdown(int yloc)
{
	yloc += 1;
	if (yloc >= 34)
	{
		yloc = 34;
	}
	return yloc;
}
