#ifndef SQL_FUNC_H
#define SQL_FUNC_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

class Sql_func
{
public:
    Sql_func(QString db_name,QString UserName,QString Password);
    bool open_db(void);
    bool Sql_insert(QString data,QString num);
    bool find_data(QString data,QString num);
    QSqlDatabase db;//创建数据库
    QString name[100];
    QString num[100];
    int i;
    bool led_state=false;
};

#endif // SQL_FUNC_H
