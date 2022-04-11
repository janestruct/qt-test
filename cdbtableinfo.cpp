#include "cdbtableinfo.h"
#include "CommonField.h"
#include <QDebug>

cDBCardTable::cDBCardTable()
{

}

cDBCardTable::cDBCardTable(
        int     userId,
        QString cardNumber,
        int     cardType,
        int     modeOperation,
        int     door1Id,
        int     door1AccessTimezone,
        int     door2Id,
        int     door2AccessTimezone,
        int     door3Id,
        int     door3AccessTimezone,
        int     door4Id,
        int     door4AccessTimezone,
        int     cardEnTimezone,
        QString validStart,
        QString validEnd,
        QString cardPin,
        int     cardWithPinTimezone,
        int     numBuddy,
        QString buddyCard1,
        QString buddyCard2,
        QString buddyCard3,
        QString buddyCard4
        )
{
    this->userId                     = userId;
    this->cardNumber                 = cardNumber;
    this->cardType                   = cardType;
    this->modeOperation              = modeOperation;
    this->door1Id                    = door1Id;
    this->door1AccessTimezone        = door1AccessTimezone;
    this->door2Id                    = door2Id;
    this->door2AccessTimezone        = door2AccessTimezone;
    this->door3Id                    = door3Id;
    this->door3AccessTimezone        = door3AccessTimezone;
    this->door4Id                    = door4Id;
    this->door4AccessTimezone        = door4AccessTimezone;
    this->cardEnTimezone             = cardEnTimezone;
    this->validStart                 = validStart;
    this->validEnd                   = validEnd;
    this->cardPin                    = cardPin;
    this->cardWithPinTimezone        = cardWithPinTimezone;
    this->numBuddy                   = numBuddy;
    this->buddyCard1                 = buddyCard1;
    this->buddyCard2                 = buddyCard2;
    this->buddyCard3                 = buddyCard3;
    this->buddyCard4                 = buddyCard4;
}

int cDBCardTable::SetCardInfo(
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
        )
{
    this->userId                     = userId;
    this->cardNumber                 = cardNumber;
    this->cardType                   = cardType;
    this->modeOperation              = modeOperation;
    this->door1Id                    = door1Id;
    this->door1AccessTimezone        = door1AccessTimezone;
    this->door2Id                    = door2Id;
    this->door2AccessTimezone        = door2AccessTimezone;
    this->door3Id                    = door3Id;
    this->door3AccessTimezone        = door3AccessTimezone;
    this->door4Id                    = door4Id;
    this->door4AccessTimezone        = door4AccessTimezone;
    this->cardEnTimezone             = cardEnTimezone;
    this->validStart                 = validStart;
    this->validEnd                   = validEnd;
    this->cardPin                    = cardPin;
    this->cardWithPinTimezone        = cardWithPinTimezone;
    this->numBuddy                   = numBuddy;
    this->buddyCard1                 = buddyCard1;
    this->buddyCard2                 = buddyCard2;
    this->buddyCard3                 = buddyCard3;
    this->buddyCard4                 = buddyCard4;

    return 1;
}

int cDBCardTable::GetCardInfo(
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
        )
{

    userId                     = this->userId;
    cardNumber                 = this->cardNumber;
    cardType                   = this->cardType;
    modeOperation              = this->modeOperation;
    door1Id                    = this->door1Id;
    door1AccessTimezone        = this->door1AccessTimezone;
    door2Id                    = this->door2Id;
    door2AccessTimezone        = this->door2AccessTimezone;
    door3Id                    = this->door3Id;
    door3AccessTimezone        = this->door3AccessTimezone;
    door4Id                    = this->door4Id;
    door4AccessTimezone        = this->door4AccessTimezone;
    cardEnTimezone             = this->cardEnTimezone;
    validStart                 = this->validStart;
    validEnd                   = this->validEnd;
    cardPin                    = this->cardPin;
    cardWithPinTimezone        = this->cardWithPinTimezone;
    numBuddy                   = this->numBuddy;
    buddyCard1                 = this->buddyCard1;
    buddyCard2                 = this->buddyCard2;
    buddyCard3                 = this->buddyCard3;
    buddyCard4                 = this->buddyCard4;

    return 1;
}

int cDBCardTable::GetUserID(void)
{
    return userId;
}

