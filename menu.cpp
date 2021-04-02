#include "menu.h"
#include "ui_menu.h"


Menu::Menu(QString path,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Menu)
{
    this->path = path + "/Game/JX3/bin/zhcn_hd/userdata";
    ui->setupUi(this);
    init();
}

Menu::~Menu()
{
    delete ui;
}

void Menu::init()
{
    selected = false;
    userList = new User_list;
    getCharacter(*userList);
    ui->user_text->setReadOnly(true);
    ui->server_text->setReadOnly(true);
    ui->id_text->setReadOnly(true);
    ui->treeWidget->header()->hide();

    QList<QTreeWidgetItem*> node_list;
    for(int i = 0;i < userList->user_num;i++)
    {
        QTreeWidgetItem * item_1 = new QTreeWidgetItem(QStringList() << userList->users[i].id);
        for(int j = 0;j < userList->users[i].server_num_1;j++)
        {
            QTreeWidgetItem * item_2 = new QTreeWidgetItem(QStringList() << userList->users[i].servers_1[j].name);
            for(int w = 0;w<userList->users[i].servers_1[j].server_num_2;w++)
            {
                QTreeWidgetItem * item_3 = new QTreeWidgetItem(QStringList() << userList->users[i].servers_1[j].servers_2[w].name);
                for(int m = 0; m < userList->users[i].servers_1[j].servers_2[w].characters_num;m++)
                {
                    QTreeWidgetItem * item_4 = new QTreeWidgetItem(QStringList() << userList->users[i].servers_1[j].servers_2[w].characters[m].name);
                    item_3->addChild(item_4);
                }
                item_2->addChild(item_3);
            }
            item_1->addChild(item_2);
        }
        node_list.append(item_1);
    }

    ui->treeWidget->addTopLevelItems(node_list);

    connect(ui->treeWidget,&QTreeWidget::clicked,[=](){
        dir_path.clear();
        QTreeWidgetItem * sel = ui->treeWidget->currentItem();
        getNodePos(sel);
        for(int i = 0 ; i < dir_path.size();i++)
        {
            switch (i)
            {
            case 0:
                ui->user_text->setText(dir_path[i]->text(0));
                break;
            case 1:
                ui->server_text->setText(dir_path[i]->text(0));
                break;
            case 2:
                ui->server_text->setText(ui->server_text->text() + ' ' + dir_path[i]->text(0));
                break;
            case 3:
                ui->id_text->setText(dir_path[i]->text(0));
                break;
            }
        }

    });

    connect(ui->backup_button,&QPushButton::clicked,[=](){
        if(dir_path.size() == 4)
        {
            backUp();
        }
        else
        {
            QMessageBox::warning(NULL,"警告","请选择一个角色后再进行此操作");
        }
    });

    connect(ui->recover_button,&QPushButton::clicked,[=](){
        if(dir_path.size() == 4)
        {
            recover();
        }
        else
        {
            QMessageBox::warning(NULL,"警告","请选择一个角色后再进行此操作");
        }
    });
    connect(ui->copy_button,&QPushButton::clicked,[=](){
        if(dir_path.size() == 4)
        {
            copy();
            selected = true;
        }
        else
        {
            QMessageBox::warning(NULL,"警告","请选择一个角色后再进行此操作");
        }
    });

}

