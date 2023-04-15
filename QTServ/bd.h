#ifndef BD_H
#define BD_H

#include "message.h"

class bd
{
public:
    bd();
    static bool ConnectToBD();
    static bool SelectData();
    static bool InputData(Message& MessageData);
};

#endif // BD_H