int cDBCardTable::SetUserID(int userId)
{
    this->userId = userId;
    return 1;
}

cDBUserTable::cDBUserTable()
{

}

cDBUserTable::cDBUserTable(
        int         overrideCard,
        QString     userName,
        QString     id1,
        QString     id2,
        QString     id3,
        QString     userPin,
        int         userType,
        QString     department,
        int         modeOperation,
        int         userEnTimezone,
        QString     validStart,
        QString     validEnd,
        int         door1Id,
        int         door1AccessTimezone,
        int         door2Id,
        int         door2AccessTimezone,
        int         door3Id,
        int         door3AccessTimezone,
        int         door4Id,
        int         door4AccessTimezone,
        QByteArray  leftFPTemplate,
        QByteArray  rightFPTemplate,
        QByteArray  photo
        )
{
    this->overrideCard         = overrideCard;
    this->userName             = userName;
    this->id1                  = id1;
    this->id2                  = id2;
    this->id3                  = id3;
    this->userPin              = userPin;
    this->userType             = userType;
    this->department           = department;
    this->modeOperation        = modeOperation;
    this->userEnTimezone       = userEnTimezone;
    this->validStart           = validStart;
    this->validEnd             = validEnd;
    this->door1Id              = door1Id;
    this->door1AccessTimezone  = door1AccessTimezone;
    this->door2Id              = door2Id;
    this->door2AccessTimezone  = door2AccessTimezone;
    this->door3Id              = door3Id;
    this->door3AccessTimezone  = door3AccessTimezone;
    this->door4Id              = door4Id;
    this->door4AccessTimezone  = door4AccessTimezone;
    this->leftFPTemplate       = leftFPTemplate;
    this->rightFPTemplate      = rightFPTemplate;
    this->photo                = photo;
}

int cDBUserTable::SetUserInfo(
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
        )
{
    this->overrideCard         = overrideCard;
    this->userName             = userName;
    this->id1                  = id1;
    this->id2                  = id2;
    this->id3                  = id3;
    this->userPin              = userPin;
    this->userType             = userType;
    this->department           = department;
    this->modeOperation        = modeOperation;
    this->userEnTimezone       = userEnTimezone;
    this->validStart           = validStart;
    this->validEnd             = validEnd;
    this->door1Id              = door1Id;
    this->door1AccessTimezone  = door1AccessTimezone;
    this->door2Id              = door2Id;
    this->door2AccessTimezone  = door2AccessTimezone;
    this->door3Id              = door3Id;
    this->door3AccessTimezone  = door3AccessTimezone;
    this->door4Id              = door4Id;
    this->door4AccessTimezone  = door4AccessTimezone;
    this->leftFPTemplate       = leftFPTemplate;
    this->rightFPTemplate      = rightFPTemplate;
    this->photo                = photo;

    return 1;
}

int cDBUserTable::GetUserInfo(
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
        )
{
    overrideCard         = this->overrideCard;
    userName             = this->userName;
    id1                  = this->id1;
    id2                  = this->id2;
    id3                  = this->id3;
    userPin              = this->userPin;
    userType             = this->userType;
    department           = this->department;
    modeOperation        = this->modeOperation;
    userEnTimezone       = this->userEnTimezone;
    validStart           = this->validStart;
    validEnd             = this->validEnd;
    door1Id              = this->door1Id;
    door1AccessTimezone  = this->door1AccessTimezone;
    door2Id              = this->door2Id;
    door2AccessTimezone  = this->door2AccessTimezone;
    door3Id              = this->door3Id;
    door3AccessTimezone  = this->door3AccessTimezone;
    door4Id              = this->door4Id;
    door4AccessTimezone  = this->door4AccessTimezone;
    leftFPTemplate       = this->leftFPTemplate;
    rightFPTemplate      = this->rightFPTemplate;
    photo                = this->photo;

    return 1;
}

cDBReaderTable::cDBReaderTable()
{

}

