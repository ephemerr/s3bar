#include "panel.h"

#include <QDebug>

LPanel::LPanel(QObject *parent)
: QAbstractTableModel(parent)
{

}

LPanel::~LPanel()
{

}

QVariant
LPanel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() ||
            index.row() >= rowCount() ||
            index.row() >= COL_COUNT)
        return QVariant();

    if (role == Qt::DisplayRole) {
        return list[index.row()][index.column()];
    }

    return QVariant();
}

int
LPanel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return list.length();
}

int
LPanel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return COL_COUNT;
}


QVariant
LPanel::headerData(int section, Qt::Orientation orientation, int role) const
{
   const char* header_data[] =  { "Изменен", "Размер", "Файл"};

   if(role != Qt::DisplayRole)
           return QVariant();

   if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
       Q_ASSERT(section < COL_COUNT);
       return QString::fromUtf8(header_data[section]); // заголовки столбцов
   } else {
       return QString("%1").arg( section + 1 ); // возвращаем номера строк
   }
}

void
LPanel::receiveListing(QString path, QString list) {
    qDebug() << path;
    qDebug() << list;
}
