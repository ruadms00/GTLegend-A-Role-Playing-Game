#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map maps[3];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    return X*(maps[active_map].w)+Y;
    // TODO: Fix me!
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return key%100;
}

void maps_init()
{   
    // TODO: Implement!    
    // Initialize hash table
    // Set width & height
    maps[0].items = createHashTable(map_hash, 100);
    maps[0].h = 55;
    maps[0].w = 55;
    maps[1].items = createHashTable(map_hash, 100);
    maps[1].h = 15;
    maps[1].w = 15;
    maps[2].items = createHashTable(map_hash, 100);
    maps[2].h = 15;
    maps[2].w = 15;
}

Map* get_active_map()
{
    // There's only one map
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
   return get_active_map()->w;
}

int map_height()
{
    return get_active_map()->h;
}

int map_area()
{
    return get_active_map()->w * get_active_map()->h;
}



MapItem* get_north(int x, int y)
{
      MapItem* north = get_here(x, y-1);
    return north;
}

MapItem* get_south(int x, int y)
{
    MapItem* south = get_here(x, y+1);
    return south;
}

MapItem* get_east(int x, int y)
{
     MapItem* east = get_here(x+1, y);
    return east;
}

MapItem* get_west(int x, int y)
{
      MapItem* west = get_here(x-1, y);
    return west;
}

MapItem* get_here(int x, int y)
{
    int key = XY_KEY(x,y);
    void* item = getItem(get_active_map()->items, key);
    return (MapItem*) item;
}


void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->destroyable = false;
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
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_NPC(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_NPC;
    w1->walkable = false;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_Door(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_Door;
    w1->walkable = false;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_carrot(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = carrot;
    w1->draw = draw_carrot;
    w1->walkable = true;
    w1->data = NULL;
    w1->destroyable = true;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_path(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Path;
    w1->draw = draw_path;
    w1->walkable = true;
    w1->destroyable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

//draw a chocolate 
void add_chocolate(int x, int y){
     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CHOCOLATE;
    w1->draw = draw_chocolate;
    w1->walkable = true;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

//draw key
void add_key(int x, int y){
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = true;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

//erase the item
void map_erase(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = dummy;
    w1->draw = draw_nothing;
    w1->walkable = true;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

// add Exclamation when NPC speaking
void add_Exclamation(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Exclamation;
    w1->draw = draw_Exclamation;
    w1->walkable = true;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
// add Building
void add_Building(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Building;
    w1->draw = draw_Building;
    w1->walkable = true;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}
void add_Building2(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = Building2;
    w1->draw = draw_Building2;
    w1->walkable = false;
    w1->data = NULL;
    w1->destroyable = false;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}






