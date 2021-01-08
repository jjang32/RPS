#include "lose.h"
#include "ui_lose.h"
#include "mainwindow.h"

Lose::Lose(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lose)
{
    ui->setupUi(this);
}

Lose::~Lose()
{
    delete ui;
}

void Lose::on_retry_clicked()
{
    MainWindow *mainwindow = new MainWindow();
    mainwindow->show();
}
