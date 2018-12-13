/*
    *        S  N  A  K  E     G  A  M  E
    *  
    *       !! M U L T I P L A Y E R !!
    * 
    * 
    *   ~ made using nCurses and Panels :)

    *   @Date of Completion:
            03.12.2018  [DD.MM.YYYY]
    *   @File:
            Snake_multiPlayer.cpp
    *   @Author:
            AMRITANSHU SIKDAR
*/


// Header Files

#include <ncurses.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <panel.h>

using namespace std;

// Custom Defitions
#define SPEED 100000           // units is microseconds
#define HeadOfSnake 'O'         // the snake's head
#define BodyOfSnake ACS_LANTERN // the snake's body/tail
#define FoodOfSnake ACS_PI      // food of the snake

// Function Declarations
int kbhit(void);

void CommenceTheGame(void);

void welcomeScreen(void);
void assignPositionToSnakes(void);
void gameBorder(void);
void foodNscore(void);
void foodCheck(void);
void playerUP_SNAKE_1(void);
void playerDOWN_SNAKE_1(void);
void playerLEFT_SNAKE_1(void);
void playerRIGHT_SNAKE_1(void);
void playerUP_SNAKE_2(void);
void playerDOWN_SNAKE_2(void);
void playerLEFT_SNAKE_2(void);
void playerRIGHT_SNAKE_2(void);
void checkCollision(void);
void pauseMenu(void);
void gamePlay_Snake_SinglePlayer();
void gamePlay_Snake_MultiPlayer();
void gameOver(void);
void thanksForPlaying(void);
void multiplayerGameMenu(void);
void winnerOfGame(const char*);
void showHighScores(void);


//  Global variables
bool newPlay = true;                           // boolean flag to check whether its a new game

int foodX,foodY;                                // for snakefood
int snakeHead_1_X,snakeHead_1_Y;                // position coordinates of Snake_1
int snakeHead_2_X,snakeHead_2_Y;                // position coordinates of Snake_2
int centerX, centerY;                           // containers for center of the screen
int lengthSnake_1 = 1;                          // length of the snake 1 body
int lengthSnake_2 = 1;                          // length of the snake 2 body
int score_1 = 0;                                // Score of Player (snake) 1
int score_2 = 0;                                // Score of Player (snake) 2
int snakeTail_1_X[200], snakeTail_1_Y[200];     // the tail of the Snake 1
int snakeTail_2_X[200], snakeTail_2_Y[200];     // the tail of the Snake 2
int i_1 = 0;                                    // directory of the tail of first snake ---- counter variable just to keep track
int i_2 = 0;                                    // directory of the tail of second snake ---- counter variable just to keep track
int gameMenuUserSelection = 0;                  // self-explanatory
int __init__Snake_1_POSITION_X;                 // initial positionX of snake 1
int __init__Snake_1_POSITION_Y;                 // initial positionY of snake 1
int __init__Snake_2_POSITION_X;                 // initial positionX of snake 2
int __init__Snake_2_POSITION_Y;                 // initial positionY of snake 2

char playerName_1[10];                          //  Name of the first player {will also be used for single player name}
char playerName_2[10];                          //  Name of the second player


/* ~~ START OF MAIN FUNCTION ~~ */


// int main()
// {
//     initscr();

//     cbreak();
//     noecho();

//     curs_set(0);

//     showHighScores();

//     endwin();
//     return 0;
// }

int main()
{
    CommenceTheGame();

    return 0;
}
/* ~~ END OF MAIN FUNCTION ~~ */



/* ~~ ~ -------------------------------------------------------- ~ ~~ */



/* ~~ USER-DEFINED FUNCTION DEFINITIONS ~~ */

/*
    Natively not supported in Unix based systems. (only in DOS systems)
    A custom defitions is written to implement the same.

    It checks for USER_INPUT without blocking the loop process.
*/

