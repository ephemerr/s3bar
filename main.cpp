#include "mainwindow.h"
#include <QApplication>
#include "listservice.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);

    ListService *serv = new ListService;
    QThread t(&a);
    serv->moveToThread(&t);
    a.connect(&t,       SIGNAL(started()),
              serv,     SLOT(run())
              );

    t.start();
    MainWindow w(serv);
    w.showMaximized();

    return a.exec();
}
