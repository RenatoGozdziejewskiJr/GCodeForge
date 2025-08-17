#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFrame>
#include <QPushButton>
#include <QVBoxLayout>


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
    qDebug() << "Creating central widget" << this->centralWidget();
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

    //test
    // QVBoxLayout *layout = new QVBoxLayout(frame);
    // QPushButton *primaryBtn = new QPushButton("Primário");
    // primaryBtn->setProperty("class", "primary"); // ou setObjectName("primary")

    // QPushButton *secondaryBtn = new QPushButton("Secundário");
    // secondaryBtn->setProperty("class", "secondary");

    // QPushButton *tertiaryBtn = new QPushButton("Tertiary");
    // tertiaryBtn->setProperty("class", "tertiary");

    // layout->addWidget(primaryBtn);
    // layout->addWidget(secondaryBtn);
    // layout->addWidget(tertiaryBtn);


}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::loadStyleSheet(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "Não foi possível abrir o arquivo:" << fileName;
        return QString();
    }
    QTextStream stream(&file);
    return stream.readAll();
}
