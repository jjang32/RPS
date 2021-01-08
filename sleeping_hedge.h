#ifndef SLEEPING_HEDGE_H
#define SLEEPING_HEDGE_H

#include <QDialog>

namespace Ui {
class sleeping_hedge;
}

class sleeping_hedge : public QDialog
{
    Q_OBJECT

public:
    explicit sleeping_hedge(QWidget *parent = nullptr);
    ~sleeping_hedge();

private slots:
    void keyPressEvent(QKeyEvent* event);

    void on_cheat_clicked();

private:
    Ui::sleeping_hedge *ui;
};

#endif // SLEEPING_HEDGE_H
