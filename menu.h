#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QTreeWidget>
#include <QDataStream>
#include <QFileDialog>

#include "user_struct.h"

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QString path,QWidget *parent = nullptr);
    ~Menu();

private:
    Ui::Menu *ui;
    void init();
    QString path;
    QList<QTreeWidgetItem *> dir_path;
    QList<QTreeWidgetItem *> source_path;

    User_list * userList;
    void getCharacter(User_list & user_list);
    void getNodePos(QTreeWidgetItem *sel);
    void backUp();
    void recover();
    void copy();
    bool selected =false;
};

#endif // MENU_H
