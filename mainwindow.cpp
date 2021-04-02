#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->setWindowTitle("剑网3角色设置管理工具");

    QMessageBox::information(NULL,"提示","在使用本工具前，请确保您已经关闭了服务器同步选项");
    QDialog * select_path = new QDialog;
    QLineEdit * path_line = new QLineEdit;
    QPushButton * path_button = new QPushButton;
    QPushButton * confirm_button = new QPushButton;

    select_path->setWindowTitle("请选择剑网3安装路径");
    select_path->setMinimumSize(500,100);
    select_path->setMaximumSize(500,100);
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    select_path->setWindowFlags(flags);

    path_button->setText("...");
    path_button->setGeometry(425,20,50,25);

    path_line->setReadOnly(true);
    path_line->setGeometry(20,20,400,25);

    confirm_button->setText("确认");
    confirm_button->setGeometry(300,55,150,25);


    path_line->setParent(select_path);
    path_button->setParent(select_path);
    confirm_button->setParent(select_path);

    connect(path_button,&QPushButton::clicked,[&](){
        path = QFileDialog::getExistingDirectory(NULL,"请选择剑网3安装路径");
        qDebug() << path;
        path_line->setText(path);
        path_line->setCursorPosition(0);
    });
    connect(confirm_button,&QPushButton::clicked,[=](){
       //qDebug() << path;
       QDir dir(path);
       QStringList dir_list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
       qDebug() << dir_list;
       QStringList file_list = dir.entryList(QDir::Files);
       qDebug() << file_list;
       if(file_list.contains("XLauncher.exe") && dir_list.contains("Game"))
       {
           show_flag =true;
           select_path->close();
           menu = new Menu(path,this);
           menu->show();
       }
       else
       {
           QMessageBox::warning(NULL,"错误","该路径不是剑网3路径");
           path.clear();
           path_line->setText("");
       }
    });

    select_path->exec();
}

