#include "backend.h"

Board *createBoard(int max_x, int max_y) {
  Board *board = malloc(sizeof(Board));

  /* Allocate memory and create the spaceship */
  /* Change parameters depending on graphics choice */
  struct Spaceship *spaceship = createSpaceship(max_x/2, max_y - 2); 
  struct Aliens **aliens      = createBlockOfAliens(1, 1);
  struct Block **blocks       = createBlocks(max_y);

  board -> spaceship       = spaceship;
  board -> aliens          = aliens;
  board -> special_alien   = NULL;
  board -> blocks          = blocks;
  board -> alien_bomb      = NULL;
  board -> lives           = 3;
  board -> max_x           = max_x;
  board -> max_y           = max_y;
  board -> alien_direction = RIGHT;
  board -> score           = 0;

  return board;
}

void destroyBoard(Board *board) {
  free(board -> spaceship);
  freeAliens(board -> aliens);
  free(board -> aliens);
  freeBlocks(board -> blocks);
  free(board -> blocks);
  free(board);
}

void checkBulletCollision(Board *board) {
  /* Check if bullet fired */
  if(board -> spaceship -> bullet == NULL) 
    return;

  int i;
  struct Bullet *bullet = board -> spaceship -> bullet;

  /* Check for contact with block */
  for(i = 0; i < NO_BLOCKS; i++) {
    struct Block *block = board -> blocks[i];
    if(block == NULL) continue;

    if( (bullet -> y == block -> y) && (bullet -> x == block -> x) ) {
      board -> spaceship -> ammo = 1;

      /* Clean memory */
      free(bullet);
      free(block);
      board -> spaceship -> bullet = NULL;
      board -> blocks[i] = NULL;
      return;
    }
  }

  /* Check for contact with alien */
  for(i = 0; i < NO_ALIENS; i++) {
    struct Alien *alien = board -> aliens[i];
    if(alien == NULL) continue;

    if( (bullet -> y == alien -> y) && (bullet -> x == alien -> x || 
      bullet -> x == alien -> x + 1 || bullet -> x == alien -> x + 2) ) {
      board -> score += alien -> reward;
      board -> spaceship -> ammo = 1;

      /* Clean it up by deallocating memory */
      free(bullet);
      free(alien);
      board -> spaceship -> bullet = NULL;
      board -> aliens[i] = NULL;
      return;
    }
  }

  /* Check contact with special Alien */
  if( board -> special_alien != NULL) {
    struct Alien *alien = board -> special_alien;

    if( (bullet -> y == alien -> y) && (bullet -> x == alien -> x || 
      bullet -> x == alien -> x + 1 || bullet -> x == alien -> x + 2
      || bullet -> x == alien -> x + 3 || bullet -> x == alien -> x + 4) ) {
      board -> spaceship -> ammo = 1;

      free(bullet);
      free(alien);
      board -> spaceship -> bullet = NULL;
      board -> special_alien = NULL;
      return;
    }
  }
}

