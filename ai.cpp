#include "ai.h"

AI::AI()
{

}

QPair<int, int> AI::play(Square game[3][3], short int depth, Square player){

    QVector< QPair<int, int> > freeSquares;
    QPair<int, int> pos;
    short int max, newVal;
    Square newGame[3][3];
    QPair<int, int> curPos;

    freeSquares = getFreeSquares(game);

    max =  MIN_LIMIT;

    for (short int i = 0; i < freeSquares.size(); ++i) {
        curPos = freeSquares.at(i);

        for(short int t = 0; t < 3; ++t){
            std::copy(game[t], game[t]+3, newGame[t]);
        }


        newGame[curPos.first][curPos.second] = player;

        //Minimize score of human player
        newVal = minMax(newGame, curPos.first, curPos.second, depth - 1, opponent(player), "min");

        if(newVal > max){
            max = newVal;
            pos = curPos;
        }
    }

    return pos;
}

QVector<QPair<int, int>> AI::getFreeSquares(Square game[3][3]){

    QVector<QPair<int, int>> freeSquares;

    for (short int r=0; r<3; ++r)
    {
        for(short int c=0; c<3; ++c)
        {
            if(game[r][c] == Empty){
                freeSquares.append(QPair<int, int>(r, c));
            }
        }
    }


    return freeSquares;
}


Square AI::opponent(Square player){
    return (player == Cross) ? Nought : Cross;
}

short int AI::minMax(Square game[3][3], short int row, short int column, short int depth, Square player, QString direction)
{
    QVector< QPair<int, int> > freeSquares;
    QPair<int, int> curPos;
    short int limit, node;
    Square newGame[3][3];

    if(depth == 0 || Toolbox::isWinning(game, row, column))
    {
        return score(game, row, column);
    }
    else{

        freeSquares = getFreeSquares(game);

        if(!freeSquares.size()){
            return score(game, row, column);
        }

        if(direction == "max"){
            limit = MIN_LIMIT;
        }
        else
        {
            limit = MAX_LIMIT;
        }

        for (short int i = 0; i < freeSquares.size(); ++i) {
            curPos = freeSquares.at(i);

            for(short int t = 0; t < 3; ++t){
                std::copy(game[t], game[t]+3, newGame[t]);
            }

            newGame[curPos.first][curPos.second] = player;

            if(direction == "max")
            {
                node = minMax(newGame, curPos.first, curPos.second, depth - 1, opponent(player), "min");
                limit = std::max(limit, node);
            }
            else
            {
                node = minMax(newGame, curPos.first, curPos.second, depth - 1, opponent(player), "max");
                limit = std::min(limit, node);
            }
        }

        return limit;
    }
}

short int AI::score(Square game[3][3], short int row, short int column){
    short int score;
    QVector< QPair<int, int> > freeSquares;

    if(Toolbox::isWinning(game, row, column))
    {
        //won by human player
        if(game[row][column] == Nought){
            return -1000;
        }
        else{
            return 1000;
        }
    }

    freeSquares = getFreeSquares(game);

    if(!freeSquares.size()){
        return 0;
    }

    score = 0;

    //scoring for each row
    for(short int r=0; r<3; ++r){
        score += impact(counting(game[r][0], game[r][1], game[r][2]));
    }

    //scoring for each column
    for(short int c=0; c<3; ++c){
        score += impact(counting(game[0][c], game[1][c], game[2][c]));
    }

    //scoring for descending diagonal
    score += impact(counting(game[0][0], game[1][1], game[2][2]));

    //scoring for ascending diagonal
    score += impact(counting(game[2][0], game[1][1], game[0][2]));

    return score;
}

//return [emptyCount, HumanCount, AICount] array
std::array<short int, 3> AI::counting(Square s1, Square s2, Square s3){
    std::array<short int, 3> count = {0, 0, 0};
    Square squares[3];

    squares[0] = s1;
    squares[1] = s2;
    squares[2] = s3;


    std::for_each(squares, squares + 3 , [&](Square s)
    {
        if(s == Empty)
        {
            count[0] += 1;
        }
        if(s == Nought){
            count[1] += 1;
        }
        if(s == Cross){
            count[2] += 1;
        }
    });

    return count;
}

short int AI::impact(std::array<short int, 3> count){
    short int score = 0;

    // 1 empty square
    if(count[0] == 1){
        // 2 human squares
        if(count[1] == 2){
            score -=50;
        }
        // 2 AI squares
        if(count[2] == 2){
            score +=50;
        }
    }

    // 2 empty squares
    if(count[0] == 2){
        // 1 human square
        if(count[1] == 1){
            score -=20;
        }
        // 1 AI square
        if(count[2] == 1){
            score +=20;
        }
    }
    return score;
}
