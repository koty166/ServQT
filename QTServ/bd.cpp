#include "bd.h"
#include <QtSql>
#include <QSqlDatabase>

bd::bd()
{

}


bool bd::ConnectToBD()
{
   QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
   db.setDatabaseName("postgres");
   db.setUserName("MypostgresUser");
   db.setHostName("localhost");
   db.setPassword("VeryStrongPassword123ForBD");
   if (!db.open())
   {
       return false;
   }
   return true;
}

bool bd::InputData(Message &MessageData){
    QSqlQueryModel* model = new QSqlQueryModel();
    QString query = QString("INSERT INTO Messages (Sender, "
                            "Message, Datetime) VALUES (%1, %2, %3)").arg(MessageData.Sender, MessageData.MessageText,
                                                                          MessageData.MessageTime.toString());
    model->setQuery(query);
}
