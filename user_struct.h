#ifndef USER_STRUCT_H
#define USER_STRUCT_H


#include <QString>
#include <QStringList>

class Character
{
public:
    QString name;
    QStringList file_list;
};

class Server_2
{
public:
    QString name;
    Character * characters;
    int characters_num = 0;
};

class Server_1
{
public:
    QString name;
    Server_2 * servers_2;
    int server_num_2 = 0;
};


class User
{
public:
    QString id;
    Server_1 * servers_1;
    int server_num_1 = 0;
};

class User_list
{
public:
    User * users;
    int user_num = 0;
};

class Character_File_Header
{
public:
    int type = 0;
    int file_num = 0;
};

class Character_info_Header
{
public:
    int name_size = 0;
    int file_size = 0;
};

class Character_info
{
public:
    QString name;
    QByteArray file;
};

#endif // USER_STRUCT_H
