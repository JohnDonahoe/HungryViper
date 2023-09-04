//=================================================================
// The main program file.
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// PROJECT INCLUDES
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "viper.h"

#include <math.h>
#include <stdio.h>
#include <ctime>

#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define INVINCIBLE 99

#define BLACK 0x000000
#define WHITE 0xFFFFFF
#define GREEN 0x00FF00
#define RED 0xFF0000
#define BROWN 0x964B00

// Important definitions


/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Viper viper;
static int viper_point_multiplier = 0;
static int viper_point_demultiplier = 0;

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
void playSound(char* wav);
int get_action(GameInputs inputs);
int update_game(int action);
int get_object(int xyz);
void draw_upper_status();
void draw_lower_status();
void draw_border();
void draw_game(int draw_option);
void init_main_map();

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION DEFINITIONS
/**
 * Program entry point! This is where it all begins.
 * This function is for all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    uLCD.cls();
    uLCD.printf("\n\nJohn's Viper Game!\n\nGood Luck!\nPress Start."); 
    while (1) {
        GameInputs inputs = read_inputs();
        int action = get_action(inputs);
        if (action == 11) {
            break;
        }
    }
    
    viper_init(&viper);
    
    // 0. Initialize the maps
    // TODO: implement maps_init() and init_main_map() function in map.cpp:
    maps_init();
    init_main_map();

    // Initialize game state
    set_active_map(0);
    viper.head_x = viper.head_y = 5;
    
    // Initial drawing
    draw_game(FULL_DRAW);
        
    
    
    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // 1. Read inputs 
        //TODO: implement read_inputs() function in hardware.cpp:
        GameInputs inputs = read_inputs();
        
        // 2. Determine action (move, act, menu, etc.)
        //TODO: implement get_action() function:
        int action = get_action(inputs);
        int tempPause = 0;
        if (action == 11) {
            uLCD.cls();
            uLCD.printf("\n\n Game Paused \n\n Press Start\n To Resume");
            wait(1);
            while (1) {
                GameInputs inputs = read_inputs();
                int action = get_action(inputs);
                if (action == 11 && tempPause > 0) {
                    break;
                }
                tempPause++;
            }
        }
        // 3. Update game
        //TODO: implement update_game() function:
        int result = update_game(action);
        
        int abc = get_object(result);
        
        if (inputs.b1 == 0) {
            viper.INVINCIBL = true;
        }
        if (viper_point_multiplier > 0) {
            viper_point_multiplier--;
        }
        if (viper_point_demultiplier > 0) {
            viper_point_demultiplier--;
        }
        
        if (result == GAME_OVER && !viper.INVINCIBL) {
            uLCD.cls();
            uLCD.printf("GAME OVER");
            while (true) {
                wait(1);
            }
        }
        if (viper.length == 50) {
            uLCD.cls();
            uLCD.printf("YOU WON!");
            while (true) {
                wait(1);
            }
        }
        
        
        
        

        uLCD.filled_rectangle(0, 0, 160, 7, BLACK);
        char str[1024];
        snprintf(str,sizeof(str),"Pos:%d,%d Score:%d",viper.head_x,viper.head_y,viper.score);
        uLCD.text_string(str,0,0,FONT_7X8,RED);
        
        // 3b. Check for game over based on result
        // and if so, handle game over

        //TODO: implement this here or as a new function.
        
        //      3b1. if game is over, then
        //      3b2. draw and display tezt for game over sign
                
        // 4. Draw screen -- provided.
        draw_game(result);
        
        // Compute update time
        t.stop();
        int dt = t.read_ms();

        // Display and wait
        // NOTE: Text is 8 pixels tall
        if (dt < 100){
        wait_ms(100 - dt);
        }
    }
}

/**
 * Plays a .wav file
 */
void playSound(char* wav)
{
    // OPTIONAL: Implement
    
}

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
int get_action(GameInputs inputs)
{
    // TODO: Implement
    if (inputs.b1 == 0) {
        return INVINCIBLE;
    }
    if (inputs.b2 == 0) {
        return 11;
    }
    if (inputs.ns_up == true) {
        return GO_UP;
    } else if (inputs.ns_right == true) {
        return GO_RIGHT;
    } else if (inputs.ns_down == true) {
        return GO_DOWN;
    } else if (inputs.ns_left == true) {
        return GO_LEFT;
    }
    
    // 1. Check your action and menu button inputs and return the corresponding action value
    
    // 2. Check for your navigation switch inputs and return the corresponding action value
    
    // If no button is pressed, just return no action value
    return NO_ACTION;
}

/**
 * This function is called by update game to check when the viper 
 * gets an object (in other words, when a viper eats an object). We 
 * update the viper and game conditions as needed based on the object type
 */
