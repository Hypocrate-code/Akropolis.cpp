#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "UI/HexagonalButton.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onHexagonClicked();

private:
    QStackedWidget *stackedWidget;

    QWidget *createPage(const QString &title, int pageIndex);


    QVector<HexagonalButton*> m_hexButtons;
};

#endif // MAINWINDOW_H