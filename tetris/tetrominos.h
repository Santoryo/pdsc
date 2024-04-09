#ifndef TETROMINOS_H
#define TETROMINOS_H

typedef struct {
  int positions[4][4][4];
  int rotation_state;
  int x;
  int y;
  int color;
} Tetromino;

extern Tetromino lblock;
extern Tetromino jblock;
extern Tetromino iblock;
extern Tetromino oblock;
extern Tetromino sblock;
extern Tetromino zblock;
extern Tetromino tblock;

#endif