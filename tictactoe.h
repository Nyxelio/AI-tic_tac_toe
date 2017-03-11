#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QWidget>
#include <QPointer>
#include "definitions.h"
#include "toolbox.h"
#include "ai.h"
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QPushButton;
class QLabel;
class QSlider;

class TicTacToe : public QWidget
{
    Q_OBJECT

public:
    TicTacToe(QWidget *parent = 0);
    ~TicTacToe();

private:
    void initBoard();
    void handle();
    QPointer<QVBoxLayout> m_layout;
    QPointer<QHBoxLayout> m_ctlLayout;
    QPointer<QGridLayout> m_boardLayout;
    QPointer<QPushButton> m_btnNewGame;
    QPointer<QPushButton> m_board[3][3];
    QPointer<QLabel> m_lblCurrentPlayer;
    QPointer<QPushButton> m_btnPvP;
    QPointer<QPushButton> m_btnPvC;
    QPointer<QSlider> m_sldrHardness;
    void initQSS();
    static QChar toSymbol(Square s);
    bool m_currentPlayer;
    void setNextPlayer();
    void setPlayer(bool player);
    bool getCurrentPlayer();
    Square squareValueOfCurrentPlayer();
    Square m_game[3][3];
    QPair<int, int> btnToPosition(QPushButton *btn);
    void updateBoard(short int row, short int column, Square value);
    bool isOver();
    bool isEmpty(short int row, short int column);
    bool hasAI();
    short int m_turnCount;
    bool m_won;
    bool m_hasAi;
    bool m_drawMatch;
    int m_aiHardness;
    bool playTurn(QPair<int, int> pos);
    void prettify(QPushButton *btn, QString styleClass);

private slots:
    void slHandleSquare();
    void initGame();
    void initPvC();
    void initPvP();
    void setHardness(int position);
};

#endif // TICTACTOE_H