int kbhit(void)
{
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void CommenceTheGame(void)
{
    initscr();                                      // Initializes the Game Screen a.k.a. Game Console

    cbreak();                                       // Exiting with Ctrl+C in case of loop hang
    noecho();                                       // not displaying the characters that are pressed during the entire gameplay

    assignPositionToSnakes();

    curs_set(0);                                    // sets the visibility of the terminal cursor (0,1,2)

    start_color();                                  // start coloring the terminal window

    welcomeScreen();

    multiplayerGameMenu();                          // The Main Menu of the Multiplayer Snake Game

    // starting the game menu and eventually the gameplay

    refresh();

    getch();
    endwin();
}

void assignPositionToSnakes(void)
{
    getmaxyx(stdscr,centerY,centerX);               // attains the maximum width and height of the console

    centerX /= 2;                                   // center of the standard Terminal Screen - Coordinate X
    centerY /= 2;                                   // center of the standard Terminal Screen - Coordinate Y


    /*
        assigning either CENTER or LEFT alignment to the X Coordinate of Snake 1
        depending upon whether the user has chosen SINGLE PLAYER or MULTIPLAYER
    */

    if(gameMenuUserSelection == 0)
        __init__Snake_1_POSITION_X = centerX;
    else if(gameMenuUserSelection == 1)
        __init__Snake_1_POSITION_X = centerX - (centerX/2);

    __init__Snake_1_POSITION_Y = centerY;


    /*  
        assigning RIGHT alignment to the X Coordinate of Snake 2
        X coordinate of Snake 2 can have the same (Right) alignment as it doens't matter in case of
        Single Player gameplay 
    */

    __init__Snake_2_POSITION_X = centerX + (centerX/2);
    __init__Snake_2_POSITION_Y = centerY;

    snakeHead_1_X = __init__Snake_1_POSITION_X;     // initializing the positionX of snake 1
    snakeHead_1_Y = __init__Snake_1_POSITION_Y;     // initializing the positionY of Snake 1
    snakeHead_2_X = __init__Snake_2_POSITION_X;     // initializing the positionX of Snake 2
    snakeHead_2_Y = __init__Snake_2_POSITION_Y;     // initializing the positionY of Snake 2
}

void gameBorder(void)
{
    int left,right,top,bottom,topLeftCorner,topRightCorner,bottomLeftCorner,bottomRightCorner;
    int line = 1;


    topLeftCorner = topRightCorner = bottomLeftCorner = bottomRightCorner = ACS_CKBOARD;
    left = right = (int)'|';
    top = bottom = (int)'~';

    init_pair(1,COLOR_CYAN,COLOR_BLACK);  // color pair 1
    attron(COLOR_PAIR(1));
    attron(A_BOLD);

    border(left,right,top,bottom,topLeftCorner,topRightCorner,bottomLeftCorner,bottomRightCorner);

    //  printing a line just below the scoreboard
    mvaddch(3,line,ACS_DIAMOND);
    while(line != getmaxx(stdscr)-3)
    {
        mvprintw(3,line+1,"-");
        line++;
    }
    addch(ACS_DIAMOND);

    attroff(COLOR_PAIR(1));
    attroff(A_BOLD);
}

void foodNscore(void)  // function to print the Food and Score [same function does both the jobs]
{
    int height = 3, width = 10, startX, startY; // foodNscore window
    int nameLimit = 8;      // Why 8? 'cause that's my roll number xD

    startX = (getmaxx(stdscr) - width) / 2; startY=1;  // Positioning the foodNscore

    if(gameMenuUserSelection == 0)
    {
        //  Getting the name of Single Player

        if(newPlay == true)
        {
            WINDOW * player_1_Name_window = newwin(height, width*3, (getmaxy(stdscr)-height)/2,startX-width);
            PANEL * player_1_Name_panel = new_panel(player_1_Name_window);


            wattron(player_1_Name_window,A_REVERSE);
            wattron(player_1_Name_window, A_BOLD);

            curs_set(1);
            echo();

            box(player_1_Name_window,(int)'!',(int)'=');
            mvwprintw(player_1_Name_window,1,1,"Enter your Name:            "); //  Extra spaces for filling up the dark gaps
            wmove(player_1_Name_window,1,18);
            wgetnstr(player_1_Name_window,playerName_1,nameLimit);      //  getting the actual input

            wattroff(player_1_Name_window,A_REVERSE);
            wattroff(player_1_Name_window, A_BOLD);

            hide_panel(player_1_Name_panel);
            doupdate();

            curs_set(0);
            noecho();

            wclear(player_1_Name_window);
        }
        
        refresh();

        // ScoreBoard of the Single Player

        WINDOW * scoreBoard_Player_1 = newwin(height, width+(nameLimit/2)+1, startY, startX);  // Creating the foodNscore {window for scoreboard on the top}

        wattron(scoreBoard_Player_1,A_REVERSE);
        wattron(scoreBoard_Player_1,A_BOLD);
        attron(A_BOLD);

        box(scoreBoard_Player_1,(int)'!',(int)'=');
        mvwprintw(scoreBoard_Player_1,1,1,"             ");  // filling up white spaces
        mvwprintw(scoreBoard_Player_1,1,((getmaxx(scoreBoard_Player_1) - strlen(playerName_1)) / 2) - 2,"%s: %d",playerName_1,score_1);  // printing the score
        wrefresh(scoreBoard_Player_1);  // refreshing foodNscore

        wattroff(scoreBoard_Player_1,A_BOLD);
        wattroff(scoreBoard_Player_1,A_REVERSE);
    }
    
    if(gameMenuUserSelection == 1)
    {
        // Getting names of First and Second Players

        if(newPlay == true)
        {
            //  Player 1

            WINDOW * player_1_Name_window = newwin(height, width*3, (getmaxy(stdscr)-height)/2,startX-width);
            PANEL * player_1_Name_panel = new_panel(player_1_Name_window);


            wattron(player_1_Name_window,A_REVERSE);
            wattron(player_1_Name_window, A_BOLD);

            curs_set(1);
            echo();

            box(player_1_Name_window,(int)'!',(int)'=');
            mvwprintw(player_1_Name_window,1,1,"Name of Player 1:            "); //  Extra spaces for filling up the dark gaps
            wmove(player_1_Name_window,1,19);
            wgetnstr(player_1_Name_window,playerName_1,nameLimit);      //  getting the actual input

            wattroff(player_1_Name_window,A_REVERSE);
            wattroff(player_1_Name_window, A_BOLD);

            hide_panel(player_1_Name_panel);
            doupdate();

            wclear(player_1_Name_window);

            //  Player 2

            WINDOW * player_2_Name_window = newwin(height, width*3, (getmaxy(stdscr)-height)/2,startX-width);
            PANEL * player_2_Name_panel = new_panel(player_2_Name_window);


            wattron(player_2_Name_window,A_REVERSE);
            wattron(player_2_Name_window, A_BOLD);

            box(player_2_Name_window,(int)'!',(int)'=');
            mvwprintw(player_2_Name_window,1,1,"Name of Player 2:            "); //  Extra spaces for filling up the dark gaps
            wmove(player_2_Name_window,1,19);
            wgetnstr(player_2_Name_window,playerName_2,nameLimit);      //  getting the actual input

            wattroff(player_2_Name_window,A_REVERSE);
            wattroff(player_2_Name_window, A_BOLD);

            hide_panel(player_2_Name_panel);
            doupdate();

            curs_set(0);
            noecho();

            wclear(player_2_Name_window);
        }
        
        refresh();


        //  ScoreBoard of Player 1

        WINDOW * scoreBoard_Player_1 = newwin(height, width+(nameLimit/2)+1, startY, startX - (getmaxx(stdscr)/4));  // Creating the foodNscore

        wattron(scoreBoard_Player_1,A_REVERSE);
        wattron(scoreBoard_Player_1,A_BOLD);
        attron(A_BOLD);

        box(scoreBoard_Player_1,(int)'!',(int)'=');     
        mvwprintw(scoreBoard_Player_1,1,1,"             ");  // filling up white spaces
        mvwprintw(scoreBoard_Player_1,1,((getmaxx(scoreBoard_Player_1) - strlen(playerName_1)) / 2) - 2,"%s: %d",playerName_1,score_1);  // printing the score
        wrefresh(scoreBoard_Player_1);  // refreshing foodNscore

        wattroff(scoreBoard_Player_1,A_BOLD);
        wattroff(scoreBoard_Player_1,A_REVERSE);

        //  ScoreBoard of Player 2

        WINDOW * scoreBoard_Player_2 = newwin(height, width+(nameLimit/2)+1, startY, startX + (getmaxx(stdscr)/4));  // Creating the foodNscore

        wattron(scoreBoard_Player_2,A_REVERSE);
        wattron(scoreBoard_Player_2,A_BOLD);
        attron(A_BOLD);

        box(scoreBoard_Player_2,(int)'!',(int)'=');
        mvwprintw(scoreBoard_Player_2,1,1,"             ");  // filling up white spaces
        mvwprintw(scoreBoard_Player_2,1,((getmaxx(scoreBoard_Player_2) - strlen(playerName_2)) / 2) - 2,"%s: %d",playerName_2,score_2);  // printing the score
        wrefresh(scoreBoard_Player_2);  // refreshing foodNscore

        wattroff(scoreBoard_Player_2,A_BOLD);
        wattroff(scoreBoard_Player_2,A_REVERSE);
    }


    // Making the Food!

    srand ((unsigned) time(NULL));              // using time as reference to randomize()
    foodX = rand() % (getmaxx(stdscr)-4) + 2;   // random X coordinate for the snakeFood according to the console size
    foodY = rand() % (getmaxy(stdscr)-8) + 6;   //random Y coordinate for the snakeFood according to the console size

    init_pair(2,COLOR_RED,COLOR_YELLOW);

    attron(A_BLINK);    // blinking the food
    attron(COLOR_PAIR(2));

    mvwaddch(stdscr,foodY,foodX,FoodOfSnake);  // printing the food item

    attroff(A_BLINK);
    attroff(COLOR_PAIR(2));
    attroff(A_BOLD);
}

void foodCheck(void)
{
    if(foodX == snakeHead_1_X && foodY == snakeHead_1_Y)
    {
        score_1 += 10;
        lengthSnake_1++;
        foodNscore();
        refresh();
    }

    refresh();

    if(foodX == snakeHead_2_X && foodY == snakeHead_2_Y)
    {
        score_2 += 10;
        lengthSnake_2++;
        foodNscore();
        refresh();
    }
    refresh();
}

//  Player Movement - SnakeBody 1

void playerUP_SNAKE_1(void)  // function to move the snake UP
{
    attron(A_BOLD);

    if(i_1 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_1_X[k] = snakeTail_1_X[k+100];
            snakeTail_1_Y[k] = snakeTail_1_Y[k+100];
        }
        i_1 = 100;
    }

    mvaddch(snakeHead_1_Y,snakeHead_1_X,BodyOfSnake);

    if(i_1 == 0)
        mvaddch(__init__Snake_1_POSITION_Y,__init__Snake_1_POSITION_X,' ');

    snakeHead_1_Y--;   // directing the snake upwards

    snakeTail_1_X[i_1] = snakeHead_1_X;
    snakeTail_1_Y[i_1] = snakeHead_1_Y;

    mvaddch(snakeTail_1_Y[i_1],snakeTail_1_X[i_1],HeadOfSnake);
    mvaddch(snakeTail_1_Y[i_1 - lengthSnake_1],snakeTail_1_X[i_1 - lengthSnake_1],' ');

    i_1++;

    usleep(SPEED/2);

    attroff(A_BOLD);

    refresh();
}

