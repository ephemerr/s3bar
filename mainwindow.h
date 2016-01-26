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
    QSettings *ini;

private slots:    
    void commandChDir(QModelIndex);
    void receiveListing(QString,QString);

    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
public slots:
    void on_newSettings(QString ak, QString sk);

signals:
    void command(QString str);
};

#endif // MAINWINDOW_H
