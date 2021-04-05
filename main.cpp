// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

#define GRAY 0x808080
// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();
void erase_map();



/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int Omnipotent_mode;
    int walkable;
    int chocolate;
    int map;  // There are 3 maps. map0, map1, map2
    int run_low; // make player run slow
    int show_menu; // In map1, when push button 2 is pressed, it will show direction at the top.
    int energy; // In map 2, player have 3 energies. These will be shown at the bottom.
} Player;


/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define Omnipotent 7

// Set Accelerometer to moves the player
int get_action(GameInputs inputs)
{   
    // pc.printf("%f\n", inputs.ay);
     if(abs(inputs.ax) > abs(inputs.ay)){
         if(inputs.ax > 0.25) return GO_RIGHT; 
         else if(inputs.ax < -0.25) return GO_LEFT; 
         } 
    else if(abs(inputs.ay) > abs(inputs.ax)){
            if(inputs.ay > 0.25)    return GO_UP;
            else if(inputs.ay < -0.25)  return GO_DOWN;
    }
    //push button1 is used to activate omnipotent mode(map0)
    //, and change to run slow(map1)
    if(!inputs.b1){  
                     pc.printf("button1_pressed_Omnipotentmode");
                     return Omnipotent;
                     }
    //push button3 is used 
    // draw a speech bubble, pick up chocolate, pick up carrots, talk to  NPC, open the door, end the game
    if(!inputs.b3){
                    pc.printf("ActionButton_Pressed");
                    return ACTION_BUTTON;
                    }
    //push button2 is used to activate menu(shows the direction)
    if(!inputs.b2){ return MENU_BUTTON;

                   }
                    
    return NO_ACTION;
}


/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2


void go_up(){
     MapItem* next_player = get_here(Player.x, Player.y - 1);
        if(Player.walkable){
            // Player can walk when next player is walkable or omnipotent mode is on.
            if(next_player->walkable||Player.Omnipotent_mode) {
                  // hide the player(inside the building)
                  if(next_player -> type == Building)
                             uLCD.filled_rectangle(58, 59, 68, 69, GRAY);
                    //show player(outside the building)
                    else if(get_here(Player.x, Player.y) -> type == PLANT && get_here(Player.x-1, Player.y)-> type == Building)
                        draw_game(true);
                // In map1 when the button 2 pressed, it will show the direction of Player
                if(Player.map == 1){
                    if(Player.show_menu){
                    uLCD.locate(0, 0);
                    uLCD.printf("Go Up   ");
                    }
                    else if(!Player.show_menu){
                        uLCD.locate(0, 0);
                        uLCD.printf(" [Menu] ");
                    }
                }
               
                Player.y = Player.y - 1; 
                //In map1 when player pass the path, the paths will be destroyed.
                if(next_player != NULL && next_player->type == Path) erase_map();
            }
        }
        
}

void go_down(){
     MapItem* next_player = get_here(Player.x, Player.y + 1);
         if(Player.walkable){
            // Player can walk when next player is walkable or omnipotent mode is on.
            if(next_player->walkable||Player.Omnipotent_mode) {
                  // hide the player(inside the building)
                  if(next_player -> type == Building )
                             uLCD.filled_rectangle(58, 59, 68, 69, GRAY);
                    else if(next_player -> type == PLANT && get_here(Player.x, Player.y)-> type == Building)
                        //show player(outside the building)
                        draw_game(true);
                                // In map1 when the button 2 pressed, it will show the direction of Player
                if(Player.show_menu&& Player.map == 1){
                uLCD.locate(0, 0);
                uLCD.printf("Go Down ");
                }
                else if(!Player.show_menu && Player.map == 1){
                    uLCD.locate(0, 0);
                    uLCD.printf(" [Menu] ");
                } 
                Player.y = Player.y + 1; 
                                //In map1 when player pass the path, the paths will be destroyed.
                if(next_player != NULL && next_player->type == Path) erase_map();
                }
            }
        
}

