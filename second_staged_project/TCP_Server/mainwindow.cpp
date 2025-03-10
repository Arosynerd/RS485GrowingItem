#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTcpSocket>
#include <QDateTime>
#include <QSqlError>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(800, 800);
    esp_server = new QTcpServer(this);

    if (!esp_server->listen(QHostAddress::AnyIPv4, 8080))
    {
        qDebug() << esp_server->errorString();
        qDebug() << "8080监听失败";
    }
    qDebug() << "8080监听成功";

    // 时间显示
    DateTime = QDateTime::currentDateTime();
    connect(&timer, &QTimer::timeout, [this]()
            {
                DateTime = DateTime.addSecs(1);
                ui->time_lab->setText(DateTime.toString("yyyy-MM-dd hh:mm:ss"));
            });
    timer.start(1000); // 每秒更新一次时间
    // 连接数据库
    db = QSqlDatabase::addDatabase("QMYSQL");

    qDebug() << QSqlDatabase::drivers();
    if (!db.isValid())
    {
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
        // QMessageBox::information(this, "连接提示", "连接成功");
        model = new QSqlTableModel;
        model->setTable("environment_data");
        //  ui->tableView->setModel(model);
        qDebug() << "连接成功";
    }
    else
    {
        QMessageBox::information(this, "连接提示", "连接失败");
        qDebug() << "连接失败";
        // 退出程序
        exit(0);
    }

    // 让连接信号和连接处理函数联系起来
    connect(esp_server, &QTcpServer::newConnection, this, &MainWindow::esp_server_New_Connect);
}

void MainWindow::esp_server_New_Connect() // 当有新的链接来的时候
{
    // 获取客户端连接
    socket_esp = esp_server->nextPendingConnection();
    QObject::connect(socket_esp, &QTcpSocket::readyRead, this, &MainWindow::socket_espRead_Data);
    QObject::connect(socket_esp, &QTcpSocket::disconnected, socket_esp, &QTcpSocket::deleteLater);
    qDebug() << "esp连接";

    if (socket_esp != nullptr)
    {
    }
}

