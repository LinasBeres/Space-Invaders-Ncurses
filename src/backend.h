#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define NO_ALIENS 50
#define NO_BLOCKS 76

#define BULLET_SPEED 50
#define ALIEN_BOMB_SPEED 105
#define SPECIAL_ALIEN_SPEED 180

extern int ALIEN_SPEED;

enum Command {LEFT, RIGHT, SHOOT, HOLD, PAUSE};
enum Status {ALIENS_WON, GAME_ON, SHOT_DOWN, ALIENS_DEFEATED, QUIT};

/* Board Object */
typedef struct {
  struct Spaceship *spaceship;
  struct Aliens **aliens;
  struct Aliens *special_alien;
  struct Blocks **blocks;
  struct Bullet *alien_bomb;
  enum Command alien_direction;
  int lives;
  int max_x, max_y;
  int score;
}Board;

Board *createBoard(int max_x, int max_y);
void destroyBoard(Board *board);
void checkBulletCollision(Board *board);

/* Block Object */
struct Block {
  int x, y;
  char symbol;
};

struct Block **createBlocks(int max_y);
struct Block *createBlock(int x, int y);
void individualBlocks(struct Block **blocks, int i, int edge_d, int y);
void freeBlocks(struct Block **blocks);
struct Block *findTopBlock(struct Block **blocks);

/* Spaceship Object */ 
struct Spaceship {
  char *image;
  int x, y;
  int ammo;
  struct Bullet *bullet;
};


struct Bullet {
  char symbol;
  int x, y;
};

struct Spaceship *createSpaceship(int x, int y);
enum Status commandSpaceship(Board *board, enum Command com);
void fireSpaceshipBullet(Board *board, int start_x, int start_y);
void moveSpaceshipBullets(Board *board);


/* Alien Object */
struct Alien {
  char *image;
  int x, y;
  char type; /* Can either be (B)big, (M)medium, (S)small or (A)special*/
  int reward;
};

extern void increaseAlienSpeed(int y);
struct Alien **createBlockOfAliens(int corner_x, int corner_y);
struct Alien *createAlien(int x, int y, char type);
enum Status moveAliens(Board *board);
struct Alien *findBottomAlien(struct Alien **aliens);
struct Alien *findLeftAlien(struct Alien **aliens);
struct Alien *findRightAlien(struct Alien **aliens);
void freeAliens(struct Alien **aliens);
void moveSpecialAlien(Board *board);
void moveAlienBomb(Board *board);