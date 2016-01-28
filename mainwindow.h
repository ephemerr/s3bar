#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSettings *ini;
    void request(QString str);

private slots:
    void on_pushButton_2_clicked();
    void on_lineEdit_returnPressed();
    void on_tableView_doubleClicked(const QModelIndex &index);

public slots:
    void on_newSettings(QString ak, QString sk);

signals:
    void command(QString str,const char* ,const char* );
};

#endif // MAINWINDOW_H
