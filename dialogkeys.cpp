#include "dialogkeys.h"
#include "ui_dialogkeys.h"

DialogKeys::DialogKeys(QString ak, QString sk, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogKeys)
{
    ui->setupUi(this);
    ui->lineEdit_2->setText(ak);
    ui->lineEdit_3->setText(sk);
}

DialogKeys::~DialogKeys()
{
    delete ui;
}

void DialogKeys::on_buttonBox_accepted()
{
    emit newSettings(ui->lineEdit_2->text(), ui->lineEdit_3->text()) ;
}
