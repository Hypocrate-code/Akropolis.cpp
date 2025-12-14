#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    // Your slots here

private:
    QStackedWidget *stackedWidget;

    QWidget *createPage(const QString &title, int pageIndex);
};

#endif // MAINWINDOW_H