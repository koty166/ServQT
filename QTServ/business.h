#ifndef BUSINESS_H
#define BUSINESS_H

#include <QObject>
#include <QJsonArray>
#include "message.h"

class Business: public QObject
{
public:
    Business();
    static void InsertIntoBD(Message& MessageData);

public slots:
    void slotMess(Message& MessageData);

signals:
    void signalMess();
};

#endif // BUSINESS_H
