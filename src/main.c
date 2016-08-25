#include "backend.h"
#include "frontend.h"

int main(int argc, char *argv[]) {
  /* Start ncurses */
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(FALSE);
  nodelay(stdscr, TRUE);  /* Stop ncurses waiting for user input */

  /* Main function here */
  mainMenu();

  endwin();
  return EXIT_SUCCESS;
}