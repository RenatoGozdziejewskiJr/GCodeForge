#ifndef LOGMESSAGEMODEL_H
#define LOGMESSAGEMODEL_H

#include <QAbstractListModel>
#include "logmessage.h"
#include <QFile>
#include <QMutex>
#include <QColor>


const QColor oliveGreen = QColor(0x85, 0x8b, 0x69, 0xff); // QColor(0x6c, 0x8e, 0x2a, 0xff);
const QColor desertSand = QColor(0xee, 0xe8, 0xaa, 0xff);
const QColor gray = QColor(0xa8, 0xa8, 0xa8, 0xFF);
const QColor orange = QColor(0xFF, 0xA6, 0x37);
const QColor empty = QColor(0x00, 0x00, 0x00, 0x7f);

const QColor military01 = QColor(0x27, 0x2b, 0x00, 0xff);
const QColor military02 = QColor(0x54, 0x58, 0x2f, 0xff);
const QColor military03 = QColor(0x86, 0x89, 0x5d, 0xff);
const QColor military04 = QColor(0xbe, 0xc0, 0x92, 0xff);
const QColor military05 = QColor(0xf8, 0xfb, 0xca, 0xff);
const QColor dark01 = QColor(0x21, 0x21, 0x21, 0xE3);
const QColor dark02 = QColor(0x31, 0x31, 0x31, 0xff);
const QColor dark03 = QColor(0x39, 0x39, 0x39, 0xff);
const QColor dark04 = QColor(0x2f, 0x2f, 0x2f, 0x7f);
const QColor dark05 = QColor(0x1f, 0x1f, 0x1f, 0xff);


class LogMessageModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString LogDir READ LogDir WRITE setLogDir NOTIFY LogDirChanged FINAL)
    Q_PROPERTY(qint64 MaxLogFileSize READ MaxLogFileSize WRITE setMaxLogFileSize NOTIFY MaxLogFileSizeChanged FINAL)
    Q_PROPERTY(int RetentionDays READ RetentionDays WRITE setRetentionDays NOTIFY RetentionDaysChanged FINAL)
    Q_PROPERTY(int NrLogMessages READ NrLogMessages WRITE setNrLogMessages NOTIFY NrLogMessagesChanged FINAL)
    Q_PROPERTY(QString UserName READ UserName WRITE setUserName NOTIFY UserNameChanged FINAL)

public:

    enum MessageRoles {
        dateTimeRole = Qt::UserRole + 1,
        typeRole,
        subjectRole,
        userNameRole,
        messageRole
    };

    explicit LogMessageModel(QObject *parent = nullptr);
    ~LogMessageModel() override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    bool addMessage(LogMessage* msg);
    bool addMessage(QString txt, LogMessage::LogMessageType type = LogMessage::MSGINFO);
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    Q_INVOKABLE void clear();

    QString LogDir();
    void setLogDir(const QString &newLogDir);

    qint64 MaxLogFileSize();
    void setMaxLogFileSize(qint64 newMaxLogFileSize);

    int RetentionDays();
    void setRetentionDays(int newRetentionDays);

    int NrLogMessages();
    void setNrLogMessages(int newNrLogMessages);

    QString UserName();
    void setUserName(const QString &newUserName);

signals:
    void newMessage();

    void LogDirChanged();

    void MaxLogFileSizeChanged();

    void RetentionDaysChanged();

    void NrLogMessagesChanged();

    void UserNameChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:

    QStringList *m_lstHeader;
    bool saveLogFile(LogMessage* msg);

    QList<LogMessage*> *m_messages;
    QFile m_logFile;

    QMutex m_mutex;

    QString m_LogDir = "Log"; // nome do diretório de logs
    qint64 m_MaxLogFileSize = 1e+6; // bytes cada arquivo - Ex.: 1Mb = 1e+6
    int m_RetentionDays = 10; // Remove arquivos de log com mais de m_RetentionDays dias
    int m_NrLogMessages = 50; // Mantém somente m_NrLogMessages objetos de log na memória
    QString m_UserName = "Master";
};

namespace Log {

extern LogMessageModel* logModel;

}


#endif // LOGMESSAGEMODEL_H
