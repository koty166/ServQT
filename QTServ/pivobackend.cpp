#include "pivobackend.h"
#include "message.h"
#include "messageconvecter.h"
#include <QtNetwork/QtNetwork>
#include <QList>

PIVObackend::PIVObackend(int port, QWidget* pwgt /*=0*/) : QWidget(pwgt)
    , NextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Unable to connect to portx` " << port << ": " << tcpServer->errorString();
        tcpServer->close();
        return;
    }
    connect(tcpServer, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection())
            );
    qDebug() << "Connected to port " << port << ": " << tcpServer->errorString();
    id_increment = 0;
}
void PIVObackend::slotNewConnection()
{
    QTcpSocket* pClientSocket = tcpServer->nextPendingConnection();
    connections[id_increment] = pClientSocket;
    connect(pClientSocket, SIGNAL(disconnected()),
            pClientSocket, SLOT(disconnectClient(id_increment))
            );
    connect(pClientSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadClient())
            );
    sendToClient(pClientSocket, "Server Response: Connected!");
    connections[id_increment] = pClientSocket;
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
        QByteArray JSONData;
        in >> JSONData;
        Message* msg = MessageConvecter::ConvertFromJSONToObject(JSONData);

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

void PIVObackend::disconnectClient(int client_id){
    connections.remove(client_id);
}
