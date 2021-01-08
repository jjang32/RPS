#include "game.h"
#include "ui_game.h"
#include "lose.h"
#include "win.h"
#include <QKeyEvent>
#include <vector>
#include <string>
#include <math.h>
#include <iostream>
using namespace std;

Game::Game(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
}

Game::~Game()
{
    delete ui;
}

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

static double max(double, double);
static void prediction();
static double w(double, double);
static double oLine(double, double);
static double res(int, int);
static void reset();
static QString print(int);

//tree construction where the experts at each node will be initialized
static tree t;

//current action of the computer and player
static int computer = -1;
static int human = -1;

//current score of the computer and player
static int cPoints = 0;
static int hPoints = 0;

//history of all choices from human
static vector<int> acts;

//to count which action you are currently on
static int counter = 0;

void Game::on_pushButton_clicked()
{
    prediction();
    ui->label_7->setText("Computer: " + print(computer));
}

void Game::keyPressEvent(QKeyEvent* event) {
    prediction();

    if(event->key() == Qt::Key_1) {
        ui->label_4->setText("Input: Rock");
        event->accept();
        human = 0;
    }
    else if(event->key() == Qt::Key_2) {
        ui->label_4->setText("Input: Paper");
        event->accept();
        human = 1;
    }
    else if(event->key() == Qt::Key_3) {
        ui->label_4->setText("Input: Scissors");
        event->accept();
        human = 2;
    }
    else {
        ui->label_4->setText("Input: Invalid");
        event->accept();
        return;
    }

    ui->label_7->setText("");

    //doing certain acts as long as it is not the first action
    if(counter != 0) {
        int temp = 0;
        node* holder = t.root;

        //finding the expectation of experts' loss and probabilities
        double expect = 0.0;
        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                holder->loss[i] = res(human, i);
                expect += holder->prob[i]*holder->loss[i];
            }
            temp++;
        }

        temp = 0;
        holder = t.root;

        //updating R and C
        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                holder->r[i] += expect - holder->loss[i];
                holder->c[i] += abs(holder->r[i]);
            }
            temp++;
        }
    }

    //counts the number of times this loop is run
    counter++;

    //records the previous acts done by the human
    acts.push_back(human);

    //the result of the human vs. computer match
    double action = res(human, computer);

    QString s;

    //determines who wins
    if(action == 0) {
        cPoints++;
        ui->label_2->setText(QString::number(cPoints));
    }
    else if(action == 1) {
        hPoints++;
        ui->label->setText(QString::number(hPoints));
    }

    //WIN SCREEN -> add buttons for restart or quit
    if(hPoints == 10) {
        this->close();
        reset();
        Win w;
        w.setModal(true);
        w.exec();
    }
    //LOSE SCREEN -> add buttons for restart or quit
    else if(cPoints == 10) {
        this->close();
        reset();
        Lose l;
        l.setModal(true);
        l.exec();
    }
}

static double max(double one, double two) {
    if(one > two) {
        return one;
    }
    return two;
}

static double oLine(double r, double c) {
    return exp(pow(max(r, 0), 2) / (3.0 * c));
}

static double w(double r, double c) {
    return (1.0/2.0) * (oLine(r + 1, c + 1) - oLine(r - 1, c + 1));
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

static void reset() {
    t.root = new node();
    computer = -1;
    human = -1;
    cPoints = 0;
    hPoints = 0;
    acts.clear();
    counter = 0;
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

static void prediction() {
    if(acts.size() == 0) {
        computer = (int) (rand() / RAND_MAX * 3);
        t.root = new node();
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

        double pSum = 0.0;
        //updates the probability of each expert
        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                pSum += w(holder->r[i], holder->c[i]);
            }
            temp++;
        }

        temp = 0;
        holder = t.root;

        //updates the probability of each expert
        while(temp < counter) {
            holder = holder->children[acts[acts.size() - temp - 1]];
            for(int i = 0; i < 3; i++) {
                holder->prob[i] = w(holder->r[i], holder->c[i]) / pSum;
                //cout << "Node " << temp << ", Expert " << i << " Prob: " << holder->prob[i] << endl;
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

        if(max(max(pRock, pPaper), pScissors) == pRock) {
            computer = 0;
        }
        else if(max(max(pRock, pPaper), pScissors) == pPaper) {
            computer = 1;
        }
        else {
            computer = 2;
        }
    }
}
