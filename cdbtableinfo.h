#ifndef CDBTABLEINFO_H
#define CDBTABLEINFO_H

#include <QObject>
#include <QDateTime>
#include <QByteArray>

class cDBCardTable: QObject
{
    Q_OBJECT
private:
    int userId;
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

public:
    cDBCardTable();
    cDBCardTable(
        int userId,
        QString cardNumber,
        int cardType,
        int modeOperation,
        int door1Id,
        int door1AccessTimezone,
        int door2Id,
        int door2AccessTimezone,
        int door3Id,
        int door3AccessTimezone,
        int door4Id,
        int door4AccessTimezone,
        int cardEnTimezone,
        QString validStart,
        QString validEnd,
        QString cardPin,
        int cardWithPinTimezone,
        int numBuddy,
        QString buddyCard1,
        QString buddyCard2,
        QString buddyCard3,
        QString buddyCard4
    );

    int SetCardInfo(
            const int userId,
            const QString cardNumber,
            const int cardType,
            const int modeOperation,
            const int door1Id,
            const int door1AccessTimezone,
            const int door2Id,
            const int door2AccessTimezone,
            const int door3Id,
            const int door3AccessTimezone,
            const int door4Id,
            const int door4AccessTimezone,
            const int cardEnTimezone,
            const QString validStart,
            const QString validEnd,
            const QString cardPin,
            const int cardWithPinTimezone,
            const int numBuddy,
            const QString buddyCard1,
            const QString buddyCard2,
            const QString buddyCard3,
            const QString buddyCard4
            );
    int GetCardInfo(
            int &userId,
            QString &cardNumber,
            int &cardType,
            int &modeOperation,
            int &door1Id,
            int &door1AccessTimezone,
            int &door2Id,
            int &door2AccessTimezone,
            int &door3Id,
            int &door3AccessTimezone,
            int &door4Id,
            int &door4AccessTimezone,
            int &cardEnTimezone,
            QString &validStart,
            QString &validEnd,
            QString &cardPin,
            int &cardWithPinTimezone,
            int &numBuddy,
            QString &buddyCard1,
            QString &buddyCard2,
            QString &buddyCard3,
            QString &buddyCard4
            );

    int GetUserID(void);
    int SetUserID(int userId);
};


class cDBUserTable: QObject
{
    Q_OBJECT
private:
    int overrideCard;
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

public:
    cDBUserTable();
    cDBUserTable(
            int overrideCard,
            QString userName,
            QString id1,
            QString id2,
            QString id3,
            QString userPin,
            int userType,
            QString department,
            int modeOperation,
            int userEnTimezone,
            QString validStart,
            QString validEnd,
            int door1Id,
            int door1AccessTimezone,
            int door2Id,
            int door2AccessTimezone,
            int door3Id,
            int door3AccessTimezone,
            int door4Id,
            int door4AccessTimezone,
            QByteArray leftFPTemplate,
            QByteArray rightFPTemplate,
            QByteArray photo
            );

public:
    int SetUserInfo(
            const int overrideCard,
            const QString userName,
            const QString id1,
            const QString id2,
            const QString id3,
            const QString userPin,
            const int userType,
            const QString department,
            const int modeOperation,
            const int userEnTimezone,
            const QString validStart,
            const QString validEnd,
            const int door1Id,
            const int door1AccessTimezone,
            const int door2Id,
            const int door2AccessTimezone,
            const int door3Id,
            const int door3AccessTimezone,
            const int door4Id,
            const int door4AccessTimezone,
            const QByteArray leftFPTemplate,
            const QByteArray rightFPTemplate,
            const QByteArray photo
            );
    int GetUserInfo(
            int &overrideCard,
            QString &userName,
            QString &id1,
            QString &id2,
            QString &id3,
            QString &userPin,
            int &userType,
            QString &department,
            int &modeOperation,
            int &userEnTimezone,
            QString &validStart,
            QString &validEnd,
            int &door1Id,
            int &door1AccessTimezone,
            int &door2Id,
            int &door2AccessTimezone,
            int &door3Id,
            int &door3AccessTimezone,
            int &door4Id,
            int &door4AccessTimezone,
            QByteArray &leftFPTemplate,
            QByteArray &rightFPTemplate,
            QByteArray &photo
            );
};

class cDBReaderTable: QObject
{
    Q_OBJECT
private:
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

public:
    cDBReaderTable();
    cDBReaderTable(
            QString name,
            QString type,
            int enTimezone,
            int direction,
            int tiedDoorId,
            int tamperInputEnTimezone,
            int tamperInputId,
            int LEDOutput,
            int BuzzerOutput,
            QString StandWiegandEnBM,
            int custWieId1,
            int custWieId2,
            int custWieId3,
            int custWieId4,
            int custWieId5,
            int custWieId6,
            int custWieId7,
            int custWieId8,
            int custWieId9,
            int custWieId10
            );