cDBReaderTable::cDBReaderTable(
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
        )
{
    this->name                   = name                  ;
    this->type                   = type                  ;
    this->enTimezone             = enTimezone            ;
    this->direction              = direction             ;
    this->tiedDoorId             = tiedDoorId            ;
    this->tamperInputEnTimezone  = tamperInputEnTimezone ;
    this->tamperInputId          = tamperInputId         ;
    this->LEDOutput              = LEDOutput             ;
    this->BuzzerOutput           = BuzzerOutput          ;
    this->StandWiegandEnBM       = StandWiegandEnBM      ;
    this->custWieId1             = custWieId1            ;
    this->custWieId2             = custWieId2            ;
    this->custWieId3             = custWieId3            ;
    this->custWieId4             = custWieId4            ;
    this->custWieId5             = custWieId5            ;
    this->custWieId6             = custWieId6            ;
    this->custWieId7             = custWieId7            ;
    this->custWieId8             = custWieId8            ;
    this->custWieId9             = custWieId9            ;
    this->custWieId10            = custWieId10           ;
}

int cDBReaderTable::SetReaderInfo(
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
        )
{
    this->name                   = name                  ;
    this->type                   = type                  ;
    this->enTimezone             = enTimezone            ;
    this->direction              = direction             ;
    this->tiedDoorId             = tiedDoorId            ;
    this->tamperInputEnTimezone  = tamperInputEnTimezone ;
    this->tamperInputId          = tamperInputId         ;
    this->LEDOutput              = LEDOutput             ;
    this->BuzzerOutput           = BuzzerOutput          ;
    this->StandWiegandEnBM       = StandWiegandEnBM      ;
    this->custWieId1             = custWieId1            ;
    this->custWieId2             = custWieId2            ;
    this->custWieId3             = custWieId3            ;
    this->custWieId4             = custWieId4            ;
    this->custWieId5             = custWieId5            ;
    this->custWieId6             = custWieId6            ;
    this->custWieId7             = custWieId7            ;
    this->custWieId8             = custWieId8            ;
    this->custWieId9             = custWieId9            ;
    this->custWieId10            = custWieId10           ;

    return 1;
}

int cDBReaderTable::GetReaderInfo(
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
        )
{
    name                  = this->name                 ;
    type                  = this->type                 ;
    enTimezone            = this->enTimezone           ;
    direction             = this->direction            ;
    tiedDoorId            = this->tiedDoorId           ;
    tamperInputEnTimezone = this->tamperInputEnTimezone;
    tamperInputId         = this->tamperInputId        ;
    LEDOutput             = this->LEDOutput            ;
    BuzzerOutput          = this->BuzzerOutput         ;
    StandWiegandEnBM      = this->StandWiegandEnBM     ;
    custWieId1            = this->custWieId1           ;
    custWieId2            = this->custWieId2           ;
    custWieId3            = this->custWieId3           ;
    custWieId4            = this->custWieId4           ;
    custWieId5            = this->custWieId5           ;
    custWieId6            = this->custWieId6           ;
    custWieId7            = this->custWieId7           ;
    custWieId8            = this->custWieId8           ;
    custWieId9            = this->custWieId9           ;
    custWieId10           = this->custWieId10          ;

    return 1;
}

cDBCustomWiegandTable::cDBCustomWiegandTable()
{

}

cDBCustomWiegandTable::cDBCustomWiegandTable(
        int cardbitLen,
        QString cardbitPosBM,
        int evenParityLen,
        int evenParityPos,
        QString evenParityPosBM,
        int oddParityLen,
        int oddParityPos,
        QString oddParityPosBM,
        int cardConversion,
        int packetLen,
        int siteCodeLen,
        QString siteCodeBM
        )
{
    this->cardbitLen = cardbitLen;
    this->cardbitPosBM = cardbitPosBM; // Has to convert to binary
    this->evenParityLen = evenParityLen;
    this->evenParityPos = evenParityPos;
    this->evenParityPosBM = evenParityPosBM; // Has to convert to binary
    this->oddParityLen = oddParityLen;
    this->oddParityPos = oddParityPos;
    this->oddParityPosBM = oddParityPosBM; // Has to convert to binary
    this->cardConversion = cardConversion;
    this->packetLen = packetLen;
    this->siteCodeLen = siteCodeLen;
    this->siteCodeBM = siteCodeBM;
}

