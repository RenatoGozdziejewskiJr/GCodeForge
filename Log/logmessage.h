#ifndef LOGMESSAGE_H
#define LOGMESSAGE_H

#include <QObject>
#include <QDateTime>

class LogMessage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDateTime dateTime READ dateTime WRITE setDateTime NOTIFY dateTimeChanged)    
    Q_PROPERTY(LogMessageType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)

public:
    enum LogMessageType {MSGINFO = 0, MSGWARNING, MSGERROR};
    Q_ENUM(LogMessageType)

    explicit LogMessage(QObject *parent = nullptr);
    explicit LogMessage(LogMessageType type, QString subject, QString userName, QString message);
    ~LogMessage();

    QDateTime dateTime() const;
    QString subject() const;
    LogMessageType type() const;
    QString userName() const;
    QString message() const;
    QString toString();

    bool operator == (const LogMessage& other) const;


signals:
    void dateTimeChanged(QDateTime dateTime);
    void subjectChanged(QString subject);
    void typeChanged(LogMessageType type);
    void messageChanged(QString message);    
    void userNameChanged(QString userName);

public slots:
    void setDateTime(QDateTime dateTime);
    void setSubject(QString subject);
    void setType(LogMessageType type);
    void setUserName(QString userName);
    void setMessage(QString message);

private:
    QDateTime m_dateTime;
    QString m_subject;
    LogMessageType m_type;
    QString m_userName;
    QString m_message;
};

#endif // LOGMESSAGE_H
