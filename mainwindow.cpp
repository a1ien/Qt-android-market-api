#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <google/protobuf/descriptor.h>
#include <QBitmap>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setEnabled(false);
    settings = new Settings(this);
    session = new MarketSession(this);

    connect(ui->actionSettings,SIGNAL(triggered()),settings,SLOT(exec()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::login()
{
    setWindowTitle(tr("Login..."));

    while(settings->someIsEmpty())
        settings->exec();

    session->login(settings->email(),settings->password(),settings->androidid(),QString("HOSTED_OR_GOOGLE"));
    connect(session,SIGNAL(logged()),this,SLOT(login_done()));
}

void MainWindow::on_pushButton_clicked()
{


}



void MainWindow::on_pushButton_2_clicked()
{


    //ui->plainTextEdit->appendPlainText(r.mutable_responsegroup(0)->appsresponse().mutable_app(0)->id().c_str());
}

void MainWindow::login_done()
{
    setWindowTitle(tr("Google play API"));
    setEnabled(true);
}
