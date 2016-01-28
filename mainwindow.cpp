#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "panel.h"
#include "dialogkeys.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Panel *lpanel = new Panel(this);
    ui->tableView->setModel(lpanel);

#if QT_VERSION >= 0x050000
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
#endif

    ini = new QSettings("../s3bar/s3cfg.ini", QSettings::IniFormat);

    connect( this,              SIGNAL(command(QString,const char* ,const char* )),
             lpanel,            SLOT(command(QString,const char* ,const char* ))
           );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::on_pushButton_2_clicked()
{
    QString ak = ini->value("default/access_key").toString();
    QString sk = ini->value("default/secret_key").toString();
    DialogKeys d(ak, sk);
    connect(&d,         SIGNAL( newSettings(QString, QString) ),
            this,       SLOT ( on_newSettings(QString, QString))
                               );
    d.exec();
}

void
MainWindow::on_newSettings(QString ak, QString sk) {
    ini->setValue("default/access_key",ak);
    ini->setValue("default/secret_key",sk);
}

void MainWindow::on_lineEdit_returnPressed()
{
    request(ui->lineEdit->text());
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    const QModelIndex nameind = index.sibling(row,Panel::COL_NAME);
    QString name = ui->tableView->model()->data(nameind).toString();
    ui->lineEdit->setText(name);
    request(name);
}

void MainWindow::request(QString str) {
    QString ak = ini->value("default/access_key").toString();
    QString sk = ini->value("default/secret_key").toString();
    emit command(str,ak.toLatin1().data(),sk.toLatin1().data());
}
