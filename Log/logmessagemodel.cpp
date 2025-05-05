#include "logmessagemodel.h"
#include <QThread>
#include <QtGui>
#include <QMutexLocker>

LogMessageModel* Log::logModel = new LogMessageModel();

LogMessageModel::LogMessageModel(QObject *parent)
    : QAbstractListModel(parent)
{    
    m_lstHeader = new QStringList();
    m_lstHeader->append(QStringList() << "Date Time" << "Type" << "Subject" << "User" << "Message");

    m_messages = new QList<LogMessage*>();

}

LogMessageModel::~LogMessageModel()
{
    if (m_logFile.isOpen()) m_logFile.close();

    clear();
    delete m_messages;

}

QVariant LogMessageModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_lstHeader->at(section);

    return QVariant();
}

int LogMessageModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_messages->count();
}

int LogMessageModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_lstHeader->count();
}

QVariant LogMessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() < 0 || index.row() >= m_messages->count())
        return QVariant();

    if ((role != Qt::DisplayRole
         //&&     role != Qt::EditRole
         //&&     role != Qt::BackgroundRole
         &&     role != Qt::ForegroundRole
         &&     role != Qt::DecorationRole
         // &&     role != Qt::TextAlignmentRole
         )
            )
        return QVariant();

    const LogMessage *message = m_messages->at(index.row());

    // if ((role == Qt::TextAlignmentRole) && (index.column() == 1)) {
    //     return Qt::AlignCenter;
    // }

    if (role == Qt::ForegroundRole) {
        if (index.column() == 0)
            return QColor(oliveGreen);

        if (index.column() == 1) {
            switch (message->type()) {
            case LogMessage::MSGINFO:
                return QColor(Qt::green);
            case LogMessage::MSGWARNING:
                return QColor(Qt::yellow);
            case LogMessage::MSGERROR:
                return QColor(Qt::red);
            }
        } else {
            return QColor(desertSand);
        }
    }

    if (role == Qt::DisplayRole) /*|| (role == Qt::EditRole)*/  {
        switch (index.column()) {
        case 0:  //datetime
            return message->dateTime().toString("dd/MM/yyyy hh:mm:ss");
        case 1: //type            
            switch (message->type()) {
            case LogMessage::MSGINFO:
                return "INFO";
            case LogMessage::MSGERROR:
                return "ERROR";
            case LogMessage::MSGWARNING:
                return "WARNING";
            }
            return message->type();
        case 2:  //subject
            return message->subject();
        case 3: //user
            return message->userName();
        case 4: //message
            return message->message();
        default:
            break;
        }
    }


    return QVariant();
}

Qt::ItemFlags LogMessageModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    //return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled;
    return QAbstractListModel::flags(index);

}


bool LogMessageModel::saveLogFile(LogMessage* msg) {
    //adiciona o log no respectivo arquivo texto
    static QString currentDate = "";
    static QString currentFile = "";
    static int id = 0;

    QString dirName = QDir::toNativeSeparators(qApp->applicationDirPath()+QDir::separator()+m_LogDir);
    //verifica se o diretorio de logs existe
    QDir logDir(dirName);

    //verifica se mudou o dia
    if (currentDate != QString("%1").arg(QDate::currentDate().toString("yyyy_MM_dd"))) {
        currentDate = QString("%1").arg(QDate::currentDate().toString("yyyy_MM_dd"));
        id = 0; //zera o id

        //apaga os arquivos com data inferior a N dias a partir de hoje
        QString filter(QDir::toNativeSeparators(QString("%1%1%1%1_%1%1_%1%1_%1%1%1.log"))); // pattern
        filter = filter.arg("[0123456789]"); // valid characters
        //varre o diretorio e remove logs com data maior que Constants::M
        Q_FOREACH (auto fileInfo, logDir.entryInfoList(QStringList(filter), QDir::Files)) {
            if (fileInfo.lastModified().date().daysTo(QDate::currentDate()) >  m_RetentionDays) {
                  QString filepath = fileInfo.absoluteFilePath();
                  QDir deletefile;
                  deletefile.setPath(filepath);
                  deletefile.remove(filepath);
                }
        }
    }

    QString logFileName = QDir::toNativeSeparators(dirName+QDir::separator()+QString("%1_%2.log").arg(QDate::currentDate().toString("yyyy_MM_dd")).arg(id, 3, 10, QLatin1Char('0')));
    //se mudou o dia
    if (logFileName != currentFile) {

        if (!logDir.exists()) {
            //se não existe cria o diretorio
            logDir.mkpath(dirName);
        }

        //fecha o arquivo caso esteja aberto
        if (m_logFile.isOpen()) m_logFile.close();

        //busca o último id do arquivo de log
        QString filter(QDir::toNativeSeparators(QString("%1_%2%2%2.log"))); // pattern
        filter = filter.arg(QDate::currentDate().toString("yyyy_MM_dd")).arg("[0123456789]"); // valid characters
        //varre o diretorio para buscar o último id
        Q_FOREACH (auto fileInfo, logDir.entryInfoList(QStringList(filter), QDir::Files)) {
            if (fileInfo.fileName().mid(11, 3).toInt() > id) {
                id = fileInfo.fileName().mid(11, 3).toInt();
                //renomeia o arquivo considerando o ultimo id
                logFileName = QDir::toNativeSeparators(dirName+QDir::separator()+QString("%1_%2.log").arg(QDate::currentDate().toString("yyyy_MM_dd")).arg(id, 3, 10, QLatin1Char('0')));
            }
        }

        //abre o novo arquivo de log
        m_logFile.setFileName(logFileName);
        if (!m_logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
            beginInsertRows(QModelIndex(), rowCount(), rowCount());
            LogMessage *logMsg = new LogMessage(LogMessage::MSGERROR, "Log Error", m_UserName, QObject::tr("Não foi possível criar arquivo de Log"));
            m_messages->append(logMsg);
            endInsertRows();
            emit newMessage();
            return false;
        }
        //novo arquivo de log corrente
        currentFile = logFileName;
    }

    //salva a mensagem no arquivo de log
    QTextStream out(&m_logFile);
    out << msg->toString().toUtf8() << Qt::endl;

    //se o arquivo ficou muito grande incrementa o id
    if (out.pos() > m_MaxLogFileSize) id++;

    return true;

}

