#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    settings=new QSettings("config.ini",QSettings::IniFormat,this);
    ui->email->setText(settings->value("email").toString());
    ui->password->setText(settings->value("password").toString());
    ui->androidid->setText(settings->value("androidid").toString());
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_Save_clicked()
{
    settings->setValue("email",ui->email->text());
    settings->setValue("password",ui->password->text());
    settings->setValue("androidid",ui->androidid->text());
    hide();
}

void Settings::on_Cancel_clicked()
{
    ui->email->setText(settings->value("email").toString());
    ui->password->setText(settings->value("password").toString());
    ui->androidid->setText(settings->value("androidid").toString());
    hide();
}
