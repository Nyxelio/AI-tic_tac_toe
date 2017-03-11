#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "definitions.h"

class Toolbox
{
public:
    Toolbox();
    static bool isWinning(Square game[3][3], short int row, short int column);
    static bool isLineWinning(Square game[3][3], short int row, short int column);
    static bool isColumnWinning(Square game[3][3], short int row, short int column);
    static bool isDiago1Winning(Square game[3][3], short int row, short int column);
    static bool isDiago2Winning(Square game[3][3], short int row, short int column);
    };

#endif // TOOLBOX_H
