#include "mainwindow.h"
#include <QApplication>
#include "listservice.h"

int main(int argc, char *argv[])
{
    QFile data("/home/me/projects/s3brr/1.ls");

    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    //QTextCodec::setCodecForTr(codec);
    //QTextCodec::setCodecForCStrings(codec);
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
