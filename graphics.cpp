#include "graphics.h"

#include "globals.h"




#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
#define BLACK  0x000000
#define DARKBROWN 0x654321
#define WHITE   0xFFFFFF
#define GRAY    0x808080
#define ORANGE  0xFFA500



void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'A') colors[i] = DARKBROWN;
        else if (img[i] == 'W') colors[i] = WHITE; 
        else if (img[i] == 'O') colors[i] = ORANGE;       
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}
//plant graph
const char plant[121] = {
    'B','B','B','B','B','R','B','B','B','B','B',
    'B','B','B','B','R','R','R','B','B','B','B',    
    'B','B','R','B','R','R','R','B','R','B','B',
    'B','R','R','R','R','Y','R','R','R','R','B',
    'B','R','R','R','Y','Y','Y','R','R','R','B',
    'B','B','B','R','R','R','R','R','B','B','B',
    'B','B','R','R','R','G','R','R','R','B','B',
    'B','B','R','R','B','G','B','R','R','B','B',
    'B','G','G','G','B','G','B','G','G','G','B',
    'B','B','G','G','G','G','G','G','G','B','B',
    'B','B','B','G','G','G','G','G','B','B','B',
    };
    
//player graph
const char player[121] = {
    'B','B','B','B','B','W','W','B','B','B','B',
    'B','B','B','B','W','W','W','W','B','B','B',    
    'B','B','B','B','W','W','W','W','B','B','W',
    'B','B','B','B','B','W','W','B','B','B','W',
    'B','B','W','W','W','W','W','B','B','W','W',
    'B','W','B','W','W','W','W','W','W','W','B',
    'B','B','B','B','W','W','W','B','B','B','B',
    'B','B','B','B','W','B','B','W','B','B','B',
    'B','B','B','W','B','B','B','W','W','B','B',
    'B','W','W','W','B','B','B','B','W','B','B',
    'B','B','W','B','B','B','B','B','W','B','B',
    };
    
//door graph
const char door[121] = {
    'B','A','A','A','A','A','A','A','A','A','B',
    'B','A','D','D','D','D','D','D','D','A','B',    
    'B','A','D','D','D','D','D','D','D','A','B',
    'B','A','D','D','D','D','D','D','D','A','B',
    'B','A','D','A','A','D','D','D','D','A','B',
    'B','A','D','A','A','D','D','D','D','A','B',
    'B','A','D','D','D','D','D','D','D','A','B',
    'B','A','D','D','D','D','D','D','D','A','B',
    'B','A','D','D','D','D','D','D','D','A','B',
    'B','A','D','D','D','D','D','D','D','A','B',
    'B','A','A','A','A','A','A','A','A','A','B',
    };
    
// NPC graph
const char NPC[121] = {
    'B','Y','Y','B','B','B','B','B','Y','Y','B',
    'B','Y','Y','B','B','B','B','B','Y','Y','B',    
    'B','Y','Y','Y','B','B','B','B','Y','Y','B',
    'B','Y','Y','Y','Y','B','B','B','Y','Y','B',
    'B','Y','Y','B','Y','Y','B','B','Y','Y','B',
    'B','Y','Y','B','B','Y','B','B','Y','Y','B',
    'B','Y','Y','B','B','B','Y','B','Y','Y','B',
    'B','Y','Y','B','B','B','Y','Y','Y','Y','B',
    'B','Y','Y','B','B','B','B','Y','Y','Y','B',
    'B','Y','Y','B','B','B','B','B','Y','Y','B',
    'B','Y','Y','B','B','B','B','B','Y','Y','B',
    };
    

