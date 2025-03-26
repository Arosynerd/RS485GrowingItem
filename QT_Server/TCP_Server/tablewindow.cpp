#include "tablewindow.h"
#include "ui_tablewindow.h"

TableWindow::TableWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("page 2");

    db = QSqlDatabase::addDatabase("QMYSQL");

    qDebug() <<  QSqlDatabase::drivers();
    if (!db.isValid()) {
        QMessageBox::information(this, "驱动提示", "QMYSQL驱动不可用");
        qDebug() << "QMYSQL驱动不可用";
        return;
    }

    db.setHostName("127.0.0.1");
    db.setDatabaseName("iot_db");
    db.setUserName("root");
    db.setPassword("1234");

    if (db.open())
    {
        model = new QSqlTableModel;
        model_another = new QSqlTableModel;
        model->setTable("environment_data");
        model_another->setTable("environment_data_another");
        qDebug() << "连接成功";
    }
    else
    {
        QMessageBox::information(this, "连接提示", "连接失败");
        qDebug() << "连接失败";
        //退出程序
        exit(0);
    }
}

TableWindow::~TableWindow()
{
    delete ui;
}

void TableWindow::on_pushButton_released()
{
    this->close();
}

void TableWindow::on_FindButton_released()
{
    ui->tableView->setModel(model);
    model->select();
}



void TableWindow::on_Find1Button_released()
{
    ui->tableView->setModel(model_another);
    // Bug 修复：查询 model_another 对应的数据表
    model_another->select();
}
