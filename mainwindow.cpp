#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "panel.h"

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

    connect( ui->tableView,     SIGNAL(doubleClicked(QModelIndex)),
             this,              SLOT(commandChDir(QModelIndex))
             );
    connect( ui->pushButton,    SIGNAL(clicked()),
             this,              SLOT(commandRun())
             );
    connect( this,              SIGNAL(command(QString)),
             serv,              SLOT(command(QString))
           );
    connect( serv,              SIGNAL(receiveListing(QString,QString)),
             lpanel,            SLOT(receiveListing(QString,QString))
             );
    connect( serv,              SIGNAL(receiveListing(QString,QString)),
             this,              SLOT(receiveListing(QString,QString))
             );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::commandRun() {
    emit command(ui->lineEdit->text());
}

void
MainWindow::commandChDir(QModelIndex ind) {
    int row = ind.row();
    const QModelIndex nameind = ind.sibling(row,Panel::COL_NAME);
    QString name = ui->tableView->model()->data(nameind).toString();
    emit command(name);
}

void
MainWindow::receiveListing(QString dir,QString listing) {
    ui->lineEdit->setText(dir);
}

void
readKeys() {
    QFile data("s3.conf");
    if (!data.open(QFile::ReadOnly)) {
        qDebug() << "Could not open file.";
        return;
    }
    QString line;
    QRegExp exp = QRegExp("AccessKey: [A-Za-z0-9]+");
    line = data.readLine();
}
