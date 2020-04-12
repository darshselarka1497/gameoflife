/* game.h ---------
 *
 * Filename: game.h
 * Description:
 * Author: Dmitrii Galantsev
 * Maintainer:
 * Created:      May 12 11:28 2018
 * Last-Updated: September 23 15:38 2018
 *           By: Dmitrii Galantsev
 */

/* Commentary:
 *
 * Implimentation based on Bruce Himebaugh's version for Tetris for C291 (2016)
 *
 */

/* Copyright (c) 2018 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

typedef struct terminal_dimensions
{
  int maxx;
  int maxy;
} terminal_dimensions_t;

// Delay timers for the main game loop.
#ifndef DELAY_US
#define DELAY_US 100000
#define BASE_TIMEOUT 150
#endif

// Game States
enum {INIT, STEP, EXIT, GAME_OVER};

void init_game(void);
void end(void);
int game(void);

/* game.h ends here */