void playerDOWN_SNAKE_1(void)  // function to move the snake DOWN
{
    attron(A_BOLD);

    if(i_1 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_1_X[k] = snakeTail_1_X[k+100];
            snakeTail_1_Y[k] = snakeTail_1_Y[k+100];
        }
        i_1 = 100;
    }

    mvaddch(snakeHead_1_Y,snakeHead_1_X,BodyOfSnake);

    if(i_1 == 0)
        mvaddch(__init__Snake_1_POSITION_Y,__init__Snake_1_POSITION_X,' ');

    snakeHead_1_Y++;   // directing the snake downwards

    snakeTail_1_X[i_1] = snakeHead_1_X;
    snakeTail_1_Y[i_1] = snakeHead_1_Y;

    mvaddch(snakeTail_1_Y[i_1],snakeTail_1_X[i_1],HeadOfSnake);
    mvaddch(snakeTail_1_Y[i_1 - lengthSnake_1],snakeTail_1_X[i_1 - lengthSnake_1],' ');

    i_1++;

    usleep(SPEED/2);

    attroff(A_BOLD);

    refresh();
}

void playerLEFT_SNAKE_1(void)  // function to move the snake LEFT
{
    attron(A_BOLD);

    if(i_1 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_1_X[k] = snakeTail_1_X[k+100];
            snakeTail_1_Y[k] = snakeTail_1_Y[k+100];
        }
        i_1 = 100;
    }

    mvaddch(snakeHead_1_Y,snakeHead_1_X,BodyOfSnake);

    if(i_1 == 0)
        mvaddch(__init__Snake_1_POSITION_Y,__init__Snake_1_POSITION_X,' ');

    snakeHead_1_X--;   // directing the snake to the left

    snakeTail_1_X[i_1] = snakeHead_1_X;
    snakeTail_1_Y[i_1] = snakeHead_1_Y;

    mvaddch(snakeTail_1_Y[i_1],snakeTail_1_X[i_1],HeadOfSnake);
    mvaddch(snakeTail_1_Y[i_1 - lengthSnake_1],snakeTail_1_X[i_1 - lengthSnake_1],' ');

    i_1++;

    usleep(SPEED/3);

    attroff(A_BOLD);

    refresh();
}

