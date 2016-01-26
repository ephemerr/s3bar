#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "panel.h"
#include "dialogkeys.h"

MainWindow::MainWindow(ListService* serv, QWidget *parent) :
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

    ini = new QSettings("/home/me/projects/s3bar/s3cfg.ini", QSettings::IniFormat);

    connect( this,              SIGNAL(command(QString)),
             serv,              SLOT(command(QString))
           );
    connect( serv,              SIGNAL(receiveListing(QString,QString)),
             lpanel,            SLOT(receiveListing(QString,QString))
             );
    connect( serv,              SIGNAL(receiveListing(QString,QString)),
             this,              SLOT(receiveListing(QString,QString))
             );

    ui->lineEdit->setText("s3://cbldev");
    emit command(ui->lineEdit->text());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::receiveListing(QString dir,QString listing) {
    ui->lineEdit->setText(dir);
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
    //ini->setValue("default/access_key","AKIAJBHJ7LQ4MJSYFBFQ");
    //ini->setValue("default/secret_key","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");
}

void MainWindow::on_lineEdit_returnPressed()
{
    emit command(ui->lineEdit->text());
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    const QModelIndex nameind = index.sibling(row,Panel::COL_NAME);
    QString name = ui->tableView->model()->data(nameind).toString();
    emit command(name);
}