bool LogMessageModel::addMessage(LogMessage *msg)
{
    QMutexLocker locker(&m_mutex);

    //verifica se a lista de log está muito grande para não comprometer a memória com os objetos de log
    //Caso o usuário desejar consultar algum registro de log passado, deverá acessar os arquivos .log na respectiva data
    if (m_messages->count() >= m_NrLogMessages) {
        clear();
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());    
    m_messages->append(msg);
    //m_messages->insert(0, msg); insere no topo da lista
    endInsertRows();

    saveLogFile(msg);

    emit newMessage();
    return true;
}

bool LogMessageModel::addMessage(QString txt, LogMessage::LogMessageType type)
{
    QMutexLocker locker(&m_mutex);

    //verifica se a lista de log está muito grande para não comprometer a memória com os objetos de log
    //Caso o usuário desejar consultar algum registro de log passado, deverá acessar os arquivos .log na respectiva data
    if (m_messages->count() >= m_NrLogMessages) {
        clear();
    }
    LogMessage *msg = new LogMessage(type, QCoreApplication::applicationName(), m_UserName, txt);

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_messages->append(msg);
    //m_messages->insert(0, msg); insere no topo da lista
    endInsertRows();

    saveLogFile(msg);

    emit newMessage();
    return true;
}

bool LogMessageModel::removeRows(int row, int count, const QModelIndex &parent)
{
    QMutexLocker locker(&m_mutex);

    beginRemoveRows(parent, row, row + count - 1);

    for (int i = count-1+row; i >= row; i--) {
        LogMessage *m = m_messages->at(i);
        m_messages->removeOne(m);
        delete m;
    }
    endRemoveRows();
    return true;
}

void LogMessageModel::clear()
{
    QMutexLocker locker(&m_mutex);

    if (m_messages->count() > 0) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        LogMessage *msg;
        while (m_messages->count()>0) {
            msg = m_messages->at(m_messages->count()-1);
            m_messages->removeOne(msg);
            delete msg;
        }
        endRemoveRows();
    }
}

QHash<int, QByteArray> LogMessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[dateTimeRole] = "dateTimeRole";
    roles[typeRole] = "typeRole";
    roles[subjectRole] = "subjectRole";
    roles[userNameRole] = "userNameRole";
    roles[messageRole] = "messageRole";

    return roles;
}

QString LogMessageModel::LogDir()
{
    QMutexLocker locker(&m_mutex);
    return m_LogDir;
}

void LogMessageModel::setLogDir(const QString &newLogDir)
{
    QMutexLocker locker(&m_mutex);
    if (m_LogDir == newLogDir)
        return;
    m_LogDir = newLogDir;
    emit LogDirChanged();
}

qint64 LogMessageModel::MaxLogFileSize()
{
    QMutexLocker locker(&m_mutex);
    return m_MaxLogFileSize;
}

void LogMessageModel::setMaxLogFileSize(qint64 newMaxLogFileSize)
{
    QMutexLocker locker(&m_mutex);
    if (m_MaxLogFileSize == newMaxLogFileSize)
        return;
    m_MaxLogFileSize = newMaxLogFileSize;
    emit MaxLogFileSizeChanged();
}

int LogMessageModel::RetentionDays()
{
    QMutexLocker locker(&m_mutex);
    return m_RetentionDays;
}

void LogMessageModel::setRetentionDays(int newRetentionDays)
{
    QMutexLocker locker(&m_mutex);
    if (m_RetentionDays == newRetentionDays)
        return;
    m_RetentionDays = newRetentionDays;
    emit RetentionDaysChanged();
}

int LogMessageModel::NrLogMessages()
{
    return m_NrLogMessages;
}

void LogMessageModel::setNrLogMessages(int newNrLogMessages)
{
    QMutexLocker locker(&m_mutex);
    if (m_NrLogMessages == newNrLogMessages)
        return;
    m_NrLogMessages = newNrLogMessages;
    emit NrLogMessagesChanged();
}

QString LogMessageModel::UserName()
{
    QMutexLocker locker(&m_mutex);
    return m_UserName;
}

void LogMessageModel::setUserName(const QString &newUserName)
{
    QMutexLocker locker(&m_mutex);
    if (m_UserName == newUserName)
        return;
    m_UserName = newUserName;
    emit UserNameChanged();
}
