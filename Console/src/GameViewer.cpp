/**
 * @file GameViewer.h
 * @author Ali Marouf (https://github.com/AliMarouf97)
 * @brief Game Viewer
 * @version 0.1
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "GameViewer.h"

GameViewer::GameViewer()
{
    // newGame();
    cleanCells();
    game.addViewListener(this);
}

GameViewer::~GameViewer()
{
}

void GameViewer::clearConsole()
{
    #if defined _WIN32
        cout << "\x1B[2J\x1B[H";
        // system("cls");
    #elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        // system("clear");
        cout << u8"\033[2J\033[1;1H"; // Using ANSI Escape Sequences
    #elif defined(__APPLE__)
        system("clear");
    #endif
}

void GameViewer::print()
{
    clearConsole();
    cout << "\n";

    for (int8_t i = 0; i < 8; i++)
    {
        cout << "|";
        for (int8_t j = 0; j < 8; j++)
        {
            if (grid[i][j] == player1)
                cout << p1_color;
            else if (grid[i][j] == player2)
                cout << p2_color;

            cout << grid[i][j] << " ";
        }

        cout << def_color << "|\n";
    }

    cout << " - - - - - - - -\n ";
    for (int8_t j = 0; j < 8; j++)
        cout << j + 1 << " ";
    cout << "\n";
}

void GameViewer::newGame()
{
    cleanCells();
    print();
}

void GameViewer::illegalMove()
{
    cout << color(FG_RED) << "illegal move: select an empty col\n"
         << def_color;
}

void GameViewer::setPlayer1Cell(int8_t row, int8_t col)
{
    grid[(7 - row)][col] = player1;
    print();
}
void GameViewer::setPlayer2Cell(int8_t row, int8_t col)
{
    grid[(7 - row)][col] = player2;
    print();
}

void GameViewer::cleanCells()
{
    for (int8_t i = 0; i < 8; i++)
        for (int8_t j = 0; j < 8; j++)
            grid[i][j] = ' ';
}

void GameViewer::finished(State flag)
{
    string txt;
    if (flag == State::Draw)
        txt = "Draw";
    else if (flag == State::Player1_Win)
        txt = "Player 1 Win";
    else if (flag == State::Player2_Win)
        txt = "Player 2 Win";

    cout << txt << "\n";
}

bool GameViewer::ask_YN(string txt)
{
    cout << txt << " (Y/N)\n";
    char c;
    cin >> c;

    switch (c)
    {
    case 'Y':
    case 'y':
        return true;
        break;
    case 'N':
    case 'n':
        return false;
        break;
    default:
        return ask_YN(txt);
    }
}

template <typename T>
string GameViewer::vec2string(const vector<T> &v)
{
    if (v.empty())
        return "";

    std::ostringstream oss;
    std::copy(v.begin(), v.end() - 1, std::ostream_iterator<T>(oss, ", "));
    oss << v.back();

    return oss.str();
}

template <typename T>
T GameViewer::ask(string txt, const vector<T> &options, bool printOptions)
{
    if (printOptions)
        cout << txt << " (" << vec2string(options) << ")\n";
    else
        cout << txt << "\n";

    T x;
    cin >> x;

    auto it = find(options.begin(), options.end(), x);

    if (it == options.end())
        return ask(txt, options, printOptions);
    return x;
}

template <typename T>
T GameViewer::ask_range(string txt, T mn, T mx)
{
    cout << txt << " [" << mn << " ," << mx << "].\n";
    T x;
    cin >> x;

    if (x < mn || x > mx)
        return ask_range(txt, mn, mx);
    return x;
}

char GameViewer::ask_for_move(const vector<int8_t> &available)
{
    cout << "Select one of avaliable col: (";

    for (int i = 0; i < available.size() - 1; i++)
        cout << (int)available[i] + 1 << ", ";

    if (!available.empty())
        cout << (int)(*--available.end()) + 1;

    cout << ") or (Q) to quit.\n\n";

    char c;
    cin >> c;
    if (c == 'Q' || c == 'q')
        return 'q';

    for (auto m : available)
        if (m == c - '1')
            return c;

    return ask_for_move(available);
}

void GameViewer::startGame()
{
    bool isAutoPlay = ask_YN("Do you want to play aginst computer?");
    bool computerFirst = true;
    if (isAutoPlay)
    {
        int level = ask_range("Select the level between", 1, 7);
        game.setMaxDepth(level);
        char whosFirst = ask("Do you want to play first or Not or Random? (Y,N,R)", vector<char>{'Y', 'y', 'N', 'n', 'R', 'r'}, false);
        if (whosFirst == 'Y' || whosFirst == 'y')
            computerFirst = false;
        else if ((whosFirst == 'R' || whosFirst == 'r') && RANDOM(2))
            computerFirst = false;
    }

    game.newGame(isAutoPlay, computerFirst);
    State g = State::NotFinished;
    char c;
    while (g == State::NotFinished)
    {
        c = ask_for_move(game.getAvailableMoves());
        if (c == 'q')
            break;
        g = game.letsPlay((c - '1'));
    }

    if (ask_YN("Do you want to play new game?"))
        startGame();
}
