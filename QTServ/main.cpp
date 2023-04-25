#include <QCoreApplication>
#include <messageconvecter.h>
#include <message.h>
#include "pivobackend.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PIVObackend backend(80);
    return a.exec();
}
