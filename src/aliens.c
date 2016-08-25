#include "backend.h"

void increaseAlienSpeed(int y) {
  switch(y) {
    case 7:
      ALIEN_SPEED -= 5;
      break;
    case 8:
      ALIEN_SPEED -= 5;
      break;
    case 10:
      ALIEN_SPEED -= 10;
      break;
    case 12:
      ALIEN_SPEED -= 10;
      break;
    case 13:
      ALIEN_SPEED -= 20;
      break;
  }
}

struct Alien **createBlockOfAliens(int corner_x, int corner_y) {
  /* Initialise random seed for special alien */
  time_t t;
  srand((unsigned) time(&t));

  struct Alien **aliens = malloc(sizeof(struct Alien)*NO_ALIENS);
  int i, x, y;
  x = corner_x; y = corner_y;

  /* For the first ten aliens - type B*/
  for(i = 0; i < 10; i++, x += 3) {
    aliens[i] = createAlien(x, y, 'B');
  }
  y++; x = corner_x;
  /* Next 20 aliens - type M*/
  for(i = 10; i < 30; i++, x += 3) {
    if(i == 20) {
      y++; x = corner_x;
    }
    aliens[i] = createAlien(x, y, 'M');
  }
  y++; x = corner_x;
  /* Next 20 aliens - type S*/
  for(i = 30; i < 50; i++, x += 3) {
    if(i == 40) {
      y++; x = corner_x;
    }
    aliens[i] = createAlien(x, y, 'S');
  }

  return aliens;
}

struct Alien *createAlien(int x, int y, char type) {
  struct Alien *alien = malloc(sizeof(struct Alien));

  alien -> x = x; alien -> y = y;
  alien -> type = type;

  switch(type) {
    case 'B':
      alien -> image  = "/-\\";
      alien -> reward = 30;
      return alien;
    case 'M':
      alien -> image  = "<o>";
      alien -> reward = 20;
      return alien;
    case 'S':
      alien -> image  = "-x-";
      alien -> reward = 10;
      return alien;
    case 'A':
      alien -> image  = "~o0o~";
      alien -> reward = 100;
      return alien;
  }
}

enum Status dropAliens(Board *board) {
  int i;

  /* If next drop down is on the blocks then delete the blocks */
  struct Alien *alien = findBottomAlien(board -> aliens);
  struct Block *block = findTopBlock(board -> blocks);
  if( block != NULL ) {
    if( alien -> y + 2 >= block -> y )
      freeBlocks(board -> blocks);
  }

  /* Aliens have reached bottom */
  if( alien -> y + 1 == board -> max_y - 2)
    return ALIENS_WON;

  increaseAlienSpeed(alien -> y + 1);

  /* If special alien does not exist then spawn it */
  if(board -> special_alien == NULL) {
    if( (rand() % 4 == 0) && (alien -> y >= 5) ) {
      board -> special_alien = createAlien(board -> max_x - 5, 1, 'A');
    }
  }

  /* Drop down 1 */
  for(i = 0; i < NO_ALIENS; i++) {
    alien = board -> aliens[i];
    if(alien == NULL) continue;
    alien -> y++;
  }

  return GAME_ON;
}

void fireAlienBomb(Board *board, int depth) {
  /* Find the chances */
  int chance;
  switch(depth) {
    case 7:
    case 8:
      chance = 15;
      break;
    case 9:
    case 10:
      chance = 12;
      break;
    case 11:
    case 12:
      chance = 10;
      break;
    case 13:
    case 14:
      chance = 9;
      break;
    default:
      chance = 15;
  }

  if( rand() % chance == 0) {
    /* Find the drop point of the bomb */
    struct Alien *alien = findLeftAlien(board -> aliens);
    int x_position;
    int max = alien -> x + 10; int min = alien -> x;
    x_position = (int) (max + rand() / (RAND_MAX / (min - max + 1) + 1));

    /* Allocate and assign memory */
    struct Bullet *alien_bomb = malloc(sizeof(struct Bullet));
    alien_bomb -> x = x_position; alien_bomb -> y = depth + 1;
    alien_bomb -> symbol = ':';

    board -> alien_bomb = alien_bomb;
  }
  /* Else don't fire bomb */
}

