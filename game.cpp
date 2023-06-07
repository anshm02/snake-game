/*game.cpp -------
*
* Filename: game.cpp
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
#include <ncurses.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include "snake.hpp"
#include "food.hpp"
#include "game_window.hpp"
#include "key.hpp"
#include "game.hpp"
#include <fstream>
#include <queue>


void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}

struct Score {
    int value;
    std::string name;
};

bool operator<(const Score& s1, const Score& s2) {
    return s1.value < s2.value;
}

void save_best_scores(int score) {
    // Step 1: Read the current scores from the file
    std::priority_queue<Score> scores;
    std::ifstream infile("./saves/save_best_10.game");
    if (infile.is_open()) {
        std::string line;
        while (std::getline(infile, line)) {
            int value = std::stoi(line);
            scores.push({value, ""});
            if (scores.size() > 10) {
                scores.pop();
            }
        }
        infile.close();
    }

    // Step 2: Add the new score to the scores
    scores.push({score, ""});
    if (scores.size() > 10) {
        scores.pop();
    }

    // Step 3: Write the new scores to the file
    std::ofstream outfile("./saves/save_best_10.game");
    if (outfile.is_open()) {
        while (!scores.empty()) {
            outfile << scores.top().value << "\n";
            scores.pop();
        }
        outfile.close();
    } else {
        // Handle error
    }
}


void game(){
    
    enum State state = INIT; // Set the initial state
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    gamewindow_t *starting_window;
    Snake *snake; // The snake
    Snake *current_snake;
    Food *foods,*new_food; // List of foods (Not an array)
    Food *current_food;
    Snake *addToSnake;
    bool paused = false;
    bool increaseSpeed = true;

    const int height = 30; 
    const int width = 70;
    char ch;
    char c;
    int snake_length = 3;
    int direction = rand() % 2 == 0 ? LEFT : RIGHT;
    int Score = 0;
    int food_count = 0;
    int speed_count = 0;
    int lives = 3;
    int rand_o;
    int rand_b;

    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;

    while(state != EXIT){
        switch(state){
        case INIT:
            initscr();
            start_color();
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // making keys work
            curs_set(0); // hide cursor
            timeout(100);

            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
            
            //Init board
            window = init_GameWindow(x_offset, y_offset, width, height);

            c = get_char();
            while (c != 'e' && c != 'h' && c != 'm') {
                mvprintw(4,14, "Hello Welcome to the snake game!\n\n");
                mvprintw(6,4, "To gain 20 points, you must eat the increasing food '+' and '*'\n\n");
                mvprintw(8,10, "Make sure to avoid the decreasing food '-' and '/'.\n\n");
                mvprintw(5,10, "To end the game, enter 'q/Q\n\n");
                mvprintw(8,10, "To start the game enter either 'e' for easy difficulty\n\n");
                mvprintw(9,10, "To start the game enter either 'm' for medium difficulty\n\n");
                mvprintw(10,10, "To start the game enter either 'h' for hard difficulty\n\n");
                
                draw_Gamewindow(window);;
                c = get_char();
                
            }
            

            if (c == 'e') { 
                food_count = 10;
                speed_count = 30;
            } else if (c == 'm') { 
                food_count = 20;
                speed_count = 20;
            } else { 
                food_count = 25;
                speed_count = 15;
            }


            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
            
            // Init foods
            int food_x, food_y, i, n, p;
            enum Type type;
            rand_o = rand() % 7;

            //Generate 20 foods
            generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
            type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
            foods = create_food(food_x, food_y, type);
            for(i = 1; i < food_count; i++){
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y))
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }

            for(n = 1; n < rand_o + 3; n++) {
                generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                while (food_exists(foods,food_x, food_y))
                    generate_points(&food_x, &food_y, width, height, x_offset, y_offset);
                type = Obstacle;
            
                rand_b = rand() % 6;

                if (rand_b == 1) { 
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food); 
                }

                if (rand_b == 2) { 
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+1, food_y, type);
                    add_new_food(foods, new_food); 
                }

                if (rand_b == 3) { 
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+1, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+2, food_y, type);
                    add_new_food(foods, new_food);
                }

                if (rand_b == 4) { 
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+1, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+2, food_y, type);
                    add_new_food(foods, new_food);
                    new_food = create_food(food_x+3, food_y, type);
                    add_new_food(foods, new_food);
                }

                if (rand_b == 5) { 
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+1, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+2, food_y, type);
                    add_new_food(foods, new_food);
                    new_food = create_food(food_x+3, food_y, type);
                    add_new_food(foods, new_food);
                    new_food = create_food(food_x+4, food_y, type);
                    add_new_food(foods, new_food);
                }

                if (rand_b == 6) { 
                    new_food = create_food(food_x, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+1, food_y, type);
                    add_new_food(foods, new_food); 
                    new_food = create_food(food_x+2, food_y, type);
                    add_new_food(foods, new_food);
                    new_food = create_food(food_x+3, food_y, type);
                    add_new_food(foods, new_food);
                    new_food = create_food(food_x+4, food_y, type);
                    add_new_food(foods, new_food);
                    new_food = create_food(food_x+5, food_y, type);
                    add_new_food(foods, new_food);
                }
                

                
            }

            state = ALIVE;
            break;

        case ALIVE:
            ch = get_char();

            if (ch == 'p' || ch == 'P') {
                paused = !paused;
                nodelay(stdscr, !paused);
            }
            
            if (!paused) {
                /* Write your code here */
                // Update the direction of the snake based on the user input
                switch (ch)
                {
                    case UP:
                        if (direction != DOWN) direction = UP;
                        break;
                    case DOWN:
                        if (direction != UP) direction = DOWN;
                        break;
                    case RIGHT:
                        if (direction != LEFT) direction = RIGHT;
                        break;
                    case LEFT:
                        if (direction != RIGHT) direction = LEFT;
                        break;
                    case 'q': 
                    case 'Q': 
                        state = EXIT;
                        break;  
                    
                }

                snake = move_snake(snake, direction);
            }

            if (state != EXIT){

                if (snake->x <= x_offset || snake->x >= x_offset + width || snake->y <= y_offset
                || snake->y >= y_offset + height) { 
                    if (lives > 1) { 
                        lives -= 1;
                        snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
                        
                    } else { 
                        save_best_scores(Score);
                        window = init_GameWindow(x_offset, y_offset, width, height);
                        c = get_char();
                        while (c != 'q') {
                            mvprintw(3,15, "Game Over!\n\n");
                            mvprintw(4, 15, "Your Final Score: %d", Score);
                            draw_Gamewindow(window);;
                            c = get_char();   
                        }
                        state = EXIT; 
                        break;
                    }
                } 

                current_snake = snake->next;

                while (current_snake != NULL) { 
                    if (current_snake->x == snake->x && current_snake->y == snake->y) { 
                        if (lives > 1) { 
                            lives -= 1;
                            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
                        } else { 
                            save_best_scores(Score);
                            c = get_char();
                            window = init_GameWindow(x_offset, y_offset, width, height);
                            while (c != 'q') {
                                mvprintw(3, 15, "Game Over!\n\n");
                                mvprintw(4, 15, "Your Final Score: %d", Score);
                                draw_Gamewindow(window);;
                                c = get_char();   
                            }
                            state = EXIT; 
                            break;
                        }
                    }
                    current_snake = current_snake->next;
                }


                current_food = foods;
                while (current_food != NULL) {
                    if (snake->x == current_food->x && snake->y == current_food->y) {
                        if (food_type(foods, current_food->x, current_food->y) == Increase) {
                            addToSnake = create_tail(snake->x, snake->y);
                            Snake *current = snake;
                            while (current->next != NULL) {
                                current = current->next;
                            }
                            current->next = addToSnake;
                            Score += 20;
                            snake_length += 1;
                        } else if (food_type(foods, current_food->x, current_food->y) == Decrease) {
                            snake = remove_tail(snake);
                            Score -= 10;
                            snake_length -= 1;

                        } else if (food_type(foods, current_food->x, current_food->y) == Obstacle) { 
                            if (lives > 1) { 
                                lives -= 1;
                                snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
                            } else { 
                                window = init_GameWindow(x_offset, y_offset, width, height);
                                save_best_scores(Score);
                                c = get_char();
                                window = init_GameWindow(x_offset, y_offset, width, height);
                                while (c != 'q') {

                                    mvprintw(3, 15, "Game Over!\n\n");
                                    mvprintw(4, 15, "Your Final Score: %d", Score);
                                    draw_Gamewindow(window);;
                                    c = get_char();
                                    
                                }
                                state = EXIT; 
                                break;
                            }
                        }
                        if (snake_length == 1) { 
                            state = EXIT;
                            break;
                        }   
                        remove_eaten_food(foods, current_food->x, current_food->y);
                        int new_food_x, new_food_y;
                        generate_points(&new_food_x, &new_food_y, width, height, x_offset, y_offset);
                        Type new_food_type = (rand() > RAND_MAX/2) ? Increase : Decrease;
                        add_new_food(foods, create_food(new_food_x, new_food_y, new_food_type));
                    }
                    current_food = current_food->next;
                }
                
            }

            if (snake_length == 1) { 
                state = EXIT;
            } 

            if (Score > 60) { 
                timeret.tv_nsec /= speed_count;
            } else if (Score > 100) { 
                timeret.tv_nsec /= speed_count;
            } else if (Score > 150) { 
                timeret.tv_nsec /= speed_count;
            }

			if (snake_length != 1) { 
                clear();
                mvprintw(20,20, "Key entered: %c", ch);
                mvprintw(2, 5, "Score: %d", Score);
                mvprintw(1, 5, "Lives: %d", lives);
                draw_Gamewindow(window);
                draw_snake(snake);
                draw_food(foods);
                break;
            }

        case EXIT:
            endwin();
            break;
            
        }
        refresh();
        nanosleep(&timeret, NULL);
    }
}