// carrot graph
const char carrot[121] = {
    'B','B','B','B','B','B','B','G','B','B','B',
    'B','B','B','B','B','B','B','G','G','B','B',    
    'B','B','B','B','B','B','O','G','B','G','B',
    'B','B','B','B','B','O','O','O','G','G','B',
    'B','B','B','B','O','O','O','O','G','G','B',
    'B','B','B','O','O','O','B','O','B','B','B',
    'B','B','O','O','O','B','O','B','B','B','B',
    'B','B','O','O','O','O','B','B','B','B','B',
    'B','O','B','O','B','B','B','B','B','B','B',
    'B','O','O','B','B','B','B','B','B','B','B',
    'O','B','B','B','B','B','B','B','B','B','B',
    };
// key graph
const char key[121] = {
    'B','B','B','B','5','5','5','B','B','B','B',
    'B','B','B','5','B','B','B','5','B','B','B',    
    'B','B','B','5','B','B','B','5','B','B','B',
    'B','B','B','B','5','B','5','B','B','B','B',
    'B','B','B','B','B','5','B','B','B','B','B',
    'B','B','B','B','B','5','B','B','B','B','B',
    'B','B','B','B','B','5','5','5','B','B','B',
    'B','B','B','B','B','5','B','B','B','B','B',
    'B','B','B','B','B','5','B','B','B','B','B',
    'B','B','B','B','B','5','5','5','B','B','B',
    'B','B','B','B','B','5','5','5','B','B','B',
    };

//chocolate graph
const char chocolate[121] = {
    'B','A','A','A','A','A','A','A','A','A','B',
    'B','A','D','D','D','A','D','D','D','A','B',    
    'B','A','D','D','D','A','D','D','D','A','B',
    'B','A','A','A','A','A','A','A','A','A','B',
    'B','A','D','D','D','A','D','D','D','A','B',
    'B','A','D','D','D','A','D','D','D','A','B',
    'B','A','D','D','D','A','D','D','D','A','B',
    'B','A','A','A','A','A','A','A','A','A','B',
    'B','A','D','D','D','A','D','D','D','A','B',
    'B','A','D','D','D','A','D','D','D','A','B',
    'B','A','A','A','A','A','A','A','A','A','B',
    };
void draw_player(int u, int v, int key)
{
       draw_img(u, v, player);

}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
}

void draw_NPC(int u, int v){
    draw_img(u, v, NPC);
}

void draw_Door(int u, int v){
    draw_img(u,v, door);
}

void draw_plant(int u, int v)
{
    draw_img(u, v, plant);
}

void draw_carrot(int u, int v)
{
    draw_img(u, v, carrot);
}

void draw_path(int u, int v){
    uLCD.filled_circle(u+5, v+5, 1, YELLOW);
}

void draw_chocolate(int u, int v){
    draw_img(u, v, chocolate);
}

void draw_key(int u, int v){
    draw_img(u, v, key);
}

//upper status prints x and y coordinates on map1
void draw_upper_status(int x, int y)
{
    // Aaw bottom border of status bar
    uLCD.line(0, 9, 127, 9, GREEN);
    // Add other status info Aawing code here
    uLCD.locate(0,0);
    uLCD.printf("x:%d, y:%d", x, y);
}

void draw_lower_status(int x)
{   uLCD.line(0, 117, 127, 117, GREEN);
    uLCD.locate(0,15);
    uLCD.printf("Energy: %d", x );
    // Aaw top border of status bar
    
    
    // Add other status info Aawing code here
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}

//draw Exclamation mark when Player speaks to NPC
void draw_Exclamation(int x, int y)
{   
    uLCD.filled_rectangle(x+4, y, x+6, y+6 ,WHITE);
    uLCD.filled_rectangle(x+4, y+8, x+6, y+10 ,WHITE);

}

//draw big object(Building) in the map that blocks the player
//Since it is the building, player can enter and exit throught the middle entrance.
void draw_Building(int x, int y)
{
    uLCD.filled_rectangle(x, y, x+10, y+10, GRAY);
}
void draw_Building2(int x, int y)
{
    uLCD.filled_rectangle(x, y, x+10, y+10, GRAY);
}




