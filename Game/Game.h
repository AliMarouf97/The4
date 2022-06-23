#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>
#include <algorithm>

#define RANDOM(mx) (rand() % (mx))

using namespace std;

enum State
{
    NotFinished = 0,
    Player1_Win = 1,
    Player2_Win = 2,
    Draw = 3,
    IllegalMove = 4,
};

// An interface to be implemented by everyone
class ViewListener
{
public:
    virtual void newGame() = 0;
    virtual void setPlayer1Cell(int8_t row, int8_t col) = 0;
    virtual void setPlayer2Cell(int8_t row, int8_t col) = 0;
    virtual void cleanCells() = 0;
    virtual void illegalMove() = 0;
    virtual void finished(State flag) = 0; // 3 draw,1,2
};

class Game
{
public:
    Game();
    ~Game();

    void addViewListener(ViewListener *toAdd);
    State letsPlay(int8_t column);
    void setMaxDepth(int8_t depth);
    void newGame(bool isAutoPlay, bool computerFirst = false);

    vector<int8_t> getAvailableMoves();
    static State judge(int8_t grid[8][8], int8_t colCells[8], int8_t col);
    State checkMove(int8_t col);

protected:
    void v_setPlayer1Cell(int8_t row, int8_t col);
    void v_setPlayer2Cell(int8_t row, int8_t col);
    void v_cleanCells();
    void v_finished(State flag);
    void v_illegalMove();
    void v_newGame();

private:
    int8_t computer(int8_t p, int8_t last_c, int8_t depth);
    State computerTurn();
    bool play(int8_t player, int8_t col);
    void cleanTable();

    int8_t Grid[8][8];
    int8_t colCells[8];
    int8_t Moves = 0;
    int8_t p = 1;
    State result = State::NotFinished;
    int8_t MaxDepth = 4;
    bool isAutoPlay = false, computerFirst = false;
    vector<ViewListener *> listeners;
};

#endif
