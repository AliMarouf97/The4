#include "Game.h"
#include <time.h>

Game::Game()
{
    srand((unsigned int)time(0));
    cleanTable();
}

Game::~Game()
{
}

vector<int8_t> Game::getAvailableMoves()
{
    vector<int8_t> v;
    if (result != State::NotFinished)
        return v;

    for (int i = 0; i < 8; i++)
        if (colCells[i] < 8)
            v.push_back(i);

    return v;
}

///////////// View
void Game::addViewListener(ViewListener *toAdd)
{
    listeners.push_back(toAdd);
}
void Game::v_setPlayer1Cell(int8_t row, int8_t col)
{
    for (ViewListener *vl : listeners)
    {
        vl->setPlayer1Cell(row, col);
    }
}
void Game::v_setPlayer2Cell(int8_t row, int8_t col)
{
    for (ViewListener *vl : listeners)
    {
        vl->setPlayer2Cell(row, col);
    }
}
void Game::v_cleanCells()
{
    for (ViewListener *vl : listeners)
    {
        vl->cleanCells();
    }
}
void Game::v_finished(State flag)
{
    for (ViewListener *vl : listeners)
    {
        vl->finished(flag);
    }
}
void Game::v_illegalMove()
{
    for (ViewListener *vl : listeners)
    {
        vl->illegalMove();
    }
}
void Game::v_newGame()
{
    for (ViewListener *vl : listeners)
    {
        vl->newGame();
    }
}

////////////////////////////
void Game::cleanTable()
{
    v_cleanCells();

    for (int8_t i = 0; i < 8; i++)
    {
        for (int8_t j = 0; j < 8; j++)
        {
            Grid[i][j] = 0;
        }
        colCells[i] = 0;
    }
    result = State::NotFinished;
    Moves = 0;
    p = 1;
}

int8_t Game::computer(int8_t p, int8_t last_c, int8_t depth)
{

    State res = judge(Grid, colCells, last_c);
    if (res == State::Draw)
        return 0;
    else if (res == State::Player1_Win)
        return 1;
    else if (res == State::Player2_Win)
        return -1;

    if (depth > MaxDepth + Moves / 16)
        return 0;

    int8_t t = ((p == 1) ? -1 : 1);
    for (int8_t i = 0; i < 8; i++)
    {
        if (colCells[i] < 8)
        {
            Grid[colCells[i]][i] = p;
            colCells[i]++;

            if (p == 1)
                t = max(t, computer(3 - p, i, depth + 1));
            else
                t = min(t, computer(3 - p, i, depth + 1));

            colCells[i]--;
            Grid[colCells[i]][i] = 0;

            if ((p == 1 && t == 1) || (p == 2 && t == -1))
                return t;
        }
    }
    return t;
}
bool Game::play(int8_t player, int8_t col)
{
    if (Grid[7][col] != 0)
        return false;

    ++Moves;
    int8_t row = colCells[col]++;
    if (player == 1)
        v_setPlayer1Cell(row, col);
    else
        v_setPlayer2Cell(row, col);

    Grid[row][col] = player;
    return true;
}

