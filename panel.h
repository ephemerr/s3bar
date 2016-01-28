#ifndef LPANEL_H
#define LPANEL_H

#include <QtCore>
#include "panelentry.h"
#include "libs3.h"


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
private:
public slots:
    void command(QString str, const char* ,const char* );
};

#endif // LPANEL_H
