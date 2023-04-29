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
        QJsonArray JsonArr;
        JsonArr.append(MessageData.Sender);
        JsonArr.append(MessageData.MessageText);
        JsonArr.append(MessageData.MessageTime.toSecsSinceEpoch());
        return (new QJsonDocument(JsonArr))->toJson();
    }
    static char* [] ConvertFromObjectToByteArray(Message& MessageData)
    {
        char* data[2];
        return data;
    }
signals:

};

#endif // MESSAGECONVECTER_H
