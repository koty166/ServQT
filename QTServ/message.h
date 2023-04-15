#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QDateTime>


class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(QObject *parent = nullptr);
    QString Sender;
    QString MessageText;
    QDateTime MessageTime;

signals:

};

#endif // MESSAGE_H
