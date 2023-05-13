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
    connect(pClientSocket, SIGNAL(disconnected()), this, SLOT(PIVOBackend::disconnectClient));  //это не будет работать, потому что disconnectClient принимает аргумент, а сигнал disconnected ничего не отправляет
    connect(pClientSocket, &QTcpSocket::readyRead, this, &PIVObackend::slotReadClient);
    sendToClient(pClientSocket, "Server Response: Connected!");
    connections[id_increment] = pClientSocket;
    id_increment++;
}

void PIVObackend::slotReadClient()
{

    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_5_15);

    QByteArray JSONData = pClientSocket->readAll();
    //sendToClient(new QTcpSocket(), "your message has been recieved");
    Message* msg = MessageConvecter::ConvertFromJSONToObject(JSONData);
    char * BufAddr = MessageConvecter::ConvertFromObjectToByteArray(JSONData);

    QTextStream(stdout) << BufAddr;
    sendMsgToAllClients(BufAddr);
    delete[] BufAddr;
    delete msg;

    //QTextStream(stdout) << JSONData;
}

void PIVObackend::sendToClient(QTcpSocket* pSocket, const QString& str)
{
    QTextStream(stdout) << str;
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
