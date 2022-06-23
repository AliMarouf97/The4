#include "GameViewer.h"
#include <QDebug>
GameViewer::GameViewer()
{
    game.addViewListener(this);

    qRegisterMetaType<int8_t>("int8_t");
    qRegisterMetaType<State>("State");

    moveToThread(&thread);
    connect(this, SIGNAL(playMove(int)), this, SLOT(move(int)));
    thread.start();
}

GameViewer::~GameViewer()
{
    thread.quit();
    thread.wait();
}

void GameViewer::startGame(bool isAutoPlay, bool isFirst, int level)
{
    game.setMaxDepth(level);
    game.newGame(isAutoPlay, !isFirst);
}

void GameViewer::startGame()
{
    startGame(false);
}

void GameViewer::move(int col)
{
    disconnect(this, SIGNAL(playMove(int)), this, SLOT(move(int))); /// To ignore new moves while processing the game.
    game.letsPlay(col);
    connect(this, SIGNAL(playMove(int)), this, SLOT(move(int)));
}
