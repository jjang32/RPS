#include "win.h"
#include "ui_win.h"
#include "mainwindow.h"

Win::Win(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Win)
{
    ui->setupUi(this);
}

Win::~Win()
{
    delete ui;
}

void Win::on_retry_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
}
