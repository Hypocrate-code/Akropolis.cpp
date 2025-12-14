#include "mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Akropolis");
    resize(400, 300);

    stackedWidget = new QStackedWidget(this);

    QWidget *homeScreen = createPage("Ecran titre", 0);
    QWidget *mainScreen = createPage("Affichage de la partie", 1);
    QWidget *endScreen = createPage("Fin de partie", 2);

    stackedWidget->addWidget(homeScreen);
    stackedWidget->addWidget(mainScreen);
    stackedWidget->addWidget(endScreen);

    setCentralWidget(stackedWidget);
}

QWidget *MainWindow::createPage(const QString &title, int pageIndex)
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);

    QLabel *label = new QLabel(title);

    // Stylisation à voir plus tard
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-size: 20px; font-weight: bold;");

    QPushButton *btnHomeScreen = new QPushButton("Aller à l'écran titre");
    QPushButton *btnMainScreen = new QPushButton("Aller dans la partie");
    QPushButton *btnEndScreen = new QPushButton("Aller à l'écran de fin");

    layout->addWidget(label);
    layout->addWidget(btnHomeScreen);
    layout->addWidget(btnMainScreen);
    layout->addWidget(btnEndScreen);

    // Liens entre les pages (grégaire)
    connect(btnHomeScreen, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(0);
    });

    connect(btnMainScreen, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(1);
    });

    connect(btnEndScreen, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(2);
    });

    return page;
}