#ifndef CDBHANDLER_H
#define CDBHANDLER_H

#include "cdbtableinfo.h"

#include <QObject>
#include <QSqlDatabase>
#include <QVariant>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <iostream>
#include <string>

using namespace std;

#define IS_DEBUG    true    //false

typedef enum _E_ERR_CODE
{
    InternalDatabaseFailure = 0,
    DatabaseNotOpen,
    InvalidField,
    UserNotExist,
    CardNoExist,
    CardNoNotExist,
    CardNoCapacityFull,
    UserCapacityFull,
} E_ERR_CODE;

extern const char *qStrErrCode[];

class cDBTimezoneInternal: QObject
{
    Q_OBJECT
private:
    int doorTZ;
    int readerTZ;
    int c_doorTZ;
    int cardTZ;
    int userTZ;

public:
    cDBTimezoneInternal();

    int set_internal_TZ(
            int doorTZ,
            int readerTZ,
            int c_doorTZ,
            int cardTZ,
            int userTZ
            );

    int get_internal_TZ(
            int &doorTZ,
            int &readerTZ,
            int &c_doorTZ,
            int &cardTZ,
            int &userTZ
            );
};

//state class
class State
{
    public:
    virtual void HandleReq() = 0;
};

class GetInput: public State
{

public:

    QString i_cardNo;
    int i_rdNo;

    static GetInput* GetInstance()
    {
        static GetInput m_GetInput;
        return &m_GetInput;
    }

    GetInput get_input(QString i_cardNo, int i_rdNo)
    {
        qDebug() << "Inside function get_input : " << i_cardNo << i_rdNo ;
    }

    virtual void HandleReq()
    {
        std::cout<<"Get Input for 1 sec"<<std::endl;
    }
};

class ProcessReader:public State
{
    public:
    static ProcessReader* GetInstance()
    {
        static ProcessReader m_ProcessReader;
            return &m_ProcessReader;
    }

        void HandleReq()
        {
                std::cout<<"Process Reader for 1 sec"<<std::endl;
        }
};

class ProcessCard: public State
{
    public:
    static ProcessCard* GetInstance()
    {
        static ProcessCard m_ProcessCard;
            return &m_ProcessCard;
    }

    void HandleReq()
        {
                std::cout<<"Process Card for 1 sec"<<std::endl;
        }
};

class RunSql: public State
{
    public:
    static RunSql* GetInstance()
    {
        static RunSql m_RunSql;
            return &m_RunSql;
    }

    void HandleReq()
        {
                std::cout<<"Run Sql for 1 sec"<<std::endl;
        }
};

class GetTZ: public State
{
    public:
    static GetTZ* GetInstance()
    {
        static GetTZ m_GetTZ;
            return &m_GetTZ;
    }

    void HandleReq()
        {
                std::cout<<"Get TZ for 1 sec"<<std::endl;
        }
};

class ProcessTZ: public State
{
    public:
    static ProcessTZ* GetInstance()
    {
        static ProcessTZ m_ProcessTZ;
            return &m_ProcessTZ;
    }

    void HandleReq()
        {
                std::cout<<"Process TZ for 1 sec"<<std::endl;
        }
};

class cProcHandler : QObject
{
public:

    int run_fsm(void);
    State* mtlstate;
    int state = 0;

};

class cDBHandler : QObject
{
private:

    QString getLastExecutedQuery(const QSqlQuery& query);

    int CommonUpdateInsertCard(const QString cardNum, const QString userID1, cDBCardTable &cCardInfo, QString &errMsg);

public:
    cDBHandler();
    cDBHandler(const QString &dbPath, const QString &dbName);
    ~cDBHandler();

    QSqlDatabase m_db;
    int random_input(void);
    int get_input(QString cardnum, int reader);

    int run_sql_card(int card_id);
    int run_sql_user(int user_id);

    int process_reader_info(int reader_id);
    int process_valid_card(QString card);
    int process_card_internal(QString cardnum, int reader);
    int process_timezone(cDBTimezoneInternal &cDBTimezone, QString &errMsg);
    int process_valid_timezone(int id, QString &errMsg);

    int SetDBPathNameAndOpen(const QString &dbPath, const QString &dbName);
    int isDBOpen(void);

    // Timset Table
    int GetTimeset(int id, cDBTimesetTable &cTimeset, QString &errMsg);
    int UpdateTimeSet(int id, cDBTimesetTable &cTimeset, QString &errMsg);

    // Timezone Table
    int GetTimezone(int id, int *iTimeZone, QString &errMsg);
    int UpdateTimezone(int id, int *iTimeZone, QString &errMsg);

    // User Type Table
    int GetUserType(int id, cDBUserTypeTable &cUserType, QString &errMsg);
    int SetUserType(int id, cDBUserTypeTable &cUserType, QString &errMsg);

    // Card Type Table
    int GetCardType(int id, cDBCardTypeTable &cCardType, QString &errMsg);
    int SetCardType(int id, cDBCardTypeTable &cCardType, QString &errMsg);

    // Input Table
    int GetInputInfo(int id, cDBInputTable &cInputTable, QString &errMsg);
    int SetInputInfo(int id, cDBInputTable &cInputTable, QString &errMsg);

    // Output Table
    int GetOutputInfo(int id, cDBOutputTable &cOutputTable, QString &errMsg);
    int SetOutputInfo(int id, cDBOutputTable &cOutputTable, QString &errMsg);

    // ModeOperation Table
    int GetModeOperation(int id, cDBModeOfOperationTable &cModeOperationTable, QString &errMsg);
    int SetModeOperation(int id, cDBModeOfOperationTable &cModeOperationTable, QString &errMsg);

    // Custom Wiegand Table
    int GetCustomWiegand(int id, cDBCustomWiegandTable &cCustomWiegand, QString &errMsg);
    int SetCustomWiegand(int id, cDBCustomWiegandTable &cCustomWiegand, QString &errMsg);

    // Door Table
    int GetDoorInfo(int id, cDBDoorTable &cDoorInfo, QString &errMsg);
    int SetDoorInfo(int id, cDBDoorTable &cDoorInfo, QString &errMsg);

    // Reader Table
    int GetReaderInfo(int id, cDBReaderTable &cReaderInfo, QString &errMsg);
    int SetReaderInfo(int id, cDBReaderTable &cReaderInfo, QString &errMsg);

    // User Table
    int GetUserIDFromID1(const QString id1, int &iUserId, QString &errMsg);
    int GetUserInfo(QString id1, cDBUserTable &cUserInfo, QString &errMsg);
    int SetUserInfo(int id, cDBUserTable &cUserInfo, QString &errMsg);

    // Card Table
    int GetCardInfo(int id, cDBCardTable &cCardInfo, QString &errMsg);
    int SetCardInfo(QString cardNum, QString userId1, cDBCardTable &cCardInfo, QString &errMsg);
};


#endif // CDBHANDLER_H
