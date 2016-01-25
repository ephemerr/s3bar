#include "mainwindow.h"
#include <QApplication>
#include "listservice.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ListService *serv = new ListService;
    QThread t(&a);
    serv->moveToThread(&t);
    a.connect(&t,       SIGNAL(started()),  serv,   SLOT(process()));


    MainWindow w(serv);
    w.showMaximized();



    return a.exec();
}
