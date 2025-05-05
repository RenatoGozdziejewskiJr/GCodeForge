#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtadvanceddocking/DockManager.h"
#include "Log/cdockmessages.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ads::CDockManager *m_DockManager;
    CDockMessages *m_dockMessages;


};
#endif // MAINWINDOW_H
