#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AdaNormal_clicked();

    void on_Basic_clicked();

    void on_Hedge_clicked();

    void on_Tree_clicked();

    void on_SleepingHedge_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
