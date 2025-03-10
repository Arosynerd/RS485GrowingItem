#include "man_client.h"

man_client::man_client()
{
    m_tcpSocket = new QTcpSocket(this);
    m_tcpServer = new QTcpServer(this);
}



bool man_client::client_connect(int port)
{
    if(!m_tcpServer->listen(QHostAddress::AnyIPv4, port))
    {
        qDebug() << "Error: " << m_tcpServer->errorString();
        return false;
    }


    connect(m_tcpServer, &QTcpServer::newConnection, this, &man_client::client_onNewConnect);
    return true;




}

void man_client::client_onNewConnect()
{
    m_tcpSocket = m_tcpServer->nextPendingConnection();
    m_tcpSocket->write("服务器连接成功");//向客户端发送连接成功的
    connect(m_tcpSocket, &QTcpSocket::disconnected, m_tcpSocket, &QTcpSocket::deleteLater);
    connect(m_tcpSocket, &QTcpSocket::readyRead, this, &man_client::client_read);
}

void man_client::client_read()
{
    QByteArray data = m_tcpSocket->readAll();
    // 处理接收到的数据，比如显示到界面上或者进行其他操作

    read_data=QString::fromUtf8(data);
    emit ReadSignal();
}

void man_client::client_send(QString data)
{
    if (m_tcpSocket)
    {
        m_tcpSocket->write(data.toUtf8());
    }
}
