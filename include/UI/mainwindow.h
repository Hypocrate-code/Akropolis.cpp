#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "UI/HexagonalButton.hpp"

//#include "UI/PlayerCountPage.hpp"
//#include "UI/PlayerNamePage.hpp"
#include "UI/StartMenu.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void onHexagonClicked();

private:
    QStackedWidget *stackedWidget;

    QWidget *createHomePage();
    QWidget *createStartingGamePage();
    QWidget *createGamePage();
    QWidget *createEndPage();

    StartMenu* playerNumberInput;

    QVector<HexagonalButton*> m_hexButtons;
};


#endif // MAINWINDOW_H