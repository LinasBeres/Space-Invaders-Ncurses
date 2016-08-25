#include "backend.h"

struct Block **createBlocks(int max_y) {
  struct Block **blocks = malloc(sizeof(struct Block)*NO_BLOCKS);
  int i = 0;

  // blocks[0] = createBlock(3, max_y - 10);

  individualBlocks(blocks, i, 0, max_y);
  i = 19;
  individualBlocks(blocks, i, 17, max_y);
  i = 38;
  individualBlocks(blocks, i, 34, max_y);
  i = 57;
  individualBlocks(blocks, i, 51, max_y);
    
  return blocks;
}

struct Block *createBlock(int x, int y) {
  struct Block *block = malloc(sizeof(struct Block));

  block -> x = x;
  block -> y = y;
  block -> symbol = '#';

  return block;
}

void individualBlocks(struct Block **blocks, int i, int edge_d, int y) {
  int max = i;
  int j;
  int x = 0;
  for(j = i; j < max + 3; j++) {
    blocks[j] = createBlock(7 + x + edge_d, y - 8);
    x++;
  }
  x = 0;   
  for(j = i + 3; j < max + 8; j++) {
    blocks[j] = createBlock(6 + x + edge_d, y - 7);
    x++;
  }
  x = 0;
  for(j = i + 8; j < max + 15; j++) {
    blocks[j] = createBlock(5 + x + edge_d, y - 6);
    x++;
  }
  blocks[max+15] = createBlock(5 + edge_d, y - 5); blocks[max+16] = createBlock(6 + edge_d, y - 5);
  blocks[max+17] = createBlock(10 + edge_d, y - 5); blocks[max+18] = createBlock(11 + edge_d, y - 5);
}

struct Block *findTopBlock(struct Block **blocks) {
  int i;
  struct Block *contender_block = NULL;

  for(i = 0; i < NO_BLOCKS; i++) {
    contender_block = blocks[i];
    if( contender_block != NULL ) break;
  }

  if( contender_block == NULL ) return contender_block;

  for(i = 0; i < NO_BLOCKS; i++) {
    struct Block *block = blocks[i];
    if( block == NULL ) continue;
    if( block -> y <= contender_block -> y )
      contender_block = block;
  }

  return contender_block;
}

void freeBlocks(struct Block **blocks) {
  int i;

  for(i = 0; i < NO_BLOCKS; i++) {
    struct Block *block = blocks[i];
    if( block == NULL ) continue; 
    free(block);
    blocks[i] = NULL;
  }
}