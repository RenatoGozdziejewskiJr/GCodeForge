#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QString buttonStyle =
    //     "QPushButton {"
    //     "   border-radius: 4px;"
    //     "   padding: 8px 16px;"
    //     "   font-size: 14px;"
    //     "}"
    //     "QPushButton.primary {"
    //     "   background-color: #1976D2;"
    //     "   color: white;"
    //     "}"
    //     "QPushButton.primary:hover {"
    //     "   background-color: #1565C0;"
    //     "}"
    //     "QPushButton.secondary {"
    //     "   background-color: transparent;"
    //     "   color: #1976D2;"
    //     "   border: 1px solid #1976D2;"
    //     "}"
    //     "QPushButton.tertiary {"
    //     "   background-color: transparent;"
    //     "   color: #1976D2;"
    //     "   text-decoration: underline;"
    //     "   border: none;"
    //     "}";

    // Aplicar o estilo globalmente
    // qApp->setStyleSheet(buttonStyle);

    MainWindow w;
    w.show();
    return a.exec();
}
