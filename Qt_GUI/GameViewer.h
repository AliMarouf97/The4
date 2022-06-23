#ifndef GAMEVIEWER_H
#define GAMEVIEWER_H

#include "Game.h"
#include <QObject>
#include <QThread>

class GameViewer : public QObject, ViewListener
{
    Q_OBJECT
public:
    GameViewer();
    ~GameViewer();
    void startGame(bool isAutoPlay, bool isFirst = true, int level = 3);
    void startGame();

private slots:
    void move(int col);

signals:
    void newGame();
    void setPlayer1Cell(int8_t row, int8_t col);
    void setPlayer2Cell(int8_t row, int8_t col);
    void cleanCells();
    void finished(State flag);
    void illegalMove();

    /// called by user
    void playMove(int col);

private:
    Game game;
    QThread thread;
};

#endif // GAMEVIEWER_H
