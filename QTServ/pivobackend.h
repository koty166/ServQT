#ifndef PIVOBACKEND_H
#define PIVOBACKEND_H

#include <QMap>
#include <QtCore>
#include <QObject>

class QTcpServer;
class QTextEdit;
class QTcpSocket;

class PIVObackend : public QObject
{
    Q_OBJECT
private:
    QTcpServer* tcpServer;
    quint16 NextBlockSize;

    int id_increment;

private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
public:
    PIVObackend(int port);
    QMap<int, QTcpSocket*> connections;
public slots:

    virtual void slotNewConnection();
    void slotReadClient();
    void disconnectClient(int client_id);
    void sendMsgToAllClients(char* msg);
};

#endif // PIVOBACKEND_H
