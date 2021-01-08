#ifndef FIXED_HEDGE_H
#define FIXED_HEDGE_H

#include <QDialog>

namespace Ui {
class Fixed_Hedge;
}

class Fixed_Hedge : public QDialog
{
    Q_OBJECT

public:
    explicit Fixed_Hedge(QWidget *parent = nullptr);
    ~Fixed_Hedge();

private slots:
    void keyPressEvent(QKeyEvent* event);

    void on_cheat_clicked();

private:
    Ui::Fixed_Hedge *ui;
};

#endif // FIXED_HEDGE_H
