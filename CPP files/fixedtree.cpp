#include "fixedtree.h"
#include "ui_fixedtree.h"
#include "lose.h"
#include "win.h"
#include <QKeyEvent>
#include <string>
#include <math.h>
#include <iostream>
#include "cheat.h"
using namespace std;

FixedTree::FixedTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FixedTree)
{
    ui->setupUi(this);
}

FixedTree::~FixedTree()
{
    delete ui;
}

const static double eta = 0.1;

//the creation of a node
struct node {
    node* children[3];
    int value;
    int pred;
};

//the creation of a tree expert
struct tree {
    node* root;
    double prob;
    double loss;
    int depth;
};

static void construct(int, int, int, node*);
static bool traverse(int, int);
static int prediction();
static double res(int, int);
static void reset();
static int predict(bool, tree, vector<int>, int);
static QString print(int);

const int eNum = 100;

static tree experts[eNum];

static int ePredictions [eNum];

//current action of the computer and player
static int computer = prediction();
static int human = -1;

//history of all choices from human
static vector<int> acts;

//current score of the computer and player
static int cPoints = 0;
static int hPoints = 0;

//to count which action you are currently on
static int counter = 0;

static int c = 0;

void FixedTree::on_cheat_clicked()
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

void FixedTree::keyPressEvent(QKeyEvent* event) {
    if(counter == 0) {
        for(int i = 0; i < eNum; i++) {
            experts[i].root = new node();
            //CHANGE INITIAL PROBABILITY BASED OFF OF CHANGE IN NUMBER OF EXPERTS
            experts[i].prob = 1 / eNum;
            experts[i].loss = 0.0;
            experts[i].depth = (rand() % 7) + 1;
            experts[i].root->pred = -1;
            experts[i].root->value = -1;
            construct(i, 0, experts[i].depth, experts[i].root);
        }
    }

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

    //records the previous actions done by the human
    acts.push_back(human);

    //adds the amount of loss performed by each expert
    //CHANGE CONDITIONAL NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    for(int i = 0; i < eNum; i++) {
        experts[i].loss += res(human, ePredictions[i]);
    }

    //the sum of all experts' loss
    //CHANGE CONDITIONAL NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    double sumLoss = 0.0;
    for(int i = 0; i < eNum; i++) {
        sumLoss += exp(-1 * eta * experts[i].loss);
    }

    //updating probability for all experts
    //CHANGE CONDITIONAL NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    double tSum = 0.0;
    for(int i = 0; i < eNum - 1; i++) {
        experts[i].prob = exp(-1 * eta * experts[i].loss) / sumLoss;
        tSum += experts[i].prob;
    }
    //to avoid errors in probability, just subtract all previous probabilities added together from 1
    //CHANGE ARRAY NUMBER BASED OFF OF CHANGE IN NUMBER OF EXPERTS
    experts[eNum - 1].prob = 1 - tSum;

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

//builds a tree expert
static void construct(int expert, int count, int d, node* r) {
    if(count < d - 1) {
        for(int i = 0; i < 3; i++) {
            r->children[i] = new node();
            r->children[i]->value = i;
            r->children[i]->pred = -1;
            construct(expert, count + 1, d, r->children[i]);
        }
    }
    else if(count == d - 1) {
        for(int i = 0; i < 3; i++) {
            r->children[i] = new node();
            r->children[i]->value = i;
            r->children[i]->pred = -1;
            construct(expert, count + 1, d, r->children[i]);
        }
    }
    else if(count == d) {
        r->pred = rand() % 3;
    }
}

//check if the tree is traversable for the amount of actions that the player has made
static bool traverse(int n, int d) {
    return n >= d;
}

static int prediction() {
    if(counter == 0) {
        return (int) (rand() / (double) RAND_MAX * 2);
    }
    //all predictions from each expert
    //CHANGE ARRAY AND CONDITIONAL NUMBERS TO CHANGE NUMBER OF EXPERTS
    for(int i = 0; i < eNum; i++) {
        ePredictions[i] = predict(traverse(counter, experts[i].depth), experts[i], acts, counter);
    }

    //random number generator for computer to select its action
    double random = (double) rand() / RAND_MAX;

    //sum of all probabilities of experts at this particular instance
    double pSum = experts[0].prob;

    //counter to help track which prediction to make
    int c = 0;

    //determines what prediction to make and what the computer will output by going through all experts' probabilities
    //and adding them up to set up barriers
    while(random > pSum) {
        c++;
        pSum += experts[c].prob;
    }

    //when the computer is finally within a certain probability, then you set the prediction of that expert into the
    //computer's output
    return ePredictions[c];
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
    acts.clear();
    cPoints = 0;
    hPoints = 0;
}

static int predict(bool out, tree t, vector<int> v, int c) {
    if(!out) {
        double random = (double) rand() / RAND_MAX;
        if(random < 0.33) {
            return 0;
        }
        else if(random < 0.66) {
            return 1;
        }
        else {
            return 2;
        }
    }
    else {
        int depth = t.depth;
        int acts [depth];
        int count = 0;
        for(int i = c - depth; i < c; i++) {
            acts[count] = v[i];
            count++;
        }
        node* temp = t.root;
        int coun = 0;
        while(temp->pred == -1) {
            for(int i = 0; i < 3; i++) {
                if(temp->children[i]->value == acts[coun]) {
                    temp = temp->children[i];
                    coun++;
                    break;
                }
            }
        }
        return temp->pred;
    }
}
