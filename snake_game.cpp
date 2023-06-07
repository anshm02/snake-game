/*snake_hame.cpp -------
*
* Filename: snake_game.cpp
* Description:
* Author: Adeel Bhutta
* Maintainer:
* Created: Sat Sep 12 13:16:12 2022
* Last-Updated: September 12 16:51 2022
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

/*Copyright (c) 2022 Adeel Bhutta
*
* All rights reserved.
*
* Additional copyrights may follow
*/
#include "game.hpp"
#include <cstdlib>
#include <cstdio>
#include "game_window.hpp"
#include <ncurses.h>

int main(void){
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    const int height = 30; 
    const int width = 70;
    gamewindow_t *window; // Name of the board


    getmaxyx(stdscr, y_max, x_max);
    // Setting height and width of the board
    x_offset = (x_max / 2) - (width / 2);
    y_offset = (y_max / 2) - (height / 2);

    window = init_GameWindow(x_offset, y_offset, width, height);

    printf("thuoeou");
    game();
    printf("tuenao");
    return(0);
}
