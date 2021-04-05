#include "speech.h"
#include "globals.h"
#include "hardware.h"
#include "map.h"



/**
 * Draw the speech bubble background.
 */
static void draw_speech_bubble();
/**
 * Erase the speech bubble.
 */
static void erase_speech_bubble();

/**
 * Draw a single line of the speech bubble.
 * @param line The text to display
 * @param which If TOP, the first line; if BOTTOM, the second line.
 */
#define TOP    0
#define BOTTOM 1
static void draw_speech_line(const char* line, int which);

/**
 * Delay until it is time to scroll.
 */
static void speech_bubble_wait();

void draw_speech_bubble()
{

    uLCD.filled_rectangle(0, 92, 127, 114, 0xFFFFFF) ;
    uLCD.rectangle(3, 93, 123, 113, 0x000000) ;
    uLCD.filled_rectangle(3, 93, 123, 113, 0x000000) ;
}

void erase_speech_bubble()
{
    uLCD.filled_rectangle(3, 92, 123, 114, 0x000000) ;

}


void draw_speech_line(const char* line, int which)
{
     switch(which){
        case TOP:   for(int i = 0; i < strlen(line); i ++){
                    uLCD.locate(i + 1, 12);
                    uLCD.printf("%c", line[i]);
                    wait(0.1);
                    }
                    break;
        case BOTTOM:     for(int i = 0; i < strlen(line); i ++){
                        uLCD.locate(i + 1, 13);
                        uLCD.printf("%c", line[i]);
                        wait(0.1);
                    }
                        break;
     }
}

void speech_bubble_wait()
{
    wait(1);
}

void speech(const char* line1, const char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}

void long_speech(const char* lines[], int n)
{ 

}
