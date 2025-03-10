#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTcpServer>
#include <QTimer>
#include <QDateTime>

#include <tablewindow.h>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:



    void socket_espRead_Data();
    void esp_server_New_Connect();






    void on_time_btn_clicked();

    void on_temp_btn_clicked();

    void on_st_btn_clicked();

    void on_led_ON_clicked();

    void on_led_OFF_clicked();

    void on_time_btn_2_released();


    void on_add_btn_clicked();

private:
    Ui::MainWindow *ui;
    int Client_num=0;
    QString temp;
    QString light;
    QTcpServer* esp_server;
    QTcpSocket* socket_esp=nullptr;
    float alarm=50;
    QList<QTcpSocket*> TcpSockets_List;

    QTimer timer;//定时器
    QDateTime DateTime;

    QSqlDatabase db;
    QSqlTableModel *model;

};

#endif // MAINWINDOW_H
