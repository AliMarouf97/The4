#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&gameViewer, SIGNAL(setPlayer1Cell(int8_t, int8_t)), this, SLOT(setPlayer1Cell(int8_t, int8_t)));
    connect(&gameViewer, SIGNAL(setPlayer2Cell(int8_t, int8_t)), this, SLOT(setPlayer2Cell(int8_t, int8_t)));
    connect(&gameViewer, SIGNAL(newGame()), this, SLOT(newGame()));
    connect(&gameViewer, SIGNAL(illegalMove()), this, SLOT(illegalMove()));
    connect(&gameViewer, SIGNAL(cleanCells()), this, SLOT(cleanCells()));
    connect(&gameViewer, SIGNAL(finished(State)), this, SLOT(finished(State)));

    connect(&animationTimer, &QTimer::timeout, [this]()
            { animation_func(); });

    gameViewer.startGame(true, true, 3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animation_func()
{
    if (q.empty())
    {
        animationTimer.stop();
        return;
    }

    AnimationData &x = q.front();

    if (x.row < x.c_row)
    {
        q.pop();
        return;
    }

    if (x.c_row != 0)
    {
        ui->tableWidget->setItem(x.c_row - 1, x.col, new QTableWidgetItem);
    }

    ui->tableWidget->setItem(x.c_row, x.col, new QTableWidgetItem);
    ui->tableWidget->item(x.c_row, x.col)->setBackground((x.p) ? p1Color : p2Color);

    x.c_row++;
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(row)

    /// Accept the new move only if the animation has been executed.
    if (q.empty())
        gameViewer.playMove(column);
}

void MainWindow::newGame()
{
    ui->label->setText("Game is running");
    ui->label_2->setText("Player1 Turn");
}
void MainWindow::setPlayer1Cell(int8_t row, int8_t col)
{
    ui->label->setText("Game is running");
    ui->label_2->setText("Player2 Turn");

    q.push(AnimationData(col, 7 - row, 1));
    if (!animationTimer.isActive())
        animationTimer.start(100);
}
void MainWindow::setPlayer2Cell(int8_t row, int8_t col)
{
    ui->label->setText("Game is running");
    ui->label_2->setText("Player1 Turn");
    q.push(AnimationData(col, 7 - row, 0));
    if (!animationTimer.isActive())
        animationTimer.start(100);
}
void MainWindow::cleanCells()
{
    ui->tableWidget->clear();
    ui->label_2->setText("");
    ui->label->setText("Game is running");
}
void MainWindow::finished(State flag)
{
    if (flag == State::Player1_Win)
        ui->label->setText("Player1 Win");
    else if (flag == State::Player2_Win)
        ui->label->setText("Player2 Win");
    else if (flag == State::Draw)
        ui->label->setText("Draw");

    ui->label_2->setText("");
}
void MainWindow::illegalMove()
{
    ui->label->setText("Illegal Move");
}

void MainWindow::on_newGame_pushButton_clicked()
{
    bool isAutoPlay, isFirst;
    int level;

    if (newGameDialog.showMe(isAutoPlay, isFirst, level))
    {
        gameViewer.startGame(isAutoPlay, isFirst, level);
    }
}
