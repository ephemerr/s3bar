#include "listservice.h"

ListService::ListService()
{

}

ListService::~ListService()
{

}

void ListService::command(QString str) {

    QString cmnd = QString("s3cmd -c ../s3bar/s3cfg.ini ls %1").arg(str);

    QProcess proc;
    proc.start(cmnd);
    if (!proc.waitForStarted())
       return;

    if (!proc.waitForFinished())
       return;

    QByteArray result = proc.readAll();

    QTextStream *from = new QTextStream(&result);
    emit receiveListing(str,from->readAll());
}

void ListService::run() {
}
