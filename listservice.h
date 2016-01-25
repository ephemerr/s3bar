#ifndef LISTSERVICE_H
#define LISTSERVICE_H
#include <QtCore>

class ListService: public QObject
{
    Q_OBJECT
public:
    ListService();
    ~ListService();
public slots:
    void command(QString str);
signals:
    void receiveListing(QString path, QString list);
};

#endif // LISTSERVICE_H
