#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QWidget>
#include <mainwindow.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QDebug>
namespace Ui {
class TableWindow;
}

class TableWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TableWindow(QWidget *parent = nullptr);
    ~TableWindow();

private slots:
    void on_pushButton_released();

    void on_FindButton_released();


    void on_Find1Button_released();

private:
    Ui::TableWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlTableModel *model_another;
};

#endif // TABLEWINDOW_H