void Menu::getCharacter(User_list &user_list)
{
    QDir data_dir(path);
    QStringList users = data_dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    if(users.empty())
    {
        QMessageBox::warning(NULL,"空目录","无用户或者路径错误，请检查路径后重试");
        this->parentWidget()->close();
    }
    else
    {
        user_list.user_num = users.size();
        user_list.users = new User[users.size()];
        for(auto user = users.begin();user != users.end();user++)
        {
            User * userData = &user_list.users[user-users.begin()];
            userData->id = * user;
            //qDebug() << *user;
            QDir user_dir(path + '/' + *user);
            QStringList server_list_1 = user_dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
            if(!server_list_1.empty())
            {
                userData->server_num_1 = server_list_1.size();
                userData->servers_1 = new Server_1[server_list_1.size()];
                for(auto server_1 = server_list_1.begin();server_1!=server_list_1.end();server_1++)
                {
                    QDir server_dir_1(path + '/' + *user + '/' + *server_1);
                    QStringList server_list_2 = server_dir_1.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
                    Server_1 * server_data_1 = &userData->servers_1[server_1 -server_list_1.begin()];
                    server_data_1->name = *server_1;
                    if(!server_list_2.empty())
                    {
                        server_data_1->server_num_2 = server_list_2.size();
                        server_data_1->servers_2 = new Server_2[server_list_2.size()];
                        for(auto server_2 = server_list_2.begin();server_2 != server_list_2.end();server_2 ++)
                        {
                            QDir server_dir_2(path + '/' + *user + '/' + *server_1 + '/' + *server_2);
                            QStringList character_list = server_dir_2.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
                            Server_2 * server_data_2  = &server_data_1->servers_2[server_2-server_list_2.begin()];
                            server_data_2->name = *server_2;

                            if(!character_list.empty())
                            {
                                server_data_2->characters_num = character_list.size();
                                server_data_2->characters = new Character[character_list.size()];
                                for(auto character = character_list.begin();character!=character_list.end();character++)
                                {
                                    server_data_2->characters[character-character_list.begin()].name = *character;
                                    qDebug() << *character;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


void Menu::getNodePos(QTreeWidgetItem *sel)
{
    dir_path.push_front(sel);
    while (sel->parent())
    {
        dir_path.push_front(sel->parent());
        sel = sel->parent();
    }
}


void Menu::backUp()
{
    QString save_name = dir_path[2]->text(0) + '_' + dir_path[3]->text(0);
    QString sava_path = QFileDialog::getSaveFileName(this,"请选择备份文件保存位置",save_name,"备份文件(*.bc)");
    if(sava_path.size() == 0)
    {
        return;
    }
    QString charc_path = path + "/" + dir_path[0]->text(0) + "/" + dir_path[1]->text(0) + "/" +dir_path[2]->text(0) + "/" + dir_path[3]->text(0);
    QDir dir(charc_path);
    QStringList file_ls = dir.entryList(QDir::Files);

    QFile back_file(sava_path);
    if(!back_file.open(QFile::WriteOnly))
    {
        QMessageBox::warning(NULL,"错误！","文件保存失败");
        return ;
    }
    QDataStream out(&back_file);


    Character_File_Header header;
    header.type = 0x4243;
    header.file_num = file_ls.size();
    qDebug() << header.type;

    out.writeRawData((char*)(&header),sizeof(header));

    for(auto file = file_ls.begin();file != file_ls.end();file++)
    {
        QFile file_temp(charc_path+ '/' +(*file));
        file_temp.open(QFile::ReadOnly);
        QDataStream fileStream(&file_temp);


        Character_info_Header info_header;
        Character_info info;

        char * temp = new char[file_temp.size()];
        memset(temp,0,file_temp.size());
        fileStream.readRawData(temp,file_temp.size());
        info.file.resize(file_temp.size());
        memcpy(info.file.data(),temp,file_temp.size());

        info.name = (*file);

        info_header.file_size = file_temp.size();
        info_header.name_size = sizeof(info.name);

        QByteArray name = info.name.toUtf8();

        out.writeBytes(name,name.length());
        qDebug() << QString(name) << name.size() << file_temp.size();

        out.writeBytes(info.file,info.file.length());

        file_temp.close();

    }
    back_file.close();
    QMessageBox::information(NULL,"完成","备份完成，已保存到" + sava_path);

}

void Menu::recover()
{
    QString file_path = QFileDialog::getOpenFileName(NULL,"请选择备份文件位置","","备份文件(*.bc)");
    if(file_path.size() == 0)
    {
        return;
    }
    QFile back_file(file_path);
    back_file.open(QFile::ReadOnly);
    QDataStream in(&back_file);

    Character_File_Header header;

    back_file.seek(0);

    in.readRawData((char*)(&header),sizeof(header));
    if(header.type!=0x4243)
    {
        QMessageBox::warning(NULL,"错误","文件不正确或有损坏");
        return ;
    }
    Character_info info;
    for(int i = 0 ; i<header.file_num;i++)
    {
        uint len;
        char * buff;
        in.readBytes(buff,len);
        info.name = buff;
        in.readBytes(buff,len);
        info.file.prepend(buff);
        QFile out("./test/" + info.name);
        out.open(QFile::WriteOnly);

        out.write(info.file,len);
        out.close();
    }
}

void Menu::copy()
{
    if(!selected)
    {
        ui->info->setText("请选择目标角色");
        source_path = dir_path;
        ui->copy_button->setText("从角色设置(选择目标)");
    }
    else
    {
        QString source_pth = path + '/' + source_path[0]->text(0) + '/' + source_path[1]->text(0) + '/' + source_path[2]->text(0) + '/'+source_path[3]->text(0) + '/';
        QString dir_pth = path + '/' + dir_path[0]->text(0) + '/' + dir_path[1]->text(0) + '/' + dir_path[2]->text(0) + '/'+ dir_path[3]->text(0) + '/';
        QDir dir(dir_pth);
        QDir source(source_pth);
        QStringList dir_files = dir.entryList(QDir::Files);
        for(auto file = dir_files.begin();file != dir_files.end();file++)
        {
            QFile::remove(dir_pth + (*file));
        }
        QStringList file_list = source.entryList(QDir::Files);
        for(auto file = file_list.begin();file != file_list.end();file++)
        {
            QFile::copy(source_pth + (*file),dir_pth + (*file));
        }
        QMessageBox::information(NULL,"成功","复制完成");
        init();
    }

}
