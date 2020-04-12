/* cell.c ---------
 *
 * Filename: cell.c
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
 *
 */

/* Copyright (c) 2018 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

/* Code: */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>
#include "cell.h"

#define DRAW_CHAR '+'

cell_t * create_cell (int init_x, int init_y)
{
    // create the cell
    cell_t * c = malloc(sizeof(cell_t));
    c->x = init_x;
    c->y = init_y;
    c->draw_char = DRAW_CHAR;

    c->state[OLD] = UNKNOWN;
    c->state[CURRENT] = DIE;
    c->state[NEW] = UNKNOWN;

    return(c);
}

int destroy_cell (cell_t * c){
    free(c);

    return SUCCESS;
}

int update_cell (cell_t * c){
    c->state[OLD] = c->state[CURRENT];
    c->state[CURRENT] = c->state[NEW];
    c->state[NEW] = UNKNOWN;

	if (c->state[OLD] == DIE){
		attron(A_STANDOUT);
		start_color();
		init_pair(2,3,0);
		attron(COLOR_PAIR(2));
		
	}
	else{
		attron(A_STANDOUT);
		start_color();
		init_pair(2,2,8);
		attron(COLOR_PAIR(2));
		
	}

    return SUCCESS;
}

// updates all of the cell states
void update_cells (int c_size_x, int c_size_y, cell_t * c[c_size_x][c_size_y])
{
    int x, y;
    for (x = 0; x < c_size_x; x++)
        for (y = 0; y < c_size_y; y++)
            update_cell(c[x][y]);
}

void print_cell (cell_t * c)
{
    printf("cell->x: %d\t cell->y: %d\t cell->draw_char %c\n", c->x, c->y, c->draw_char);
    printf("cell->state[0]: %d\t cell->state[1]: %d\t cell->state[2]: %d\n",
           c->state[OLD], c->state[CURRENT], c->state[NEW]);
}

void display_cell (cell_t * c)
{
    if (c->state[CURRENT] == LIVE)
        mvprintw(c->y, c->x, "%c", c->draw_char);
    else
        mvprintw(c->y, c->x, " ");
}

// display all the cells
void display_cells (int c_size_x, int c_size_y, cell_t * c[c_size_x][c_size_y])
{
    int x, y;
    for (x = 0; x < c_size_x; x++)
        for (y = 0; y < c_size_y; y++)
            display_cell(c[x][y]);
}

// updates the NEW state in a given cell
void update_cell_state (cell_t * c, int neighbours)
{

    if (c->state[CURRENT] != LIVE) {
        if (neighbours == 3){
            c->state[NEW] = LIVE;
        }
    }
    else if (c->state[CURRENT] == LIVE) {
        if (neighbours < 2){
            c->state[NEW] = DIE;
        }
        else if (neighbours > 3){
            c->state[NEW] = DIE;
        }
		
		else{
		c->state[NEW] = LIVE;
	}
    }
	
	

}

void undisplay_cell (cell_t * c)
{
    mvprintw(c->y, c->x, " ");
}

// count neighbours of each cell and update the state
void update_neighbours (int c_size_x, int c_size_y, cell_t * c[c_size_x][c_size_y])
{
    int neighbours;

    int x, y;
    for (x = 0; x < c_size_x-1; x++) {
        for (y = 0; y < c_size_y-1; y++) {
            neighbours = 0;

            if (x - 1 >= 0) {
                if (y - 1 >= 0){
                    neighbours += c[x-1][y-1]->state[CURRENT] == LIVE ? 1 : 0;
                }

                neighbours += c[x-1][y]->state[CURRENT] == LIVE ? 1 : 0;

                if (y <= c_size_y){
                    neighbours += c[x-1][y+1]->state[CURRENT] == LIVE ? 1 : 0;
                }
            }

            if (y - 1 >= 0){
                neighbours += c[x][y-1]->state[CURRENT] == LIVE ? 1 : 0;
            }
            if (y <= c_size_y){
                neighbours += c[x][y+1]->state[CURRENT] == LIVE ? 1 : 0;
            }

            if (x <= c_size_x) {
                if (y - 1 >= 0){
                    neighbours += c[x+1][y-1]->state[CURRENT] == LIVE ? 1 : 0;
                }

                neighbours += c[x+1][y]->state[CURRENT] == LIVE ? 1 : 0;

                if (y <= c_size_y){
                    neighbours += c[x+1][y+1]->state[CURRENT] == LIVE ? 1 : 0;
                }
            }

            update_cell_state(c[x][y], neighbours);
        }
    }
}
