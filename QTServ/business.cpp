#include "business.h"
#include <QJsonArray>
#include "messageconvecter.h"


Business::Business()
{

}

void Business::slotMess(Message& MessageData){

    this->InsertIntoBD(MessageData);
}

void Business::InsertIntoBD(Message& MessageData){
    //вставка данных в бд
}