void playerRIGHT_SNAKE_1(void)  // function to move the snake RIGHT
{
    attron(A_BOLD);

    if(i_1 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_1_X[k] = snakeTail_1_X[k+100];
            snakeTail_1_Y[k] = snakeTail_1_Y[k+100];
        }
        i_1 = 100;
    }

    mvaddch(snakeHead_1_Y,snakeHead_1_X,BodyOfSnake);

    if(i_1 == 0)
        mvaddch(__init__Snake_1_POSITION_Y,__init__Snake_1_POSITION_X,' ');

    snakeHead_1_X++;   // directing the snake to the right

    snakeTail_1_X[i_1] = snakeHead_1_X;
    snakeTail_1_Y[i_1] = snakeHead_1_Y;

    mvaddch(snakeTail_1_Y[i_1],snakeTail_1_X[i_1],HeadOfSnake);
    mvaddch(snakeTail_1_Y[i_1 - lengthSnake_1],snakeTail_1_X[i_1 - lengthSnake_1],' ');

    i_1++;

    usleep(SPEED/3);

    attroff(A_BOLD);

    refresh();
}

//  Player Movement - SnakeBody 2

void playerUP_SNAKE_2(void)  // function to move the snake UP
{
    attron(A_BOLD);

    if(i_2 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_2_X[k] = snakeTail_2_X[k+100];
            snakeTail_2_Y[k] = snakeTail_2_Y[k+100];
        }
        i_2 = 100;
    }

    mvaddch(snakeHead_2_Y,snakeHead_2_X,BodyOfSnake);

    if(i_2 == 0)
        mvaddch(__init__Snake_2_POSITION_Y,__init__Snake_2_POSITION_X,' ');

    snakeHead_2_Y--;   // directing the snake upwards

    snakeTail_2_X[i_2] = snakeHead_2_X;
    snakeTail_2_Y[i_2] = snakeHead_2_Y;

    mvaddch(snakeTail_2_Y[i_2],snakeTail_2_X[i_2],HeadOfSnake);
    mvaddch(snakeTail_2_Y[i_2 - lengthSnake_2],snakeTail_2_X[i_2 - lengthSnake_2],' ');

    i_2++;

    usleep(SPEED/2);

    attroff(A_BOLD);

    refresh();
}

