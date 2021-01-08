#ifndef GAME_H
#define GAME_H

#include <QDialog>

namespace Ui {
class Game;
}

class Game : public QDialog
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

private slots:
    void keyPressEvent(QKeyEvent* event);

    void on_pushButton_clicked();

private:
    Ui::Game *ui;
};

#endif // GAME_H
