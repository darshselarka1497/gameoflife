/* game.c ---------
 *
 * Filename: game.c
 * Description:
 * Author: Dmitrii Galantsev
 * Maintainer:
 * Created:      May 12 11:28 2018
 * Last-Updated: September 23 15:38 2018
 *           By: Dmitrii Galantsev
 */

/* Commentary:
 *
 *
 */

/* Copyright (c) 2018 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <ncurses.h>
#include <stdbool.h>
#include <time.h>
#include "cell.h"
#include "game.h"
#include "well.h"
#include "key.h"
#include <stdio.h>
#include <stdlib.h>

void drawShape(cell_t * cells[WELL_WIDTH-1][WELL_HEIGHT-1])
{
	bool cell = false;
	bool glider = false;
	bool oscillator = false;
    bool still = false;
	bool tub = false;
	int xCoord;
	int yCoord;
	int choice;
	
	mvprintw(5,25,"Please enter the number of the shape: \n");
									mvprintw(6,25,"1. Glider \n");
									mvprintw(7,25,"2. Oscilator \n");
									mvprintw(8,25,"3. Still Life \n");
									mvprintw(9,25,"4. Random \n");
									mvprintw(10,25,"4. Single Cell \n");
									scanw(" %d", &choice);
								
									mvprintw(5,25,"Please enter the X coordinate: \n");
									scanw(" %d", &xCoord);
									mvprintw(5,25,"Please enter the Y coordinate: \n");
									scanw(" %d", &yCoord);
									
									
									if (choice == 1){
										glider = true;
									}
									else if (choice == 2){
										oscillator = true;
									}
									else if (choice == 3){
										still = true;
									}
									else if (choice == 4){
										tub = true;
									}
									else{
										cell = true;
									}
				if (glider){
						// glider
					cells[xCoord][yCoord]->state[CURRENT] = LIVE;
					cells[xCoord][yCoord+1]->state[CURRENT] = LIVE;
					cells[xCoord][yCoord+2]->state[CURRENT] = LIVE;
					cells[xCoord+1][yCoord+2]->state[CURRENT] = LIVE;
					cells[xCoord+2][yCoord+1]->state[CURRENT] = LIVE;
					glider = false;
					}
					else if (still){
						// still life
					cells[5][5]->state[CURRENT] = LIVE;
					cells[5][6]->state[CURRENT] = LIVE;
					cells[6][5]->state[CURRENT] = LIVE;
					cells[6][6]->state[CURRENT] = LIVE;
					still = false;
						
					}
					else if (oscillator){
						// oscillator
					cells[20][20]->state[CURRENT] = LIVE;
					cells[20][21]->state[CURRENT] = LIVE;
					cells[20][22]->state[CURRENT] = LIVE;
					oscillator = false;
						
					}
					else if (tub){
						 //Tub
					cells[xCoord][yCoord]->state[CURRENT] = LIVE;
					cells[xCoord][yCoord+2]->state[CURRENT] = LIVE;
					cells[xCoord-1][yCoord+1]->state[CURRENT] = LIVE;
					cells[xCoord+1][yCoord+1]->state[CURRENT] = LIVE;
					tub = false;
					}
					else if (cell) // cell was chosen
					{
						cells[xCoord][yCoord]->state[CURRENT] = LIVE;
						cell = false;
					}
	
	
	
}


void writeFile(cell_t * cells[WELL_WIDTH-1][WELL_HEIGHT-1])
{
    struct stat st = {0};
   
    if (stat("./saves", &st) == -1) {
        mkdir("./saves", 0700);
    }//make dir if the /saves folder does not exist

    int fcount = 0;
    DIR * dir;
    struct dirent * entry;
    dir = opendir("./saves");
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            fcount++;
        }
    }
    closedir(dir);

    int num = fcount % 10;
    
    char name [20];
    sprintf(name,"./saves/save_%d.life", num);
    char mode[] = "w";
    FILE *fp = fopen(name,mode); // open file


    int i;
    int j;

    for(i = 0; i < (WELL_HEIGHT-1); i++){
        for(j = 0; j < (WELL_WIDTH-1); j++){
            if(cells[i][j]->state[CURRENT] == LIVE){
                fprintf(fp,"1");
            }
            else {
                fprintf(fp,"0");
            }
        }
        fprintf(fp,"\n");
    }  

    fclose(fp);
}

void loadFile(cell_t * cells[WELL_WIDTH-1][WELL_HEIGHT-1], char s)
{

    struct stat st = {0};

    if (stat("./saves", &st) == -1) {
        mkdir("./saves", 0700);
    }

    int fcount = 0;
    DIR * dir;
    struct dirent * entry;

    dir = opendir("./saves"); 
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            fcount++;
        }
    }
    closedir(dir);

    if (fcount > s){
        return;
    }
    mvprintw(9, 20, "two \n");

    char name [20];
    sprintf(name,"./saves/save_%d.life", fcount);
    char mode[] = "w";
    FILE *fp = fopen(name,mode);

    mvprintw(10, 20, "File Open\n");
    int i;
    int j;

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
	
    for(i = 0; i < (WELL_HEIGHT-1); i++){
        read = getline(&line, &len, fp);
        for(j = 0; j < (WELL_WIDTH-1); j++){

            if(line[j] == "1"){
                cells[i][j]->state[CURRENT] = LIVE;
            }
            else {
                cells[i][j]->state[CURRENT] = DIE;
            }

        }
        fprintf(fp,"\n");
    }

    fclose(fp);
    mvprintw(11, 20, "File Closed!! \n");
}//loadFile ends

   

void init_game(void)
{

    int x,y;
}

int game(void)
{


    static int state = INIT;
    bool isRandom = false;
    struct timespec tim = {0,1000000};  // Each execution of while(1) is approximately 1mS
    struct timespec tim_ret;

    cell_t * cells[WELL_WIDTH-1][WELL_HEIGHT-1];

    well_t *w;

    int x_offset, y_offset;
    int x_max, y_max;
    int arrow;
    int move_counter = 0;
    int move_timeout = BASE_TIMEOUT;

    int test = 0;

    int xCoord,yCoord;
    int start = 0;

    while(1) {
        switch(state) {
        case INIT:
            initscr();
            int i;
            while(i == 0)
            {
                // Print Title Screen
                mvprintw(1,50,"Welcome to game of life by Conway\n");
                mvprintw(4,50,"Would you like to start with a random shape (r) or predefined shape (p)? \n");
                if (read_escape(&arrow))
				{
                if (arrow == 'r' || arrow == 'R')
                {
                    isRandom = true;
                }
				}
                mvprintw(5,50,"Please enter the X coordinate: \n");
                scanw(" %d", &xCoord);
                mvprintw(5,50,"Please enter the Y coordinate: \n");
                scanw(" %d", &yCoord);
				mvprintw(6,50,"Press 's' to start the game: \n");
				


                if (read_escape(&arrow) == REGCHAR) // get character
                {
                    if (arrow == 's' || arrow == 'S')
                    {
                    	clear();
                        i = 1;
                    }
                }

            }


            nodelay(stdscr, TRUE);   // Do not wait for characters using getch.
            noecho();                // Do not echo input characters
            getmaxyx(stdscr, y_max, x_max);  // Get the screen dimensions

            x_offset = (x_max / 2) - (WELL_WIDTH / 2);
            y_offset = (y_max / 2) - (WELL_HEIGHT / 2);

            int j;
            for(i = 0; i < (WELL_WIDTH-1); i++){
                for(j = 0; j < (WELL_HEIGHT-1); j++){
                    cells[i][j] = create_cell(x_offset + i, y_offset + j);
                }
            }

            if (isRandom){

                //Tub
                cells[xCoord][yCoord]->state[CURRENT] = LIVE;
                cells[xCoord][yCoord+2]->state[CURRENT] = LIVE;
                cells[xCoord-1][yCoord+1]->state[CURRENT] = LIVE;
                cells[xCoord+1][yCoord+1]->state[CURRENT] = LIVE;
            }
            else{
            // glider
                cells[xCoord][yCoord]->state[CURRENT] = LIVE;
                cells[xCoord][yCoord+1]->state[CURRENT] = LIVE;
                cells[xCoord][yCoord+2]->state[CURRENT] = LIVE;
                cells[xCoord+1][yCoord+2]->state[CURRENT] = LIVE;
                cells[xCoord+2][yCoord+1]->state[CURRENT] = LIVE;
            }

            // still life
            cells[5][5]->state[CURRENT] = LIVE;
            cells[5][6]->state[CURRENT] = LIVE;
            cells[6][5]->state[CURRENT] = LIVE;
            cells[6][6]->state[CURRENT] = LIVE;

            // glider
            cells[20][5]->state[CURRENT] = LIVE;
            cells[20][6]->state[CURRENT] = LIVE;
            cells[20][7]->state[CURRENT] = LIVE;
            cells[21][7]->state[CURRENT] = LIVE;
            cells[22][6]->state[CURRENT] = LIVE;

            // oscillator
            cells[20][20]->state[CURRENT] = LIVE;
            cells[20][21]->state[CURRENT] = LIVE;
            cells[20][22]->state[CURRENT] = LIVE;

            //cells[22][20]->state[CURRENT] = LIVE;
            //cells[23][20]->state[CURRENT] = LIVE;
            //cells[23][18]->state[CURRENT] = LIVE;
            //cells[25][19]->state[CURRENT] = LIVE;
            //cells[26][20]->state[CURRENT] = LIVE;
            //cells[27][20]->state[CURRENT] = LIVE;
            //cells[28][20]->state[CURRENT] = LIVE;


            w = init_well(x_offset - 1, y_offset - 1, WELL_WIDTH, WELL_HEIGHT);
            draw_well(w);

            state = STEP;
            break;

        case STEP:
            if (move_counter > move_timeout) {
                mvprintw(1, 50, "cells[6][6] state[old]:%d\n", cells[6][6]->state[OLD]);
                mvprintw(2, 50, "cells[6][6] state[current]:%d\n", cells[6][6]->state[CURRENT]);
                update_neighbours(WELL_WIDTH - 1, WELL_HEIGHT - 1, cells);
                mvprintw(3, 50, "cells[6][6] state[new]:%d\n", cells[6][6]->state[NEW]);
                update_cells(WELL_WIDTH - 1, WELL_HEIGHT - 1, cells);
                display_cells(WELL_WIDTH - 1, WELL_HEIGHT - 1, cells);
                move_counter = 0;
            }
            move_counter++;
            int ch;
            if (read_escape(&arrow) == REGCHAR){

                if (arrow == 'q' || arrow == 'Q'){
                    state = GAME_OVER;
                }
                else if (arrow == '*'){
                    move_timeout = move_timeout/ 1.2;
                }

                else if (arrow == '/'){
                    move_timeout = move_timeout * 1.2;
                }

                else if (arrow == '+'){
                    if (w->width < 70 && w->height < 70){
                        undraw_well(w);
                        w = changeWellSize(w->upper_left_x - 1, w->upper_left_y - 1, w->width +2, w->height+2, w);
                        draw_well(w);
                    }

                }
                else if (arrow == '-'){
                    if (w->width > 25 && w->height > 25){
                        undraw_well(w);
                        w = changeWellSize(w->upper_left_x + 1, w->upper_left_y + 1, w->width - 2, w->height - 2, w);
                        draw_well(w);
                    }

                }
                else if (arrow == 'r' || arrow == 'R'){
                    clear();
                    state = INIT;
                }


                else if (arrow == 'p' || arrow == 'P'){
					char c[6][20] = {"Resume", "Place Shape", "Save File", "Load File", "Help", "Quit"};
					int pos = 0;
					bool resume = false;
					int choice;
					do{
						int i;
						mvprintw(4,20,"Menu: \n");
						for (i = 0; i < 7; i++){
							if (pos == i){
								attron(A_STANDOUT);
								start_color();
								init_pair(2,3,0);
								attron(COLOR_PAIR(2));
								mvprintw(i ,20," %s ", c[i]);
								attroff(A_STANDOUT);
								attroff(COLOR_PAIR(2));
							}
							else{
								start_color();
								init_pair(1,7,0);
								attron(COLOR_PAIR(1));
								mvprintw(i,20," %s ", c[i]);
								attroff(COLOR_PAIR(1));
							}
					
						}

						int input = read_escape(&arrow);
						if (input == UP){
							if (pos > 0){
								pos--;
							}
						}
						else if (input == DOWN){
							if (pos < 5){
								pos++;
							}
						}
						else if (input == REGCHAR){
							if (arrow == '\n'){
								switch (pos){
									case 0:
									for (i = 0; i < 7; i++){
										mvprintw(i,20,"                    ");
									}
									
									for (i = 8; i < 19; i++){
										mvprintw(i,25,"                        ");
									}
									resume = true;

									break;
									
									case 1:
									
									drawShape(cells);
									break;
									
									case 2:

									writeFile(cells);
									mvprintw(10,30,"File Saved! ");
									break;
									
									
									case 3:
									
									mvprintw(4,50,"What file number would you like to open? \n");
									do {
									ch = read_escape(&arrow);
									} while (! ch == REGCHAR);
									loadFile(cells, ch);
									mvprintw(4,50,"File loaded \n");
									
									break;
									
									case 4:
					
									mvprintw(8,25,"Keybindings");
									mvprintw(9,25, "P to pause");
									mvprintw(10,25, "Q to quit");
									mvprintw(11,25, "F to save file");
									mvprintw(12,25, "L to load file");
									mvprintw(13,25, "+ to increase board size");
									mvprintw(14,25, "- to decrease board size");
									mvprintw(15,25, "* to speed up game");
									mvprintw(16,25, "/ to slow down game");
									mvprintw(17,25, "g to display GNU");
									mvprintw(18,25, "r to reset the board");
									
									pos = 0;
									
									
									break;
									case 5:
									endwin();
									return(0);
									break;
									
									
									
								}// end switch case
							}
						}
                      //  ch = read_escape(&arrow);
						
					//}

            
                    } while (!resume);
					
				}

                else if (arrow == 'f' || arrow == 'F'){
                    writeFile(cells);
                }// file case ends

                else if (arrow == 'l' || arrow == 'L'){

                    mvprintw(4,50,"What file number would you like to open? \n");
                    do {
                        ch = read_escape(&arrow);
                    } while (! ch == REGCHAR);
                    loadFile(cells, ch);

                }
				
		else if (arrow == 'g' || arrow == 'G'){
		int i;
		int j;
                for (i = 0; i < WELL_WIDTH-1; i++){
	              for (j = 0; j < WELL_HEIGHT-1; j++){
				cells[i][j]->state[CURRENT] = DIE;
		      }
		}
					
                    //G
					
					cells[0][0]->state[CURRENT] = LIVE;
					cells[0][1]->state[CURRENT] = LIVE;
					cells[0][2]->state[CURRENT] = LIVE;
					cells[0][3]->state[CURRENT] = LIVE;
					cells[0][4]->state[CURRENT] = LIVE;
					cells[1][0]->state[CURRENT] = LIVE;
					cells[2][0]->state[CURRENT] = LIVE;
					cells[3][0]->state[CURRENT] = LIVE;
					cells[4][0]->state[CURRENT] = LIVE;
					cells[1][4]->state[CURRENT] = LIVE;
					cells[2][4]->state[CURRENT] = LIVE;
					cells[3][4]->state[CURRENT] = LIVE;
					cells[4][4]->state[CURRENT] = LIVE;
					cells[5][2]->state[CURRENT] = LIVE;
					cells[4][3]->state[CURRENT] = LIVE;
					cells[3][2]->state[CURRENT] = LIVE;
					cells[2][2]->state[CURRENT] = LIVE;
					
				}



            } 

            break;


        case EXIT:
            endwin();
            return(0);
            break;

        case GAME_OVER:
            clear();
            mvprintw(2,50,"All Life Has Been Terminated \n");
            mvprintw(3,50,"Press any key to end the game! \n");
            int c;
            if ((c = getch()) != ERR){
                state = EXIT;
            }
            break;

        }
        refresh();
        nanosleep(&tim,&tim_ret);
    }



}
