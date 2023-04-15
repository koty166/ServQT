#include <QCoreApplication>
#include <messageconvecter.h>
#include <message.h>

int main(int argc, char *argv[])
{
    //some test

    QString data = "{\"sender\":\"pc1\",\"message\":\"Some very important text\",\"datetime\":4123656412}";
    Message* msg = MessageConvecter::ConvertFromJSONToObject(data.toLocal8Bit());
    QTextStream(stdout) << msg->Sender << "\n";
    QTextStream(stdout) << msg->MessageText << "\n";
    QTextStream(stdout) << msg->MessageTime.toString("ddd MMMM d yy") << "\n";

    // end some test


    QCoreApplication a(argc, argv);

    return a.exec();
}
