#ifndef MAN_CLIENT_H
#define MAN_CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
class man_client
{
    Q_OBJECT
public:
    man_client();
    QTcpSocket* m_tcpSocket;
    QTcpServer* m_tcpServer;
    QString read_data;
signals: void ReadSignal();
public slots:
    void client_read();
    void client_send(QString data);
    bool client_connect(int port);
    void client_onNewConnect();
};

#endif // MAN_CLIENT_H
