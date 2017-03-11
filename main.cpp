#include "tictactoe.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load(":/beauty/lang/tictactoe_fr.qm");
    a.installTranslator(&translator);

    TicTacToe w;
    w.show();

    return a.exec();
}