void playerDOWN_SNAKE_2(void)  // function to move the snake DOWN
{
    attron(A_BOLD);

    if(i_2 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_2_X[k] = snakeTail_2_X[k+100];
            snakeTail_2_Y[k] = snakeTail_2_Y[k+100];
        }
        i_2 = 100;
    }

    mvaddch(snakeHead_2_Y,snakeHead_2_X,BodyOfSnake);

    if(i_2 == 0)
        mvaddch(__init__Snake_2_POSITION_Y,__init__Snake_2_POSITION_X,' ');

    snakeHead_2_Y++;   // directing the snake downwards

    snakeTail_2_X[i_2] = snakeHead_2_X;
    snakeTail_2_Y[i_2] = snakeHead_2_Y;

    mvaddch(snakeTail_2_Y[i_2],snakeTail_2_X[i_2],HeadOfSnake);
    mvaddch(snakeTail_2_Y[i_2 - lengthSnake_2],snakeTail_2_X[i_2 - lengthSnake_2],' ');

    i_2++;

    usleep(SPEED/2);

    attroff(A_BOLD);

    refresh();
}

void playerLEFT_SNAKE_2(void)  // function to move the snake LEFT
{
    attron(A_BOLD);

    if(i_2 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_2_X[k] = snakeTail_2_X[k+100];
            snakeTail_2_Y[k] = snakeTail_2_Y[k+100];
        }
        i_2 = 100;
    }

    mvaddch(snakeHead_2_Y,snakeHead_2_X,BodyOfSnake);

    if(i_2 == 0)
        mvaddch(__init__Snake_2_POSITION_Y,__init__Snake_2_POSITION_X,' ');

    snakeHead_2_X--;   // directing the snake to the left

    snakeTail_2_X[i_2] = snakeHead_2_X;
    snakeTail_2_Y[i_2] = snakeHead_2_Y;

    mvaddch(snakeTail_2_Y[i_2],snakeTail_2_X[i_2],HeadOfSnake);
    mvaddch(snakeTail_2_Y[i_2 - lengthSnake_2],snakeTail_2_X[i_2 - lengthSnake_2],' ');

    i_2++;

    usleep(SPEED/3);

    attroff(A_BOLD);

    refresh();
}

void playerRIGHT_SNAKE_2(void)  // function to move the snake RIGHT
{
    attron(A_BOLD);

    if(i_2 == 200)
    {
        for(int k = 0; k < 100; k++)
        {
            snakeTail_2_X[k] = snakeTail_2_X[k+100];
            snakeTail_2_Y[k] = snakeTail_2_Y[k+100];
        }
        i_2 = 100;
    }

    mvaddch(snakeHead_2_Y,snakeHead_2_X,BodyOfSnake);

    if(i_2 == 0)
        mvaddch(__init__Snake_2_POSITION_Y,__init__Snake_2_POSITION_X,' ');

    snakeHead_2_X++;   // directing the snake to the right

    snakeTail_2_X[i_2] = snakeHead_2_X;
    snakeTail_2_Y[i_2] = snakeHead_2_Y;

    mvaddch(snakeTail_2_Y[i_2],snakeTail_2_X[i_2],HeadOfSnake);
    mvaddch(snakeTail_2_Y[i_2 - lengthSnake_2],snakeTail_2_X[i_2 - lengthSnake_2],' ');

    i_2++;

    usleep(SPEED/3);

    attroff(A_BOLD);

    refresh();
}

// To print the GAME OVER message when the snake gets killed
void gameOver(void)
{
    int height = 3, width = 30, startX, startY;

    startX = (getmaxx(stdscr) - width) / 2;
    startY = getmaxy(stdscr) / 2;

    WINDOW * gameOver = newwin(height, width, startY, startX);

    box(gameOver,(int)'!',(int)'=');  // border over the "GameOver" message

    init_pair(3,COLOR_BLACK,COLOR_WHITE);

    wattron(gameOver,A_BOLD);
    wattron(gameOver,A_STANDOUT);
    wattron(gameOver,COLOR_PAIR(3));

    mvwprintw(gameOver,getmaxy(gameOver)/2,3,"!! G A M E    O V E R !!");

    wattroff(gameOver,A_BOLD);
    wattroff(gameOver,A_STANDOUT);
    wattroff(gameOver,COLOR_PAIR(3));

    wrefresh(gameOver);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 13);   // waits till ENTER key is pressed

    wclear(gameOver);
    wrefresh(gameOver);

    multiplayerGameMenu();
    refresh();
}