int cDBCustomWiegandTable::SetCustomWiegandInfo(
        const int cardbitLen,
        const QString cardbitPosBM,
        const int evenParityLen,
        const int evenParityPos,
        const QString evenParityPosBM,
        const int oddParityLen,
        const int oddParityPos,
        const QString oddParityPosBM,
        const int cardConversion,
        const int packetLen,
        const int siteCodeLen,
        const QString siteCodeBM
        )
{
    this->cardbitLen = cardbitLen;
    this->cardbitPosBM = cardbitPosBM; // Has to convert to binary
    this->evenParityLen = evenParityLen;
    this->evenParityPos = evenParityPos;
    this->evenParityPosBM = evenParityPosBM; // Has to convert to binary
    this->oddParityLen = oddParityLen;
    this->oddParityPos = oddParityPos;
    this->oddParityPosBM = oddParityPosBM; // Has to convert to binary
    this->cardConversion = cardConversion;
    this->packetLen = packetLen;
    this->siteCodeLen = siteCodeLen;
    this->siteCodeBM = siteCodeBM;

    return 1;
}

int cDBCustomWiegandTable::GetCustomWiegandInfo(
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
        )
{
    cardbitLen = this->cardbitLen;
    cardbitPosBM = this->cardbitPosBM;
    evenParityLen = this->evenParityLen;
    evenParityPos = this->evenParityPos;
    evenParityPosBM = this->evenParityPosBM;
    oddParityLen = this->oddParityLen;
    oddParityPos = this->oddParityPos;
    oddParityPosBM = this->oddParityPosBM;
    cardConversion = this->cardConversion;
    packetLen = this->packetLen;
    siteCodeLen = this->siteCodeLen;
    siteCodeBM = this->siteCodeBM;

    return 1;
}

cDBDoorTable::cDBDoorTable()
{

}

cDBDoorTable::cDBDoorTable(
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
        )
{
    this->name = name;
    this->outputId = outputId;
    this->enTimezone = enTimezone;
    this->securityLevel = securityLevel;
    this->doorSensorInputId = doorSensorInputId;
    this->doorSensorEnTimezone = doorSensorEnTimezone;
    this->pushButtonInputId = pushButtonInputId;
    this->pushButtonEnTimzeone = pushButtonEnTimzeone;
    this->inhibitTimezone = inhibitTimezone;
    this->securityOffTimezone = securityOffTimezone;
}

int cDBDoorTable::SetDoorInfo(
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
        )
{
    this->name = name;
    this->outputId = outputId;
    this->enTimezone = enTimezone;
    this->securityLevel = securityLevel;
    this->doorSensorInputId = doorSensorInputId;
    this->doorSensorEnTimezone = doorSensorEnTimezone;
    this->pushButtonInputId = pushButtonInputId;
    this->pushButtonEnTimzeone = pushButtonEnTimzeone;
    this->inhibitTimezone = inhibitTimezone;
    this->securityOffTimezone = securityOffTimezone;

    return 1;
}

int cDBDoorTable::GetDoorInfo(
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
        )
{
    name = this->name;
    outputId = this->outputId;
    enTimezone = this->enTimezone;
    securityLevel = this->securityLevel;
    doorSensorInputId = this->doorSensorInputId;
    doorSensorEnTimezone = this->doorSensorEnTimezone;
    pushButtonInputId = this->pushButtonInputId;
    pushButtonEnTimzeone = this->pushButtonEnTimzeone;
    inhibitTimezone = this->inhibitTimezone;
    securityOffTimezone = this->securityOffTimezone;

    return 1;
}

cDBModeOfOperationTable::cDBModeOfOperationTable()
{

}

cDBModeOfOperationTable::cDBModeOfOperationTable(int card, int pin, int face, int qr, int ble)
{
    // TODO: TBD
    this->card = card;
    this->pin = pin;
    this->face = face;
    this->qr = qr;
    this->ble = ble;
}

int cDBModeOfOperationTable::SetInputInfo(int card, int pin, int face, int qr, int ble)
{
    // TODO: TBD
    this->card = card;
    this->pin = pin;
    this->face = face;
    this->qr = qr;
    this->ble = ble;

    return 1;
}

int cDBModeOfOperationTable::GetInputInfo(int &card, int &pin, int &face, int &qr, int &ble)
{
    // TODO: TBD
    card = this->card  ;
    pin  = this->pin   ;
    face = this->face  ;
    qr   = this->qr    ;
    ble  = this->ble   ;

    return 1;
}