void go_right(){
     MapItem* next_player = get_here(Player.x + 1, Player.y);
    if(Player.walkable){
                if(next_player->walkable||Player.Omnipotent_mode) {
                    //hide player
                    if(next_player -> type == Building)
                             uLCD.filled_rectangle(58, 59, 68, 69, GRAY);
                    else if(next_player -> type != Building && get_here(Player.x, Player.y)-> type == Building)
                        //draw player
                        draw_game(true);
                 // show direction
                 if(Player.show_menu&& Player.map == 1){
                uLCD.locate(0, 0);
                uLCD.printf("Go right");
                }else if(!Player.show_menu && Player.map == 1){
                    uLCD.locate(0, 0);
                    uLCD.printf(" [Menu] ");
                } 
                Player.x = Player.x + 1; 
                //destroy path
                if(next_player != NULL && next_player->type == Path) erase_map();
               

            // Actually draw the tile
            }
        }
}

void go_left(){
     MapItem* next_player = get_here(Player.x - 1, Player.y);
         if(Player.walkable){
           
            if(next_player->walkable||Player.Omnipotent_mode) {
                  //hide player
                  if(next_player -> type == Building)
                             uLCD.filled_rectangle(58, 59, 68, 69, GRAY);
                    // draw player
                    else if(next_player -> type != Building && get_here(Player.x, Player.y)-> type == Building)
                        draw_game(true);
                //direction
                if(Player.show_menu&& Player.map == 1){
                uLCD.locate(0, 0);
                uLCD.printf("Go left ");
                }else if(!Player.show_menu && Player.map == 1){
                    uLCD.locate(0, 0);
                    uLCD.printf(" [Menu] ");
                } 
                Player.x = Player.x - 1; 
                if(next_player != NULL && next_player->type == Path) erase_map();
            }
        }
    
}

// (map1) destroy map when Player is destroyable(used in destroy path)
void erase_map(){
    if(get_here(Player.x,Player.y) != NULL && get_here(Player.x,Player.y) -> destroyable) map_erase(Player.x,Player.y);
}

//  (map1) draw path
void add_path(){
    int i;
    Player.chocolate = 0;
    add_path(2, map_height()/2+1);
    add_path(2, map_height()/2+2);
    add_path(2, map_height()/2+3);
    add_path(2, map_height()/2+4);
    for(i = 2; i <= 11; i ++){
        add_path(i, map_height()/2+4);
    }
    add_path(11, map_height()/2+3);
    add_path(11, map_height()/2+2);
 
    add_chocolate(11, map_height()/2-2);
    add_path(11, map_height()/2-4);
    for(i = 11; i >= 7; i --){
        add_path(i, map_height()/2-4);
    }
    add_path(map_height()/2, map_height()/2-2);
    add_path(map_height()/2, map_height()/2-3);
    add_path(map_height()/2, map_height()/2-4);
}
//(map0)
void draw_speech_bubble(){
    // when player is near NPC, stop the player and draw a speech bubble
    if((get_south(Player.x, Player.y)-> type == NPC)||(NPC == get_north(Player.x, Player.y)-> type)||(NPC == get_east(Player.x, Player.y)-> type)||(NPC ==get_west(Player.x, Player.y)-> type)){
        Player.walkable = 0;    
       //draw exclamation mark above the NPC
        add_Exclamation(map_width()/2, map_width()/2 - 1);    
        draw_game(true);
        
          speech("Hello!", "");
          speech("I lost ", "my chocolate");
          speech("Could you", "find it for me?");
          speech("I'll give you", "the key");
          speech("if you find it", "");
          speech("Don't pick up", " the carrot");
          speech("It will reduce", "your energy");
          speech("If you get", "three carrots");
          speech("Game will", "be over");
            wait(0.5);
            map_erase(map_width()/2, map_width()/2 - 1);
            draw_game(true);

    // set Player characteristic in map1
    Player.x = 1;
    Player.y = 5;
    Player.walkable = 1;
    Player.map = 1;
    uLCD.filled_rectangle(0,0,128, 8, BLACK);
    // change the map
    set_active_map(1);
    draw_game(true);

    }
    
}

