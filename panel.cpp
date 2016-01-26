#include "panel.h"

#include <QDebug>

Panel::Panel(QObject *parent)
: QAbstractTableModel(parent)
{
    QFile data("../s3bar/1.ls");
    if (!data.open(QFile::ReadOnly)) {
        qDebug() << "Could not open file.";
        return;
    }
    receiveListing("",data.readAll());
}

Panel::~Panel()
{

}

QVariant
Panel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
            index.row() >= rowCount() ||
            index.column() >= COL_COUNT)
        return QVariant();

    if (role == Qt::DisplayRole) {
        return list[index.row()][index.column()];
    }

    if (role == Qt::TextAlignmentRole && index.column() == 1) {
        return Qt::AlignRight;
    }

    return QVariant();
}

int
Panel::rowCount(const QModelIndex &parent) const
{
    (void)parent;
    return list.length();
}

int
Panel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return COL_COUNT;
}

QVariant
Panel::headerData(int section, Qt::Orientation orientation, int role) const
{
   const char* header_data[] =  { "Изменен", "Размер", "Файл"};

   if(role != Qt::DisplayRole)
           return QVariant();

   if(orientation == Qt::Horizontal && role == Qt::DisplayRole) {
       Q_ASSERT(section < COL_COUNT);
       return QString::fromUtf8(header_data[section]); // заголовки столбцов
   }
   return QVariant();
}

void
Panel::receiveListing(QString path, QString listchunk) {
    list.clear();
    list = PanelEntry::parse(listchunk);
    QModelIndex ind = index(0,0);

    emit dataChanged( ind.sibling(0,0),/// redraw signal
                      ind.sibling(list.length()-1,COL_NAME));
}
