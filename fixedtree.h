#ifndef FIXEDTREE_H
#define FIXEDTREE_H

#include <QDialog>

namespace Ui {
class FixedTree;
}

class FixedTree : public QDialog
{
    Q_OBJECT

public:
    explicit FixedTree(QWidget *parent = nullptr);
    ~FixedTree();

private slots:
    void keyPressEvent(QKeyEvent* event);

    void on_cheat_clicked();

private:
    Ui::FixedTree *ui;
};

#endif // FIXEDTREE_H
