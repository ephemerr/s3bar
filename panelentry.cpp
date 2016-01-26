#include "panelentry.h"



PanelEntry::~PanelEntry()
{
}

PanelEntry::PanelEntry(QString line)
{
    const char* data = line.toLatin1().data();
    isdir = data[23] == 'D' ? 1 : 0;
    int namestarts = strstr(data, "s3") - data;
    name = line.mid(namestarts,line.length()-namestarts);
    if ( ! isdir ) {
        date = line.mid(0,16);
        size = line.mid(17, namestarts-17);
    }
    else {
        date=size="";
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
        if (line == NULL) {
            break;
        }
        PanelEntry f(line);
        res.append(f);
    }
    return res;
}