int get_object(int xyz){
    
    //TODO: Implement
    
    
    //MapItem *currr = get_here(viper -> locations[0].x, viper -> locations[0].y);
    if (xyz == GO_LEFT) {
            
            viper.head_px = viper.head_x;
            viper.head_py = viper.head_y;
            viper.head_x = viper.head_x - 1;
            add_viper_body(viper.head_px, viper.head_py);
            add_viper_head(viper.head_x, viper.head_y);
            if ((get_here(viper.locations[0].x, viper.locations[0].y)) -> type == CHEST) {
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->type = CLEAR;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->draw = draw_nothing;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->walkable = 1; 
            } else {
            map_erase(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y);
            }
            
            for (int i = viper.length - 1; i > 0; i--) {
                viper.locations[i].x = viper.locations[i - 1].x;
                viper.locations[i].y = viper.locations[i - 1].y;
                viper.locations[i].direction = viper.locations[i - 1].direction;
            }
            add_viper_tail(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
            viper.locations[0].x = viper.head_x;
            viper.locations[0].y = viper.head_y;
            viper.locations[0].direction = GO_LEFT;
        } else if (xyz == GO_RIGHT) {
            
            viper.head_px = viper.head_x;
            viper.head_py = viper.head_y;
            viper.head_x = viper.head_x + 1;
            add_viper_body(viper.head_px, viper.head_py);
            add_viper_head(viper.head_x, viper.head_y);
            if (get_here(viper.locations[0].x, viper.locations[0].y)-> type == CHEST) {
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->type = CLEAR;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->draw = draw_nothing;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->walkable = 1; 
            } else {
            map_erase(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y);
            }
            for (int i = viper.length - 1; i > 0; i--) {
                viper.locations[i].x = viper.locations[i - 1].x;
                viper.locations[i].y = viper.locations[i - 1].y;
                viper.locations[i].direction = viper.locations[i - 1].direction;
            }                       
            add_viper_tail(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
            viper.locations[0].x = viper.head_x;
            viper.locations[0].y = viper.head_y;
            viper.locations[0].direction = GO_RIGHT;
        } else if (xyz == GO_UP) {
            
            viper.head_px = viper.head_x;
            viper.head_py = viper.head_y;
            viper.head_y = viper.head_y - 1;
            add_viper_body(viper.head_px, viper.head_py);
            add_viper_head(viper.head_x, viper.head_y);
            if (get_here(viper.locations[0].x, viper.locations[0].y)-> type == CHEST) {
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->type = CLEAR;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->draw = draw_nothing;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->walkable = 1; 
            } else {
            map_erase(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y);
            }
            for (int i = viper.length - 1; i > 0; i--) {
                viper.locations[i].x = viper.locations[i - 1].x;
                viper.locations[i].y = viper.locations[i - 1].y;
                viper.locations[i].direction = viper.locations[i - 1].direction;
            }
            add_viper_tail(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
            viper.locations[0].x = viper.head_x;
            viper.locations[0].y = viper.head_y;
            viper.locations[0].direction = GO_UP;
        } else if (xyz == GO_DOWN) {
            viper.head_px = viper.head_x;
            viper.head_py = viper.head_y;
            viper.head_y = viper.head_y + 1;
            add_viper_body(viper.head_px, viper.head_py);
            add_viper_head(viper.head_x, viper.head_y);
            if (get_here(viper.locations[0].x, viper.locations[0].y)-> type == CHEST) {
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->type = CLEAR;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->draw = draw_nothing;
                get_here(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y)->walkable = 1; 
            } else {
            map_erase(viper.locations[viper.length-1].x, viper.locations[viper.length-1].y);
            }
            for (int i = viper.length - 1; i > 0; i--) {
                viper.locations[i].x = viper.locations[i - 1].x;
                viper.locations[i].y = viper.locations[i - 1].y;
                viper.locations[i].direction = viper.locations[i - 1].direction;
            }
            add_viper_tail(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
            viper.locations[0].x = viper.head_x;
            viper.locations[0].y = viper.head_y;
            viper.locations[0].direction = GO_DOWN;
        } else if (xyz == INVINCIBLE) {
            viper.INVINCIBL = true;
            return NO_ACTION;
        }
        
        
        else {
            return NO_ACTION;
        }

    return 0;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the viper position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the viper has not moved.
 */
 
 
int update_game(int action)
{

    MapItem* next;
    
    
    switch (action) {
        case GO_LEFT:
            next = get_west(viper.head_x, viper.head_y);
            if (next->walkable) {
                switch (next->type) {
                    case CHEST:
                        viper.length += 1;
                        if (viper_point_demultiplier > 0) {
                            viper.score += 0;
                        } else if (viper_point_multiplier > 0){
                        viper.score += 4;
                        } else {
                            viper.score += 2;
                        }
                        break;
                    
                    case TREE:
                        viper.length += 1;
                        break;
                        
                    case PLANT:
                        viper_point_multiplier = 15;
                        break;
                        
                    case BOMB:
                        viper_point_multiplier = 0;
                        viper_point_demultiplier = 20;
                        break;
                    default:
                        break;
                }
                return GO_LEFT;
            }
            if (viper.INVINCIBL) {
                return NO_ACTION;
            }
            return GAME_OVER;
        case GO_RIGHT:
            next = get_east(viper.head_x, viper.head_y);
            if (next->walkable) {
                switch (next->type) {
                    case CHEST:
                        viper.length += 1;
                        if (viper_point_demultiplier > 0) {
                            viper.score += 0;
                        } else if (viper_point_multiplier > 0){
                        viper.score += 4;
                        } else {
                            viper.score += 2;
                        }
                        break;
                    case TREE:
                        viper.length += 1;
                        break;
                    case PLANT:
                        viper_point_multiplier = 15;
                        break;
                    case BOMB:
                        viper_point_multiplier = 0;
                        viper_point_demultiplier = 20;
                        break;
                    default:
                        break;
                }
                return GO_RIGHT;
            }
            if (viper.INVINCIBL) {
                return NO_ACTION;
            }
            return GAME_OVER;
        case GO_UP:
            next = get_north(viper.head_x, viper.head_y);
            if (next->walkable) {
                switch (next->type) {
                    case CHEST:
                        viper.length += 1;
                        if (viper_point_demultiplier > 0) {
                            viper.score += 0;
                        } else if (viper_point_multiplier > 0){
                        viper.score += 4;
                        } else {
                            viper.score += 2;
                        }
                        break;
                    case TREE:
                        viper.length += 1;
                        break;
                    case PLANT:
                        viper_point_multiplier = 15;
                        break;
                    case BOMB:
                        viper_point_multiplier = 0;
                        viper_point_demultiplier = 20;
                        break;
                    default:
                        break;
                }
                return GO_UP;
            }
            if (viper.INVINCIBL) {
                return NO_ACTION;
            }
            return GAME_OVER;
        case GO_DOWN:
            next = get_south(viper.head_x, viper.head_y);
            if (next->walkable) {
                switch (next->type) {
                    case CHEST:
                        viper.length += 1;
                        if (viper_point_demultiplier > 0) {
                            viper.score += 0;
                        } else if (viper_point_multiplier > 0){
                        viper.score += 4;
                        } else {
                            viper.score += 2;
                        }
                        break;
                    case TREE:
                        viper.length += 1;
                        break;
                    case PLANT:
                        viper_point_multiplier = 15;
                        break;
                    case BOMB:
                        viper_point_multiplier = 0;
                        viper_point_demultiplier = 20;
                        break;
                    default:
                        break;
                }
                return GO_DOWN;
            }
            if (viper.INVINCIBL) {
                return NO_ACTION;
            }
            return GAME_OVER; 
        default:
            return update_game(viper.locations[0].direction);
    
    }
    
}




/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    uLCD.line(0, 9, 127, 9, GREEN);
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    uLCD.line(0, 118, 127, 118, GREEN);
}

/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(  0,   9, 127,  14, WHITE);   // Top
    uLCD.filled_rectangle(  0,  13,   2, 114, WHITE);   // Left
    uLCD.filled_rectangle(  0, 114, 127, 117, WHITE);   // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE);   // Right
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option)
{
    // Draw game border first
    if(draw_option == FULL_DRAW) 
    {
        draw_border();
        int u = 58;
        int v = 56;
        draw_viper_head(u, v);
        draw_viper_body(u-11, v);
        draw_viper_tail(u-22, v);
        return;
    }

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + viper.head_x;
            int y = j + viper.head_y;

            // Compute the previous map (px, py) of this tile
            int px = i + viper.head_px;
            int py = j + viper.head_py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (draw_option || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            } else if (draw_option) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    srand(time(0));
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = 0; i < 75; i++) {
        int ra = rand() % map_area();
        add_chest(ra % map_width(), ra / map_width());
    }
    for(int i = 0; i < 50; i++) {
        int ra = rand() % map_area();
        add_plant(ra % map_width(), ra / map_width());
    }
    for (int i = 0; i < 30; i++) {
        int ra = rand() % map_area();
        add_bomb(ra % map_width(), ra / map_width());
    }
    
    for (int i = 0; i < 40; i++) {
        int ra = rand() % map_area();
        add_tree(ra % map_width(), ra / map_width());
    }
    
    pc.printf("plants\r\n");

    pc.printf("Adding walls!\r\n");
    add_wall(            0,              0, HORIZONTAL, map_width());
    add_wall(            0, map_height()-1, HORIZONTAL, map_width());
    add_wall(            0,              0,   VERTICAL, map_height());
    add_wall(map_width()-1,              0,   VERTICAL, map_height());
    pc.printf("Walls done!\r\n");
    
    add_viper_head(viper.locations[0].x, viper.locations[0].y);
    add_viper_body(viper.locations[1].x, viper.locations[1].y);
    add_viper_tail(viper.locations[2].x, viper.locations[2].y);
    
    
    pc.printf("Add extra chamber\r\n");
    add_wall(30,  0,   VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39,  0,   VERTICAL, 10);
    pc.printf("Added!\r\n");
    
    print_map();
}