void pauseMenu(void)
{
    int height = 8, width = 20, startX = (getmaxx(stdscr) - width) / 2, startY = (getmaxy(stdscr) - height) / 2, choice;
    char menuOptions[2][15] = {"Continue Game", "Exit"};
    int maxx=0;     // limiting the boundary of Exit button [BUG FIX]
    int pauseMenuUserSelection = 0;

    WINDOW * pauseMenu_window = newwin(height, width, startY, startX);
    PANEL * pauseMenu_panel = new_panel(pauseMenu_window);

    box(pauseMenu_window,(int)'^',(int)'|');

    keypad(pauseMenu_window, TRUE);

    while(true)
    {
        for(int j = 0; j < 2; j++)
        {
            if(j == pauseMenuUserSelection)
            {
                wattron(pauseMenu_window,A_REVERSE);
            }
            //  adjusting the "Exit" option, as it was going out of bounds
            if(j == 0)
                maxx = 5;
            else if(j == 1)
                maxx = 3;

            mvwprintw(pauseMenu_window, ((getmaxy(pauseMenu_window) - strlen(menuOptions[j])) / 2) + maxx, (getmaxx(pauseMenu_window) - strlen(menuOptions[j])) / 2, menuOptions[j]);
            wattroff(pauseMenu_window,A_REVERSE);
        }

        choice = wgetch(pauseMenu_window);

        switch(choice)
        {
            case KEY_UP:
                pauseMenuUserSelection--;
                if(pauseMenuUserSelection == -1)
                    pauseMenuUserSelection = 0;
                break;
            case KEY_DOWN:
                pauseMenuUserSelection++;
                if(pauseMenuUserSelection == 2)
                    pauseMenuUserSelection = 1;
                break;
            default:
                break;

        }
        if(choice == 10)    // pressing the enter/return key
        {
            wclear(pauseMenu_window);
            break;
        }
    }

    if(pauseMenuUserSelection == 0 && gameMenuUserSelection == 0)
    {
        hide_panel(pauseMenu_panel);
        refresh();
        gamePlay_Snake_SinglePlayer();
    }
    else if(pauseMenuUserSelection == 0 && gameMenuUserSelection == 1)
    {
        hide_panel(pauseMenu_panel);
        refresh();
        gamePlay_Snake_MultiPlayer();
    }
    else if(pauseMenuUserSelection == 1)
    {
        wclear(stdscr);
        wrefresh(stdscr);
        newPlay = true;
        multiplayerGameMenu();
    }

    update_panels();    // Updating the Panels
    doupdate();         // Showing it on the Terminal Screen

    refresh();
}


void checkCollision(void)
{
    // checking collision with just the boundaries

    if(snakeHead_1_Y<5 || snakeHead_1_X<2 || snakeHead_1_X>getmaxx(stdscr)-3 || snakeHead_1_Y>getmaxy(stdscr)-3)    // in case the snake 1 head hits the game boundaries
    {
        score_1 = 0;
        score_2 = 0;
        lengthSnake_1 = 1;
        lengthSnake_2 = 1;

        newPlay = true;

        if(gameMenuUserSelection == 1)
            winnerOfGame("! ~ Player 2 WINS ~ !");
        else if(gameMenuUserSelection == 0)
            gameOver();

        refresh();
    }

    if(snakeHead_2_Y<5 || snakeHead_2_X<2 || snakeHead_2_X>getmaxx(stdscr)-3 || snakeHead_2_Y>getmaxy(stdscr)-3)    //  in case the snake 2 head hits the game boundaries
    {
        score_1 = score_2 = 0;
        lengthSnake_1 = lengthSnake_2 = 1;
        
        newPlay = true;

        winnerOfGame("! ~ Player 1 WINS ~ !");

        refresh();
    }


    attron(COLOR_PAIR(1));
    mvaddch(0,0,ACS_CKBOARD);
    attroff(COLOR_PAIR(1));
}


//      Gameplay Of Snake 1

void gamePlay_Snake_SinglePlayer(void)
{
    int arrow_input; // takes in input from the arrow keys to control the snake

    keypad(stdscr,TRUE);    // initializes keyboard input

    gameBorder();       // printing the game borders
    foodNscore();       // initializing the Food and ScoreBoard
    foodCheck();        // calling the referee

    if(newPlay == true)
    {
        i_1 = 0;
        newPlay = false;

        clear();
        gameBorder();       // printing the game borders
        foodNscore();       // initializing the Food and ScoreBoard
        foodCheck();        // calling the referee

        refresh();
    }

    while(true)
    {
        arrow_input = getch();  // takes input from the user
        foodCheck();

        switch(arrow_input)
        {
            case KEY_UP:
            {
                while(!kbhit())
                {
                    playerUP_SNAKE_1();
                    checkCollision();
                    foodCheck();
                    refresh();
                }
                break;
            }

            case KEY_LEFT:
            {
                while(!kbhit())
                {
                    playerLEFT_SNAKE_1();
                    checkCollision();
                    foodCheck();
                    refresh();
                }
                break;
            }

            case KEY_DOWN:
            {
                while(!kbhit())
                {
                    playerDOWN_SNAKE_1();
                    checkCollision();
                    foodCheck();
                    refresh();
                }
                break;
            }

            case KEY_RIGHT:
            {
                while(!kbhit())
                {
                    playerRIGHT_SNAKE_1();
                    checkCollision();
                    foodCheck();
                    refresh();
                }
                break;
            }
            default:
                pauseMenu();
                refresh();
                break;
        }

        refresh();
    }
}

//      Gameplay of Snake 2

