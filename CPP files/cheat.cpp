#include "cheat.h"
#include "ui_cheat.h"
#include "mainwindow.h"

cheat::cheat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cheat)
{
    ui->setupUi(this);
}

cheat::~cheat()
{
    delete ui;
}

void cheat::on_retry_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
}
