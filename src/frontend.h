#include <unistd.h>
#include <ncurses.h>
#include <menu.h>

extern int EXIT;

/* For displays */
enum Command getCommand(int d);
void displayBoard(Board *board, WINDOW *win, int lvl);
enum Status respawnShip(Board *board, WINDOW *win, int lvl);

/* Game functions */
enum Status play(WINDOW *win, Board *board, int lvl);
void updateBoard(Board *board);
void writeScoreToFile(int score);

/* Menu functions */
void mainMenu(void);
void playGame(void);