void gamePlay_Snake_MultiPlayer(void)
{
    char mp1, mp2, multiplayerInput; // takes in input from the arrow keys to control the snake

    keypad(stdscr,TRUE);    // initializes keyboard input

    gameBorder();       // printing the game borders
    foodNscore();       // initializing the Food and ScoreBoard
    foodCheck();        // calling the referee

    if(newPlay == true)
    {
        i_1 = 0;
        i_2 = 0;
        newPlay = false;

        clear();
        gameBorder();       // printing the game borders
        foodNscore();       // initializing the Food and ScoreBoard
        foodCheck();        // calling the referee

        refresh();
    }

    while(true)
    {
        if(kbhit())
        {
            multiplayerInput = getch();  // takes input from the user
            
            switch(multiplayerInput)
            {
                case 'w':
                case 'a':
                case 's':
                case 'd':
                    mp2 = multiplayerInput;
                    break;
                
                case (char) KEY_UP:
                case (char) KEY_DOWN:
                case (char) KEY_LEFT:
                case (char) KEY_RIGHT:
                    mp1 = multiplayerInput;
                    break;

                default:
                    pauseMenu();
                    refresh();
            }
            foodCheck();
        }
        switch(mp1)
        {
            case (char) KEY_UP:
                playerUP_SNAKE_1();
                break;

            case (char) KEY_LEFT:
                playerLEFT_SNAKE_1();
                break;

            case (char) KEY_DOWN:
                playerDOWN_SNAKE_1();
                break;

            case (char) KEY_RIGHT:
                playerRIGHT_SNAKE_1();
                break;


            default:
                refresh();
                break;
        }

        switch(mp2)
        {
            case 'w':
                playerUP_SNAKE_2();
                break;

            case 'a':
                playerLEFT_SNAKE_2();
                break;

            case 's':
                playerDOWN_SNAKE_2();
                break;

            case 'd':
                playerRIGHT_SNAKE_2();
                break;

                
            default:
                refresh();
                break;
        }
        

        checkCollision();
        foodCheck();
        refresh();
    }
}


void thanksForPlaying(void)
{
    int height=3,width=33,startX,startY;

    startX = (getmaxx(stdscr) - width) / 2;
    startY = getmaxy(stdscr)/2;

    WINDOW * thanksForPlaying = newwin(height, width, startY, startX);

    box(thanksForPlaying,(int)'=',(int)'*');  // border over the "Thanks for Playing" message

    init_pair(4,COLOR_BLACK,COLOR_WHITE);

    wattron(thanksForPlaying,A_BOLD);
    wattron(thanksForPlaying,A_STANDOUT);
    wattron(thanksForPlaying,COLOR_PAIR(4));

    mvwprintw(thanksForPlaying,getmaxy(thanksForPlaying)/2,2," !! ~ THANKS FOR PLAYING ~ !!");

    wattroff(thanksForPlaying,A_BOLD);
    wattroff(thanksForPlaying,A_STANDOUT);
    wattroff(thanksForPlaying,COLOR_PAIR(4));

    refresh();
    wrefresh(thanksForPlaying);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 13);   // waits till ENTER key is pressed

    system("clear");
    endwin();
    exit(0);    // final exit of the game console/screen
}

void welcomeScreen(void)
{
    int height=10,width=33,startX,startY;

    startX = (getmaxx(stdscr) - width) / 2;
    startY = (getmaxy(stdscr) - height) / 2;

    WINDOW * welcomeScreen = newwin(height, width, startY, startX);

    box(welcomeScreen,(int)'=',(int)'|');  // border over the "Thanks for Playing" message

    init_pair(5,COLOR_BLACK,COLOR_WHITE);

    wattron(welcomeScreen,A_BOLD);
    wattron(welcomeScreen,A_STANDOUT);
    wattron(welcomeScreen,COLOR_PAIR(5));

    mvwprintw(welcomeScreen,getmaxy(welcomeScreen)/5,3,"!! ~   S  N  A  K  E   ~ !!");
    mvwprintw(welcomeScreen,getmaxy(welcomeScreen)/3,3,"!! ~~  G   A   M   E  ~~ !!");
    mvwprintw(welcomeScreen,getmaxy(welcomeScreen)/2,2,"$ ~ M U L T I P L A Y E R ~ $");
    mvwprintw(welcomeScreen,getmaxy(welcomeScreen)-3,12,"@Author:");
    mvwprintw(welcomeScreen,getmaxy(welcomeScreen)-2,8,"AMRITANSHU SIKDAR");

    wattroff(welcomeScreen,A_BOLD);
    wattroff(welcomeScreen,A_STANDOUT);
    wattroff(welcomeScreen,COLOR_PAIR(5));

    refresh();
    wrefresh(welcomeScreen);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), 13);   // waits till ENTER key is pressed

}

