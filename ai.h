#ifndef AI_H
#define AI_H

#include "definitions.h"
#include "toolbox.h"
#include <QPair>
#include <QVector>
#include <QDebug>


class AI
{
public:
    AI();
    static QPair<int, int> play(Square game[3][3], short int depth=6, Square player=Cross);

private:
    static QVector<QPair<int, int>> getFreeSquares(Square game[3][3]);
    static Square opponent(Square player);
    static short int minMax(Square game[3][3], short int row, short int column, short int depth, Square player, QString direction);
    static short int score(Square game[3][3], short int row, short int column);
    static std::array<short int, 3> counting(Square s1, Square s2, Square s3);
    static short int impact(std::array<short int, 3> count);
};

#endif // AI_H
