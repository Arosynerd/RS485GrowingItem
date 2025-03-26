#include "sql_func.h"
#include <QSqlError>
Sql_func::Sql_func(QString db_name,QString UserName,QString Password)
{
    qDebug() << QSqlDatabase::drivers();//打印qt支持的数据库类型


    db = QSqlDatabase::addDatabase("QSQLITE");//设置驱动，设置数据库类型,
//  db.setHostName("127.0.1.0");//设置主机名
    db.setDatabaseName(db_name);//设置数据库名
    db.setUserName(UserName);//设置用户名
    db.setPassword("123");//设置密码

}

bool Sql_func::open_db(void)
{

    if(db.open())
    {
        QSqlQuery query;
        //创建表

        bool success = query.exec("CREATE TABLE information ("
                                  "name VARCHAR(40) PRIMARY KEY, "
                                  "id VARCHAR(40) NOT NULL)");

        if(success)
        {
            qDebug()<<"creat success";
        }
        else
        {
            qDebug()<<"creat fail";
        }


      return 1;
    }
    else
    {
       return 0;
    }

}

 bool Sql_func::Sql_insert(QString data,QString num)
 {
     QString Str = QString("INSERT INTO information (name,id)"
                               "VALUES ('%1','%2')").arg(data).arg(num);

     QSqlQuery query;
     if(query.exec(Str))
     {
      qDebug()<<"插入成功";
       return 1;
     }
     else
     {
       return 0;
     }

 }

 bool Sql_func::find_data(QString data, QString Num)
 {
     QSqlQuery query;
     QString Str = QString("SELECT * FROM information WHERE name = '%1' AND id = '%2'").arg(data).arg(Num);
     if (!query.exec(Str))
     {
         qDebug() << "查找失败：" << query.lastError().text();
         return false;
     }

     QVector<QString> names;
     QVector<QString> nums;

     while (query.next()) {
         names.append(query.value("name").toString());
         nums.append(query.value("id").toString());
     }

     for (int i = 0; i < names.size(); ++i) {
         name[i] = names.at(i);
         num[i] = nums.at(i);
     }

     return true;
 }