void multiplayerGameMenu(void)
{
    int height = 11, width = 34, startX = (getmaxx(stdscr) - width) / 2, startY = (getmaxy(stdscr) - height) / 2, choice;
    char menuOptions[4][30] = {"Single Player (1P)", "Multiplayer (2P)", "High Scores", "Exit"};

    WINDOW * multiplayerGameMenu = newwin(height, width, startY, startX);

    box(multiplayerGameMenu,(int)'|',(int)'^');

    keypad(multiplayerGameMenu, TRUE);

    while(true)
    {
        for(int j = 0; j < 4; j++)  // 'j' is a counter variable
        {
            if(j == gameMenuUserSelection)
                wattron(multiplayerGameMenu,A_REVERSE);

            mvwprintw(multiplayerGameMenu, (getmaxy(multiplayerGameMenu) - (2*(4-j)-(j-1/3)+(j+1/4)-2)-3), (getmaxx(multiplayerGameMenu) - strlen(menuOptions[j])) / 2, menuOptions[j]);
            wattroff(multiplayerGameMenu,A_REVERSE);
        }

        choice = wgetch(multiplayerGameMenu);

        switch(choice)
        {
            case KEY_UP:
                gameMenuUserSelection--;
                if(gameMenuUserSelection == -1)
                    gameMenuUserSelection = 0;
                break;
            case KEY_DOWN:
                gameMenuUserSelection++;
                if(gameMenuUserSelection == 4)
                    gameMenuUserSelection = 3;
                break;
            default:
                break;

        }
        if(choice == 10)    // pressing the enter/return key
        {
            wclear(multiplayerGameMenu);
            break;
        }
    }

    if(gameMenuUserSelection == 0)
    {
        wclear(multiplayerGameMenu);
        refresh();
        wrefresh(multiplayerGameMenu);
        assignPositionToSnakes();
        gamePlay_Snake_SinglePlayer();
    }
    else if(gameMenuUserSelection == 1)
    {
        wclear(multiplayerGameMenu);
        refresh();
        wrefresh(multiplayerGameMenu);
        assignPositionToSnakes();
        gamePlay_Snake_MultiPlayer();
    }
    else if(gameMenuUserSelection == 2)
    {
        showHighScores();
        /* 
        
        high score stuff to be added here
        
        first ----> store all the results of single player in a separate file
        second ---> store all the results of multiplayer in a separate file
        third ----> display them separately 
        
        */
    }
    else if(gameMenuUserSelection == 3)
    {
        wclear(stdscr);
        wrefresh(stdscr);
        thanksForPlaying();
    }

    wrefresh(multiplayerGameMenu);
    refresh();
}


void winnerOfGame(const char *nameOfWinner)
{
    int height = 3, width = 30, startX, startY;

    startX = (getmaxx(stdscr) - width) / 2;
    startY = (getmaxy(stdscr) - height) / 2;

    WINDOW * winnerOfGame_window = newwin(height, width, startY, startX);

    box(winnerOfGame_window,(int)'|',(int)'=');
    PANEL * winnerOfGame_panel = new_panel(winnerOfGame_window);

    init_pair(6,COLOR_WHITE,COLOR_BLACK);

    wattron(winnerOfGame_window,COLOR_PAIR(6));
    wattron(winnerOfGame_window,A_BOLD);
    wattron(winnerOfGame_window,A_REVERSE);
    wattron(winnerOfGame_window,A_BLINK);

    mvwprintw(winnerOfGame_window, getmaxy(winnerOfGame_window)/2, 4, " %s", nameOfWinner);

    wattroff(winnerOfGame_window,COLOR_PAIR(6));
    wattroff(winnerOfGame_window,A_BOLD);
    wattroff(winnerOfGame_window,A_REVERSE);
    wattroff(winnerOfGame_window,A_BLINK);

    update_panels();
    doupdate();

    getch();

    gameOver();
    refresh();
}

void showHighScores(void)
{
    int height = 20, width = 40, startX = (getmaxx(stdscr) - width) / 2, startY = (getmaxy(stdscr) - height) / 2;
    
    WINDOW * highScores_window = newwin(height, width, startY, startX);
    PANEL * highScores_panel = new_panel(highScores_window);

    box(highScores_window, (int)'^', (int)'|');
    mvwprintw(highScores_window,1,5," ~~~ H I G H   S C O R E S ~~~");
    mvwprintw(highScores_window,2,2,"====================================");
    
    mvwprintw(highScores_window,3,getmaxx(highScores_window)/4,"|| SINGLE PLAYER ||");
    mvwprintw(highScores_window,4,getmaxx(highScores_window)/4 + 3,"-------------");

    mvwprintw(highScores_window,10,4,"* * * * * * * * * * * * * * * * ");

    mvwprintw(highScores_window,11,getmaxx(highScores_window)/4 + 1,"|| MULTI PLAYER ||");
    mvwprintw(highScores_window,12,getmaxx(highScores_window)/4 + 3,"-------------");


    update_panels();
    doupdate();
    getch();

    hide_panel(highScores_panel);   //  hiding the HIGH SCORES panel

    update_panels();                //  updating ~~~ panels ~~~ showing ~~ the ~~~~~~~~ screen
    doupdate();                     //  ~~~~~~~~ the ~~~~~~ and ~~~~~~~ on ~~~ Terminal

    multiplayerGameMenu();
}
