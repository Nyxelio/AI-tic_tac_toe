#include "toolbox.h"

Toolbox::Toolbox()
{

}


//check line, column, then diagonals
bool Toolbox::isWinning(Square game[3][3], short int row, short int column){
    if(isLineWinning(game, row, column)){
        return true;
    }

    if(isColumnWinning(game, row, column)){
        return true;
    }

    if(row == column){

        //Middle square, we need to check both diagonals
        if(row == 1){
            return isDiago1Winning(game, row, column) || isDiago2Winning(game, row, column);
        }
        else{
            return isDiago1Winning(game, row, column);
        }
    }
    else if((row == 2 && column == 0) || (row == 0 && column == 2)){
        return isDiago2Winning(game, row, column);
    }

    return false;
}


bool Toolbox::isLineWinning(Square game[3][3], short int row, short int column){
    Square value = game[row][column];

    for(short int col=0; col<3; ++col){
            if ((game[row][col] != value)){
                return false;
            }

    }

    return true;
}

bool Toolbox::isColumnWinning(Square game[3][3], short int row, short int column){
    Square value = game[row][column];

    for(short int r=0; r<3; ++r){
            if ((game[r][column] != value)){
                return false;
            }

    }

    return true;
}

//Diago1: Descending left-right diagonal
bool Toolbox::isDiago1Winning(Square game[3][3], short int row, short int column){
    Square value = game[row][column];

    for(short int s=0; s<3; ++s){
      if ((game[s][s] != value)){
          return false;
      }
    }

    return true;
}

//Diago2: Ascending left-right diagonal
bool Toolbox::isDiago2Winning(Square game[3][3], short int row, short int column){
    Square value = game[row][column];

    if(game[2][0] != value || game[0][2] != value || game[1][1] != value){
        return false;
    }

    return true;
}
