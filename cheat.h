#ifndef CHEAT_H
#define CHEAT_H

#include <QDialog>

namespace Ui {
class cheat;
}

class cheat : public QDialog
{
    Q_OBJECT

public:
    explicit cheat(QWidget *parent = nullptr);
    ~cheat();

private slots:
    void on_retry_clicked();

private:
    Ui::cheat *ui;
};

#endif // CHEAT_H
