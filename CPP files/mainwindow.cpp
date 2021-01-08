#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adanormal.h"
#include "basic_rps.h"
#include "fixed_hedge.h"
#include "fixedtree.h"
#include "sleeping_hedge.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_AdaNormal_clicked()
{
    AdaNormal a;
    a.setModal(true);
    a.exec();
}

void MainWindow::on_Basic_clicked()
{
    basic_RPS b;
    b.setModal(true);
    b.exec();
}

void MainWindow::on_Hedge_clicked()
{
    Fixed_Hedge f;
    f.setModal(true);
    f.exec();
}

void MainWindow::on_Tree_clicked()
{
    FixedTree t;
    t.setModal(true);
    t.exec();
}

void MainWindow::on_SleepingHedge_clicked()
{
    sleeping_hedge s;
    s.setModal(true);
    s.exec();
}
