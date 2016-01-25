#ifndef LPANEL_H
#define LPANEL_H

#include <QAbstractTableModel>

typedef QString Elem[3];

class LPanel : public QAbstractTableModel
{
    static const int COL_COUNT = 3;
    QList<QList<QVariant> > list;
public:
    LPanel(QObject *parent = 0);
    ~LPanel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;

    QString getSize(int row) const;
    QString getName(int row) const;
    QString getTime(int row) const;
public slots:
    void receiveListing(QString path, QString list);
};

#endif // LPANEL_H
