#include "pivobackend.h"
#include "message.h"
#include "messageconvecter.h"
#include <QtNetwork/QtNetwork>
#include <QList>
#include <QObject>

PIVObackend::PIVObackend(int port) : QObject()
    , NextBlockSize(0)
{
    tcpServer = new QTcpServer(this);
    if (!tcpServer->listen(QHostAddress::Any, port)) {
        QTextStream(stdout) << "Unable to connect to portx` " << port << ": " << tcpServer->errorString();
        tcpServer->close();
        return;
    }
    connect(tcpServer, SIGNAL(newConnection()),
            this, SLOT(slotNewConnection())
            );
    QTextStream(stdout) << "Connected to port " << port << ": " << tcpServer->errorString();
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

    QByteArray JSONData = pClientSocket->readAll();

    sendToClient(new QTcpSocket(), "your message has been recieved");
    Message* msg = MessageConvecter::ConvertFromJSONToObject(JSONData);
    sendMsgToAllClients(MessageConvecter::ConvertFromObjectToByteArray(msg));
    //QTextStream(stdout) << JSONData;
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

void PIVObackend::sendMsgToAllClients(char* msg){
    for (int i = 0; i < connections.count(); ++i){
        connections[i]->write(msg);
    }
}
