// ============================================
// The Map class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "map.h"

#include "globals.h"
#include "graphics.h"

#define MHF_NBUCKETS 97

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 1

static Map maps[NUM_MAPS];
static int active_map;

static const MapItem CLEAR_SENTINEL = {
    .type = CLEAR,
    .draw = draw_nothing
};

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // TODO: Implement
    // Hint: Think about how you accessed location in Project 1
    // 1. Return the 1-D xy key
    unsigned keyMap = X + Y * map_width();
    return keyMap;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Implement
    // Hint: what math operator can be used as an easy way to hash a key?
    // 1. Use MHF_NBUCKETS as part of your hash function
    // 2. Return the hashed key
    return key % 50;

}

/**
 * Initializes the map, using a hash_table, setting the width and height.
 */
void maps_init()
{
    // TODO: Implement
    // 1. Loop through all map items and and initialize hash table to hold the buckets
    // 2. Set width & height for any maps 
    // 3. Set the first map to be active
    maps[active_map].h = 50;
    maps[active_map].w = 50;
    maps[active_map].items = createHashTable(map_hash, 50);
}

Map* get_active_map()
{
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

/**
 * Returns width of active map
 */
int map_width()
{
    int widt = get_active_map() -> w;
    return widt;
}

/**
 * Returns height of active map
 */
int map_height()
{
    int heit = get_active_map() -> h;
    return heit;
}

/**
 * Returns the area of the active map
 */
int map_area()
{
    return map_width() * map_height();
}

/**
 * Returns MapItem at current coordinate location
 */
MapItem* get_current(int x, int y)
{
    // TODO: Implement  
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
    MapItem* temp =(MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y));
    if (temp -> type == CLEAR) {
        map_erase(x,y);
        return temp;
    }
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y));
}

/**
 * Returns the MapItem immediately above the given location.
 */
MapItem* get_north(int x, int y)
{
    // TODO: Implement
    // 1. Get map item 
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item 
    MapItem* temp =(MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y-1));
    if (temp -> type == CLEAR) {
        map_erase(x,y-1);
        return temp;
    }
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y-1));
}

/**
 * Returns the MapItem immediately below the given location.
 */
MapItem* get_south(int x, int y)
{
    // TODO: Implement 
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
    MapItem* temp =(MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y+1));
    if (temp -> type == CLEAR) {
        map_erase(x,y+1);
        return temp;
    }
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y+1));;
}

/**
 * Returns the MapItem immediately right the given location.
 */
MapItem* get_east(int x, int y)
{
    // TODO: Implement 
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
    MapItem* temp =(MapItem*) getItem(get_active_map() -> items, XY_KEY(x+1,y));
    if (temp -> type == CLEAR) {
        map_erase(x+1,y);
        return temp;
    }
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x+1,y));
}

/**
 * Returns the MapItem immediately left the given location.
 */
MapItem* get_west(int x, int y)
{
    // TODO: Implement 
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item 
    MapItem* temp =(MapItem*) getItem(get_active_map() -> items, XY_KEY(x-1,y));
    if (temp -> type == CLEAR) {
        map_erase(x-1,y);
        return temp;
    }
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x-1,y));
}

/**
 * Returns the MapItem at current coordinate location
 */
 MapItem* get_here(int x, int y)
 {
     // TODO: Implement  
     // HINT: This is the same as get_current()
     // You may copy the same code fro get_current for get_here
     // 1. Get map item
     // 2. Check and remove item if item exist and is clear type
     // 3. Return the item
     MapItem* temp =(MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y));
    if (temp -> type == CLEAR) {
        map_erase(x,y);
        return temp;
    }
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x,y));
 }

/**
 * Erase current location map item
 */
void map_erase(int x, int y)
{
    deleteItem(get_active_map() -> items, XY_KEY(x, y));
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_bomb(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BOMB;
    w1->draw = draw_bomb;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_chest(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CHEST;
    w1->draw = draw_chest;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_chest(int x, int y) // I'm lazy so overwrite it with a plant
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_body(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    w1->draw = draw_viper_body;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_head(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    w1->draw = draw_viper_head;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_tail(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    w1->draw = draw_viper_tail;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_tree(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = TREE;
    w1->draw = draw_tree;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
