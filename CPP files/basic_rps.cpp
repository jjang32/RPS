#include "basic_rps.h"
#include "ui_basic_rps.h"
#include "lose.h"
#include "win.h"
#include <QKeyEvent>
#include <string>
#include <iostream>
#include "cheat.h"
using namespace std;

basic_RPS::basic_RPS(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::basic_RPS)
{
    ui->setupUi(this);
}

basic_RPS::~basic_RPS()
{
    delete ui;
}

static int prediction();
static double res(int, int);
static void reset();
static QString print(int);

//current action of the computer and player
static int computer = prediction();
static int human = -1;

//current score of the computer and player
static int cPoints = 0;
static int hPoints = 0;

//to count which action you are currently on
static int counter = 0;

static int c = 0;

void basic_RPS::on_cheat_clicked()
{
    if(computer == 0) {
        QPixmap pix("/Users/justin_jang/Downloads/rock.png");
        ui->computer->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
    }
    else if(computer == 1) {
        QPixmap pix("/Users/justin_jang/RPS/paper.png");
        ui->computer->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
    }
    else {
        QPixmap pix("/Users/justin_jang/Downloads/scissors.png");
        ui->computer->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
    }
    ui->cheating->setText("CHEATING");
    c++;
}

void basic_RPS::keyPressEvent(QKeyEvent* event) {
    if(event->key() == Qt::Key_1) {
        QPixmap pix("/Users/justin_jang/Downloads/rock.png");
        ui->human->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
        event->accept();
        human = 0;
    }
    else if(event->key() == Qt::Key_2) {
        QPixmap pix("/Users/justin_jang/RPS/paper.png");
        ui->human->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
        event->accept();
        human = 1;
    }
    else if(event->key() == Qt::Key_3) {
        QPixmap pix("/Users/justin_jang/Downloads/scissors.png");
        ui->human->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
        event->accept();
        human = 2;
    }
    else {
        ui->human->setText("Input: Invalid");
        event->accept();
        return;
    }

    if(computer == 0) {
        QPixmap pix("/Users/justin_jang/Downloads/rock.png");
        ui->computer->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
    }
    else if(computer == 1) {
        QPixmap pix("/Users/justin_jang/RPS/paper.png");
        ui->computer->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
    }
    else {
        QPixmap pix("/Users/justin_jang/Downloads/scissors.png");
        ui->computer->setPixmap(pix.scaled(120, 120, Qt::KeepAspectRatio));
    }

    ui->cheating->setText("");

    //counts the number of times this loop is run
    counter++;

    //the result of the human vs. computer match
    double action = res(human, computer);

    computer = prediction();

    QString s;

    //determines who wins
    if(action == 0) {
        cPoints++;
        ui->cScore->setText(QString::number(cPoints));
        ui->determine->setText("LOSE");
    }
    else if(action == 1) {
        hPoints++;
        ui->hScore->setText(QString::number(hPoints));
        ui->determine->setText("WIN");
    }
    else if(action == 0.999999999) {
        ui->determine->setText("DRAW");
    }

    //WIN SCREEN -> add buttons for restart or quit
    if(hPoints == 100) {
        this->close();
        reset();
        if(c > 0) {
            cheat c;
            c.setModal(true);
            c.exec();
            return;
        }
        Win w;
        w.setModal(true);
        w.exec();
    }
    //LOSE SCREEN -> add buttons for restart or quit
    else if(cPoints == 100) {
        this->close();
        reset();
        Lose l;
        l.setModal(true);
        l.exec();
    }
}

static int prediction() {
    return (int) (rand() / (double) RAND_MAX * 3);
}

//outcome of rock-paper-scissors
static double res(int human, int opponent) {
    if((human == 0 && opponent == 1) || (human == 1 && opponent == 2) || (human == 2 && opponent == 0)) {
        return 0;
    }
    else if((human == 1 && opponent == 0) || (human == 2 && opponent == 1) || (human == 0 && opponent == 2)) {
        return 1;
    }
    return 0.999999999;
}

static QString print(int n) {
    if(n == 0) {
        return "Rock";
    }
    else if(n == 1) {
        return "Paper";
    }
    else if(n == 2) {
        return "Scissors";
    }
    return "";
}

static void reset() {
    counter = 0;
    computer = prediction();
    human = -1;
    cPoints = 0;
    hPoints = 0;
}
