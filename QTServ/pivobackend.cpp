#include "pivobackend.h"
#include <QtNetwork/QtNetwork>

PIVObackend::PIVObackend(int port, QWidget* pwgt /*=0*/) : QWidget(pwgt)
    , NextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Unable to connect to port " << port << ": " << tcpServer->errorString();
        tcpServer->close();
        return;
    }
    connect(tcpServer, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection())
            );
    qDebug() << "Connected to port " << port << ": " << tcpServer->errorString();
}
void PIVObackend::slotNewConnection()
{
    QTcpSocket* pClientSocket = tcpServer->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(deleteLater())
            );
    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient())
            );
    sendToClient(pClientSocket, "Server Response: Connected!");
}

void PIVObackend::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_15);
    for (;;) {
        if (!NextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> NextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < NextBlockSize) {
            break;
        }
        QTime time;
        QString str;
        in >> time >> str;
        QString strMessage =
            time.toString() + " " + "Client has sended - " + str;
        NextBlockSize = 0;
        sendToClient(pClientSocket, "Server Response: Received \"" + str + "\"");
    }
}

void PIVObackend::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << quint16(0) << QTime::currentTime() << str;
    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));
    pSocket->write(arrBlock);
}
