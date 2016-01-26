#ifndef LPANEL_H
#define LPANEL_H

#include <QtCore>
#include "panelentry.h"


class Panel : public QAbstractTableModel
{
    Q_OBJECT
    QList<PanelEntry> list;
public:
    enum {
        COL_DATE,
        COL_SIZE,
        COL_NAME,
        COL_COUNT
    };
    Panel(QObject *parent = 0);
    ~Panel();
    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant headerData(int section,Qt::Orientation orientation, int role=Qt::DisplayRole) const;
public slots:
    void receiveListing(QString path, QString list);
};

#endif // LPANEL_H
