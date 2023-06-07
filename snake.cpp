/* snake.cpp -------
 *
 * Filename: snake.cpp
 * Description:
 * Author: Adeel Bhutta
 * Maintainer:
 * Created: Sun Sep 13 9:12:30 2022
 * Last-Updated: September 13 22:40 2022
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

/* Copyright (c) 2022 Adeel Bhutta
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */

#include <cstring>
#include <cstdbool>
#include <cstdlib>
#include "snake.hpp"
#include "key.hpp"
#include <ncurses.h>

// Initialize snake
Snake *init_snake(int x, int y)
{
  Snake *head = create_tail(x, y);
  Snake *tail1 = create_tail(x - 1, y);
  Snake *tail2 = create_tail(x - 2, y);
  tail1->next = tail2;
  head->next = tail1;
  return head;
}

// Creates one tail
Snake *create_tail(int x, int y)
{
  Snake *snake = new Snake;
  snake->color[0] = 0;
  snake->color[1] = 255;
  snake->color[2] = 0;
  /* snake->color = {0, 0, 255}; */
  snake->symbol = '#';
  snake->next = NULL;
  snake->x = x;
  snake->y = y;
  return snake;
}

// Moves the snake in the input direction
Snake *move_snake(Snake *snake, int direction)
{
  // TODO
  Snake *new_head = new Snake;

  // Set the new head to have the x and y coordinates as the existing head of the snake
  new_head->x = snake->x;
  new_head->y = snake->y;

  switch (direction)
  {
  case UP:
    new_head->y--;
    break;
  case DOWN:
    new_head->y++;
    break;
  case RIGHT:
    new_head->x++;
    break;
  case LEFT:
    new_head->x--;
    break;
  }

  // Set new head as the new head of the entire snake
  // Add all the features (color and symbol) to the new cell
  new_head->next = snake;
  new_head->color[0] = 0;
  new_head->color[1] = 0;
  new_head->color[2] = 255;
  new_head->symbol = '#';

  //  Delete the tail from the snake: HINT - there is a remove tail function below
  snake = remove_tail(snake);

  return new_head;
}

Snake *remove_tail(Snake *snake)
{
  Snake *end = snake;
  while (end->next->next)
    end = end->next;
  free(end->next);
  end->next = NULL;
  return snake;
}

// draws the snake on the board
void draw_snake(Snake *snake)
{
  while (snake)
  {
    attron(COLOR_PAIR(2)); // use green color pair
    mvprintw(snake->y, snake->x, "%c", snake->symbol);
    attroff(COLOR_PAIR(2));
    snake = snake->next;
  }
}

// checks if it eats itself, if it does, then return true
bool eat_itself(Snake *snake)
{
  // TODO for Milestone 2 only
}
