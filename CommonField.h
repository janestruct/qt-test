#ifndef COMMONFIELD_H
#define COMMONFIELD_H

#include <QString>
#include <QException>



#define SERVER_CERT_FILE "/home/whwong/QTWorkspace/hcb2-webapi/hcb2-webapi/https/example.crt"
#define SERVER_PRIVATE_KEY_FILE "/home/whwong/QTWorkspace/hcb2-webapi/hcb2-webapi/https/example.key"

//#define HCB_ACS_DB_PATH "/home/whwong/QTWorkspace/hcb2-webapi/hcb2-webapi/Database/HCB2-lite-1M.db"
#define HCB_ACS_DB_PATH "HCB2-lite.db"

#define MAX_CARD_NUM        1000000
#define MAX_USER            1000000

#define MAX_TIMEZONE            10
#define MAX_TIMESET_IN_TIMEZONE 10
#define MAX_TIMESET             32

#define MAX_USER_INPUT          16

#define MAX_RELAY               4
#define MAX_USER_OUTPUT         8

#define MAX_DOOR                4
#define MAX_READER              8

class Exception : public QException
{
public:
    void raise() const override { throw *this; }
    Exception *clone() const override { return new Exception(*this); }
};

#endif // COMMONFIELD_H
