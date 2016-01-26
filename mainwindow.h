#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "listservice.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(ListService* serv, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void commandRun();
    void commandChDir(QModelIndex);
    void receiveListing(QString,QString);

signals:
    void command(QString str);
};

#endif // MAINWINDOW_H
