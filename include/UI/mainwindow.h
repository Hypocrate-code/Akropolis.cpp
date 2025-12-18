#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "UI/HexItem.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

// private slots:
    // void onHexagonClicked();

private:
    QStackedWidget *stackedWidget;

    QWidget *createHomePage();
    QWidget *createStartingGamePage();
    QWidget *createGamePage();
    QWidget *createEndPage();

};

#endif // MAINWINDOW_H