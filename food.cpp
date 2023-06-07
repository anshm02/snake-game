/*food.cpp -------
*
* Filename: food.cpp
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

#include <cstdlib>
#include <ncurses.h>
#include <cstdbool>
#include "food.hpp"



//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = (Food*)malloc(sizeof(Food));
    int rand_m = rand() % 2;
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
        if (rand_m == 0) { 
            new_food->type = '+';
        } else { 
            new_food->type = '*';
        }
    }
    else if(type == Decrease){
        if (rand_m == 0) { 
            new_food->type = '-';
        } else { 
            new_food->type = '/';
        }
    }
    else if(type == Obstacle) { 
        new_food->type = '$';
    }
    new_food->next = NULL;

    return new_food;
}

//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}

enum Type food_type(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y) {
            if (temp->type == '+' || temp->type == '*') { 
                return Increase;
            } else if (temp->type == '-' || temp->type == '/') { 
                return Decrease;
            } else if (temp->type == '$') { 
                return Obstacle;
            }
        }

        temp = temp->next;
    }
}


Food* remove_eaten_food(Food* foods, int x, int y){
	Food* prev = NULL;
    Food* curr = foods;

    while(curr){
        if(curr->x == x && curr->y == y){
            if (curr->type == Obstacle) { 
                break;
            }
            if(prev == NULL){
                foods = curr->next;
            }
            else{
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    return foods;
}
// Display all the food
void draw_food (Food *foods)
{   start_color(); 

    init_color(COLOR_RED, 1000, 0, 0);
    init_color(COLOR_GREEN, 0, 1000, 0);
    init_color(COLOR_BLUE, 0, 0, 1000);
    init_color(COLOR_YELLOW, 1000, 1000, 0);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    
    
    Food* temp = foods;
    while(temp) {
        switch (temp->type) {
            case '+':
                attron(COLOR_PAIR(1));
                mvprintw(temp->y, temp->x, "%c", temp->type);
                attroff(COLOR_PAIR(1));
                break;
            case '-':
                attron(COLOR_PAIR(2));
                mvprintw(temp->y, temp->x, "%c", temp->type);
                attroff(COLOR_PAIR(2));
                break;
            case '*':
                attron(COLOR_PAIR(3));
                mvprintw(temp->y, temp->x, "%c", temp->type);
                attroff(COLOR_PAIR(3));
                break;
            case '/':
                attron(COLOR_PAIR(4));
                mvprintw(temp->y, temp->x, "%c", temp->type);
                attroff(COLOR_PAIR(4));
                break;
            default:
                mvprintw(temp->y, temp->x, "%c", temp->type);
                break;
        }
        temp = temp->next;
    }
}