    int SetReaderInfo(
            const QString name,
            const QString type,
            const int enTimezone,
            const int direction,
            const int tiedDoorId,
            const int tamperInputEnTimezone,
            const int tamperInputId,
            const int LEDOutput,
            const int BuzzerOutput,
            const QString StandWiegandEnBM,
            const int custWieId1,
            const int custWieId2,
            const int custWieId3,
            const int custWieId4,
            const int custWieId5,
            const int custWieId6,
            const int custWieId7,
            const int custWieId8,
            const int custWieId9,
            const int custWieId10
            );
    int GetReaderInfo(
            QString &name,
            QString &type,
            int &enTimezone,
            int &direction,
            int &tiedDoorId,
            int &tamperInputEnTimezone,
            int &tamperInputId,
            int &LEDOutput,
            int &BuzzerOutput,
            QString &StandWiegandEnBM,
            int &custWieId1,
            int &custWieId2,
            int &custWieId3,
            int &custWieId4,
            int &custWieId5,
            int &custWieId6,
            int &custWieId7,
            int &custWieId8,
            int &custWieId9,
            int &custWieId10
            );
};

class cDBCustomWiegandTable: QObject
{
    Q_OBJECT
private:
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
    QString siteCodeBM;

public:
    cDBCustomWiegandTable();
    cDBCustomWiegandTable(
            int cardbitLen,
            QString cardbitPosBM,
            int evenParityPos,
            int evenParityLen,
            QString evenParityPosBM,
            int oddParityPos,
            int oddParityLen,
            QString oddParityPosBM,
            int cardConversion,
            int packetLen,
            int siteCodeLen,
            QString siteCodeBM
            );

    int SetCustomWiegandInfo(
            const int cardbitLen,
            const QString cardbitPosBM,
            const int evenParityPos,
            const int evenParityLen,
            const QString evenParityPosBM,
            const int oddParityPos,
            const int oddParityLen,
            const QString oddParityPosBM,
            const int cardConversion,
            const int packetLen,
            const int siteCodeLen,
            const QString siteCodeBM
            );
    int GetCustomWiegandInfo(
            int &cardbitLen,
            QString &cardbitPosBM,
            int &evenParityLen,
            int &evenParityPos,
            QString &evenParityPosBM,
            int &oddParityLen,
            int &oddParityPos,
            QString &oddParityPosBM,
            int &cardConversion,
            int &packetLen,
            int &siteCodeLen,
            QString &siteCodeBM
            );
};

class cDBDoorTable: QObject
{
    Q_OBJECT
private:
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

public:
    cDBDoorTable();
    cDBDoorTable(
            QString name,
            int outputId,
            int enTimezone,
            int securityLevel,
            int doorSensorInputId,
            int doorSensorEnTimezone,
            int pushButtonInputId,
            int pushButtonEnTimzeone,
            int inhibitTimezone,
            int securityOffTimezone
            );

    int SetDoorInfo(
            const QString name,
            const int outputId,
            const int enTimezone,
            const int securityLevel,
            const int doorSensorInputId,
            const int doorSensorEnTimezone,
            const int pushButtonInputId,
            const int pushButtonEnTimzeone,
            const int inhibitTimezone,
            const int securityOffTimezone
            );
    int GetDoorInfo(
            QString &name,
            int &outputId,
            int &enTimezone,
            int &securityLevel,
            int &doorSensorInputId,
            int &doorSensorEnTimezone,
            int &pushButtonInputId,
            int &pushButtonEnTimzeone,
            int &inhibitTimezone,
            int &securityOffTimezone
            );
};

class cDBModeOfOperationTable: QObject
{
    Q_OBJECT
private:
    int card;
    int pin;
    int face;
    int qr;
    int ble;

public:
    typedef enum _E_MODEOPERATION
    {
        E_CARD_ONLY = 0,
    } E_MODEOPERATION;

    cDBModeOfOperationTable();
    cDBModeOfOperationTable(int card, int pin, int face, int qr, int ble);

    int SetInputInfo(int card, int pin, int face, int qr, int ble);
    int GetInputInfo(int &card, int &pin, int &face, int &qr, int &ble);

    E_MODEOPERATION GetWorkingModeOfOperation(int incomingInterface);
};

class cDBInputTable: QObject
{
    Q_OBJECT
private:
    QString inputName;
    QString inputType;
    int closeOpenTrigger;   // Trigger type
    double analogReadingToTrigger;
    int gpioPin;

public:
    cDBInputTable();
    cDBInputTable(
            const QString &name,
            const QString &type,
            int closeTrigger,
            double analogReading,
            int gpioPin
            );

    int SetInputInfo(
            const QString &name,
            const QString &type,
            int closeTrigger,
            double analogReading,
            int gpioPin
            );
    int GetInputInfo(
            QString &name,
            QString &type,
            int &closeTrigger,
            double &analogReading,
            int &gpioPin
            )
    ;
};

class cDBOutputTable: QObject
{
    Q_OBJECT
private:
    QString outputName;
    QString outputType;
    int releaseSeconds;
    int openSeconds;
    int pulseSeconds;
    int gpioPin;

public:
    cDBOutputTable();
    cDBOutputTable(
            const QString &outputName,
            const QString &outputType,
            int releaseSec,
            int openSec,
            int pulseSec,
            int gpioPin
            );

