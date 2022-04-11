#include <memory>

#include "cdbhandler.h"
#include "cdbtableinfo.h"
#include "CommonField.h"

#include <QDebug>
#include <QString>
#include <QCoreApplication>
#include <QRandomGenerator>

using namespace  std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
     //start

    cDBHandler(HCB_ACS_DB_PATH, "AutoGenProcess");

    //cDBHandler(HCB_ACS_DB_PATH, "AutoGenProcess");

//    shared_ptr<cDBHandler> p_cDBHandler (new cDBHandler(HCB_ACS_DB_PATH, "AutoGenProcess"));
//    int i;

//    cDBTimesetTable m_cTimeset;
//    QString strStartime = "00:00";
//    QString strEndTime = "23:59";
//    QString strDbHandlerErrMsg;
//    int ret;
//    m_cTimeset.SetTimeset(strStartime, strEndTime);

//    /* Insert or Update Timeset Table */
//    for (i = 0; i < MAX_TIMESET; i++)
//    {
//        if ((ret = p_cDBHandler->UpdateTimeSet(i, m_cTimeset, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Timeset Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            break;
//        }
//    }
//    if (i == MAX_TIMESET)
//        qDebug() << "\r\nUpdate Default Timeset Success";

//    /* Insert or Update Timezone Table */
//    int iTimezone[MAX_TIMESET_IN_TIMEZONE] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
//    for (i = 0; i < MAX_TIMEZONE; i++)
//    {
//        if ((ret = p_cDBHandler->UpdateTimezone(i, iTimezone, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Timezone Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            break;
//        }
//    }
//    if (i == MAX_TIMEZONE)
//        qDebug() << "\r\nUpdate Default Timezone Success";
//    else
//        goto endProgram;

//    /* Insert or Update Input Table */
//    {
//        shared_ptr<cDBInputTable> m_pcDBInputTable(new cDBInputTable("Door Sensor", "Door Sensor", 1, 0, 0));
//        if((ret = p_cDBHandler->SetInputInfo(1, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 1 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }

//        if((ret = p_cDBHandler->SetInputInfo(3, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 3 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }

//        if((ret = p_cDBHandler->SetInputInfo(5, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 5 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }

//        if((ret = p_cDBHandler->SetInputInfo(7, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 7 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }


//        m_pcDBInputTable->SetInputInfo("Door Push Button", "Push Button", 1, 0, 0);
//        if((ret = p_cDBHandler->SetInputInfo(2, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 2 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }

//        if((ret = p_cDBHandler->SetInputInfo(4, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 4 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }

//        if((ret = p_cDBHandler->SetInputInfo(6, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 6 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }

//        if((ret = p_cDBHandler->SetInputInfo(8, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//        {
//            qDebug() << "\r\nUpdate Input 6 Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//            goto endProgram;
//        }

//        m_pcDBInputTable->SetInputInfo("General Purpose Input", "General Purpose", 1, 0, 0);
//        for (i = 9; i < MAX_USER_INPUT; i++)
//        {
//            if((ret = p_cDBHandler->SetInputInfo(i, *m_pcDBInputTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate Input "<< i << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }
//        qDebug() << "\r\nUpdate Input Success";
//    }

//    /* Insert or Update Output Table */
//    {
//        shared_ptr<cDBOutputTable> m_pcDBOutputTable(new cDBOutputTable("Door Relay", "Door Relay", 15, 30, 0, 0));
//        for (i = 0; i < MAX_RELAY; i++)
//        {
//            if((ret = p_cDBHandler->SetOutputInfo((i + 1), *m_pcDBOutputTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate Output " << (i + 1) << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }

//        m_pcDBOutputTable->SetOutputInfo("General Purpose Output", "General Purpose Output", 0, 0, 0, 0);
//        for (i = MAX_RELAY; i < (MAX_RELAY + MAX_USER_OUTPUT); i++)
//        {
//            if((ret = p_cDBHandler->SetOutputInfo((i + 1), *m_pcDBOutputTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate Output " << (i + 1) << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }

//        qDebug() << "\r\nUpdate Output Success";
//    }

//    /* Insert or Update Door Table */
//    {
//        shared_ptr<cDBDoorTable> m_pcDoorTable(new cDBDoorTable(QString("Door 1"), 1, 1, 0, 1, 1, 2, 1, 0, 0));
//        for (i = 0; i < MAX_DOOR; i++)
//        {
//            m_pcDoorTable->SetDoorInfo((QString("Door %1").arg(i + 1)), (i + 1), 1, 0, ((i * 2) + 1), 1, ((i * 2) + 2), 1, 0, 0);
//            if((ret = p_cDBHandler->SetDoorInfo((i + 1), *m_pcDoorTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate Door " << (i + 1) << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }

//        qDebug() << "\r\nUpdate Door Success";
//    }

