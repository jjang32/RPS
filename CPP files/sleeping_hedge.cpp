#include "sleeping_hedge.h"
#include "ui_sleeping_hedge.h"
#include "lose.h"
#include "win.h"
#include <QKeyEvent>
#include <string>
#include <math.h>
#include <iostream>
#include "cheat.h"
using namespace std;

//the creation of a node
struct node {
    node* children[3] = {NULL, NULL, NULL};
    double r[3];
    double c[3];
    double prob[3];
    double loss[3];
};

//the creation of a tree
struct tree {
    node* root = new struct node();
};

//eta constant used for probability calculation
const double eta = 0.5;

sleeping_hedge::sleeping_hedge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sleeping_hedge)
{
    ui->setupUi(this);
}

sleeping_hedge::~sleeping_hedge()
{
    delete ui;
}

static int prediction();
static double res(int, int);
static void reset();
static QString print(int);

//tree construction where the experts at each node will be initialized
static tree t;

//current action of the computer and player
static int computer = prediction();
static int human = -1;

//current score of the computer and player
static int cPoints = 0;
static int hPoints = 0;

//history of all choices from human
static vector<int> acts;

//to count which action you are currently on
static int counter = 0;

static int c = 0;

void sleeping_hedge::on_cheat_clicked()
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

void sleeping_hedge::keyPressEvent(QKeyEvent* event) {
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

    if(counter != 0) {
        int temp = 0;
        node* holder = t.root;

        //finding the expectation of experts' loss and probabilities
        double expect = 0.0;
        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                expect += holder->prob[i]*holder->loss[i];
            }
            temp++;
        }

        temp = 0;
        holder = t.root;

        while(temp < counter) {
            //loss expectation is based off of previous probability and loss for all awake experts, not current probability and loss
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                holder->loss[i] = holder->loss[i] + res(human, i) - expect;
            }
            temp++;
        }
    }

    //counts the number of times this loop is run
    counter++;

    //records the previous actions done by the human
    acts.push_back(human);

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
    if(acts.size() == 0) {
        t.root = new node();
        return (int) (rand() / RAND_MAX * 3);
    }
    else {
        int temp = 0;
        node* holder = t.root;

        while(temp < counter) {
            if(holder->children[acts[acts.size() - temp - 1]] == NULL) {
                holder->children[acts[acts.size() - temp - 1]] = new node();
            }
            holder = holder->children[acts[acts.size() - temp - 1]];
            temp++;
        }

        temp = 0;
        holder = t.root;

        //sum of all loss across all experts
        double sumLoss = 0.0;

        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                sumLoss += exp(-1 * eta * holder->loss[i]);
            }
            temp++;
        }

        temp = 0;
        holder = t.root;

        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                holder->prob[i] = exp(-1 * eta * holder->loss[i]) / sumLoss;
            }
            temp++;
        }

        //sum of all probabilities of experts at this particular instance
        double pRock = 0.0;
        double pPaper = 0.0;
        double pScissors = 0.0;

        temp = 0;
        holder = t.root;

        //determines what prediction to make and what the computer will output by going through allexperts'
        //probabilities and adding them up to set up barriers
        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            pRock += holder->prob[0];
            pPaper += holder->prob[1];
            pScissors += holder->prob[2];
            temp++;
        }

        //random number generator for computer to select its action
        double random = (double) rand() / RAND_MAX;

        //sum of all probabilities of experts at this particular instance
        double pSum = pRock;

        if(random < pSum) {
            return 0;
        }

        pSum += pPaper;

        if(random < pSum) {
            return 1;
        }

        return 2;
    }
}
static double res(int human, int opponent) {
    if((human == 0 && opponent == 1) || (human == 1 && opponent == 2) || (human == 2 && opponent == 0)) {
        return 0;
    }
    else if((human == 1 && opponent == 0) || (human == 2 && opponent == 1) || (human == 0 && opponent == 2)) {
        return 1;
    }
    return 0.999999999;
}
static void reset() {
    t.root = new node();
    counter = 0;
    computer = prediction();
    human = -1;
    acts.clear();
    cPoints = 0;
    hPoints = 0;
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
