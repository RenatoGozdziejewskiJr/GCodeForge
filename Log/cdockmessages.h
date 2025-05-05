#ifndef CDOCKMESSAGES_H
#define CDOCKMESSAGES_H

#include <QObject>
#include <QTableView>
#include "logmessagemodel.h"
#include "qtadvanceddocking/DockManager.h"


class CDockMessages : public ads::CDockWidget
{
    Q_OBJECT

public:
    explicit CDockMessages(LogMessageModel *logModel, ads::CDockManager *manager, const QString &title, QWidget* parent = nullptr);
    ~CDockMessages();

private:
    QTableView *m_log;
    QMetaObject::Connection m_con;

signals:

};

#endif // CDOCKMESSAGES_H
