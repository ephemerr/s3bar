#ifndef DIALOGKEYS_H
#define DIALOGKEYS_H

#include <QDialog>

namespace Ui {
class DialogKeys;
}

class DialogKeys : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogKeys(QString ak, QString sk, QWidget *parent = 0);
    ~DialogKeys();
    
private slots:
    void on_buttonBox_accepted();

signals:
    void newSettings(QString, QString);

private:
    Ui::DialogKeys *ui;
};

#endif // DIALOGKEYS_H