//    /* Insert or Update Reader Table */
//    {
//        shared_ptr<cDBReaderTable> m_pcReaderTable(new cDBReaderTable(
//                    "Reader 1", "Standard Wiegand", 1, 1, 1, 0, 0, 5, 6, "003F", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//                    ));
//        for (i = 0; i < 4; i++)
//        {
//            m_pcReaderTable->SetReaderInfo((QString("Reader %1").arg(i + 1)),
//                                           "Standard Wiegand", 1, ((i % 2)?0:1), ((i / 2) + 1), 0, 0, ((i * 2) + 5), ((i * 2) + 6), "003F", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//            if((ret = p_cDBHandler->SetReaderInfo((i + 1), *m_pcReaderTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate Reader " << (i + 1) << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }

//        for (i = 4; i < MAX_READER; i++)
//        {
//            m_pcReaderTable->SetReaderInfo((QString("Reader %1").arg(i + 1)),
//                                           "OSDP", 1, ((i % 2)?0:1), ((i / 2) + 1), 0, 0, 0, 0, "003F", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
//            if((ret = p_cDBHandler->SetReaderInfo((i + 1), *m_pcReaderTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate Reader " << (i + 1) << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }

//        qDebug() << "\r\nUpdate Reader Success";
//    }

//#if 0
//    /* insert user table */
//    {
//        int iStaffId = 1;
//        QByteArray leftFPTemplate;
//        QByteArray rightFPTemplate;
//        QByteArray photo;

//        shared_ptr<cDBUserTable> m_pcUserTable(new cDBUserTable(
//                    QRandomGenerator::global()->bounded(0, 1),
//                    QString("Staff Name - %1").arg(iStaffId, 10, 16, QLatin1Char('0')),
//                    QString("ID-%1").arg(iStaffId, 10, 16, QLatin1Char('0')),
//                    "",
//                    "",
//                    "000000",
//                    QRandomGenerator::global()->bounded(1, 5),
//                    QString("Dept-%1").arg(QRandomGenerator::global()->bounded(0, 10)),
//                    1, 2,
//                    "01/01/2020 00:00",
//                    "01/01/2100 00:00",
//                    1, 2,
//                    2, 2,
//                    3, 2,
//                    4, 2,
//                    leftFPTemplate, rightFPTemplate, photo
//                    ));

//        for (i = 0; i < 50; i++, iStaffId++)
//        {
//            m_pcUserTable->SetUserInfo(
//                        QRandomGenerator::global()->bounded(0, 2),
//                        QString("Staff Name - %1").arg(iStaffId, 10, 16, QLatin1Char('0')),
//                        QString("ID-%1").arg(iStaffId, 10, 16, QLatin1Char('0')),
//                        "",
//                        "",
//                        "000000",
//                        QRandomGenerator::global()->bounded(1, 6),
//                        QString("Dept-%1").arg(QRandomGenerator::global()->bounded(0, 10)),
//                        1, 2,
//                        "01/01/2020 00:00",
//                        "01/01/2100 00:00",
//                        1, 2,
//                        2, 2,
//                        3, 2,
//                        4, 2,
//                        leftFPTemplate, rightFPTemplate, photo
//                        );

//            if((ret = p_cDBHandler->SetUserInfo((i + 1), *m_pcUserTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate User " << (i + 1) << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }

//        qDebug() << "\r\nUpdate User Success";
//    }

//    /* insert card table */
//    {
//        int iCardNum = 1;

//        shared_ptr<cDBCardTable> m_pcCardTable(new cDBCardTable(
//                                                   0, // retrieve from database in the db handler
//                                                   QString("%1").arg(iCardNum, 20, 16, QLatin1Char('0')),
//                                                   QRandomGenerator::global()->bounded(1, 9),
//                                                   1,
//                                                   1, 2,
//                                                   2, 2,
//                                                   3, 2,
//                                                   4, 2,
//                                                   2,
//                                                   "01/01/2020 00:00",
//                                                   "01/01/2100 00:00",
//                                                   "000000",
//                                                   1, 0,
//                                                   "",
//                                                   "",
//                                                   "",
//                                                   ""
//                                                   ));


//        for (i = 0; i < 50; i++, iCardNum++)
//        {
//            m_pcCardTable->SetCardInfo(
//                        0, // retrieve from database in the db handler
//                        QString("%1").arg(iCardNum, 20, 16, QLatin1Char('0')),
//                        QRandomGenerator::global()->bounded(1, 9),
//                        1,
//                        1, 2,
//                        2, 2,
//                        3, 2,
//                        4, 2,
//                        2,
//                        "01/01/2020 00:00",
//                        "01/01/2100 00:00",
//                        "000000",
//                        1, 0,
//                        "",
//                        "",
//                        "",
//                        ""
//                        );

//            if((ret = p_cDBHandler->SetCardInfo(
//                        QString("%1").arg(iCardNum, 10, 16, QLatin1Char('0')),
//                        QString("ID-%1").arg(iCardNum, 10, 16, QLatin1Char('0')), *m_pcCardTable, strDbHandlerErrMsg)) != -1)
//            {
//                qDebug() << "\r\nUpdate Card " << (i + 1) << " Failed: " << ret << " " << qStrErrCode[ret] << "\r\n" << strDbHandlerErrMsg;
//                goto endProgram;
//            }
//        }

//        qDebug() << "\r\nUpdate Card Success";

//    }
//#endif

//endProgram:
    return a.exec();
}