void moveAlienBomb(Board *board) {
  struct Bullet *alien_bomb = board -> alien_bomb;

  if( alien_bomb == NULL ) return;
  if( alien_bomb -> y >= board -> max_y - 1) {
    free(alien_bomb);
    board -> alien_bomb = NULL;
  }else {
    /* Check for contact with block */
    int i;
    for(i = 0; i < NO_BLOCKS; i++) {
      struct Block *block = board -> blocks[i];
      if(block == NULL) continue;

      if( (alien_bomb -> y == block -> y) && (alien_bomb -> x == block -> x) ) {
        /* Clean memory */
        free(alien_bomb);
        free(block);
        board -> alien_bomb = NULL;
        board -> blocks[i] = NULL;
        return;
      }
    }
    alien_bomb -> y++;
  }
}

enum Status moveAliens(Board *board) {
  int i;

  /* If reached right side then drop down */
  struct Alien *alien = findRightAlien(board -> aliens);
  if(board -> alien_direction == RIGHT && alien -> x + 1 == board -> max_x - 3) {
    /* Switch Direction */
    board -> alien_direction = LEFT;

    return dropAliens(board);
  }

  /* If reached left side then drop down */
  alien = findLeftAlien(board -> aliens);
  if(board -> alien_direction == LEFT && alien -> x - 1 <= 0) {
    /* Switch Direction */
    board -> alien_direction = RIGHT;

    return dropAliens(board);
  }

  /* Fire alien bomb if there currently is no bomb */
  if(board -> alien_bomb == NULL) {
    alien = findBottomAlien(board -> aliens);
    fireAlienBomb(board, alien -> y);
  }

  /* Move each alien along by 1 */
  for(i = 0; i < NO_ALIENS; i++) {
    alien = board -> aliens[i];
    if(alien == NULL) continue;
    if(board -> alien_direction == RIGHT)
      alien -> x++;
    else
      alien -> x--;
  }
  return GAME_ON;
}

void moveSpecialAlien(Board *board) {
  struct Alien *special_alien = board -> special_alien;
  if(special_alien == NULL) return;
  if(special_alien -> x - 1 == 0) {
    free(special_alien);
    board -> special_alien = NULL;
  }
  special_alien -> x--;
}


struct Alien *findNonNullAlien(struct Alien **aliens) {
  int i;
  struct Alien *contender_alien;

  for(i = 0; i < NO_ALIENS; i++) { 
    contender_alien = aliens[i];
    if( contender_alien != NULL) break;
  }

  return contender_alien;
}

struct Alien *findBottomAlien(struct Alien **aliens) {
  int i;
  struct Alien *contender_alien = findNonNullAlien(aliens);

  for(i = 0; i < NO_ALIENS; i++) {
    struct Alien *alien = aliens[i];
    if( alien == NULL ) continue;
    if( alien -> y >= contender_alien -> y )
      contender_alien = alien;
  }

  return contender_alien;
} 

struct Alien *findLeftAlien(struct Alien **aliens) {
  int i;

  struct Alien *contender_alien = findNonNullAlien(aliens);

  for(i = 0; i < NO_ALIENS; i++) {
    struct Alien *alien = aliens[i];
    if( alien == NULL ) continue;
    if( alien -> x <= contender_alien -> x )
      contender_alien = alien;
  }

  return contender_alien;
} 

struct Alien *findRightAlien(struct Alien **aliens) {
  int i;

  struct Alien *contender_alien = findNonNullAlien(aliens);

  for(i = 0; i < NO_ALIENS; i++) {
    struct Alien *alien = aliens[i];
    if( alien == NULL ) continue;
    if( alien -> x >= contender_alien -> x )
      contender_alien = alien;
  }

  return contender_alien;
} 

void freeAliens(struct Alien **aliens) {
  int i;

  for(i = 0; i < NO_ALIENS; i++) {
    struct Alien *alien = aliens[i];
    free(alien);
    aliens[i] = NULL;
  }
}

/* Special Alien Functions */