    int SetOutputInfo(
            const QString &outputName,
            const QString &outputType,
            int releaseSec,
            int openSec,
            int pulseSec,
            int gpioPin
            );
    int GetOutputInfo(
            QString &outputName,
            QString &outputType,
            int &releaseSec,
            int &openSec,
            int &pulseSec,
            int &gpioPin
            );
};

class cDBCardTypeTable: QObject
{
    Q_OBJECT
private:
    QString cardType;
    QString cardTypeDescription;

public:
    cDBCardTypeTable();
    cDBCardTypeTable(const QString &cardType, const QString &cardTypeDescription);

    int SetCardType(QString &cardType, QString &cardTypeDescription);
    int GetCardType(QString &cardType, QString &cardTypeDescription);
};

class cDBUserTypeTable: QObject
{
    Q_OBJECT
private:
    QString userType;
    QString description;

public:
    cDBUserTypeTable();
    cDBUserTypeTable(const QString &userType, const QString &description);

    int SetUserType(QString &userType, QString &description);
    int GetUserType(QString &userType, QString &description);
};

class cDBTimesetTable: QObject
{
    Q_OBJECT
private:
    QTime m_startTime;
    QTime m_EndTime;

public:
    cDBTimesetTable();
    cDBTimesetTable(const QString &startTime, const QString &endTime);

    int SetTimeset(QString &startTime, QString &endTime);
    int GetTimeSet(QTime &startTime, QTime &endTime);
};

class cDBTimezoneTable: QObject
{
    Q_OBJECT
private:
    int iTimeset[10];

public:
    cDBTimezoneTable();
    cDBTimezoneTable(int *iTimeSet);

    int SetTimeZone(int *iTimeSet);
    int GetTimeZone(int *iTimeSet);
};

class cDBCardTableInfo : QObject
{
    Q_OBJECT
private:
    int iUserID;
    QString qStrCardNo;
    int iCardType;
    int iModeOfOperation;
    int iDoor1Id;
    int iDoor1AccessTimezone;
    int iDoor2Id;
    int iDoor2AccessTimezone;
    int iDoor3Id;
    int iDoor3AccessTimezone;
    int iDoor4Id;
    int iDoor4AccessTimezone;
    int iCardEnableTimezone;
    QDateTime qStrValidStartTime;
    QDateTime qStrValidEndTime;
    QString qStrCardPin;
    int iCardPinEnTimezone;
    int iNumBuddy;
    QString qStrBuddy1CardNo;
    QString qStrBuddy2CardNo;
    QString qStrBuddy3CardNo;
    QString qStrBuddy4CardNo;

    void SetDefaultParams(void);

public:
    cDBCardTableInfo();

    cDBCardTableInfo(int iUserID,
                     QString qStrCardNo);

    /* Get Method */
    int GetUserID(void);
    QString GetCardNo(void);
    int GetCardType(void);
    int GetModeOfOperation(void);
    int GetDoor1Id(void);
    int GetDoor1AccessTimezone(void);
    int GetDoor2Id(void);
    int GetDoor2AccessTimezone(void);
    int GetDoor3Id(void);
    int GetDoor3AccessTimezone(void);
    int GetDoor4Id(void);
    int GetDoor4AccessTimezone(void);
    int GetCardEnableTimezone(void);
    QDateTime GetValidStartTime(void);
    QString GetValidStartTime(const QString &format);
    QString GetValidEndTime(const QString &format);
    QDateTime GetValidEndTime(void);
    QString GetCardPin(void);
    int GetCardPinEnTimezone(void);
    int GetNumBuddy(void);
    QString GetBuddy1CardNo(void);
    QString GetBuddy2CardNo(void);
    QString GetBuddy3CardNo(void);
    QString GetBuddy4CardNo(void);

    /* Set Method */
    int SetUserID(int userID);
    int SetCardNo(QString cardNumber);
    int SetCardType(int cardType);
    int SetModeOfOperation(int modeOfOperation);
    int SetDoor1Id(int door1Id);
    int SetDoor1AccessTimezone(int door1Timezone);
    int SetDoor2Id(int door2Id);
    int SetDoor2AccessTimezone(int door2Timezone);
    int SetDoor3Id(int door3Id);
    int SetDoor3AccessTimezone(int door3Timezone);
    int SetDoor4Id(int door4Id);
    int SetDoor4AccessTimezone(int door4Timezone);
    int SetCardEnableTimezone(int cardEnTimezone);
    int SetValidStartTime(QDateTime validStartTime);
    int SetValidEndTime(QDateTime validEndTime);
    int SetCardPin(QString cardPin);
    int SetCardPinEnTimezone(int cardPinEnTimezone);
    int SetNumBuddy(int numBuddy);
    int SetBuddy1CardNo(QString cardNumber);
    int SetBuddy2CardNo(QString cardNumber);
    int SetBuddy3CardNo(QString cardNumber);
    int SetBuddy4CardNo(QString cardNumber);
};

#endif // CDBTABLEINFO_H

