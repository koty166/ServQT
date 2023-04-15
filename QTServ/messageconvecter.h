#ifndef MESSAGECONVECTER_H
#define MESSAGECONVECTER_H

#include <QObject>
#include <message.h>
#include <QJsonDocument>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>

class MessageConvecter : public QObject
{
    Q_OBJECT
public:
    explicit MessageConvecter(QObject *parent = nullptr);

    static Message* ConvertFromJSONToObject(QByteArray JSONData)
    {
        QJsonDocument JSONDoc = QJsonDocument::fromJson(JSONData);
        Message* MData = new Message();

        MData->Sender = JSONDoc["sender"].toString();
        MData->MessageText = JSONDoc["message"].toString();
        MData->MessageTime = QDateTime::fromSecsSinceEpoch(JSONDoc["datetime"].isDouble());

        return MData;
    }
    static QByteArray ConvertFromObjectToJSON(Message& MessageData)
    {
       /* QJs JSONDoc;


        JSONDoc["sender"] = MessageData.Sender;
        MData->MessageText = JSONDoc["message"].toString();
        MData->MessageTime = QDateTime::fromSecsSinceEpoch(JSONDoc["datetime"].isDouble());

        return new QJsonDocument() toJson();*/
    }
signals:

};

#endif // MESSAGECONVECTER_H
