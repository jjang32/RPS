#ifndef BASIC_RPS_H
#define BASIC_RPS_H

#include <QDialog>

namespace Ui {
class basic_RPS;
}

class basic_RPS : public QDialog
{
    Q_OBJECT

public:
    explicit basic_RPS(QWidget *parent = nullptr);
    ~basic_RPS();

private slots:
    void keyPressEvent(QKeyEvent* event);

    void on_cheat_clicked();

private:
    Ui::basic_RPS *ui;
};

#endif // BASIC_RPS_H
