#include "tictactoe.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QFile>
#include <QDebug>
#include <QLabel>
#include <QStyle>
#include <QSlider>

TicTacToe::TicTacToe(QWidget *parent)
    : QWidget(parent)
{

    initBoard();
    initQSS();
    initGame();
    handle();


    m_btnPvC->click();
}

TicTacToe::~TicTacToe()
{
}


void TicTacToe::initBoard()
{

    resize(300,350);

    m_layout = new QVBoxLayout(this);

    m_ctlLayout = new QHBoxLayout();

    m_btnPvP = new QPushButton(tr("Joueur VS Joueur"));
    m_btnPvC = new QPushButton(tr("Joueur VS Ordinateur"));
    m_btnPvP->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_btnPvC->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_btnPvP->setFixedHeight(200);
    m_btnPvC->setFixedHeight(200);

    m_sldrHardness = new QSlider();
    m_sldrHardness->setOrientation(Qt::Horizontal);
    m_sldrHardness->setRange(EASY,HARD);
    m_sldrHardness->setValue(HARD);
    m_sldrHardness->setTickInterval(3);
    m_sldrHardness->setTickPosition(QSlider::TicksBothSides);
    m_sldrHardness->setSingleStep(3);

    m_btnPvP->setObjectName("btnPvP");
    m_btnPvC->setObjectName("btnPvC");

    m_ctlLayout->addWidget(m_btnPvP);
    m_ctlLayout->addWidget(m_btnPvC);	

    m_boardLayout = new QGridLayout();

    for(unsigned short int i=0; i < 3 ; ++i)
    {
        for(unsigned short int j=0; j < 3; j++)
        {
            m_board[i][j] = new QPushButton();

            m_boardLayout->addWidget(m_board[i][j],i, j, Qt::AlignCenter);

        }
    }

    m_btnNewGame = new QPushButton(tr("Nouvelle partie"));
    m_btnNewGame->setObjectName("btnNewGame");

    m_btnNewGame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_btnNewGame->setFixedHeight(200);

    m_lblCurrentPlayer = new QLabel();

    m_layout->addLayout(m_ctlLayout);
    m_layout->addWidget(m_sldrHardness);
    m_layout->addWidget(m_lblCurrentPlayer);
    m_layout->addLayout(m_boardLayout);
    m_layout->addWidget(m_btnNewGame);

}

void TicTacToe::handle()
{
    for(unsigned i=0; i < 3; ++i){
        std::for_each(m_board[i], m_board[i] + 3 , [this](QPointer<QPushButton> _p)
        {
            connect(_p.data(), &QPushButton::clicked, this, &TicTacToe::slHandleSquare);
        });
    }

    connect(m_btnNewGame, &QPushButton::clicked, this, &TicTacToe::initGame);
    connect(m_btnPvC, &QPushButton::clicked, this, &TicTacToe::initPvC);
    connect(m_btnPvP, &QPushButton::clicked, this, &TicTacToe::initPvP);

    connect(m_sldrHardness, &QSlider::sliderMoved, this, &TicTacToe::setHardness);
}

void TicTacToe::initQSS()
{
    QFile file(":/beauty/beauty.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    file.close();
    setStyleSheet(styleSheet);
}

void TicTacToe::initPvC(){
    m_hasAi = true;

    m_aiHardness = m_sldrHardness->value();

    initGame();
}

void TicTacToe::initPvP(){
    m_hasAi = false;
    initGame();
}

void TicTacToe::setHardness(int position){
    m_aiHardness = position;
}

void TicTacToe::slHandleSquare(){
    bool res;
    QPair<int,int> pos;
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    pos = btnToPosition(btn);

    res = playTurn(pos);

    if(m_turnCount == 9 && m_drawMatch)
    {
       prettify(btn, "player1");
    }

    if(res)
    {
        prettify(btn, QString("player%1").arg(!getCurrentPlayer()+1));
    }

    if(m_won){
        prettify(btn, QString("player%1").arg(getCurrentPlayer()+1));
    }

    if(hasAI() && getCurrentPlayer() == PLAYER2)
    {

        QPair<int, int> posAI;
        posAI = AI::play(m_game, m_aiHardness, Cross);

        QPointer<QPushButton> button = m_board[posAI.first][posAI.second];

        res = playTurn(posAI);

        if(res)
        {
            prettify(button, "player2");
        }
    }
}

void TicTacToe::prettify(QPushButton *btn, QString styleClass){
    btn->setProperty("StyleClass", styleClass);
    btn->style()->unpolish(btn);
    btn->style()->polish(btn);
    btn->update();
}

bool TicTacToe::playTurn(QPair<int, int> pos){

    ++m_turnCount;

    if (!isEmpty(pos.first, pos.second) || isOver()){
        return false;
    }

    m_game[pos.first][pos.second] = squareValueOfCurrentPlayer();

    updateBoard(pos.first, pos.second, m_game[pos.first][pos.second]);

    if((m_won = Toolbox::isWinning(m_game, pos.first, pos.second))){
        m_lblCurrentPlayer->setText(tr("&Player %n won !", "",getCurrentPlayer()+1));
        return true;
    }

    if(m_turnCount > 8)
    {
        m_drawMatch = true;
        m_lblCurrentPlayer->setText(tr("&Draw match !"));
        return false;
    }

    setNextPlayer();

    return true;
}

bool TicTacToe::isOver(){
    return m_won || m_turnCount >= 10;
}

bool TicTacToe::isEmpty(short int row, short int column){
    return (m_game[row][column] == Empty) ? true : false;
}

void TicTacToe::updateBoard(short int row, short int column, Square value){

    m_board[row][column]->setText(toSymbol(value));
}

QPair<int, int> TicTacToe::btnToPosition(QPushButton *btn){
    QPair<int, int> pos;
    int rowspan, colspan;
    int index = m_boardLayout->indexOf(btn);

    m_boardLayout->getItemPosition(index, &pos.first, &pos.second, &rowspan, &colspan );

    return pos;
}


void TicTacToe::initGame(){
    for (short int i=0; i<3; ++i)
    {
        for(short int j=0; j<3; ++j)
        {
            m_game[i][j] = Empty;
            m_board[i][j]->setText(toSymbol(m_game[i][j]));
            prettify(m_board[i][j], "board");
        }
    }

    m_won = 0;

    setPlayer(0);

    m_turnCount = 0;

    m_drawMatch = false;
    m_aiHardness = m_sldrHardness->value();

}

bool TicTacToe::hasAI(){
    return m_hasAi;
}

void TicTacToe::setNextPlayer(){
    setPlayer(!m_currentPlayer);
}

bool TicTacToe::getCurrentPlayer(){
   return m_currentPlayer;
}

void TicTacToe::setPlayer(bool player)
{
    m_lblCurrentPlayer->setText(tr("Player %n", "", player+1));
    m_currentPlayer = player;
}

Square TicTacToe::squareValueOfCurrentPlayer(){
    return (m_currentPlayer == PLAYER1) ? Nought : Cross ;
}

QChar TicTacToe::toSymbol(Square s)
{
    char c;

    switch(s){
        case Empty:
            break;
        case Nought:
            c = NOUGHT_SYMBOL;
            break;
        case Cross:
            c = CROSS_SYMBOL;
            break;
    }
    return c;
}
