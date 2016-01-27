#include "mainwindow.h"
#include <QApplication>
#include "listservice.h"

int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    qputenv("S3_SECRET_ACCESS_KEY", "ZLE4fKwQvQ5Q2yx2uI7g5QX8O/ANhW5OI2n2WXIN");
    qputenv("S3_ACCESS_KEY_ID","AKIAJBHJ7LQ4MJSYFBFQ");

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