// Open the door when NPC is near the door. 
void open_the_door(){
    if((get_south(Player.x, Player.y)-> type == DOOR)||(NPC == get_north(Player.x, Player.y)-> type)||(DOOR == get_east(Player.x, Player.y)-> type)||(DOOR ==get_west(Player.x, Player.y)-> type))
    //Set Player features in map2
    Player.x = map_width()/2 + 1;
    Player.y = map_width()/2 , map_width()/2 + 3;
    Player.walkable = 1;
    Player.map = 2;
    // change the map
    set_active_map(2);
    draw_game(true);
 }

int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP: go_up(); 
                    break;
        case GO_LEFT:   go_left();
                        break;            
        case GO_DOWN:   go_down();
                        break;
        case GO_RIGHT:  go_right();
                        break;
        case Omnipotent:  //make a sound when button is pressed
                          speaker.period(1.0/400);
                          speaker = 0.02;
                          wait(0.05);
                          speaker.period(1.0/800.0);
                          wait(0.05);
                          speaker = 0;
                          //map0
                            if(Player.map == 0){
                            //change Omnipotent_mode from off to on, and on to off
                            if(Player.Omnipotent_mode) Player.Omnipotent_mode = 0;
                            else if(!Player.Omnipotent_mode) Player.Omnipotent_mode = 1;
                         }
                         //map1
                         else if(Player.map == 1){
                               Player.Omnipotent_mode = 0;
                            //change run_low from off to on, and on to off
                            if(Player.run_low)Player.run_low = 0;
                             else if(!Player.run_low) Player.run_low = 1;
                         }
                            break;
        case ACTION_BUTTON: //make a sound when button is pressed
                            speaker.period(1.0/400);
                            speaker = 0.02;
                            wait(0.05);
                            speaker.period(1.0/800.0);
                            wait(0.05);
                            speaker = 0;
                            
                            //when map1, and palyer doesn't have a key, talk to NPC 
                            if((Player.has_key == 0)&&(Player.map == 0)){
                                draw_speech_bubble();
                             }
                             
                             //When Player is on the chocolate, grab it
                             if((Player.has_key == 0)&&(Player.map == 1)&&(get_here(Player.x, Player.y)-> type == CHOCOLATE)){
                                Player.chocolate ++;
                                map_erase(Player.x, Player.y);
                                 wait(1.5);
                                 speaker.period(1.0/500);
                                 speaker = 0.025;
                                 wait(0.1);
                                 speaker.period(1.0/700);
                                 wait(0.1);
                                 speaker.period(1.0/1000);
                                 wait(0.1);
                                 speaker = 0;
        
                             }
                             
                             //If player grab carrot, reduce the energy
                         
                             if((Player.has_key == 0)&&(Player.map == 1)&&(get_here(Player.x, Player.y)-> type == carrot)){
                                Player.energy --;
                                map_erase(Player.x, Player.y);
                                //if energy = 0, the game is over
                                if(Player.energy == 0){
                                    Player.walkable = 0;
                                    while( Player.walkable == 0){
                                        uLCD.filled_rectangle(0, 0, 128, 128, BLACK);
                                        uLCD.locate(5, 5);
                                        uLCD.printf("GAME OVER");
                                        // make a sound when game is over
                                        wait(1.5);
                                        speaker.period(1.0/1000);
                                        speaker = 0.025;
                                        wait(0.1);
                                        speaker.period(1.0/700);
                                        wait(0.1);
                                        speaker.period(1.0/500);
                                        wait(0.1);
                                        speaker = 0;
                                    }
                                }
        
                             }
                             else if((Player.has_key == 0)&&(Player.map == 1)){
                                 //(map1) when player talks to NPC without chocolate, continue the game without any change
                                 if(Player.chocolate == 0){
                                          if((get_south(Player.x, Player.y)-> type == NPC)||(NPC == get_north(Player.x, Player.y)-> type)||(NPC == get_east(Player.x, Player.y)-> type)||(NPC ==get_west(Player.x, Player.y)-> type)){
                                                Player.walkable = 0;
                
                                                add_Exclamation(map_width()/2, map_width()/2 - 1);    
                                                 draw_game(true);
                                                speech("Where is", "my chocolate?");
                                                wait(1);
                                                map_erase(map_width()/2, map_width()/2 - 1);
                                                 draw_game(true);
                                                if(!Player.walkable) Player.walkable = 1;
                                                else if(Player.walkable) Player.walkable = 0;
                                            }
                                            
                                     }
                                //Quest2
                                //(map1) when Player has chocolate, give another quest and change the map
                                else if(Player.chocolate == 1 && get_here(Player.x, Player.y) -> type == KEY){
                                     Player.has_key ++;
                                     map_erase(Player.x, Player.y);
                                     draw_game(true);
                                            }  
                                  else if(Player.chocolate == 1){
                                     if((get_south(Player.x, Player.y)-> type == NPC)||(NPC == get_north(Player.x, Player.y)-> type)||(NPC == get_east(Player.x, Player.y)-> type)||(NPC ==get_west(Player.x, Player.y)-> type)){
                                                Player.walkable = 0;
                                               add_Exclamation(map_width()/2, map_width()/2 - 1);    
                                                draw_game(true);
                                            speech("Follow me", "");
                                            speech("I'll give you", " the key");
                                            speech("You can open", "the door");
                                            speech("There are many", "chocolates");
                                            speech(" in the room", "");
                                            speech("After you eat,", "please return key");
                                            wait(1);
                                            map_erase(map_width()/2, map_width()/2 - 1);
                                            draw_game(true);

                                            add_key(12, map_width()/2);
                                            int NPCx = map_width()/2;
                                            while(get_here(NPCx + 1, map_width()/2)-> type != KEY){
                                                     map_erase(NPCx, map_width()/2); 
                                                     NPCx ++;      
                                                    add_NPC(NPCx, map_width()/2);
                                                    draw_game(true);

                                            }
                                             Player.walkable = 1;
                                        }
                                        }
                            }
                            //(map1) open the door and change the map with another quest.
                             else if((Player.has_key == 1)&&(Player.map == 1)){
                                 uLCD.filled_rectangle(0, 0, 127, 118, BLACK);
                                 uLCD.locate(5, 5);
                                 uLCD.printf("Eat chocolate \n\n    as much as \n\n    you want");
                                 wait(1);
                                 open_the_door();
                             }
                             
                             // (map2) eat chocolate, if player eat more than 3, NPC will be appear
                             else if((Player.has_key == 1)&&(Player.map == 2)&&(get_here(Player.x, Player.y)-> type == CHOCOLATE)){
                                 Player.chocolate ++;
                                map_erase(Player.x, Player.y);
                                if(Player.chocolate >= 4){
                                    add_NPC(map_width()/2, map_width()/2);
                                    draw_game(true);
                                }
                             }
                             
                             //(map2) Talk to NPC and end the game
                             else if((Player.map == 2)&&((get_south(Player.x, Player.y)-> type == NPC)||(NPC == get_north(Player.x, Player.y)-> type)||(NPC == get_east(Player.x, Player.y)-> type)||(NPC ==get_west(Player.x, Player.y)-> type))){
                                    add_key(map_width()/2 + 1, map_width()/2);
                                    add_Exclamation(map_width()/2, map_width()/2 - 1);    
                                    draw_game(true);
                                    Player.walkable = 0;
                                    speech("Thank you", "");
                                    speech("Are you", "satisfied?");
                                    speech("Then let's", "end this game");
                                    while(Player.walkable == 0){
                                        uLCD.filled_rectangle(0, 0, 128, 128, BLACK);
                                        uLCD.locate(5, 5);
                                        uLCD.printf("END");
                                    }

                                            
                            
                            }
                             
                             break;
        case MENU_BUTTON:   //make a sound when button is pressed
                            speaker.period(1.0/400);
                            speaker = 0.02;
                            wait(0.05);
                            speaker.period(1.0/800.0);
                            wait(0.05);
                            speaker = 0;
                            //change menu mode from off to on, and on to off
                            if(Player.show_menu && Player.map == 1) Player.show_menu = 0;
                            else if(!Player.show_menu&& Player.map == 1) Player.show_menu = 1;
                            break;
        default:        break;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
