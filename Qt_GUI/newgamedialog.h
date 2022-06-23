#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>

namespace Ui
{
    class NewGameDialog;
}

class NewGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGameDialog(QWidget *parent = nullptr);
    ~NewGameDialog();
    bool showMe(bool &isAutoPlay, bool &isFirst, int &level);

private slots:
    void on_computer_checkBox_stateChanged(int arg1);

    void on_ok_pushButton_clicked();

    void on_cancel_pushButton_clicked();

private:
    Ui::NewGameDialog *ui;
    bool ok;
};

#endif // NEWGAMEDIALOG_H
