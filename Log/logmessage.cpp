#include "logmessage.h"

LogMessage::LogMessage(QObject *parent) : QObject(parent)
{

}

LogMessage::LogMessage(LogMessage::LogMessageType type, QString subject, QString userName, QString message)
{
    m_dateTime = QDateTime::currentDateTime();
    m_type = type;
    m_subject = subject;
    m_message = message;
    m_userName = userName;
}

LogMessage::~LogMessage()
{
}

QDateTime LogMessage::dateTime() const
{
    return m_dateTime;
}

QString LogMessage::subject() const
{
    return m_subject;
}

LogMessage::LogMessageType LogMessage::type() const
{
    return m_type;
}

QString LogMessage::message() const
{
    return m_message;
}

QString LogMessage::toString()
{
    QString strLine;
    QString strType;
    switch (type()) {
    case MSGINFO: strType = "INFO";
        break;
    case MSGERROR: strType = "ERROR";
        break;
    case MSGWARNING: strType = "WARNING";
        break;
    }

    //salva somente a hora, a data está no nome do arquivo de log
    strLine = dateTime().toString("hh:mm:ss").leftJustified(10) +
              strType.leftJustified(9) +
              subject().leftJustified(24) +
              userName().leftJustified(15) +
              message();
    return strLine;    
}

bool LogMessage::operator==(const LogMessage &other) const
{    
    return (this == &other);
}

QString LogMessage::userName() const
{
    return m_userName;
}

void LogMessage::setDateTime(QDateTime dateTime)
{
    if (m_dateTime == dateTime)
        return;

    m_dateTime = dateTime;
    emit dateTimeChanged(m_dateTime);
}

void LogMessage::setSubject(QString subject)
{
    if (m_subject == subject)
        return;

    m_subject = subject;
    emit subjectChanged(m_subject);
}

void LogMessage::setType(LogMessageType type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(m_type);
}

void LogMessage::setMessage(QString message)
{
    if (m_message == message)
        return;

    m_message = message;
    emit messageChanged(m_message);
}

void LogMessage::setUserName(QString userName)
{
    if (m_userName == userName)
        return;

    m_userName = userName;
    emit userNameChanged(m_userName);
}
