#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFrame>


#include "qtadvanceddocking/AutoHideDockContainer.h"
#include "qtadvanceddocking/DockAreaWidget.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the dock manager. Because the parent parameter is a QMainWindow
    // the dock manager registers itself as the central widget.
    ads::CDockManager::setConfigFlag(ads::CDockManager::OpaqueSplitterResize, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::XmlCompressionEnabled, false);
    ads::CDockManager::setConfigFlag(ads::CDockManager::FocusHighlighting, true);
    ads::CDockManager::setAutoHideConfigFlags(ads::CDockManager::DefaultAutoHideConfig);
    m_DockManager = new ads::CDockManager(this);

    //add the central widget - a frame
    ads::CDockWidget* CentralDockWidget = new ads::CDockWidget("CentralWidget");
    QFrame* frame = new QFrame(this);
    CentralDockWidget->setWidget(frame);
    auto* CentralDockArea = m_DockManager->setCentralWidget(CentralDockWidget);
    CentralDockArea->setAllowedAreas(ads::DockWidgetArea::OuterDockAreas);

    //add the dock of the logs
    m_dockMessages = new CDockMessages(Log::logModel, m_DockManager, "Messages", this);
    ui->menu_View->addAction(m_dockMessages->toggleViewAction());

    Log::logModel->addMessage("INICIANDO SISTEMA...", LogMessage::MSGINFO);
    Log::logModel->addMessage("SYNTAX ERROR", LogMessage::MSGERROR);
    Log::logModel->addMessage("WARNING! It's time to change your life!", LogMessage::MSGWARNING);


}

MainWindow::~MainWindow()
{
    delete ui;
}
