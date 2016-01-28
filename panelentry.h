#ifndef PANELENTRY_H
#define PANELENTRY_H

#include <QtCore>

#define PARSER_MAX_LINES    1024
#define PARSER_MAX_LINE_LEN 1024

class PanelEntry {
private:
    QString size;
    QString name;
    QString date;
    bool isdir;
public:
    PanelEntry(QString);
    PanelEntry(QString,QString,QString,bool);
    ~PanelEntry();
    QVariant value(int colum);
    void print();
    QVariant operator[](int)  const;
    static QList<PanelEntry> parse(QString str);
};

#endif // PANELENTRY_H
