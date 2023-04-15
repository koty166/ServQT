#include "bd.h"
#include <QtSql>
#include <QSqlDatabase>

bd::bd()
{

}


bool bd::ConnectToBD()
{
   QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
   db.setDatabaseName("mirea");
   db.setUserName("mirea");
   db.setHostName("localhost");
   db.setPassword("mirea");
   if (!db.open())
   {
       return false;
   }
   return true;
}


bool bd::SelectData()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employee;");

    QSqlError err = model->lastError();
    if (err.isValid()) {
        qDebug() << err;
        return false;
    }
    return true;
}

b
