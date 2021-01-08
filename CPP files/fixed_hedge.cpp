#include "fixed_hedge.h"
#include "ui_fixed_hedge.h"
#include "lose.h"
#include "win.h"
#include <QKeyEvent>
#include <string>
#include <math.h>
#include <iostream>
#include "cheat.h"
using namespace std;

Fixed_Hedge::Fixed_Hedge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fixed_Hedge)
{
    ui->setupUi(this);
}

Fixed_Hedge::~Fixed_Hedge()
{
    delete ui;
}

const static double eta = 0.1;

//the creation of a node
struct expert {
    int value = -1;
    double prob;
    double loss;
};

static int prediction();
static double res(int, int);
static void reset();
static int predict();
static QString print(int);

static expert experts[100];

//current action of the computer and player
static int computer = prediction();
static int human = -1;

//current score of the computer and player
static int cPoints = 0;
static int hPoints = 0;

//to count which action you are currently on
static int counter = 0;

static int c = 0;

void Fixed_Hedge::on_cheat_clicked()
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

void Fixed_Hedge::keyPressEvent(QKeyEvent* event) {
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

    //adds the amount of loss performed by each expert
    //CHANGE CONDITIONAL NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    for(int i = 0; i < 100; i++) {
        experts[i].loss = experts[i].loss + res(human, experts[i].value);
    }

    //the sum of all experts' loss
    //CHANGE CONDITIONAL NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    double sumLoss = 0.0;
    for(int i = 0; i < 100; i++) {
        sumLoss += exp(-1 * eta * experts[i].loss);
    }

    //updating probability for all experts
    //CHANGE CONDITIONAL NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    double tSum = 0.0;
    for(int i = 0; i < 99; i++) {
        experts[i].prob = exp(-1 * eta * experts[i].loss) / sumLoss;
        tSum += experts[i].prob;
    }
    //to avoid errors in probability, just subtract all previous probabilities added together from 1
    //CHANGE ARRAY NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    experts[99].prob = 1 - tSum;

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
    for(int i = 0; i < 100; i++) {
        //CHANGE INITIAL PROBABILITY BASED OFF OF CHANGE IN NUMBER OF EXPERTS
        experts[i].prob = 0.01;
        experts[i].loss = 0.0;
    }
    for(int i = 0; i < 100; i++) {
        experts[i].value = predict();
    }

    //random number generator for computer to select its action
    double random = (double) rand() / INT_MAX;

    //sum of all probabilities of experts at this particular instance
    double pSum = experts[0].prob;

    //counter to help track which prediction to make
    int counter = 0;

    //determines what prediction to make and what the computer will output by going through all experts'
    //probabilities and adding them up to set up barriers
    while(random > pSum) {
        counter++;
        pSum += experts[counter].prob;
    }

    //when the computer is finally within a certain probability, then you set the prediction of that expert into the
    //computer's output
    return experts[counter].value;
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

static int predict() {
    return (int) (rand() / (double) RAND_MAX * 3);
}