State Game::judge(int8_t grid[8][8], int8_t colCells[8], int8_t col)
{
    int8_t R = colCells[col] - 1;
    int8_t flag = grid[R][col];
    State Win = (flag == 1) ? State::Player1_Win : State::Player2_Win;

    int8_t cnt = 1;
    /// check col

    for (int8_t k = R - 1; k >= 0; k--)
    {
        if (grid[k][col] != flag)
            break;
        ++cnt;
    }
    if (cnt >= 4)
        return Win;

    /// check rows
    int8_t A = min(col + 3, 7);
    int8_t B = max(col - 3, 0);
    cnt = 1;
    for (int8_t i = col + 1; i <= A; i++)
    {
        if (grid[R][i] != flag)
            break;
        ++cnt;
    }
    for (int8_t i = col - 1; i >= B; i--)
    {
        if (grid[R][i] != flag)
            break;
        ++cnt;
    }
    if (cnt >= 4)
        return Win;

    /// check main d
    int8_t diff = col - R;
    A = min(col + 3, 7);
    B = max(col - 3, 0);
    cnt = 1;
    for (int8_t i = col + 1; i <= A && i - diff >= 0 && i - diff <= 7; i++)
    {
        if (grid[i - diff][i] != flag)
            break;
        ++cnt;
    }
    for (int8_t i = col - 1; i >= B && i - diff >= 0 && i - diff <= 7; i--)
    {
        if (grid[i - diff][i] != flag)
            break;
        ++cnt;
    }
    if (cnt >= 4)
        return Win;

    /// check another d
    cnt = 1;
    int8_t d = R + col;
    A = min(R + 3, 7);
    B = max(0, R - 3);
    for (int8_t i = R + 1; i <= A; i++)
    {
        int8_t k = d - i;
        if (k < 0 || grid[i][k] != flag)
            break;
        ++cnt;
    }
    for (int8_t i = R - 1; i >= B; i--)
    {
        int8_t k = d - i;
        if (k < 0 || grid[i][k] != flag)
            break;
        ++cnt;
    }
    if (cnt >= 4)
        return Win;

    /// check if not finished yet
    for (int8_t i = 0; i < 8; i++)
    {
        if (colCells[i] < 8)
            return State::NotFinished;
    }
    return State::Draw;
}

State Game::computerTurn()
{
    int8_t com;
    int8_t move;
    if (Moves < 2)
    {
        if (Moves == 0)
            move = RANDOM(8);
        else
            move = RANDOM(4) + 2;

        play(p, move);
        p = 3 - p;
        result = judge(Grid, colCells, move);
        if (result != State::NotFinished)
        {
            v_finished(result);
        }
        return result;
    }
    std::vector<int8_t> best, draw, lose;

    for (int8_t i = 0; i < 8; i++)
    {
        if (colCells[i] < 8)
        {
            Grid[colCells[i]][i] = p;
            colCells[i]++;
            com = computer(3 - p, i, 0);
            colCells[i]--;
            Grid[colCells[i]][i] = 0;

            if ((p == 1 && com > 0) || (p == 2 && com < 0))
                best.push_back(i);
            else if (com == 0)
                draw.push_back(i);
            else
                lose.push_back(i);
        }
    }

    if (!best.empty())
        move = best[RANDOM(best.size())];
    else if (!draw.empty())
    {
        if (RANDOM(100) < MaxDepth * 10 + 20)
            move = draw[draw.size() / 2];
        else
            move = draw[RANDOM(draw.size())];
    }
    else if (Moves < 10)
        move = lose[lose.size() / 2];
    else
        move = lose[RANDOM(lose.size())];

    play(p, move);
    p = 3 - p;
    result = judge(Grid, colCells, move);
    if (result != State::NotFinished)
    {
        v_finished(result);
    }
    return result;
}

State Game::letsPlay(int8_t column)
{
    if (result != State::NotFinished)
        return result;

    if (!play(p, column))
    {
        v_illegalMove();
        return State::IllegalMove;
    }

    p = 3 - p;
    result = judge(Grid, colCells, column);
    if (result != State::NotFinished)
    {
        v_finished(result);
        return result;
    }

    if (isAutoPlay)
    {
        return computerTurn();
    }

    return State::NotFinished;
}
void Game::setMaxDepth(int8_t depth)
{
    MaxDepth = depth;
}
void Game::newGame(bool isAutoPlay, bool computerFirst)
{
    this->isAutoPlay = isAutoPlay;
    this->computerFirst = computerFirst;

    cleanTable();
    v_newGame();
    if (isAutoPlay && computerFirst)
        computerTurn();
}

State Game::checkMove(int8_t col)
{
    if (colCells[col] < 8)
    {
        Grid[colCells[col]][col] = p;
        colCells[col]++;
        int8_t com = computer(3 - p, col, 0);
        colCells[col]--;
        Grid[colCells[col]][col] = 0;
        if (com > 0)
            return State::Player1_Win;
        else if (com < 0)
            return State::Player1_Win;
        else
            return State::Draw;
    }
    return State::IllegalMove;
}