//
void draw_game(int init)
{      
    
    // Draw game border first
        if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if ( i == 0 && j == 0) // Only draw the player on init
            {
                if(init)
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {

                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {   if(Player.map == 0)uLCD.filled_rectangle(0, 0, 127, 8, BLACK);
                    if (curr_item) // There's something here! Draw it
                    {
                       draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }
            if(Player.chocolate == 1)     add_Door(map_width()/2 , map_width()/2 + 2);
            if (draw) draw(u, v);

           // MapItem* curr_item = get_here(Player.x, Player.y);
    }
    }

                
    // Draw status bars    
    if(Player.map == 0) draw_upper_status(Player.x, Player.y); // x,y coordinates on the top status(map0)
    if(Player.map == 1)draw_lower_status(Player.energy); // Energy on the bottom status(map1)
   
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    //set initial Player's features
    set_active_map(0);
    Player.run_low = 0;
    Player.x = Player.y = 5;
    Player.energy = 3;
    Player.show_menu = 0;
    Player.walkable = 1;
    Player.map = 0;
    Player.has_key = 0;
    // map 0
    Map* map = set_active_map(0);
    // add plant
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
    
    // add Building
    add_Building2(map_width()/2 -2,map_width()/2 -1 );
    add_Building(map_width()/2 -2,map_width()/2 );
    add_Building2(map_width()/2 -2,map_width()/2 + 1 );
  
   // add plant
    add_plant(map_width()/2 -1,map_width()/2 -1 );
    add_plant(map_width()/2 -1,map_width()/2 + 1 );
    add_plant(map_width()/2 -3,map_width()/2 -1 );
    add_plant(map_width()/2 -3,map_width()/2 + 1 );
   
    // add wall
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    

    // draw_NPC
    add_NPC(map_width()/2, map_width()/2);
    print_map();
    
    //map 1
    set_active_map(1);
    // add carrot
    add_carrot(11, map_width()/2 - 6);
    add_carrot(map_width()/2 - 2, map_width()/2 -4);
    add_carrot(13, map_width()/2 + 4);
    // add wall
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    // add path
    add_path();
    // add NPC
    add_NPC(map_width()/2, map_width()/2); 
    
    //map 2
    set_active_map(2);
    // add wall
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    // add chocolate
    for(int i = map_width() + 3; i < map_area(); i += 35)
    {
        add_chocolate(i % map_width(), i / map_width());
    }


}


/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // Initialize the maps
    maps_init();
    uLCD.filled_rectangle(0, 118, 127, 118, BLACK);
    uLCD.locate(5, 7);
    uLCD.printf("Let's Start \n\n      the game");
    wait(1);
    init_main_map();
    
    // Initialize game state
   set_active_map(0); 
    // Initial drawing

    draw_game(true);


    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        // Actuall do the game update:
        // 1. Read inputs     
        GameInputs in = read_inputs();    
        // 2. Determine action (get_action)  
        get_action(in);      
        // 3. Update game (update_game)
        update_game(get_action(in));
        // 3b. Check for game over
        // 4. Draw frame (draw_game)
        draw_game(false);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        // if run_low wait increase the wait time
        if (Player.run_low) {
            wait_ms(1000 - dt);
            }
        else if(!Player.run_low) {
            if (dt < 100) wait_ms(100 - dt);
            }
            
        }
}
