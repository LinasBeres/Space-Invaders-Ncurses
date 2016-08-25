#include "backend.h"
#include "frontend.h"
#include <menu.h>

/* Main menu choices */
char *m_choices[] = {
  "Play the game",
  "How to play",
  "High Scores",
  "Exit",
  (char *)NULL,
};

int EXIT = false;

/* Function taken from the ncurses HOWTO page */
void printInMiddle(WINDOW *win, int start_y, int start_x, int width, char *string) {
  int length, x, y;
  float temp;

  
  if(win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if(start_x != 0)
    x = start_x;
  if(start_y != 0)
    y = start_y;
  if(width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length)/ 2;
  x = start_x + (int)temp;
  mvwprintw(win, y, x, "%s", string);
  refresh();
}

void playGame(void) {
  int width, height;
  width = 67; height = 21;

  /* Set up window */
  WINDOW *win;
  win = newwin(height , width, (LINES - height) /2, (COLS - width) /2);
  nodelay(win, TRUE); 
  keypad(win, TRUE);

  /* Set up board */
  int level = 1;
  enum Status s = GAME_ON;
  Board *board = createBoard(width, height - 2);

  /* Game starts here */
  while(s != ALIENS_WON) {
    s = play(win, board, level);
    if(s == ALIENS_DEFEATED) {
      level++;
      ALIEN_SPEED = 190 - (level*10);
      updateBoard(board);
      usleep(600000);
    }
    if(s == QUIT)
      break;
  }
  writeScoreToFile(board -> score);

  werase(win);
  wrefresh(win);
  delwin(win);
  destroyBoard(board);
}

void howToPlay(void) {
  int width, height;
  width = 28; height = 10;

  /* Set up window */
  WINDOW *win;
  win = newwin(height , width, (LINES - height) /2, (COLS - width) /2);
  
  /* Draw the window */ 
  box(win, 0, 0);
  printInMiddle(win, 1, 0, 28, "High Scores");
  mvwaddch(win, 2, 0, ACS_LTEE);
  mvwhline(win, 2, 1, ACS_HLINE, 26);
  mvwaddch(win, 2, 27, ACS_RTEE);

  wattron(win , A_STANDOUT);
  printInMiddle(win, 8, 0, 28, "* Back");
  wattroff(win ,A_STANDOUT);

  mvwprintw(win, 3, 1, "-x-   is worth 10 points");
  mvwprintw(win, 4, 1, "<o>   is worth 20 points");
  mvwprintw(win, 5, 1, "/-\\   is worth 30 points");
  mvwprintw(win, 6, 1, "~o0o~ is worth 50 points");

  wrefresh(win);

  int ch;
  while( (ch = wgetch(win)) != 10 ) {

  }

  werase(win);
  wrefresh(win);
  delwin(win);
}

void highScores(void) {
  FILE *fp;
  fp = fopen("high-scores.txt", "r");

  char *ptr, str[70];
  int scores[3], i;

  /* Store high scores for each mode into an array */

  for(i = 0; i < 3; i++) {
    fgets(str, 70, fp);
    scores[i] = strtol(str, &ptr, 10);
 
  }

  /* Files not needed anymore so close */
  fclose(fp);

  WINDOW *high_scores_win;  
  int ch;

  high_scores_win = newwin(8, 20, (LINES - 8) /2, (COLS - 20) /2);
  keypad(high_scores_win, TRUE);

  /* Draw the window */ 
  box(high_scores_win, 0, 0);
  printInMiddle(high_scores_win, 1, 0, 20, "High Scores");
  mvwaddch(high_scores_win, 2, 0, ACS_LTEE);
  mvwhline(high_scores_win, 2, 1, ACS_HLINE, 18);
  mvwaddch(high_scores_win, 2, 19, ACS_RTEE);

  wattron(high_scores_win , A_STANDOUT);
  mvwprintw(high_scores_win, 6, 7, "* Back");
  wattroff(high_scores_win ,A_STANDOUT);

  for(i = 0; i < 3; i++ ) {
    mvwprintw(high_scores_win, 3 + i, 2, "%d", scores[i]);
  }

  wrefresh(high_scores_win);

  while( (ch = wgetch(high_scores_win)) != 10 ) {

  }

  wclear(high_scores_win);
  wrefresh(high_scores_win);
  delwin(high_scores_win);
  return;
}

void exitGame(void) {
  EXIT = true;
}

void mainMenu(void) {
  ITEM **m_items;
  MENU *main_menu;
  WINDOW *main_menu_win;
  int c, i, n_choices;

  /* Create items */
  n_choices = 4;
  m_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
      m_items[i] = new_item(m_choices[i], "");
  set_item_userptr(m_items[0], playGame);
  set_item_userptr(m_items[1], howToPlay);
  set_item_userptr(m_items[2], highScores);
  set_item_userptr(m_items[3], exitGame);

  /* Crate menu */
  main_menu = new_menu((ITEM **)m_items);

  /* Set menu option to not show description */
  menu_opts_off(main_menu, O_SHOWDESC);

  /* Create the window to be associated with the menu */
  main_menu_win = newwin(8, 20, (LINES - 8) /2, (COLS - 20) /2);
  keypad(main_menu_win, TRUE);
  
  /* Set main window and sub window */
  set_menu_win(main_menu, main_menu_win);
  set_menu_sub(main_menu, derwin(main_menu_win, 4, 18, 3, 1));

  /* Set menu mark to the string " * " */
  set_menu_mark(main_menu, " * ");

  while(!EXIT) {
    /* Display menu */
    post_menu(main_menu); 
    wrefresh(main_menu_win); 
    box(main_menu_win, 0, 0);
    printInMiddle(main_menu_win, 1, 0, 20, "Main Menu");
    mvwaddch(main_menu_win, 2, 0, ACS_LTEE);
    mvwhline(main_menu_win, 2, 1, ACS_HLINE, 18);
    mvwaddch(main_menu_win, 2, 19, ACS_RTEE);


    c = wgetch(main_menu_win);
    switch(c) {
      case KEY_DOWN:
        menu_driver(main_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
        menu_driver(main_menu, REQ_UP_ITEM);
        break;
      case 10: {  /* Enter */
        ITEM *cur;
        void (*p)(void);

        // unpost_menu(main_menu);
        // wrefresh(main_menu_win);

        cur = current_item(main_menu);
        p = item_userptr(cur);
        p();
        pos_menu_cursor(main_menu);
        break;
      }
    }
    wrefresh(main_menu_win);
  }
  /* Free up the memory */
  unpost_menu(main_menu); 
  free_menu(main_menu); 
  for(i = 0; i < n_choices; ++i)
    free_item(m_items[i]);
}

