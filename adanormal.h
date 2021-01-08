#ifndef ADANORMAL_H
#define ADANORMAL_H

#include <QDialog>

namespace Ui {
class AdaNormal;
}

class AdaNormal : public QDialog
{
    Q_OBJECT

public:
    explicit AdaNormal(QWidget *parent = nullptr);
    ~AdaNormal();

private slots:
    void keyPressEvent(QKeyEvent* event);

    void on_cheat_clicked();

private:
    Ui::AdaNormal *ui;
};

#endif // ADANORMAL_H
