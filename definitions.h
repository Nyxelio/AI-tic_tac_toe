#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define NOUGHT_SYMBOL 'O'
#define CROSS_SYMBOL 'X'
#define EASY 1
#define MEDIUM 4
#define HARD 6
#define PLAYER1 0
#define PLAYER2 1

#define MAX_LIMIT 32767 //short int
#define MIN_LIMIT -32767 //short int

enum Square {
    Empty=0,
    Nought,
    Cross
};

#endif // DEFINITIONS_H
