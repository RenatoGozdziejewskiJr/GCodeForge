#include "cdockmessages.h"
#include <QVBoxLayout>
#include <QToolBar>
#include <QDebug>
#include <QHeaderView>
#include "qtadvanceddocking/AutoHideDockContainer.h"
#include "qtadvanceddocking/DockAreaWidget.h"


CDockMessages::CDockMessages(LogMessageModel *logModel, ads::CDockManager *manager, const QString &title, QWidget* parent): ads::CDockWidget(manager, title, parent)
{
    QWidget *dockWidgetContents = new QWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    dockWidgetContents->setContentsMargins(0,0,0,0);

    QVBoxLayout *horizontalLayout = new QVBoxLayout(dockWidgetContents);
    horizontalLayout->setContentsMargins(4, 2, 4, 2);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));

    QToolBar *tb = new QToolBar("Messages", this);
    tb->setAutoFillBackground(true);
    tb->setIconSize(QSize(24,24));

    m_log = new QTableView(dockWidgetContents);
    m_log->setObjectName(QString::fromUtf8("lstMsg"));
    m_log->setModel(logModel);

    QFont font;
    //font.setFamily("Source Code Pro");
    font.setFamily("Consolas");
    font.setPointSize(9);
    m_log->setFont(font);

    tb->addAction(QIcon(":/Resources/Trash.svg"), "Clear", [=](bool) {
        logModel->clear();
    });

    m_log->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_log->setTabKeyNavigation(false);
    m_log->setDragDropOverwriteMode(false);
    m_log->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_log->setShowGrid(false);
    m_log->setWordWrap(false);
    m_log->horizontalHeader()->setVisible(false);
    m_log->horizontalHeader()->setCascadingSectionResizes(false);
    m_log->horizontalHeader()->setDefaultSectionSize(20);
    m_log->horizontalHeader()->setHighlightSections(false);
    m_log->horizontalHeader()->setMinimumSectionSize(20);

    m_log->verticalHeader()->setVisible(false);
    m_log->verticalHeader()->setHighlightSections(false);
    //m_log->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_log->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_log->verticalHeader()->setMinimumSectionSize(18);
    m_log->verticalHeader()->setMaximumSectionSize(18);
    m_log->verticalHeader()->setDefaultSectionSize(18);

    m_log->setSelectionBehavior(QAbstractItemView::SelectRows);

    //header << "Date Time" << "Type" << "Subject" << "User" << "Message"

    m_log->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
    m_log->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeMode::ResizeToContents);
    m_log->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeMode::ResizeToContents);
    m_log->horizontalHeader()->setSectionHidden(2, true);
    m_log->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeMode::ResizeToContents);
    m_log->horizontalHeader()->setSectionHidden(3, true);
    m_log->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeMode::Stretch);
    m_log->setFocusPolicy(Qt::NoFocus);

    m_con = connect(logModel, &LogMessageModel::rowsInserted, [=](){
        m_log->scrollToBottom();
    });

    horizontalLayout->addWidget(tb);
    horizontalLayout->addWidget(m_log);

    setWidget(dockWidgetContents);

    const QColor hlClr = military03; // highlight color to set
    const QColor txtClr = desertSand; // highlighted text color to set

    QPalette p = palette();
    p.setColor(QPalette::Highlight, hlClr);
    p.setColor(QPalette::HighlightedText, txtClr);
    m_log->setPalette(p);
    m_log->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                             .arg(dark05.red())
                             .arg(dark05.green())
                             .arg(dark05.blue())
                         );

    // Add the dock widget to the top dock widget area
    const auto autohide = manager->addAutoHideDockWidget(ads::SideBarLocation::SideBarBottom, this);
    autohide->setSize(200);
    //manager->addDockWidget(ads::BottomDockWidgetArea, this);
}

CDockMessages::~CDockMessages()
{
    QObject::disconnect(m_con); //remove lambda function
}

