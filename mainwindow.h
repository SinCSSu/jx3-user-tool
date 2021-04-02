#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

#include "menu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool show_flag = false;
private:
    Ui::MainWindow *ui;
    void init();
    QString path;
    Menu * menu;

signals:
    void quit();
};

#endif // MAINWINDOW_H
