#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "UI/HexItem.hpp"
#include <QCloseEvent>

//#include "UI/PlayerCountPage.hpp"
//#include "UI/PlayerNamePage.hpp"
#include "UI/StartMenu.hpp"
#include "UI/GameMenu.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

// private slots:
    // void onHexagonClicked();
//overide on close
    void closeEvent(QCloseEvent *event) override;

private:
    QStackedWidget *stackedWidget;

    QWidget *createHomePage();
    QWidget *createStartingGamePage();
    QWidget *createGamePage();
    QWidget *createEndPage();

    StartMenu* playerNumberInput;
    GameMenu* gameMenu;


    //QVector<HexagonalButton*> m_hexButtons;
};


#endif // MAINWINDOW_H