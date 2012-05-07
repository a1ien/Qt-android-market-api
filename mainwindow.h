#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplashScreen>
#include "marketsession.h"
#include "settings.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void login_done();

private:
    Ui::MainWindow *ui;
    Settings* settings;
    MarketSession* session;
    QSplashScreen* splash;
};

#endif // MAINWINDOW_H
