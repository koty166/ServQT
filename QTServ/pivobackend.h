#ifndef PIVOBACKEND_H
#define PIVOBACKEND_H

#include <QWidget>
class QTcpServer;
class QTextEdit;
class QTcpSocket;

class PIVObackend : public QWidget
{
    Q_OBJECT
private:
    QTcpServer* tcpServer;
    quint16 NextBlockSize;
private:
    void sendToClient(QTcpSocket* pSocket, const QString& str);
public:
    PIVObackend(int port, QWidget* pwgt = 0);
public slots:
    virtual void slotNewConnection();
    void slotReadClient ();

};

#endif // PIVOBACKEND_H
