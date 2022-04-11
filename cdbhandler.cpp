#include "cdbhandler.h"
#include "CommonField.h"

#include <QDebug>
#include <QBuffer>
#include <QElapsedTimer>
#include <iostream>
#include <string>
#include<thread>

using namespace std;

QElapsedTimer timer;

const char *qStrErrCode[] =
{
    "Internal Database Failure",
    "Database Not Open",
    "Invalid Parameter",
    "User Not Exist",
    "Card Number Exist. Please Use Update Command",
    "Card Number Not Exist",
    "Card Capacity Full",
    "User Capacity Full"
};

int cProcHandler::run_fsm()
{
        int result = 0;
        cDBHandler c;

    switch(state)
    {
        case 1:
        mtlstate = GetInput::GetInstance();
        mtlstate->HandleReq();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 2;
        break;

        case 2:
        mtlstate = ProcessReader::GetInstance();
        mtlstate->HandleReq();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 3;
        break;

        case 3:
        mtlstate = ProcessCard::GetInstance();
        mtlstate->HandleReq();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 4;
        break;

        case 4:
        mtlstate = RunSql::GetInstance();
        mtlstate->HandleReq();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 5;
        break;

        case 5:
        mtlstate = GetTZ::GetInstance();
        mtlstate->HandleReq();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 6;
        break;

        case 6:
        mtlstate = ProcessTZ::GetInstance();
        mtlstate->HandleReq();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        state = 1;
        break;

        default:
         qDebug() << "\n\rAccess and open database..\n" ;
         result = c.random_input();
         state = 1;
         break;

    }

    if (result == -1)
    {
        qDebug () << "Process return OK - valid entry";
    }
    else
    {
        qDebug() << "Process return Error - invalid entry";
    }
    return -1;
}

cDBHandler::cDBHandler()
{

}

// true = Database opened successfully
// false = Database failed
int cDBHandler::SetDBPathNameAndOpen(const QString &dbPath, const QString &dbName)
{
    int ret;

    m_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    m_db.setDatabaseName(dbPath);

    ret = m_db.open();
#if (IS_DEBUG == true)
//    sql_get_test();
#endif

    /* Execute Vacuum Command
    QSqlQuery query(m_db);

    if (query.exec(QString("VACUUM")) == false)
    {
        qDebug() << "VACUUM Failed:"
                 << query.lastError().text();
        qDebug() << "\r\n" << getLastExecutedQuery(query);
    }
    */
    return ret;
}

cDBHandler::cDBHandler(const QString &dbPath, const QString &dbName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    m_db.setDatabaseName(dbPath);

    m_db.open();
#if (IS_DEBUG == true)
//    input_console();
    cProcHandler p;
    for (;;)
    {
       p.run_fsm();
    }
#endif

    /* Execute Vacuum Command */
//    QSqlQuery query(m_db);

//    if (query.exec(QString("VACUUM")) == false)
//    {
//        qDebug() << "VACUUM Failed:"
//                 << query.lastError().text();
//        qDebug() << "\r\n" << getLastExecutedQuery(query);
//    }
}

int cDBHandler::isDBOpen(void)
{
    return m_db.isOpen();
}

cDBHandler::~cDBHandler()
{
    if (m_db.isOpen())
        m_db.close();
}

QString cDBHandler::getLastExecutedQuery(const QSqlQuery& query)
{
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    while (it.hasNext())
    {
        it.next();
        str.replace(it.key(), it.value().toString());
    }
    return str;
}

#if (IS_DEBUG == true)

int cDBHandler::get_input(QString cardnum, int reader)
{
    qDebug() << "Input details" ;
    qDebug() << "Card Number : " << cardnum;
    qDebug() << "Reader : " << reader;

    int result ;
    result = process_card_internal(cardnum,reader);
    return result;
}

int cDBHandler::process_valid_card(QString card)
{
    QString errMsg;

    if (card.length() != 10)
    {
        cout << "Invalid input : card number must be of 10 digit" ;
        return false ;
    }

    for (int i =0;i<card.size();i++) {
        if (card[i].isLetterOrNumber())
        {
            qDebug() << "Valid input" ;
            return true;
        }
        else
        {
            qDebug() << "Invalid input : card number must be of alphanumeric character only" ;
            return false;
        }
    }
    return -1;
}