void MainWindow::socket_espRead_Data()
{
    QByteArray buffer;
    buffer = socket_esp->readAll();
    QString data = QString(buffer); // 接收数据格式:年份月份日期小时分钟秒（12位）温度湿度光强度（各占6位，共18位）

    // 输出接收到的数据
    qDebug() << "Received data: " << data;
    // 将接收到的数据插入数据库
    QSqlQuery query;
    query.prepare("INSERT INTO environment_data (record_time, temperature, humidity, light_intensity) "
                  "VALUES (:time, :temp, :humidity, :light)");

    query.bindValue(":time", DateTime.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":temp", data.mid(14, 6));
    query.bindValue(":humidity", data.mid(20, 6));
    query.bindValue(":light", data.mid(26, 6));
    // test
    qDebug() << "time: " << DateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "temperature: " << data.mid(14, 6);
    qDebug() << "humidity: " << data.mid(20, 6);
    qDebug() << "light_intensity: " << data.mid(26, 6) << endl;
    if (!query.exec())
    {
        qDebug() << "Error inserting data: " << query.lastError().text();
    }
    else
    {
        qDebug() << "Data inserted successfully";
    }
    //再插入另一个表
    QSqlQuery query1;
    query1.prepare("INSERT INTO environment_data_another (record_time, temperature, humidity, light_intensity) "
                   "VALUES (:time, :temp, :humidity, :light)");

    query1.bindValue(":time", DateTime.toString("yyyy-MM-dd hh:mm:ss"));
    query1.bindValue(":temp", data.mid(32, 6));
    query1.bindValue(":humidity", data.mid(38, 6));
    query1.bindValue(":light", data.mid(44, 6));
    // test
    qDebug() << "time: " << DateTime.toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "temperature: " << data.mid(32, 6);
    qDebug() << "humidity: " << data.mid(38, 6);
    qDebug() << "light_intensity: " << data.mid(44, 6) << endl;
    if (!query1.exec())
    {
        qDebug() << "Error inserting data: " << query1.lastError().text();
    }
    else
    {
        qDebug() << "Another table: Data inserted successfully";
    }


    // QString envir;

    // if(data.contains("Start"))//接收到连接信号,发送1次初始时间
    // {
    //     DateTime = QDateTime::currentDateTime();
    //     qDebug() << "Current Date and Time: " << "time"+DateTime.toString("yyyy-MM-dd hh:mm:ss");
    //     socket_esp->write("time"+DateTime.toString("yyyy-MM-dd hh:mm:ss").toUtf8());
    // }
    // if(data.contains("not pass"))//接收到无人活动标志
    // {
    // ui->led_data_lab->setText("无人活动，建议关灯");
    // }

    // if(data.contains("temp"))//接收到无人活动标志
    // {
    //   envir =data;
    // }

     QString tem;
     QString light;
     QString tem1;
     QString light1;
     tem=data.mid(14, 6);//从小标0开始的5位数据
     light=data.mid(26, 6);
     tem1 = data.mid(32, 6);
     light1 = data.mid(44, 6);
     
     ui->temp_lab->setText(tem.toUtf8());
     ui->light_lab->setText(light.toUtf8());

     ui->temp_lab_2->setText(tem1.toUtf8());
     ui->light_lab_2->setText(light1.toUtf8());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_time_btn_clicked()
{

    DateTime = QDateTime::fromString(ui->dateTimeEdit->text(), "yyyy-MM-dd hh:mm:ss");

    if (socket_esp != nullptr)
    {

        qDebug() << ui->dateTimeEdit->text();
        QString t1 = "time" + ui->dateTimeEdit->text();
        socket_esp->write(t1.toUtf8());
    }
}

void MainWindow::on_temp_btn_clicked() // 温度阈值设置
{
    if (socket_esp != nullptr)
    {
        QString t = "temp" + ui->temp_Edit->text();
        socket_esp->write(t.toUtf8());
    }
}

void MainWindow::on_st_btn_clicked() // 时间常量设置
{
    if (socket_esp != nullptr)
    {
        QString t = "pass" + ui->ST_Edit->text();
        socket_esp->write(t.toUtf8());
    }
}

void MainWindow::on_led_ON_clicked() // led开启按钮
{
    if (socket_esp != nullptr)
    {
        QString t = "ledon";
        socket_esp->write(t.toUtf8());
    }
    ui->led_data_lab->setText("灯已开启");
}

void MainWindow::on_led_OFF_clicked() // led关闭按钮
{
    if (socket_esp != nullptr)
    {
        QString t = "ledoff";
        socket_esp->write(t.toUtf8());
    }
    ui->led_data_lab->setText("灯已关闭");
}

void MainWindow::on_time_btn_2_released()
{
    TableWindow *pic = new TableWindow();
    pic->show();
}

void MainWindow::on_add_btn_clicked()
{
    QSqlQuery query;
    query.prepare("INSERT INTO environment_data (record_time, temperature, humidity, light_intensity) "
                  "VALUES "
                  "(:time1, :temp1, :humidity1, :light1), "
                  "(:time2, :temp2, :humidity2, :light2), "
                  "(:time3, :temp3, :humidity3, :light3)");

    query.bindValue(":time1", "2025-03-09 17:00:00");
    query.bindValue(":temp1", 20.0);
    query.bindValue(":humidity1", 55.0);
    query.bindValue(":light1", 300.5);

    query.bindValue(":time2", "2025-03-09 18:00:00");
    query.bindValue(":temp2", 19.5);
    query.bindValue(":humidity2", 60.0);
    query.bindValue(":light2", 150.0);

    query.bindValue(":time3", "2025-03-09 19:00:00");
    query.bindValue(":temp3", 18.0);
    query.bindValue(":humidity3", 65.0);
    query.bindValue(":light3", 50.0);

    if (!query.exec())
    {
        qDebug() << "Error inserting data: " << query.lastError();
    }
    else
    {
        qDebug() << "Data inserted successfully";
    }
}
