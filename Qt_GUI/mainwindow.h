#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "newgamedialog.h"
#include "GameViewer.h"
#include <queue>

namespace Ui
{
    class MainWindow;
}

struct AnimationData
{
    AnimationData(int col, int row, bool p) : col(col), row(row), c_row(0), p(p) {}
    int col;
    int row;
    int c_row;
    bool p; /// true for player1
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newGame();
    void setPlayer1Cell(int8_t row, int8_t col);
    void setPlayer2Cell(int8_t row, int8_t col);
    void cleanCells();
    void finished(State flag);
    void illegalMove();

private slots:
    void on_tableWidget_cellClicked(int row, int column);

    void on_newGame_pushButton_clicked();

    void animation_func();

private:
    Ui::MainWindow *ui;
    NewGameDialog newGameDialog;
    GameViewer gameViewer;
    queue<AnimationData> q;
    QTimer animationTimer;

    Qt::GlobalColor p1Color = Qt::green;
    Qt::GlobalColor p2Color = Qt::red;
};

#endif // MAINWINDOW_H
