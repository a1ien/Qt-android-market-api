#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    QString email() { return settings->value("email").toString();}
    QString password() { return settings->value("password").toString();}
    QString androidid() { return settings->value("androidid").toString();}

    bool someIsEmpty() { return settings->value("email").toString().isEmpty() ||
                                settings->value("password").toString().isEmpty() ||
                                settings->value("androidid").toString().isEmpty();}

private slots:
    void on_Save_clicked();

    void on_Cancel_clicked();

private:
    Ui::Settings *ui;
    QSettings* settings;
};

#endif // SETTINGS_H