cDBModeOfOperationTable::E_MODEOPERATION GetWorkingModeOfOperation(int incomingInterface)
{

    return cDBModeOfOperationTable::E_CARD_ONLY;
}

cDBInputTable::cDBInputTable()
{

}

cDBInputTable::cDBInputTable(
        const QString &name,
        const QString &type,
        int closeTrigger,
        double analogReading,
        int gpioPin
        )
{
    this->inputName = name;
    this->inputType = type;
    this->closeOpenTrigger = closeTrigger;
    this->analogReadingToTrigger = analogReading;
    this->gpioPin = gpioPin;
}

int cDBInputTable::SetInputInfo(
        const QString &name,
        const QString &type,
        int closeTrigger,
        double analogReading,
        int gpioPin
        )
{
    this->inputName = name;
    this->inputType = type;
    this->closeOpenTrigger = closeTrigger;
    this->analogReadingToTrigger = analogReading;
    this->gpioPin = gpioPin;

    return 1;
}

int cDBInputTable::GetInputInfo(
        QString &name,
        QString &type,
        int &closeTrigger,
        double &analogReading,
        int &gpioPin
        )
{
    name = this->inputName;
    type = this->inputType;
    closeTrigger = this->closeOpenTrigger;
    analogReading = this->analogReadingToTrigger;
    gpioPin = this->gpioPin;

    return 1;
}

cDBOutputTable::cDBOutputTable()
{

}

cDBOutputTable::cDBOutputTable(
        const QString &outputName,
        const QString &outputType,
        int releaseSec,
        int openSec,
        int pulseSec,
        int gpioPin
        )
{
    this->outputName = outputName;
    this->outputType = outputType;
    this->releaseSeconds = releaseSec;
    this->openSeconds = openSec;
    this->pulseSeconds = pulseSec;
    this->gpioPin = gpioPin;
}

int cDBOutputTable::SetOutputInfo(
        const QString &outputName,
        const QString &outputType,
        int releaseSec,
        int openSec,
        int pulseSec,
        int gpioPin
        )
{
    this->outputName = outputName;
    this->outputType = outputType;
    this->releaseSeconds = releaseSec;
    this->openSeconds = openSec;
    this->pulseSeconds = pulseSec;
    this->gpioPin = gpioPin;

    return 1;
}

int cDBOutputTable::GetOutputInfo(
        QString &outputName,
        QString &outputType,
        int &releaseSec,
        int &openSec,
        int &pulseSec,
        int &gpioPin
        )
{
    outputName = this->outputName;
    outputType = this->outputType;
    releaseSec = this->releaseSeconds;
    openSec = this->openSeconds;
    pulseSec = this->pulseSeconds;
    gpioPin = this->gpioPin;

    return 1;
}

cDBCardTypeTable::cDBCardTypeTable()
{

}

cDBCardTypeTable::cDBCardTypeTable(const QString &cardType, const QString &cardTypeDescription)
{
    this->cardType = cardType;
    this->cardTypeDescription = cardTypeDescription;
}

int cDBCardTypeTable::SetCardType(QString &cardType, QString &cardTypeDescription)
{
    this->cardType = cardType;
    this->cardTypeDescription = cardTypeDescription;

    return 1;
}

int cDBCardTypeTable::GetCardType(QString &cardType, QString &cardTypeDescription)
{
    cardType = this->cardType;
    cardTypeDescription = this->cardTypeDescription;

    return 1;
}

cDBUserTypeTable::cDBUserTypeTable()
{

}

cDBUserTypeTable::cDBUserTypeTable(const QString &userType, const QString &description)
{
    this->userType = userType;
    this->description = description;
}

int cDBUserTypeTable::SetUserType(QString &userType, QString &description)
{
    this->userType = userType;
    this->description = description;

    return 1;
}

int cDBUserTypeTable::GetUserType(QString &userType, QString &description)
{
    userType = this->userType;
    description = this->description;


    return 1;
}

cDBTimezoneTable::cDBTimezoneTable()
{

}

cDBTimezoneTable::cDBTimezoneTable(int *iTimeset)
{
    int i;
    for (i = 0; i < MAX_TIMEZONE; i++)
    {
        this->iTimeset[i] = iTimeset[i];
    }
}

int cDBTimezoneTable::SetTimeZone(int *iTimeSet)
{
    int i;
    for (i = 0; i < MAX_TIMEZONE; i++)
    {
        this->iTimeset[i] = iTimeSet[i];
    }

    return i;
}