int cDBHandler::process_card_internal(QString cardNum, int reader)
{
    QString errMsg;
    cDBTimezoneInternal c;

    process_valid_card(cardNum);
    int door_id = process_reader_info(reader);
    int cardID ;
    int userID ;
    int c_door_TZ ;
    int card_TZ ;
    int user_TZ ;
    int door_TZ ;
    int reader_TZ ;
    int result;

    timer.start();

    qDebug() << "\n\rProcess card details" ;

    try {
        QSqlQuery query(m_db);
        query.prepare("SELECT Card.CardID, User.UserID, User.OverrideCard"
                      " FROM Card"
                      " INNER JOIN User ON Card.UserID=User.UserID"
                      " WHERE CardNumber = (:card)");
        query.bindValue(":card",cardNum);
        query.exec();

        if(!query.exec())
        {
            qDebug() << errMsg ;
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_CardID = query.record().indexOf("Card.CardID");
        int rec_UserID = query.record().indexOf("UserID");
        int rec_Override = query.record().indexOf("OverrideCard");

        if (query.next())
        {
            cardID  = query.value(rec_CardID).toInt();
            userID  = query.value(rec_UserID).toInt();
            int override = query.value(rec_Override).toInt();

            qDebug() << "Card ID :" << cardID;
            qDebug() << "User ID :" << userID;
            qDebug() << "Override card :" << override <<endl;

            if (override == 1)
            {
                run_sql_user(userID);
            }
            else
            {
               run_sql_card(cardID);
            }
        }
        else
        {
            qDebug() << "No Card Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    qDebug() << "\nProcess reader-door timezone details";
    try {
        QSqlQuery query(m_db);;

        query.prepare("SELECT Door.DoorEnTimezone, Reader.ReaderEnTimezone"
                      " FROM Door"
                      " INNER JOIN Reader ON Door.DoorID=Reader.TiedToDoorID"
                      " WHERE DoorID = (:id) AND ReaderID = (:reader)");
        query.bindValue(":id",door_id);
        query.bindValue(":reader",reader);
        query.exec();

     if(!query.exec())
     {
         qDebug() << "No reader-door timezone found: \n" << getLastExecutedQuery(query);
         errMsg = query.lastError().text();
         return InternalDatabaseFailure;
     }

     int rec_door_timezone = query.record().indexOf("DoorEnTimezone");
     int rec_reader_timezone = query.record().indexOf("ReaderEnTimezone");

     if (query.next())
     {
        door_TZ = query.value(rec_door_timezone).toInt();
        reader_TZ = query.value(rec_reader_timezone).toInt();
        qDebug() << "Door en timezone : " << door_TZ;
        qDebug() << "Reader en timezone : " << reader_TZ;
     }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    QString door [4] = {"Door1ID", "Door2ID", "Door3ID", "Door4ID"};
    QString timezone [4] = {"Door1AccessTimezone", "Door2AccessTimezone", "Door3AccessTimezone", "Door4AccessTimezone"};
    QString t_door = door[door_id-1];
    QString t_timezone = timezone[door_id-1];

    qDebug() << "\nProcess card-door timezone details";

    try {

        QSqlQuery query(m_db);

        switch (door_id) {
                case 1:
            query.prepare("SELECT Card.Door1ID, Door.DoorName, Card.Door1AccessTimezone, Card.CardEnTimezone, User.UserEnTimezone"
                          " FROM Card"
                          " INNER JOIN Door ON Card.Door1ID=Door.DoorID"
                          " INNER JOIN User ON Card.Door1ID=User.UserID"
                          " WHERE Card.Door1ID = (:door) AND CardID = (:card) AND Card.UserID = (:user)");
            query.bindValue(":door",door_id);
            query.bindValue(":card",cardID);
            query.bindValue(":user",userID);

           //qDebug() << "Case query: \n" << getLastExecutedQuery(query);
                    break;
                case 2:
            query.prepare("SELECT Card.Door2ID, Door.DoorName, Card.Door2AccessTimezone, Card.CardEnTimezone, User.UserEnTimezone"
                          " FROM Card"
                          " INNER JOIN Door ON Card.Door2ID=Door.DoorID"
                          " INNER JOIN User ON Card.Door2ID=User.UserID"
                          " WHERE Card.Door2ID = (:door) AND CardID = (:card) AND Card.UserID = (:user)");
            query.bindValue(":door",door_id);
            query.bindValue(":card",cardID);
            query.bindValue(":user",userID);

            //qDebug() << "Case query: \n" << getLastExecutedQuery(query);
                    break;
                case 3:
            query.prepare("SELECT Card.Door3ID, Door.DoorName, Card.Door3AccessTimezone, Card.CardEnTimezone, User.UserEnTimezone"
                          " FROM Card"
                          " INNER JOIN Door ON Card.Door3ID=Door.DoorID"
                          " INNER JOIN User ON Card.Door3ID=User.UserID"
                          " WHERE Card.Door3ID = (:door) AND CardID = (:card) AND Card.UserID = (:user)");
            query.bindValue(":door",door_id);
            query.bindValue(":card",cardID);
            query.bindValue(":user",userID);

            //qDebug() << "Case query: \n" << getLastExecutedQuery(query);
                    break;
                case 4:
            query.prepare("SELECT Card.Door4ID, Door.DoorName, Card.Door4AccessTimezone, Card.CardEnTimezone, User.UserEnTimezone"
                          " FROM Card"
                          " INNER JOIN Door ON Card.Door4ID=Door.DoorID"
                          " INNER JOIN User ON Card.Door4ID=User.UserID"
                          " WHERE Card.Door4ID = (:door) AND CardID = (:card) AND Card.UserID = (:user)");
            query.bindValue(":door",door_id);
            query.bindValue(":card",cardID);
            query.bindValue(":user",userID);

            //qDebug() << "Case query: \n" << getLastExecutedQuery(query);
                    break;
                default:
            query.prepare("SELECT Card.Door1ID, Door.DoorName, Card.Door1AccessTimezone, Card.CardEnTimezone, User.UserEnTimezone"
                          " FROM Card"
                          " INNER JOIN Door ON Card.Door1ID=Door.DoorID"
                          " INNER JOIN User ON Card.Door1ID=User.UserID"
                          " WHERE Card.Door1ID = (:door) AND CardID = (:card) AND Card.UserID = (:user)");
            query.bindValue(":door",door_id);
            query.bindValue(":card",cardID);
            query.bindValue(":user",userID);

            //qDebug() << "Case query: \n" << getLastExecutedQuery(query);
                    break;
            }

        query.exec();

        if(!query.exec())
        {
            qDebug() << "Query exec fail" << errMsg ;
            errMsg = query.lastError().text();
            //return InternalDatabaseFailure;
        }

        int rec_DoorID = query.record().indexOf(t_door);
        int rec_DoorName = query.record().indexOf("DoorName");
        int rec_door_TZ = query.record().indexOf(t_timezone);
        int rec_card_TZ = query.record().indexOf("CardEnTimezone");
        int rec_user_TZ = query.record().indexOf("UserEnTimezone");


        if (query.next())
        {
            int DoorID  = query.value(rec_DoorID).toInt();
            QString DoorName = query.value(rec_DoorName).toString();
            c_door_TZ = query.value(rec_door_TZ).toInt();
            card_TZ = query.value(rec_card_TZ).toInt();
            user_TZ = query.value(rec_user_TZ).toInt();


            qDebug() << "Door ID : " << DoorID;
            qDebug() << "Door Name : " << DoorName;
            qDebug() << "Door Access TZ : " << door_TZ;
            qDebug() << "Card En TZ : " << card_TZ;
            qDebug() << "User En TZ : " << user_TZ;

            c.set_internal_TZ(door_TZ, reader_TZ, c_door_TZ, card_TZ, user_TZ);
            result = process_timezone(c,errMsg);
        }
        else
        {
            qDebug() << "No Time Info Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

   qDebug() << "Total execution time :" << timer.elapsed() << "ms" << endl;

      return result;
}

int cDBHandler::run_sql_card(int card_id)
{
    qDebug() << "Run sql (card)" ;

    QString errMsg;

    try {
        QSqlQuery query(m_db);
        query.prepare("SELECT Card.CardID, Card.CardNumber, CardType.CardType, Card.ValidStart, Card.ValidEnd"
                      " FROM Card"
                      " INNER JOIN CardType ON Card.CardType=CardType.CardTypeID"
                      " WHERE CardID = (:card)");
        query.bindValue(":card",card_id);
        query.exec();

        if(!query.exec())
        {
            qDebug() << "No Card Found: \n" << getLastExecutedQuery(query);
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_CardID = query.record().indexOf("Card.CardID");
        int rec_CardType = query.record().indexOf("CardType");
        int rec_CardNumber = query.record().indexOf("CardNumber");
        int rec_ValidStart = query.record().indexOf("ValidStart");
        int rec_ValidEnd = query.record().indexOf("ValidEnd");

        if (query.next())
        {
            int cardID  = query.value(rec_CardID).toInt();
            QString cardNum = query.value(rec_CardNumber).toString();
            QString CardType = query.value(rec_CardType).toString();
            QString ValidStart = query.value(rec_ValidStart).toString();
            QString ValidEnd = query.value(rec_ValidEnd).toString();

            qDebug() << "Card ID : " << cardID;
            qDebug() << "Card Number : " << cardNum;
            qDebug() << "Card Type : " << CardType;
            qDebug() << "Valid Start : " << ValidStart;
            qDebug() << "Valid End : " << ValidEnd;
        }
        else
        {
            qDebug() << "No Card Found: \n" << getLastExecutedQuery(query);
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
      return -1;
}

int cDBHandler::run_sql_user(int id)
{
    qDebug() << "Run sql (user)" ;

    QString errMsg;

    try {
        QSqlQuery query(m_db);
        query.prepare("SELECT User.UserID, UserType.NameofUserType, User.ValidStart, User.ValidEnd"
                      " FROM User"
                      " INNER JOIN UserType ON User.UserType=UserType.UserTypeID"
                      " WHERE UserID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            qDebug() << "query exec fail" << query.executedQuery() ;
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_UserID = query.record().indexOf("UserID");
        int rec_UserType = query.record().indexOf("NameofUserType");
        int rec_ValidStart = query.record().indexOf("ValidStart");
        int rec_ValidEnd = query.record().indexOf("ValidEnd");

        if (query.next())
        {
            int UserID  = query.value(rec_UserID).toInt();
            QString UserType = query.value(rec_UserType).toString();
            QString ValidStart = query.value(rec_ValidStart).toString();
            QString ValidEnd = query.value(rec_ValidEnd).toString();

            qDebug() << "User ID :" << UserID;
            qDebug() << "User Type :" << UserType;
            qDebug() << "Valid Start :" << ValidStart;
            qDebug() << "Valid End :" << ValidEnd;
        }
        else
        {
            qDebug() << "No User Found: \n" << getLastExecutedQuery(query);
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
      return -1;
}

int cDBHandler::process_reader_info(int reader_id)
{
    qDebug() << "\n\rProcess reader details" ;
    qDebug() << "Reader : " << reader_id;
    QString errMsg;

    try {
        QSqlQuery query(m_db);;

     query.prepare("SELECT TiedToDoorID"
                   " FROM Reader"
                   " WHERE ReaderID = (:reader_id)");
     query.bindValue(":reader_id",reader_id);
     query.exec();

     if(!query.exec())
     {
         qDebug() << "No reader found: \n" << getLastExecutedQuery(query);
         errMsg = query.lastError().text();
         return InternalDatabaseFailure;
     }

     int rec_door = query.record().indexOf("TiedToDoorID");

     if (query.next())
     {
        int door = query.value(rec_door).toInt();
        qDebug() << "Door : " << door;
        return door;
     }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

cDBTimezoneInternal::cDBTimezoneInternal()
{

}

int cDBTimezoneInternal::set_internal_TZ(int doorTZ, int readerTZ, int c_doorTZ, int cardTZ, int userTZ)
{
    this -> doorTZ   = doorTZ;
    this -> readerTZ = readerTZ;
    this -> c_doorTZ = c_doorTZ;
    this -> cardTZ   = cardTZ;
    this -> userTZ   = userTZ;

    return -1;
}

int cDBTimezoneInternal::get_internal_TZ(int &doorTZ, int &readerTZ, int &c_doorTZ, int &cardTZ, int &userTZ)
{
    doorTZ = this -> doorTZ ;
    readerTZ = this -> readerTZ ;
    c_doorTZ = this -> c_doorTZ ;
    cardTZ = this -> cardTZ ;
    userTZ = this -> userTZ ;

    return -1;
}

int cDBHandler::process_timezone(cDBTimezoneInternal &cDBTimezone, QString &errMsg)
{
    int doorTZ ;
    int readerTZ ;
    int c_doorTZ ;
    int cardTZ ;
    int userTZ ;

    cDBTimezone.get_internal_TZ(doorTZ,readerTZ, c_doorTZ, cardTZ, userTZ);

    qDebug() << "\n\rGet internal TZ ";
    qDebug() << "Door En TZ : " << doorTZ;
    qDebug() << "Reader En TZ : " << readerTZ;
    qDebug() << "Card Door TZ : " << c_doorTZ;
    qDebug() << "Card En TZ : " << cardTZ;
    qDebug() << "User En TZ : " << userTZ;

    for(;;)
    {
    //qDebug() << "\nGet info doorTZ : " << doorTZ;
    if(process_valid_timezone(doorTZ,errMsg))
    {
        //qDebug() << "\nGet info readerTZ : " << readerTZ;
        if(process_valid_timezone(readerTZ,errMsg))
        {
            //qDebug() << "\nGet info cardTZ : " << cardTZ;
            if(process_valid_timezone(cardTZ,errMsg))
            {
                //qDebug() << "\nGet info userTZ : " << userTZ;
                if(process_valid_timezone(userTZ,errMsg))
                {
                    //qDebug() << "\nGet info c_doorTZ : " << c_doorTZ;
                    if(process_valid_timezone(c_doorTZ,errMsg))
                    {
                            qDebug() << "Valid entry";
                    }
                    else
                    {
                        qDebug() << "Invalid entry : c_doorTZ";
                        return InternalDatabaseFailure;
                        break;
                    }
                }
                else
                {
                    qDebug() << "Invalid entry : userTZ";
                    return InternalDatabaseFailure;
                    break;
                }
            }
            else
            {
                qDebug() << "Invalid entry : cardTZ";
                return InternalDatabaseFailure;
                break;
            }
        }
        else
        {
            qDebug() << "Invalid entry : readerTZ";
            return InternalDatabaseFailure;
            break;
        }
    }
    else
    {
        qDebug() << "Invalid entry : doorTZ";
        return InternalDatabaseFailure;
        break;
    }
    break;
   }

    return -1;
}

int cDBHandler::process_valid_timezone(int id, QString &errMsg)
{
    try {
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM Timezone WHERE TimezoneID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_iTimeset[10]={};
        QString Timeset[10] = {"TimeSet1","TimeSet2","TimeSet3","TimeSet4","TimeSet5",
                         "TimeSet6","TimeSet7","TimeSet8","TimeSet9","TimeSet10"};
        cDBTimezoneTable cTimezone;

        for (int i=0; i<10; i++)
        {
            rec_iTimeset[i] = query.record().indexOf(Timeset[i]);
        }

        int iTimeset[10];

        if (query.next())
        {
            for (int i=0; i<10; i++)
            {
                iTimeset[i] = query.value(rec_iTimeset[i]).toInt();

                try {
                    QSqlQuery query(m_db);
                    query.prepare("SELECT * FROM Timeset WHERE TimesetID = (:id)");
                    query.bindValue(":id",iTimeset[i]);
                    query.exec();

                    if(!query.exec())
                    {
                        errMsg = query.lastError().text();
                        return InternalDatabaseFailure;
                    }

                    int rec_startTime = query.record().indexOf("StartTime");
                    int rec_endTime = query.record().indexOf("EndTime");

                    QString startTime;
                    QString endTime;

                    if (query.next())
                    {
                        startTime = query.value(rec_startTime).toString();
                        endTime = query.value(rec_endTime).toString();

                        QTime current = QTime::currentTime();
                        QTime m_startTime = QTime::fromString(startTime, "HH:mm");
                        QTime m_endTime = QTime::fromString(endTime, "HH:mm");

                        //qDebug() << "Start Time : " << startTime;
                        //qDebug() << "End Time : " << endTime;

                                if((current >= m_startTime) && (current <= m_endTime))
                                {
                                    return true;
                                    break;
                                }
                                else
                                {
                                    return false;
                                    break;
                                }
                    }
                    else
                    {
                        qDebug() << "No Timeset Found: \n" << getLastExecutedQuery(query);

                        errMsg = query.lastError().text();
                        return InternalDatabaseFailure;
                    }

                } catch (Exception e) {
                    errMsg = e.what();
                    return InternalDatabaseFailure;
                }
            }
        }
        else
        {
            qDebug() << "No Timezone Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
        return -1;
}

#endif

int cDBHandler::random_input(void)
{
    QString errMsg;

    srand(time(0));
//    int id = rand() % 1000000 ;
//    int reader = rand() % 8;
    int result = -1;
    int id = 7;
    int reader = 1;

    try {
        QSqlQuery query(m_db);;

     query.prepare("SELECT CardID, CardNumber"
                   " FROM Card"
                   " WHERE CardID = (:id)");
     query.bindValue(":id",id);
     query.exec();

     if(!query.exec())
     {
         qDebug() << "No Card Found: \n" << getLastExecutedQuery(query);
         errMsg = query.lastError().text();
         return InternalDatabaseFailure;
     }

     int rec_CardNumber = query.record().indexOf("CardNumber");

     if (query.next())
     {
        QString cardNum = query.value(rec_CardNumber).toString();
        result = get_input(cardNum,reader);
     }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return result;
}
/*
 * Timeset Handling
 */
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::UpdateTimeSet(int id, cDBTimesetTable &cTimeset, QString &errMsg)
{
    /* Check whether the timeset id is exist in the table */
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        /* Starting from 3 only valid, index 1 for no access, index 2 for full access */
        id += 3;

        if (id > (MAX_TIMESET + 2))
        {
            return InvalidField;
        }

        QTime startTime;
        QTime endTime;
        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM Timeset WHERE TimesetID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "timeset: " << id << " count: " << count;
#endif
        cTimeset.GetTimeSet(startTime, endTime);

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO Timeset ("
                          "TimesetID, "
                          "StartTime, "
                          "EndTime) VALUES ("
                          ":id, "
                          ":startTime, "
                          ":endTime"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE Timeset SET "
                          "StartTime = :startTime, "
                          "EndTime = :endTime "
                          "WHERE TimesetID = :id");
        }

        query.bindValue(":id", id);
        query.bindValue(":startTime", startTime.toString("HH:mm"));
        query.bindValue(":endTime", endTime.toString("HH:mm"));

        if(!query.exec())
        {
            qDebug() << "UpdateTimeSet Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (QException &e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::GetTimeset(int id, cDBTimesetTable &cTimeset, QString &errMsg)
{
    // TODO: To implement
    if (id > (MAX_TIMESET + 2))
    {
        return InvalidField;
    }

    try {
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM Timeset WHERE TimesetID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_startTime = query.record().indexOf("StartTime");
        int rec_endTime = query.record().indexOf("EndTime");

        QString startTime;
        QString endTime;

        if (query.next())
        {
            startTime = query.value(rec_startTime).toString();
            endTime = query.value(rec_endTime).toString();

//            qDebug() << "Timeset | Start Time : " << startTime;
//            qDebug() << "Timeset | End Time : " << endTime;
        }
        else
        {
            qDebug() << "No Timeset Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cTimeset.SetTimeset(startTime, endTime);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

/*
 * Timezone Handling
 */
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetTimezone(int id, int *iTimeSet, QString &errMsg)
{
    if (iTimeSet == nullptr)
        return InvalidField;

    /* Starting from 3 only valid, index 1 for full access, index 2 for no access */
    if (id == 0)
        return InvalidField;

    //id += 3;

    if (id > MAX_TIMEZONE + 2)
        return InvalidField;

    // TODO: To implement
    try {
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM Timezone WHERE TimezoneID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_iTimeset[10]={};
        QString Timeset[10] = {"TimeSet1","TimeSet2","TimeSet3","TimeSet4","TimeSet5",
                         "TimeSet6","TimeSet7","TimeSet8","TimeSet9","TimeSet10"};
        cDBTimezoneTable cTimezone;

        for (int i=0; i<10; i++)
        {
            rec_iTimeset[i] = query.record().indexOf(Timeset[i]);

        }

        int iTimeset[10];

        if (query.next())
        {
            for (int i=0; i<10; i++)
            {
                iTimeset[i] = query.value(rec_iTimeset[i]).toInt();
            }
        }
        else
        {
            qDebug() << "No Timezone Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cTimezone.SetTimeZone(iTimeset);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::UpdateTimezone(int id, int *iTimeSetId, QString &errMsg)
{
    /* Check whether the timeset id is exist in the table */
    qDebug() << "Update Timeset->" ;
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        /* Starting from 3 only valid, index 1 for full access, index 2 for no access */
        id += 3;

        if (id > MAX_TIMEZONE + 2)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM Timezone WHERE TimezoneID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "timeset: " << id << " count: " << count;
#endif

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO Timezone ("
                          "TimezoneID, "
                          "Timeset1, "
                          "Timeset2, "
                          "Timeset3, "
                          "Timeset4, "
                          "Timeset5, "
                          "Timeset6, "
                          "Timeset7, "
                          "Timeset8, "
                          "Timeset9, "
                          "Timeset10 "
                          ") VALUES ("
                          ":id, "
                          ":Timeset1, "
                          ":Timeset2, "
                          ":Timeset3, "
                          ":Timeset4, "
                          ":Timeset5, "
                          ":Timeset6, "
                          ":Timeset7, "
                          ":Timeset8, "
                          ":Timeset9, "
                          ":Timeset10"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE Timezone SET "
                          "Timeset1 = :Timeset1, "
                          "Timeset2 = :Timeset2, "
                          "Timeset3 = :Timeset3, "
                          "Timeset4 = :Timeset4, "
                          "Timeset5 = :Timeset5, "
                          "Timeset6 = :Timeset6, "
                          "Timeset7 = :Timeset7, "
                          "Timeset8 = :Timeset8, "
                          "Timeset9 = :Timeset9, "
                          "Timeset10 = :Timeset10 "
                          "WHERE TimezoneID = :id");
        }
        query.bindValue(":id", id);


        for (int i = 0; i < MAX_TIMESET_IN_TIMEZONE; i++)
        {
            qDebug() << "Timeset " << i << ": " << iTimeSetId[i];
        }

        for (int i = 0; i < MAX_TIMESET_IN_TIMEZONE; i++)
        {
            QString strBind = ":Timeset" + QString("%1").arg(i + 1);
            query.bindValue(strBind, iTimeSetId[i]);
        }

        if(!query.exec())
        {
            qDebug() << "UpdateTimezone Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (QException &e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

/*
 * User Type Handling
 */
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetUserType(int id, cDBUserTypeTable &cUserType, QString &errMsg)
{
    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM UserType WHERE UserTypeID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_nameUserType = query.record().indexOf("NameofUserType");
        int rec_description = query.record().indexOf("Description");

        QString type;
        QString desc;

        if (query.next())
        {
            type = query.value(rec_nameUserType).toString();
            desc = query.value(rec_description).toString();

            qDebug() << "User Type | Type : " << type;
            qDebug() << "User Type | Description : " << desc;
        }
        else
        {
            qDebug() << "No User Type Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cUserType.SetUserType(type, desc);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

int cDBHandler::SetUserType(int id, cDBUserTypeTable &cUserType, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM UserType WHERE UserTypeID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "User Type: " << id << " count: " << count;
#endif
        QString userType;
        QString description;

        cUserType.GetUserType(userType, description);

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO UserType ("
                          "UserTypeID, "
                          "NameofUserType, "
                          "Description"
                          ") VALUES ("
                          ":id, "
                          ":Name, "
                          ":Description"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE UserType SET "
                          "NameofUserType = :Name, "
                          "Description = :Description "
                          "WHERE UserTypeID = :id");
        }

        query.bindValue(":id", id);
        query.bindValue(":Name", userType);
        query.bindValue(":Description", description);

        if(!query.exec())
        {
            qDebug() << "Update User Type Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

/*
 * Card Type Handling
 */
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetCardType(int id, cDBCardTypeTable &cCardType, QString &errMsg)
{
    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM CardType WHERE CardTypeID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_nameCardType = query.record().indexOf("CardType");
        int rec_description = query.record().indexOf("Description");

        QString cardName;
        QString desc;

        if (query.next())
        {
            cardName = query.value(rec_nameCardType).toString();
            desc = query.value(rec_description).toString();

            qDebug() << "Card Type | Type : " << cardName;
            qDebug() << "Card Type | Description : " << desc;
        }
        else
        {
            qDebug() << "No Card Type Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cCardType.SetCardType(cardName, desc);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::SetCardType(int id, cDBCardTypeTable &cCardType, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM CardType WHERE CardTypeID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "Card Type: " << id << " count: " << count;
#endif
        QString cardType;
        QString cardTypeDescription;

        cCardType.GetCardType(cardType, cardTypeDescription);

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO CardType ("
                          "CardTypeID, "
                          "CardType, "
                          "Description"
                          ") VALUES ("
                          ":id, "
                          ":Name, "
                          ":Description"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE CardType SET "
                          "CardType = :Name, "
                          "Description = :Description "
                          "WHERE CardTypeID = :id");
        }

        query.bindValue(":id", id);
        query.bindValue(":Name", cardType);
        query.bindValue(":Description", cardTypeDescription);

        if(!query.exec())
        {
            qDebug() << "Update Card Type Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

/*
 * Input Table Handling
 */
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetInputInfo(int id, cDBInputTable &cInputTable, QString &errMsg)
{
    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM Input WHERE InputID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_name = query.record().indexOf("Name");
        int rec_type = query.record().indexOf("Type");
        int rec_closeOpenTrigger = query.record().indexOf("CloseOpenTrigger");
        int rec_analogReadingTrigger = query.record().indexOf("AnalogReadingTrigger");
        int rec_gpio = query.record().indexOf("GPIOPin");

        QString name;
        QString type;
        int coTrigger;
        int arTrigger;
        int gpio;

        if (query.next())
        {
            name = query.value(rec_name).toString();
            type = query.value(rec_type).toString();
            coTrigger = query.value(rec_closeOpenTrigger).toInt();
            arTrigger = query.value(rec_analogReadingTrigger).toInt();
            gpio = query.value(rec_gpio).toInt();

            qDebug() << "Input | Name : " << name;
            qDebug() << "Input | Type : " << type;
            qDebug() << "Input | Close Open Trigger : " << coTrigger;
            qDebug() << "Input | Analog Reading Trigger : " << arTrigger;
            qDebug() << "Input | GPIO : " << gpio;
        }
        else
        {
            qDebug() << "No Input Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cInputTable.SetInputInfo(name,type,coTrigger,arTrigger,gpio);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::SetInputInfo(int id, cDBInputTable &cInputTable, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0 || id > MAX_USER_INPUT)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM Input WHERE InputID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "input: " << id << " count: " << count;
#endif

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO Input ("
                          "InputID, "
                          "Name, "
                          "Type, "
                          "CloseOpenTrigger, "
                          "AnalogReadingTrigger, "
                          "GPIOPin "
                          ") VALUES ("
                          ":id, "
                          ":Name, "
                          ":Type, "
                          ":CloseOpenTrigger, "
                          ":AnalogReadingTrigger, "
                          ":GPIOPin"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE Input SET "
                          "Name = :Name, "
                          "Type = :Type, "
                          "CloseOpenTrigger = :CloseOpenTrigger, "
                          "AnalogReadingTrigger = :AnalogReadingTrigger, "
                          "GPIOPin = :GPIOPin "
                          "WHERE InputID = :id");
        }

        QString inputName;
        QString type;
        int CloseOpenTrigger;
        double analogReadingTrigger;
        int gpioPin;

        cInputTable.GetInputInfo(inputName, type, CloseOpenTrigger, analogReadingTrigger, gpioPin);

        query.bindValue(":id", id);
        query.bindValue(":Name", inputName);
        query.bindValue(":Type", type);
        query.bindValue(":CloseOpenTrigger", CloseOpenTrigger);
        query.bindValue(":AnalogReadingTrigger", analogReadingTrigger);
        query.bindValue(":GPIOPin", gpioPin);

        if(!query.exec())
        {
            qDebug() << "Update Input Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

/*
 * Output Table Handling
 */
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetOutputInfo(int id, cDBOutputTable &cOutputTable, QString &errMsg)
{
    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM Output WHERE OutputID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_name = query.record().indexOf("OutputName");
        int rec_type = query.record().indexOf("Type");
        int rec_releaseSec = query.record().indexOf("ReleaseSeconds");
        int rec_openSec = query.record().indexOf("OpenSeconds");
        int rec_pulseSec = query.record().indexOf("PulseSeconds");
        int rec_gpioPin = query.record().indexOf("GPIOPin");

        QString name;
        QString type;
        int releaseSec;
        int openSec;
        int pulseSec;
        int gpioPin;

        if (query.next())
        {
            name = query.value(rec_name).toString();
            type = query.value(rec_type).toString();
            releaseSec = query.value(rec_releaseSec).toInt();
            openSec = query.value(rec_openSec).toInt();
            pulseSec = query.value(rec_pulseSec).toInt();
            gpioPin = query.value(rec_gpioPin).toInt();

            qDebug() << "Output | Name : " << name;
            qDebug() << "Output | Type : " << type;
            qDebug() << "Output | Release Seconds : " << releaseSec;
            qDebug() << "Output | Open Seconds : " << openSec;
            qDebug() << "Output | Pulse Seconds : " << pulseSec;
            qDebug() << "Output | GPIO : " << gpioPin;

        }
        else
        {
            qDebug() << "No Output Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cOutputTable.SetOutputInfo(name,type,releaseSec,openSec,pulseSec,gpioPin);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::SetOutputInfo(int id, cDBOutputTable &cOutputTable, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0 || id > (MAX_RELAY + MAX_USER_OUTPUT))
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM Output WHERE OutputID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "Output: " << id << " count: " << count;
#endif

        QString outputName;
        QString type;
        int releaseSec;
        int openSec;
        int pulseSec;
        int gpioPin;

        cOutputTable.GetOutputInfo(outputName, type, releaseSec, openSec, pulseSec, gpioPin);

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO Output ("
                          "OutputID, "
                          "OutputName, "
                          "Type, "
                          "ReleaseSeconds, "
                          "OpenSeconds, "
                          "PulseSeconds, "
                          "GPIOPin"
                          ") VALUES ("
                          ":id, "
                          ":Name, "
                          ":Type, "
                          ":ReleaseSeconds, "
                          ":OpenSeconds, "
                          ":PulseSeconds, "
                          ":GPIOPin"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE Output SET "
                          "OutputName = :Name, "
                          "Type = :Type, "
                          "ReleaseSeconds = :ReleaseSeconds, "
                          "OpenSeconds = :OpenSeconds, "
                          "PulseSeconds = :PulseSeconds, "
                          "GPIOPin = :GPIOPin "
                          "WHERE OutputID = :id");
        }

        query.bindValue(":id", id);
        query.bindValue(":Name", outputName);
        query.bindValue(":Type", type);
        query.bindValue(":ReleaseSeconds", releaseSec);
        query.bindValue(":OpenSeconds", openSec);
        query.bindValue(":PulseSeconds", pulseSec);
        query.bindValue(":GPIOPin", gpioPin);

        if(!query.exec())
        {
            qDebug() << "Update Output Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

/*
    Mode Operation Table
*/
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetModeOperation(int id, cDBModeOfOperationTable &cModeOperationTable, QString &errMsg)
{

    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM ModeOperation WHERE OperationID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_card = query.record().indexOf("Card");
        int rec_pin = query.record().indexOf("Pin");
        int rec_face = query.record().indexOf("Face");
        int rec_qr = query.record().indexOf("QR");
        int rec_ble = query.record().indexOf("BLE");

        int card;
        int pin;
        int face;
        int qr;
        int ble;

        if (query.next())
        {
            card = query.value(rec_card).toInt();
            pin = query.value(rec_pin).toInt();
            face = query.value(rec_face).toInt();
            qr = query.value(rec_qr).toInt();
            ble = query.value(rec_ble).toInt();

            qDebug() << "Mode Operation | Card : " << card;
            qDebug() << "Mode Operation | Pin : " << pin;
            qDebug() << "Mode Operation | Face : " << face;
            qDebug() << "Mode Operation | QR : " << qr;
            qDebug() << "Mode Operation | Ble : " << ble;
        }
        else
        {
            qDebug() << "No Mode Operation Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cModeOperationTable.SetInputInfo(card,pin,face,qr,ble);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::SetModeOperation(int id, cDBModeOfOperationTable &cModeOperationTable, QString &errMsg)
{

    try {

        if (m_db.isOpen() == false)
        {
            errMsg = "";
            qDebug() << "m_db.isOpen() == false" ;
            return DatabaseNotOpen;
        }

        if (id == 0)
        {
            return InvalidField;
        }

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM ModeOperation WHERE OperationID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "Mode of Operation: " << id << " count: " << count;
#endif
        int card;
        int pin;
        int face;
        int qr;
        int ble;

        cModeOperationTable.GetInputInfo(card,pin,face,qr,ble);

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO ModeOperation ("
                          "OperationID, "
                          "Card, "
                          "Pin, "
                          "Face, "
                          "QR, "
                          "BLE"
                          ") VALUES ("
                          ":card, "
                          ":pin, "
                          ":face, "
                          ":qr, "
                          ":ble"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE ModeOperation SET "
                          "Card = :card, "
                          "Pin = :pin, "
                          "Face = :face, "
                          "QR = :qr, "
                          "BLE = :ble "
                          "WHERE OperationID = :id");
        }

        query.bindValue(":id", id);
        query.bindValue(":card", card);
        query.bindValue(":pin", pin);
        query.bindValue(":face", face);
        query.bindValue(":qr", qr);
        query.bindValue(":ble", ble);

        if(!query.exec())
        {
            qDebug() << "Update Mode Operation Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

/*
    Custom Wiegand
*/
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetCustomWiegand(int id, cDBCustomWiegandTable &cCustomWiegand, QString &errMsg)
{
    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM WiegandFormat WHERE WiegandID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_cardbitLen = query.record().indexOf("CardBitLen");
        int rec_cardbitPosBM = query.record().indexOf("CardBitPosBM");
        int rec_evenParityPos = query.record().indexOf("EvenParityBitPos");
        int rec_evenParityLen = query.record().indexOf("EvenParityBitLen");
        int rec_evenParityPosBM = query.record().indexOf("EvenParityBM");
        int rec_oddParityPos = query.record().indexOf("OddParityBitPos");
        int rec_oddParityLen = query.record().indexOf("OddParityBitLen");
        int rec_oddParityPosBM = query.record().indexOf("OddParityBM");
        int rec_cardConversion = query.record().indexOf("CardConversionType");
        int rec_packetLen = query.record().indexOf("PacketLen");
        int rec_siteCodeLen = query.record().indexOf("SiteCodeLen");
        int rec_siteCodeBM = query.record().indexOf("SiteCodeBM");

        int cardbitLen;
        QString cardbitPosBM;
        int evenParityPos;
        int evenParityLen;
        QString evenParityPosBM;
        int oddParityPos;
        int oddParityLen;
        QString oddParityPosBM;
        int cardConversion;
        int packetLen;
        int siteCodeLen;
        QString siteCodeBM;

        if (query.next())
        {
            cardbitLen = query.value(rec_cardbitLen).toInt();
            cardbitPosBM = query.value(rec_cardbitPosBM).toString();
            evenParityPos = query.value(rec_evenParityPos).toInt();
            evenParityLen = query.value(rec_evenParityLen).toInt();
            evenParityPosBM = query.value(rec_evenParityPosBM).toString();
            oddParityPos = query.value(rec_oddParityPos).toInt();
            oddParityLen = query.value(rec_oddParityLen).toInt();
            oddParityPosBM = query.value(rec_oddParityPosBM).toString();
            cardConversion = query.value(rec_cardConversion).toInt();
            packetLen = query.value(rec_packetLen).toInt();
            siteCodeLen = query.value(rec_siteCodeLen).toInt();
            siteCodeBM = query.value(rec_siteCodeBM).toString();

            qDebug() << "Wiegand | Card Bit Len : " << cardbitLen;
            qDebug() << "Wiegand | Card Bit Pos BM : " << cardbitPosBM;
            qDebug() << "Wiegand | Even Parity Pos : " << evenParityPos;
            qDebug() << "Wiegand | Even Parity Len : " << evenParityLen;
            qDebug() << "Wiegand | Even Parity Pos BM : " << evenParityPosBM;
            qDebug() << "Wiegand | Odd Parity Pos : " << oddParityPos;
            qDebug() << "Wiegand | Odd Parity Len : " << oddParityLen;
            qDebug() << "Wiegand | Odd Parity Pos BM : " << oddParityPosBM;
            qDebug() << "Wiegand | Card Conversion : " << cardConversion;
            qDebug() << "Wiegand | Packet Len : " << packetLen;
            qDebug() << "Wiegand | Site Code Len : " << siteCodeLen;
            qDebug() << "Wiegand | Site Code BM : " << siteCodeBM;
        }
        else
        {
            qDebug() << "No Wiegand Format Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cCustomWiegand.SetCustomWiegandInfo(cardbitLen,
                                                   cardbitPosBM,
                                                   evenParityPos,
                                                   evenParityLen,
                                                   evenParityPosBM,
                                                   oddParityPos,
                                                   oddParityLen,
                                                   oddParityPosBM,
                                                   cardConversion,
                                                   packetLen,
                                                   siteCodeLen,
                                                   siteCodeBM);
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::SetCustomWiegand(int id, cDBCustomWiegandTable &cCustomWiegand, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM WiegandFormat WHERE WiegandID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "Wiegand Format: " << id << " count: " << count;
#endif

        int cardbitLen;
        QString cardbitPosBM;
        int evenParityLen;
        int evenParityPos;
        QString evenParityPosBM;
        int oddParityLen;
        int oddParityPos;
        QString oddParityPosBM;
        int cardConversion;
        int packetLen;
        int siteCodeLen;
        QString siteCodeBM;;

        cCustomWiegand.GetCustomWiegandInfo(cardbitLen,
                                            cardbitPosBM,
                                            evenParityPos,
                                            evenParityLen,
                                            evenParityPosBM,
                                            oddParityPos,
                                            oddParityLen,
                                            oddParityPosBM,
                                            cardConversion,
                                            packetLen,
                                            siteCodeLen,
                                            siteCodeBM);

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO WiegandFormat ("
                          "WiegandID, "
                          "CardBitLen, "
                          "CardBitPosBM, "
                          "EvenParityBitPos, "
                          "EvenParityBitLen, "
                          "EvenParityBM, "
                          "OddParityBitPos, "
                          "OddParityBitLen, "
                          "OddParityBM, "
                          "CardConversionType, "
                          "PacketLen, "
                          "SiteCodeLen, "
                          "SiteCodeBM"
                          ") VALUES ("
                          ":cardbitLen, "
                          ":cardbitPosBM, "
                          ":evenParityPos, "
                          ":evenParityLen, "
                          ":evenParityPosBM, "
                          ":oddParityPos, "
                          ":oddParityLen, "
                          ":oddParityPosBM, "
                          ":cardConversion, "
                          ":packetLen, "
                          ":siteCodeLen, "
                          ":siteCodeBM"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE WiegandFormat SET "
                          "CardBitLen = :cardbitLen, "
                          "CardBitPosBM = :cardbitPosBM, "
                          "EvenParityBitPos = :evenParityLen, "
                          "EvenParityBitLen = :evenParityPos, "
                          "EvenParityBM = :evenParityPosBM"
                          "OddParityBitPos = :oddParityLen, "
                          "OddParityBitLen = :oddParityPos, "
                          "OddParityBM = :oddParityPosBM, "
                          "CardConversionType = :cardConversion, "
                          "PacketLen = :packetLen"
                          "SiteCodeLen = :siteCodeLen, "
                          "SiteCodeBM = :siteCodeBM "
                          "WHERE WiegandID = :id");
        }

        query.bindValue(":cardbitLen", cardbitLen);
        query.bindValue(":cardbitPosBM", cardbitPosBM);
        query.bindValue(":evenParityLen", evenParityLen);
        query.bindValue(":evenParityPos", evenParityPos);
        query.bindValue(":evenParityPosBM", evenParityPosBM);
        query.bindValue(":oddParityLen", oddParityLen);
        query.bindValue(":oddParityPos", oddParityPos);
        query.bindValue(":oddParityPosBM", oddParityPosBM);
        query.bindValue(":cardConversion", cardConversion);
        query.bindValue(":packetLen", packetLen);
        query.bindValue(":siteCodeLen", siteCodeLen);
        query.bindValue(":siteCodeBM", siteCodeBM);

        if(!query.exec())
        {
            qDebug() << "Update Wiegand Format Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return 1;
}

/*
    Door Table
*/
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetDoorInfo(int id, cDBDoorTable &cDoorInfo, QString &errMsg)
{
    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM Door WHERE DoorID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_name = query.record().indexOf("DoorName");
        int rec_outputId = query.record().indexOf("OutputID");
        int rec_enTimezone = query.record().indexOf("DoorEnTimezone");
        int rec_securityLevel = query.record().indexOf("SecurityLevel");
        int rec_doorSensorInputId = query.record().indexOf("DoorSensorInputID");
        int rec_doorSensorEnTimezone = query.record().indexOf("DoorSensorEnTimezone");
        int rec_pushButtonInputId = query.record().indexOf("PushButtonInputID");

        int rec_pushButtonEnTimzeone = query.record().indexOf("PushButtonEnTimezone");
        int rec_inhibitTimezone = query.record().indexOf("InhibitTimezone");
        int rec_SecurityOffTimezone = query.record().indexOf("SecurityOffTimezone");

        QString name;
        int outputId;
        int enTimezone;
        int securityLevel;
        int doorSensorInputId;
        int doorSensorEnTimezone;
        int pushButtonInputId;
        int pushButtonEnTimzeone;
        int inhibitTimezone;
        int securityOffTimezone;

        if (query.next())
        {

            name = query.value(rec_name).toString();
            outputId = query.value(rec_outputId).toInt();
            enTimezone = query.value(rec_enTimezone).toInt();
            securityLevel = query.value(rec_securityLevel).toInt();
            doorSensorInputId = query.value(rec_doorSensorInputId).toInt();
            doorSensorEnTimezone = query.value(rec_doorSensorEnTimezone).toInt();
            pushButtonInputId = query.value(rec_pushButtonInputId).toInt();
            pushButtonEnTimzeone = query.value(rec_pushButtonEnTimzeone).toInt();
            inhibitTimezone = query.value(rec_inhibitTimezone).toInt();
            securityOffTimezone = query.value(rec_SecurityOffTimezone).toInt();

            qDebug() << "Door | Name : " << name;
            qDebug() << "Door | Output ID : " << outputId;
            qDebug() << "Door | En Timezone : " << enTimezone;
            qDebug() << "Door | Security Level : " << securityLevel;
            qDebug() << "Door | Door Sensor Input ID : " << doorSensorInputId;
            qDebug() << "Door | Door Sensor En TZ : " << doorSensorEnTimezone;
            qDebug() << "Door | Push Button Input ID : " << pushButtonInputId;
            qDebug() << "Door | Push Button En TZ: " << pushButtonEnTimzeone;
            qDebug() << "Door | Inhibit TZ : " << inhibitTimezone;
            qDebug() << "Door | Security Off TZ : " << securityOffTimezone;
        }
        else
        {
            qDebug() << "No Door Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cDoorInfo.SetDoorInfo(name,
                              outputId,
                              enTimezone,
                              securityLevel,
                              doorSensorInputId,
                              doorSensorEnTimezone,
                              pushButtonInputId,
                              pushButtonEnTimzeone,
                              inhibitTimezone,
                              securityOffTimezone);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::SetDoorInfo(int id, cDBDoorTable &cDoorInfo, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0 || id > MAX_DOOR)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM Door WHERE DoorID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "Door: " << id << " count: " << count;
#endif

        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO Door ("
                          "DoorID, "
                          "DoorName, "
                          "OutputID, "
                          "DoorEnTimezone, "
                          "SecurityLevel, "
                          "DoorSensorInputId, "
                          "DoorSensorEnTimezone, "
                          "PushButtonInputId, "
                          "PushButtonEnTimezone, "
                          "InhibitTimezone, "
                          "SecurityOffTimezone "
                          ") VALUES ("
                          ":DoorID, "
                          ":DoorName, "
                          ":OutputID, "
                          ":DoorEnTimezone, "
                          ":SecurityLevel, "
                          ":DoorSensorInputId, "
                          ":DoorSensorEnTimezone, "
                          ":PushButtonInputId, "
                          ":PushButtonEnTimezone, "
                          ":InhibitTimezone, "
                          ":SecurityOffTimezone "
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE Door SET "
                          "DoorName = :DoorName, "
                          "OutputID = :OutputID, "
                          "DoorEnTimezone = :DoorEnTimezone, "
                          "SecurityLevel = :SecurityLevel, "
                          "DoorSensorInputId = :DoorSensorInputId, "
                          "DoorSensorEnTimezone = :DoorSensorEnTimezone, "
                          "PushButtonInputId = :PushButtonInputId, "
                          "PushButtonEnTimezone = :PushButtonEnTimezone, "
                          "InhibitTimezone = :InhibitTimezone, "
                          "SecurityOffTimezone = :SecurityOffTimezone "
                          "WHERE DoorID = :DoorID");
        }

        QString name;
        int outputId;
        int enTimezone;
        int securityLevel;
        int doorSensorInputId;
        int doorSensorEnTimezone;
        int pushButtonInputId;
        int pushButtonEnTimzeone;
        int inhibitTimezone;
        int securityOffTimezone;

        cDoorInfo.GetDoorInfo(
                    name, outputId, enTimezone, securityLevel, doorSensorInputId,
                    doorSensorEnTimezone, pushButtonInputId, pushButtonEnTimzeone,
                    inhibitTimezone, securityOffTimezone);

        query.bindValue(":DoorID", id);
        query.bindValue(":DoorName", name);
        query.bindValue(":OutputID", outputId);
        query.bindValue(":DoorEnTimezone", enTimezone);
        query.bindValue(":SecurityLevel", securityLevel);
        query.bindValue(":DoorSensorInputId", doorSensorInputId);
        query.bindValue(":DoorSensorEnTimezone", doorSensorEnTimezone);
        query.bindValue(":PushButtonInputId", pushButtonInputId);
        query.bindValue(":PushButtonEnTimezone", pushButtonEnTimzeone);
        query.bindValue(":InhibitTimezone", inhibitTimezone);
        query.bindValue(":SecurityOffTimezone", securityOffTimezone);

        if(!query.exec())
        {
            qDebug() << "Update Table Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

/*
    Reader Table
*/
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetReaderInfo(int id, cDBReaderTable &cReaderInfo, QString &errMsg)
{
    try {
        // TODO: To implement
        QSqlQuery query(m_db);
        query.prepare("SELECT * FROM Reader WHERE ReaderID = (:id)");
        query.bindValue(":id",id);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_name = query.record().indexOf("ReaderName");
        int rec_type = query.record().indexOf("ReaderType");
        int rec_enTimezone = query.record().indexOf("ReaderEnTimezone");
        int rec_direction = query.record().indexOf("ReaderDirection");
        int rec_tiedDoorId = query.record().indexOf("TiedToDoorID");
        int rec_tamperInputEnTimezone = query.record().indexOf("TamperInputEnTZ");
        int rec_tamperInputId = query.record().indexOf("TamperInputID");
        int rec_LEDOutput = query.record().indexOf("LEDOutput");
        int rec_BuzzerOutput = query.record().indexOf("BuzzerOutput");
        int rec_StandWiegandEnBM = query.record().indexOf("StandardWiegandEnBM");
        int rec_custWieId1 = query.record().indexOf("CustomWieID1");
        int rec_custWieId2 = query.record().indexOf("CustomWieID2");
        int rec_custWieId3 = query.record().indexOf("CustomWieID3");
        int rec_custWieId4 = query.record().indexOf("CustomWieID4");
        int rec_custWieId5 = query.record().indexOf("CustomWieID5");
        int rec_custWieId6 = query.record().indexOf("CustomWieID6");
        int rec_custWieId7 = query.record().indexOf("CustomWieID7");
        int rec_custWieId8 = query.record().indexOf("CustomWieID8");
        int rec_custWieId9 = query.record().indexOf("CustomWieID9");
        int rec_custWieId10 = query.record().indexOf("CustomWieID10");

        QString name;
        QString type;
        int enTimezone;
        int direction;
        int tiedDoorId;
        int tamperInputEnTimezone;
        int tamperInputId;
        int LEDOutput;
        int BuzzerOutput;
        QString StandWiegandEnBM;
        int custWieId1;
        int custWieId2;
        int custWieId3;
        int custWieId4;
        int custWieId5;
        int custWieId6;
        int custWieId7;
        int custWieId8;
        int custWieId9;
        int custWieId10;

        if (query.next())
        {
            name = query.value(rec_name).toString();
            type = query.value(rec_type).toString();
            enTimezone = query.value(rec_enTimezone).toInt();
            direction = query.value(rec_direction).toInt();
            tiedDoorId = query.value(rec_tiedDoorId).toInt();
            tamperInputEnTimezone = query.value(rec_tamperInputEnTimezone).toInt();
            tamperInputId = query.value(rec_tamperInputId).toInt();
            LEDOutput = query.value(rec_LEDOutput).toInt();
            BuzzerOutput = query.value(rec_BuzzerOutput).toInt();
            StandWiegandEnBM = query.value(rec_StandWiegandEnBM).toString();
            custWieId1 = query.value(rec_custWieId1).toInt();
            custWieId2 = query.value(rec_custWieId2).toInt();
            custWieId3 = query.value(rec_custWieId3).toInt();
            custWieId4 = query.value(rec_custWieId4).toInt();
            custWieId5 = query.value(rec_custWieId5).toInt();
            custWieId6 = query.value(rec_custWieId6).toInt();
            custWieId7 = query.value(rec_custWieId7).toInt();
            custWieId8 = query.value(rec_custWieId8).toInt();
            custWieId9 = query.value(rec_custWieId9).toInt();
            custWieId10 = query.value(rec_custWieId10).toInt();

            qDebug() << "Reader | Name : " << name;
            qDebug() << "Reader | Type : " << type;
            qDebug() << "Reader | En Timezone : " << enTimezone;
            qDebug() << "Reader | Direction : " << direction;
            qDebug() << "Reader | Tied Door ID : " << tiedDoorId;
            qDebug() << "Reader | Tamper Input En TZ : " << tamperInputEnTimezone;
            qDebug() << "Reader | Tamper Input ID : " << tamperInputId;
            qDebug() << "Reader | LED Output: " << LEDOutput;
            qDebug() << "Reader | Buzzer Output : " << BuzzerOutput;
            qDebug() << "Reader | Std Wiegand En BM: " << StandWiegandEnBM;
            qDebug() << "Reader | Custom Wiegand 1 : " << custWieId1;
            qDebug() << "Reader | Custom Wiegand 2 : " << custWieId2;
            qDebug() << "Reader | Custom Wiegand 3 : " << custWieId3;
            qDebug() << "Reader | Custom Wiegand 4 : " << custWieId4;
            qDebug() << "Reader | Custom Wiegand 5 : " << custWieId5;
            qDebug() << "Reader | Custom Wiegand 6 : " << custWieId6;
            qDebug() << "Reader | Custom Wiegand 7 : " << custWieId7;
            qDebug() << "Reader | Custom Wiegand 8 : " << custWieId8;
            qDebug() << "Reader | Custom Wiegand 9 : " << custWieId9;
            qDebug() << "Reader | Custom Wiegand 10 : " << custWieId10;
        }
        else
        {
            qDebug() << "No Reader Found: \n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        cReaderInfo.SetReaderInfo(name,
                                  type,
                                  enTimezone,
                                  direction,
                                  tiedDoorId,
                                  tamperInputEnTimezone,
                                  tamperInputId,
                                  LEDOutput,
                                  BuzzerOutput,
                                  StandWiegandEnBM,
                                  custWieId1,
                                  custWieId2,
                                  custWieId3,
                                  custWieId4,
                                  custWieId5,
                                  custWieId6,
                                  custWieId7,
                                  custWieId8,
                                  custWieId9,
                                  custWieId10
                                  );

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }

    return -1;
}

int cDBHandler::SetReaderInfo(int id, cDBReaderTable &cReaderInfo, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0 || id > MAX_READER)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);
        query.prepare("SELECT COUNT(*) FROM Reader WHERE ReaderID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "Reader: " << id << " count: " << count;
#endif

        QString     name;
        QString     type;
        int         enTimezone;
        int         direction;
        int         tiedDoorId;
        int         tamperInputEnTimezone;
        int         tamperInputId;
        int         LEDOutput;
        int         BuzzerOutput;
        QString     StandWiegandEnBM;
        int         custWieId1;
        int         custWieId2;
        int         custWieId3;
        int         custWieId4;
        int         custWieId5;
        int         custWieId6;
        int         custWieId7;
        int         custWieId8;
        int         custWieId9;
        int         custWieId10;

        cReaderInfo.GetReaderInfo(
                    name,
                    type,
                    enTimezone,
                    direction,
                    tiedDoorId,
                    tamperInputEnTimezone,
                    tamperInputId,
                    LEDOutput,
                    BuzzerOutput,
                    StandWiegandEnBM,
                    custWieId1,
                    custWieId2,
                    custWieId3,
                    custWieId4,
                    custWieId5,
                    custWieId6,
                    custWieId7,
                    custWieId8,
                    custWieId9,
                    custWieId10
                    );


        if (count == 0)
        {
            /* Perform insert */
            query.prepare("INSERT INTO Reader ("
                          "ReaderID, "
                          "ReaderName, "
                          "ReaderType, "
                          "ReaderEnTimezone, "
                          "ReaderDirection, "
                          "TiedToDoorID, "
                          "TamperInputEnTZ, "
                          "TamperInputID, "
                          "LEDOutput, "
                          "BuzzerOutput, "
                          "StandardWiegandEnBM, "
                          "CustomWieID1, "
                          "CustomWieID2, "
                          "CustomWieID3, "
                          "CustomWieID4, "
                          "CustomWieID5, "
                          "CustomWieID6, "
                          "CustomWieID7, "
                          "CustomWieID8, "
                          "CustomWieID9, "
                          "CustomWieID10 "
                          ") VALUES ("
                          ":ReaderID, "
                          ":ReaderName, "
                          ":ReaderType, "
                          ":ReaderEnTimezone, "
                          ":ReaderDirection, "
                          ":TiedToDoorID, "
                          ":TamperInputEnTZ, "
                          ":TamperInputID, "
                          ":LEDOutput, "
                          ":BuzzerOutput, "
                          ":StandardWiegandEnBM, "
                          ":CustomWieID1, "
                          ":CustomWieID2, "
                          ":CustomWieID3, "
                          ":CustomWieID4, "
                          ":CustomWieID5, "
                          ":CustomWieID6, "
                          ":CustomWieID7, "
                          ":CustomWieID8, "
                          ":CustomWieID9, "
                          ":CustomWieID10"
                          ")");
        }
        else
        {
            /* perform update */
            query.prepare("UPDATE Reader SET "
                          "ReaderName = :ReaderName, "
                          "ReaderType = :ReaderType, "
                          "ReaderEnTimezone = :ReaderEnTimezone, "
                          "ReaderDirection = :ReaderDirection, "
                          "TiedToDoorID = :TiedToDoorID, "
                          "TamperInputEnTZ = :TamperInputEnTZ, "
                          "TamperInputID = :TamperInputID, "
                          "LEDOutput = :LEDOutput, "
                          "BuzzerOutput = :BuzzerOutput, "
                          "StandardWiegandEnBM = :StandardWiegandEnBM, "
                          "CustomWieID1 = :CustomWieID1, "
                          "CustomWieID2 = :CustomWieID2, "
                          "CustomWieID3 = :CustomWieID3, "
                          "CustomWieID4 = :CustomWieID4, "
                          "CustomWieID5 = :CustomWieID5, "
                          "CustomWieID6 = :CustomWieID6, "
                          "CustomWieID7 = :CustomWieID7, "
                          "CustomWieID8 = :CustomWieID8, "
                          "CustomWieID9 = :CustomWieID9, "
                          "CustomWieID10 = :CustomWieID10 "
                          "WHERE ReaderID = :ReaderID");
        }

        query.bindValue(":ReaderID", id);
        query.bindValue(":ReaderName", name);
        query.bindValue(":ReaderType", type);
        query.bindValue(":ReaderEnTimezone", enTimezone);
        query.bindValue(":ReaderDirection", direction);
        query.bindValue(":TiedToDoorID", tiedDoorId);
        query.bindValue(":TamperInputEnTZ", tamperInputEnTimezone);
        query.bindValue(":TamperInputID", tamperInputId);
        query.bindValue(":LEDOutput", LEDOutput);
        query.bindValue(":BuzzerOutput", BuzzerOutput);
        query.bindValue(":StandardWiegandEnBM", StandWiegandEnBM);
        query.bindValue(":CustomWieID1", custWieId1);
        query.bindValue(":CustomWieID2", custWieId2);
        query.bindValue(":CustomWieID3", custWieId3);
        query.bindValue(":CustomWieID4", custWieId4);
        query.bindValue(":CustomWieID5", custWieId5);
        query.bindValue(":CustomWieID6", custWieId6);
        query.bindValue(":CustomWieID7", custWieId7);
        query.bindValue(":CustomWieID8", custWieId8);
        query.bindValue(":CustomWieID9", custWieId9);
        query.bindValue(":CustomWieID10", custWieId10);

        if(!query.exec())
        {
            qDebug() << "Update Reader Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

/*
    User Table
*/
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetUserIDFromID1(const QString id1, int &iUserId, QString &errMsg)
{
    try {
        QSqlQuery query(m_db);
        query.prepare("SELECT UserID FROM User WHERE ID1 = (:id1)");
        query.bindValue(":id1", id1);
        query.exec();

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_userId = query.record().indexOf("UserID");

        if (query.next())
        {
            iUserId = query.value(rec_userId).toInt();
        }
        else
        {
            qDebug() << "No User Found: \n" << getLastExecutedQuery(query);
            return UserNotExist;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

int cDBHandler::SetUserInfo(int id, cDBUserTable &cUserInfo, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        if (id == 0 || id > MAX_USER)
            return InvalidField;

        int count = 0;
        QSqlQuery query(m_db);

        query.prepare("SELECT COUNT(*) FROM User WHERE UserID = :id");
        query.bindValue(":id", id);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "User: " << id << " count: " << count;
#endif
        QString sqlStatement;

        int     overrideCard;
        QString userName;
        QString id1;
        QString id2;
        QString id3;
        QString userPin;
        int userType;
        QString department;
        int modeOperation;
        int userEnTimezone;
        QString validStart;
        QString validEnd;
        int door1Id;
        int door1AccessTimezone;
        int door2Id;
        int door2AccessTimezone;
        int door3Id;
        int door3AccessTimezone;
        int door4Id;
        int door4AccessTimezone;
        QByteArray leftFPTemplate;
        QByteArray rightFPTemplate;
        QByteArray photo;

        cUserInfo.GetUserInfo(
                    overrideCard       ,
                    userName           ,
                    id1                ,
                    id2                ,
                    id3                ,
                    userPin            ,
                    userType           ,
                    department         ,
                    modeOperation      ,
                    userEnTimezone     ,
                    validStart         ,
                    validEnd           ,
                    door1Id            ,
                    door1AccessTimezone,
                    door2Id            ,
                    door2AccessTimezone,
                    door3Id            ,
                    door3AccessTimezone,
                    door4Id            ,
                    door4AccessTimezone,
                    leftFPTemplate     ,
                    rightFPTemplate    ,
                    photo
                    );

        /* Check is there any FP template, and photo or not */
        if (count == 0)
        {
            /* To check number of users */
            sqlStatement = "SELECT COUNT(*) FROM User";
            query.prepare(sqlStatement);

            if(!query.exec())
            {
                errMsg = query.lastError().text();
                return InternalDatabaseFailure;
            }

            if (query.next())
            {
                count = query.value(0).toInt();
            }
#if (IS_DEBUG == true)
            qDebug() << "Number of Row: " << count;
#endif

            if (count >= MAX_CARD_NUM)
                return CardNoCapacityFull;

            /* Perform insert */
            sqlStatement = "INSERT INTO User ("
                           "UserID, "
                           "OverrideCard, "
                           "Name, "
                           "ID1, "
                           "ID2, "
                           "ID3, "
                           "UserPin, "
                           "UserType, "
                           "Department, "
                           "ModeOperationID, "
                           "UserEnTimezone, "
                           "ValidStart, "
                           "ValidEnd, "
                           "Door1ID, "
                           "Door1AccessTimezone, "
                           "Door2ID, "
                           "Door2AccessTimezone, "
                           "Door3ID, "
                           "Door3AccessTimezone, "
                           "Door4ID, "
                           "Door4AccessTimezone ";

            if (leftFPTemplate.size() > 0)
                sqlStatement += ", LeftFPTemplate ";

            if (rightFPTemplate.size() > 0)
                sqlStatement += ", RightFPTemplate ";

            if (photo.size() > 0)
                sqlStatement += ", Photo ";

            sqlStatement += ") VALUES ("
                            ":UserID, "
                            ":OverrideCard, "
                            ":Name, "
                            ":ID1, "
                            ":ID2, "
                            ":ID3, "
                            ":UserPin, "
                            ":UserType, "
                            ":Department, "
                            ":ModeOperationID, "
                            ":UserEnTimezone, "
                            ":ValidStart, "
                            ":ValidEnd, "
                            ":Door1ID, "
                            ":Door1AccessTimezone, "
                            ":Door2ID, "
                            ":Door2AccessTimezone, "
                            ":Door3ID, "
                            ":Door3AccessTimezone, "
                            ":Door4ID, "
                            ":Door4AccessTimezone ";
            if (leftFPTemplate.size() > 0)
                sqlStatement += ", :LeftFPTemplate ";

            if (rightFPTemplate.size() > 0)
                sqlStatement += ", :RightFPTemplate ";

            if (photo.size() > 0)
                sqlStatement += ", :Photo ";

            sqlStatement += ")";
        }
        else
        {
            /* perform update */
            sqlStatement = "UPDATE User SET "
                           "OverrideCard = :OverrideCard, "
                           "Name = :Name, "
                           "ID1 = :ID1, "
                           "ID2 = :ID2, "
                           "ID3 = :ID3, "
                           "UserPin = :UserPin, "
                           "UserType = :UserType, "
                           "Department = :Department,  "
                           "ModeOperationID = :ModeOperationID, "
                           "UserEnTimezone = :UserEnTimezone, "
                           "ValidStart = :ValidStart, "
                           "ValidEnd = :ValidEnd, "
                           "Door1ID = :Door1ID, "
                           "Door1AccessTimezone = :Door1AccessTimezone, "
                           "Door2ID = :Door2ID, "
                           "Door2AccessTimezone = :Door2AccessTimezone, "
                           "Door3ID = :Door3ID, "
                           "Door3AccessTimezone = :Door3AccessTimezone, "
                           "Door4ID = :Door4ID, "
                           "Door4AccessTimezone = :Door4AccessTimezone ";
            if (leftFPTemplate.size() > 0)
                sqlStatement += ", LeftFPTemplate = :LeftFPTemplate ";

            if (rightFPTemplate.size() > 0)
                sqlStatement += ", RightFPTemplate = :RightFPTemplate ";

            if (photo.size() > 0)
                sqlStatement += ", Photo = :Photo ";

            sqlStatement += "WHERE UserID = :UserID";
        }

        query.prepare(sqlStatement);

        query.bindValue(":UserID", id);
        query.bindValue(":OverrideCard", overrideCard);
        query.bindValue(":Name", userName);
        query.bindValue(":ID1", id1);
        query.bindValue(":ID2", id2);
        query.bindValue(":ID3", id3);
        query.bindValue(":UserPin", userPin);
        query.bindValue(":UserType", userType);
        query.bindValue(":Department", department);
        query.bindValue(":ModeOperationID", modeOperation);
        query.bindValue(":UserEnTimezone", userEnTimezone);
        query.bindValue(":ValidStart", validStart);
        query.bindValue(":ValidEnd", validEnd);
        query.bindValue(":Door1ID", door1Id);
        query.bindValue(":Door1AccessTimezone", door1AccessTimezone);
        query.bindValue(":Door2ID", door2Id);
        query.bindValue(":Door2AccessTimezone", door2AccessTimezone);
        query.bindValue(":Door3ID", door3Id);
        query.bindValue(":Door3AccessTimezone", door3AccessTimezone);
        query.bindValue(":Door4ID", door4Id);
        query.bindValue(":Door4AccessTimezone", door4AccessTimezone);

        if (leftFPTemplate.size() > 0)
            query.bindValue(":LeftFPTemplate", leftFPTemplate);

        if (rightFPTemplate.size() > 0)
            query.bindValue(":RightFPTemplate", rightFPTemplate);

        if (photo.size() > 0)
            query.bindValue(":Photo", photo);

        if(!query.exec())
        {
            qDebug() << "Update user Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

/*
    Card Table
*/
/*
    return -1 is true
    > 0 is with error string
*/
int cDBHandler::GetCardInfo(int id, cDBCardTable &cCardInfo, QString &errMsg)
{
    try {
        // TODO: To implement
    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

int cDBHandler::CommonUpdateInsertCard(const QString cardNum, const QString userID1, cDBCardTable &cCardInfo, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        int count = 0;
        QSqlQuery query(m_db);

        /* Get the userID from ID1 field */
        query.prepare("SELECT UserID FROM User WHERE ID1 = :userID1");
        query.bindValue(":userID1", userID1);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        int rec_userId = query.record().indexOf("UserID");

        int userId;
        QString desc;

        if (query.next())
        {
            userId = query.value(rec_userId).toInt();
            cCardInfo.SetUserID(userId);

            qDebug() << "\nUserID Found : " << userId;
        }
        else
        {
            qDebug() << "\nNo User Found: \n" << getLastExecutedQuery(query);
            return UserNotExist;
        }

        query.prepare("SELECT COUNT(*) FROM Card WHERE CardNumber = :cardNum");
        query.bindValue(":cardNum", cardNum);

        if(!query.exec())
        {
            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

        if (query.next())
        {
            count = query.value(0).toInt();
        }

#if (IS_DEBUG == true)
        qDebug() << "Card: " << cardNum << " count: " << count;
#endif
        QString sqlStatement;
        QString cardNumber;
        int cardType;
        int modeOperation;
        int door1Id;
        int door1AccessTimezone;
        int door2Id;
        int door2AccessTimezone;
        int door3Id;
        int door3AccessTimezone;
        int door4Id;
        int door4AccessTimezone;
        int cardEnTimezone;
        QString validStart;
        QString validEnd;
        QString cardPin;
        int cardWithPinTimezone;
        int numBuddy;
        QString buddyCard1;
        QString buddyCard2;
        QString buddyCard3;
        QString buddyCard4;

        cCardInfo.GetCardInfo(
                    userId,
                    cardNumber,
                    cardType,
                    modeOperation,
                    door1Id,
                    door1AccessTimezone,
                    door2Id,
                    door2AccessTimezone,
                    door3Id,
                    door3AccessTimezone,
                    door4Id,
                    door4AccessTimezone,
                    cardEnTimezone,
                    validStart,
                    validEnd,
                    cardPin,
                    cardWithPinTimezone,
                    numBuddy,
                    buddyCard1,
                    buddyCard2,
                    buddyCard3,
                    buddyCard4
                    );

        /* Check is there any FP template, and photo or not */
        if (count == 0)
        {
            /* Check count whether is it within the capacity  */
            query.prepare("SELECT COUNT(*) FROM Card");

            if(!query.exec())
            {
                errMsg = query.lastError().text();
                return InternalDatabaseFailure;
            }

            if (query.next())
            {
                count = query.value(0).toInt();
            }
#if (IS_DEBUG == 1)
            qDebug() << "Number of Row: " << count;
#endif

            if (count > MAX_CARD_NUM)
                return CardNoCapacityFull;

            /* Perform insert */
            sqlStatement = "INSERT INTO Card ("
                           "UserID, "
                           "CardNumber, "
                           "CardType, "
                           "ModeOperation, "
                           "Door1ID, "
                           "Door1AccessTimezone, "
                           "Door2ID, "
                           "Door2AccessTimezone, "
                           "Door3ID, "
                           "Door3AccessTimezone, "
                           "Door4ID, "
                           "Door4AccessTimezone, "
                           "CardEnTimezone, "
                           "ValidStart, "
                           "ValidEnd, "
                           "CardPin, "
                           "CardWithPinTimezone, "
                           "NumOfBuddy, "
                           "BuddyCardNo1, "
                           "BuddyCardNo2, "
                           "BuddyCardNo3, "
                           "BuddyCardNo4"
                           ") VALUES ("
                           ":UserID, "
                           ":CardNumber, "
                           ":CardType, "
                           ":ModeOperation, "
                           ":Door1ID, "
                           ":Door1AccessTimezone, "
                           ":Door2ID, "
                           ":Door2AccessTimezone, "
                           ":Door3ID, "
                           ":Door3AccessTimezone, "
                           ":Door4ID, "
                           ":Door4AccessTimezone, "
                           ":CardEnTimezone, "
                           ":ValidStart, "
                           ":ValidEnd, "
                           ":CardPin, "
                           ":CardWithPinTimezone, "
                           ":NumOfBuddy, "
                           ":BuddyCardNo1, "
                           ":BuddyCardNo2, "
                           ":BuddyCardNo3, "
                           ":BuddyCardNo4)";
        }
        else
        {
            /* perform update */
            sqlStatement = "UPDATE Card SET "
                           "UserID = :UserID, "
                           "CardType = :CardType, "
                           "ModeOperation = :ModeOperation, "
                           "Door1ID = :Door1ID, "
                           "Door1AccessTimezone = :Door1AccessTimezone, "
                           "Door2ID = :Door2ID, "
                           "Door2AccessTimezone = :Door2AccessTimezone, "
                           "Door3ID = :Door3ID, "
                           "Door3AccessTimezone = :Door3AccessTimezone, "
                           "Door4ID = :Door4ID, "
                           "Door4AccessTimezone = :Door4AccessTimezone, "
                           "CardEnTimezone = :CardEnTimezone, "
                           "ValidStart = :ValidStart, "
                           "ValidEnd = :ValidEnd, "
                           "CardPin = :CardPin, "
                           "CardWithPinTimezone = :CardWithPinTimezone, "
                           "NumOfBuddy = :NumOfBuddy, "
                           "BuddyCardNo1 = :BuddyCardNo1, "
                           "BuddyCardNo2 = :BuddyCardNo2, "
                           "BuddyCardNo3 = :BuddyCardNo3, "
                           "BuddyCardNo4 = :BuddyCardNo4 "
                           "WHERE CardNumber = :CardNumber";
        }

        query.prepare(sqlStatement);

        query.bindValue(":CardNumber", cardNum);
        query.bindValue(":UserID", userId);
        query.bindValue(":CardType", cardType);
        query.bindValue(":ModeOperation", modeOperation);
        query.bindValue(":Door1ID", door1Id);
        query.bindValue(":Door1AccessTimezone", door2AccessTimezone);
        query.bindValue(":Door2ID", door2Id);
        query.bindValue(":Door2AccessTimezone", door3AccessTimezone);
        query.bindValue(":Door3ID", door3Id);
        query.bindValue(":Door3AccessTimezone", door3AccessTimezone);
        query.bindValue(":Door4ID", door4Id);
        query.bindValue(":Door4AccessTimezone", door4AccessTimezone);
        query.bindValue(":CardEnTimezone", cardEnTimezone);
        query.bindValue(":ValidStart", validStart);
        query.bindValue(":ValidEnd", validEnd);
        query.bindValue(":CardPin", cardPin);
        query.bindValue(":CardWithPinTimezone", cardWithPinTimezone);
        query.bindValue(":NumOfBuddy", numBuddy);
        query.bindValue(":BuddyCardNo1", buddyCard1);
        query.bindValue(":BuddyCardNo2", buddyCard1);
        query.bindValue(":BuddyCardNo3", buddyCard1);
        query.bindValue(":BuddyCardNo4", buddyCard1);

        if(!query.exec())
        {
            qDebug() << "Update Card Failed:"
                     << query.lastError().text();
            qDebug() << "\r\n" << getLastExecutedQuery(query);

            errMsg = query.lastError().text();
            return InternalDatabaseFailure;
        }

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}

int cDBHandler::SetCardInfo(QString cardNum, QString userId1, cDBCardTable &cCardInfo, QString &errMsg)
{
    try {
        if (m_db.isOpen() == false)
        {
            errMsg = "";
            return DatabaseNotOpen;
        }

        CommonUpdateInsertCard(cardNum, userId1, cCardInfo, errMsg);

    } catch (Exception e) {
        errMsg = e.what();
        return InternalDatabaseFailure;
    }
    return -1;
}
