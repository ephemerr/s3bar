#include "panel.h"
#include "lister.h"


Panel::Panel(QObject *parent)
: QAbstractTableModel(parent)
{
    list_bucket("cbldev", "", "", "/", 0, "AKIAJBHJ7LQ4MJSYFBFQ","ZLE4fKwQvQ5Q2yx2uI7g5QX8O/ANhW5OI2n2WXIN");

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
        switch(index.column()) {
            case COL_DATE: return getDate(index.row());
            case COL_NAME: return getName(index.row());
            case COL_SIZE: return getSize(index.row());
        }
        return QVariant();
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
    return getCount();
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

void Panel::command(QString str, const char* ak, const char* sk) {
    list_bucket("cbldev", str.toLatin1().data(), "", "/", 0, ak, sk);
    emit dataChanged( index(0,0),/// redraw signal
                      index(getCount()-1,COL_NAME));
}