int cDBTimezoneTable::GetTimeZone(int *iTimeSet)
{
    if (iTimeSet == nullptr)
        return -1;

    int i;
    for (i = 0; i < MAX_TIMEZONE; i++)
    {
        iTimeSet[i] = this->iTimeset[i];
    }

    return 1;
}


cDBTimesetTable::cDBTimesetTable()
{

}

cDBTimesetTable::cDBTimesetTable(const QString &startTime, const QString &endTime)
{
    m_startTime = QTime::fromString(startTime, "HH:mm");
    m_EndTime = QTime::fromString(endTime, "HH:mm");
}

int cDBTimesetTable::SetTimeset(QString &startTime, QString &endTime)
{
    m_startTime = QTime::fromString(startTime, "HH:mm");
    m_EndTime = QTime::fromString(endTime, "HH:mm");

    return 1;
}

int cDBTimesetTable::GetTimeSet(QTime &startTime, QTime &endTime)
{
    startTime = m_startTime;
    endTime = m_EndTime;

    return 1;
}

void cDBCardTableInfo::SetDefaultParams()
{
    this->iCardType = 1;
    this->iModeOfOperation = 10000000;
    this->iDoor1Id = 1;
    this->iDoor1AccessTimezone = 1;
    this->iDoor2Id = 2;
    this->iDoor2AccessTimezone = 1;
    this->iDoor3Id = 3;
    this->iDoor3AccessTimezone = 1;
    this->iDoor4Id = 4;
    this->iDoor4AccessTimezone = 1;
    this->iCardEnableTimezone = 1;
    this->qStrValidStartTime = QDateTime(QDateTime::fromString("01/01/2020 00:00", "dd/MM/yyyy HH:mm"));
    this->qStrValidEndTime = QDateTime(QDateTime::fromString("01/01/2021 00:00", "dd/MM/yyyy HH:mm"));
    this->qStrCardPin = "000000";
    this->iCardPinEnTimezone = 0;
    this->iNumBuddy = 0;
    this->qStrBuddy1CardNo = "null";
    this->qStrBuddy2CardNo = "null";
    this->qStrBuddy3CardNo = "null";
    this->qStrBuddy4CardNo = "null";
}

cDBCardTableInfo::cDBCardTableInfo()
{
    this->iUserID = 0xFF;
    this->qStrCardNo = "0000000000";
    SetDefaultParams();
}

cDBCardTableInfo::cDBCardTableInfo(int iUserID,
                                   QString qStrCardNo)
{
    this->iUserID = iUserID;
    this->qStrCardNo = qStrCardNo;
    SetDefaultParams();
}

/* Get Method */
int cDBCardTableInfo::GetUserID(void)
{
    return iUserID;
}

QString cDBCardTableInfo::GetCardNo(void)
{
    return qStrCardNo;
}

int cDBCardTableInfo::GetCardType(void)
{
    return iCardType;
}

int cDBCardTableInfo::GetModeOfOperation(void)
{
    return iModeOfOperation;
}

int cDBCardTableInfo::GetDoor1Id(void)
{
    return iDoor1Id;
}

int cDBCardTableInfo::GetDoor1AccessTimezone(void)
{
    return iDoor1AccessTimezone;
}

int cDBCardTableInfo::GetDoor2Id(void)
{
    return iDoor2Id;
}

int cDBCardTableInfo::GetDoor2AccessTimezone(void)
{
    return iDoor2AccessTimezone;
}

int cDBCardTableInfo::GetDoor3Id(void)
{
    return iDoor3Id;
}

int cDBCardTableInfo::GetDoor3AccessTimezone(void)
{
    return iDoor3AccessTimezone;
}

int cDBCardTableInfo::GetDoor4Id(void)
{
    return iDoor4Id;
}

int cDBCardTableInfo::GetDoor4AccessTimezone(void)
{
    return iDoor4AccessTimezone;
}

int cDBCardTableInfo::GetCardEnableTimezone(void)
{
    return iCardEnableTimezone;
}

QDateTime cDBCardTableInfo::GetValidStartTime(void)
{
    return qStrValidStartTime;
}

QString cDBCardTableInfo::GetValidStartTime(const QString &format)
{
    return qStrValidStartTime.toString(format);
}

