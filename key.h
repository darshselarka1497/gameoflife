/* key.h ---------
 *
 * Filename: key.h
 * Description:
 * Author: Bryce Himebaugh
 * Maintainer:
 * Created: Thu Sep 15 16:36:21 2017
 * Last-Updated: September 23 15:38 2018
 *           By: Dmitrii Galantsev
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 *
 */

/* Copyright (c) 2016 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

enum {NOCHAR, REGCHAR, UP, DOWN, LEFT, RIGHT, BADESC};

int read_escape(int *);
