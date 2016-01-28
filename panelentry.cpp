#include "panelentry.h"



PanelEntry::~PanelEntry()
{
}

PanelEntry::PanelEntry(QString size,QString name,QString date,bool isdir) :
    size(size),
    name(name),
    date(date),
    isdir(isdir)
{

}

//PanelEntry::PanelEntryP(QString line)
//{
//    const char* data = line.toLatin1().data();
//    isdir = data[23] == 'D' ? 1 : 0;
//    int namestarts = strstr(data, "s3") - data;
//    name = line.mid(namestarts,line.length()-namestarts);
//    if ( ! isdir ) {
//        date = line.mid(0,16);
//        size = line.mid(17, namestarts-17);
//    }
//    else {
//        date=size="";
//    }
//}

PanelEntry::PanelEntry(QString line)
{
    const char* data = line.toLatin1().data();
    isdir = data[13] == ':' ? 1 : 0;

    if ( ! isdir ) {
        name = line.mid(0, line.length()-29);
        date = line.mid(line.length()-27,20);
        size = line.mid(line.length()-5, 5);
    }
    else {
        date=size="";
        name = line.mid(15, line.length());
    }
}

void PanelEntry::print() {
    QTextStream out(stdout);
    if ( ! isdir ) {
        out << date << " " << size;
    }
    else {
        out << "                       DIR   ";
    }
    out << " " << name << endl;
}

QVariant PanelEntry::operator [](int i) const{
    switch(i) {
    case 0:
        return isdir ? "" : date;
    case 1:
        return isdir ? "DIR   " : size;
    case 2:
        return name;
    default :
        return QVariant();
    }
}

QList<PanelEntry> PanelEntry::parse(QString str) {
    QTextStream chunk(&str);
    QList<PanelEntry> res;
    int i=0;
    for(i=0; i<PARSER_MAX_LINES; i++) {
        QString line = chunk.readLine(PARSER_MAX_LINE_LEN);
        if (line == "") {
            continue;
        }
        if (line == NULL) {
            break;
        }
        PanelEntry f(line);
        res.append(f);
    }
    return res;
}
