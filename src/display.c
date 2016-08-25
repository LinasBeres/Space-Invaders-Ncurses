#include "backend.h"
#include "frontend.h"

enum Command getCommand(int d) {

  switch(d) {
    case 'a':
      return LEFT;
    case KEY_LEFT:
      return LEFT;
    case 'd':
      return RIGHT;
    case KEY_RIGHT:
      return RIGHT; 
    case ' ':
      return SHOOT;
    case 'p':
      return PAUSE;
    default:
      return HOLD;   
  }
}

void displaySpaceship(struct Spaceship *spaceship, WINDOW *win) {
  mvwprintw(win, spaceship -> y, spaceship -> x, spaceship -> image);
}

void displayBullet(struct Bullet *bullet, WINDOW *win) {
  if(bullet != NULL)
    mvwaddch(win, bullet -> y, bullet -> x, bullet -> symbol);
}

void displayAliens(struct Aliens **aliens, WINDOW *win) {
  int i;

  for(i = 0; i < NO_ALIENS; i++) {
    struct Alien *alien = aliens[i];
    if(alien == NULL) continue;

    mvwprintw(win, alien -> y, alien -> x, alien -> image);
  }
}

void displayBlocks(struct Block **blocks, WINDOW *win) {
  int i;

  for(i = 0; i < NO_BLOCKS; i++) {
    struct Block *block = blocks[i];
    if(block == NULL) continue;

    mvwaddch(win, block -> y, block -> x, block -> symbol);
  }
}

void displaySpecialAlien(struct Alien *alien, WINDOW *win) {
  if(alien != NULL)
    mvwprintw(win, alien -> y, alien -> x, alien -> image);
}

void displayLevel(Board *board, int lvl, WINDOW *win) {
  mvwprintw(win, board -> max_y, 1, "Level: %d", lvl);
}

void displayScore(Board *board, WINDOW *win) {
  mvwprintw(win, board -> max_y , 12, "Score: %d", board -> score);
}

void displayLives(Board *board, WINDOW *win) {
  int i, x;
  x = 0;
  mvwprintw(win, board -> max_y , 31, "Number of lives: ");
  for(i = 0; i < board -> lives; i++, x += 6) {
    mvwprintw(win, board -> max_y , 48 + x, "%s ", board -> spaceship -> image);
  }
}


void displayBoard(Board *board, WINDOW *win, int lvl) {
  displaySpaceship(board -> spaceship, win);
  displayAliens(board -> aliens, win);
  displayBlocks(board -> blocks, win);
  displayBullet(board -> spaceship -> bullet, win);
  displayBullet(board -> alien_bomb, win);
  displaySpecialAlien(board -> special_alien, win);
  /* Display bottom border **NOTE THIS WILL BE TAKEN OUT AND PUT INTO THE WINDOW DEF** */
  box(win, 0, 0);
  mvwaddch(win, board -> max_y - 1, 0, ACS_LTEE);
  mvwhline(win, board -> max_y - 1, 1, ACS_HLINE, board -> max_x - 2);
  mvwaddch(win, board -> max_y - 1, board -> max_x - 1, ACS_RTEE);
  displayLevel(board, lvl, win);
  displayScore(board, win);
  displayLives(board, win);
  
}

enum Status respawnShip(Board *board, WINDOW *win, int lvl) {
  if( board -> lives == 0 ) 
    return ALIENS_WON;
  else {
    board -> lives--;
    displayBoard(board, win, lvl);
    mvwprintw(win, board -> spaceship -> y, board -> spaceship -> x, "*/#\\*");
    wrefresh(win);
    usleep(100000);
    mvwprintw(win, board -> spaceship -> y, board -> spaceship -> x, "#-:+\\");
    wrefresh(win);
    usleep(100000);
    mvwprintw(win, board -> spaceship -> y, board -> spaceship -> x, "-<.<\\");
    wrefresh(win);
    usleep(100000);
    mvwprintw(win, board -> spaceship -> y, board -> spaceship -> x, "#..\\@");
    wrefresh(win);
    usleep(100000);
    mvwprintw(win, board -> spaceship -> y, board -> spaceship -> x, "*/#\\*");
    wrefresh(win);
    usleep(100000);
    return GAME_ON;
  }
}