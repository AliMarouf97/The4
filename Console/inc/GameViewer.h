/**
 * @file GameViewer.h
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief Game Viewer
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef GAMEVIEWER_H
#define GAMEVIEWER_H

#include "Game/Game.h"
#include <string>
#include <sstream>
#include <iterator>
//          foreground background
// black        30         40
// red          31         41
// green        32         42
// yellow       33         43
// blue         34         44
// magenta      35         45
// cyan         36         46
// white        37         47

enum Color
{
    FG_BLACK = 30,
    FG_RED = 31,
    FG_GREEN = 32,
    FG_YELLOW = 33,
    FG_BLUE = 34,
    FG_MAGENTA = 35,
    FG_CYAN = 36,
    FG_WHITE = 37,
    FG_DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_BLUE = 44,
    BG_DEFAULT = 49
};

#define color(c) "\033[" << c << "m"

#define p1_color color(FG_YELLOW)
#define p2_color color(FG_CYAN)
#define def_color color(FG_DEFAULT)

#define player1 'o'
#define player2 'x'

class GameViewer : public ViewListener
{
public:
    GameViewer();
    ~GameViewer();
    void startGame();

private:
    void newGame();
    void setPlayer1Cell(int8_t row, int8_t col);
    void setPlayer2Cell(int8_t row, int8_t col);
    void cleanCells();
    void finished(State flag); // 3 draw,1,2
    void illegalMove();

    void print();
    void clearConsole();

    bool ask_YN(string txt);
    char ask_for_move(const vector<int8_t> &available);
    
    template <typename T>
    T ask(string txt, const vector<T> &options, bool printOptions = false);
    template <typename T>
    T ask_range(string txt, T mn, T mx);

    template <typename T>
    string vec2string(const vector<T> &v);

    char grid[8][8];
    Game game;
};

#endif