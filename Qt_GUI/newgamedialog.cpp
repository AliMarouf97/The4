#include "newgamedialog.h"
#include "ui_newgamedialog.h"

NewGameDialog::NewGameDialog(QWidget *parent) : QDialog(parent),
                                                ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);
}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}

bool NewGameDialog::showMe(bool &isAutoPlay, bool &isFirst, int &level)
{
    ok = false;
    exec();
    isAutoPlay = ui->computer_checkBox->isChecked();
    isFirst = ui->First_radioButton->isChecked() || (ui->Random_radioButton->isChecked() && rand() % 2);
    level = ui->level_spinBox->value();
    return ok;
}

void NewGameDialog::on_computer_checkBox_stateChanged(int arg1)
{
    ui->level_spinBox->setEnabled(arg1);
    ui->groupBox->setEnabled(arg1);
}

void NewGameDialog::on_ok_pushButton_clicked()
{
    ok = true;
    close();
}

void NewGameDialog::on_cancel_pushButton_clicked()
{
    close();
}
