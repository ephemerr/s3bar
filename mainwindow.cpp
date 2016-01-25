#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "panel.h"

MainWindow::MainWindow(ListService* serv, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LPanel *lpanel = new LPanel(this);
    ui->tableView->setModel(lpanel);

    connect( ui->pushButton, SIGNAL(clicked()),
             serv,           SLOT(command(QString))
            );

    connect( serv,          SIGNAL(receiveListing(QString,QString)),
             lpanel,        SLOT(receiveListing(QString,QString))
             );

}

MainWindow::~MainWindow()
{
    delete ui;
}