QString cDBCardTableInfo::GetValidEndTime(const QString &format)
{
    return qStrValidStartTime.toString(format);
}

QString cDBCardTableInfo::GetCardPin(void)
{
    return qStrCardPin;
}

int cDBCardTableInfo::GetCardPinEnTimezone(void)
{
    return iCardEnableTimezone;
}

int cDBCardTableInfo::GetNumBuddy(void)
{
    return iNumBuddy;
}

QString cDBCardTableInfo::GetBuddy1CardNo(void)
{
    return qStrBuddy1CardNo;
}

QString cDBCardTableInfo::GetBuddy2CardNo(void)
{
    return qStrBuddy2CardNo;
}

QString cDBCardTableInfo::GetBuddy3CardNo(void)
{
    return qStrBuddy3CardNo;
}

QString cDBCardTableInfo::GetBuddy4CardNo(void)
{
    return qStrBuddy4CardNo;
}

/* Set Method */
int cDBCardTableInfo::SetUserID(int userID)
{
    iUserID = userID;
    return 1;
}

int cDBCardTableInfo::SetCardNo(QString cardNumber)
{
    qStrCardNo = cardNumber;
    return 1;
}

int cDBCardTableInfo::SetCardType(int cardType)
{
    iCardType = cardType;
    return 1;
}

int cDBCardTableInfo::SetModeOfOperation(int modeOfOperation)
{
    iModeOfOperation = modeOfOperation;
    return 1;
}

int cDBCardTableInfo::SetDoor1Id(int door1Id)
{
    iDoor1Id = door1Id;
    return 1;
}

int cDBCardTableInfo::SetDoor1AccessTimezone(int door1Timezone)
{
    iDoor1AccessTimezone = door1Timezone;
    return 1;
}

int cDBCardTableInfo::SetDoor2Id(int door2Id)
{
    iDoor2Id = door2Id;
    return 1;
}

int cDBCardTableInfo::SetDoor2AccessTimezone(int door2Timezone)
{
    iDoor2AccessTimezone = door2Timezone;
    return 1;
}

int cDBCardTableInfo::SetDoor3Id(int door3Id)
{
    iDoor3Id = door3Id;
    return 1;
}

int cDBCardTableInfo::SetDoor3AccessTimezone(int door3Timezone)
{
    iDoor3AccessTimezone = door3Timezone;
    return 1;
}

int cDBCardTableInfo::SetDoor4Id(int door4Id)
{
    iDoor4Id = door4Id;
    return 1;
}

int cDBCardTableInfo::SetDoor4AccessTimezone(int door4Timezone)
{
    iDoor4AccessTimezone = door4Timezone;
    return 1;
}

int cDBCardTableInfo::SetCardEnableTimezone(int cardEnTimezone)
{
    iCardEnableTimezone = cardEnTimezone;
    return 1;
}

int cDBCardTableInfo::SetValidStartTime(QDateTime validStartTime)
{
    qStrValidStartTime = validStartTime;
    return 1;
}

int cDBCardTableInfo::SetValidEndTime(QDateTime validEndTime)
{
    qStrValidEndTime = validEndTime;
    return 1;
}

int cDBCardTableInfo::SetCardPin(QString cardPin)
{
    qStrCardPin = cardPin;
    return 1;
}

int cDBCardTableInfo::SetCardPinEnTimezone(int cardPinEnTimezone)
{
    iCardPinEnTimezone = cardPinEnTimezone;
    return 1;
}

int cDBCardTableInfo::SetNumBuddy(int numBuddy)
{
    iNumBuddy = numBuddy;
    return 1;
}

int cDBCardTableInfo::SetBuddy1CardNo(QString cardNumber)
{
    qStrBuddy1CardNo = cardNumber;
    return 1;
}

int cDBCardTableInfo::SetBuddy2CardNo(QString cardNumber)
{
    qStrBuddy2CardNo = cardNumber;
    return 1;
}

int cDBCardTableInfo::SetBuddy3CardNo(QString cardNumber)
{
    qStrBuddy3CardNo = cardNumber;
    return 1;
}

int cDBCardTableInfo::SetBuddy4CardNo(QString cardNumber)
{
    qStrBuddy4CardNo = cardNumber;
    return 1;
}
