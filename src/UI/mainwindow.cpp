#include "UI/mainwindow.h"
#include "UI/HexItem.hpp"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>

#include "UI/HexView.hpp"
#include "Tuile.hpp"

#include <QLineEdit>
#include "Jeu.hpp"
#include <UI/GamePushButton.hpp>

MainWindow::MainWindow()
    : QMainWindow()
{

    setWindowTitle("Akropolis");
    resize(400, 400);
    
    mainLayout = new QStackedWidget(this);  // attribut mainLayout initialisé

    QWidget *homeScreen = createHomePage();
    QWidget *startGameScreen = createStartingGamePage();
    QWidget *gameScreen = createGamePage();
    QWidget *endScreen = createEndPage();

    mainLayout->addWidget(homeScreen);
    mainLayout->addWidget(startGameScreen);
    mainLayout->addWidget(gameScreen);
    mainLayout->addWidget(endScreen);

    setCentralWidget(mainLayout);
    
    // Connect startMenu vers gameScreen
    connect(static_cast<StartMenu*>(playerNumberInput), &StartMenu::playerSelectionConfirmed, this, [this](std::vector<std::string> names, uint32_t difficultyLevel) {
        Jeu* jeu = Jeu::getInstance();
        jeu->setQtDisplay(true);
        jeu->InitialiserPartie(names, difficultyLevel);
        
        gameMenu->updateDisplay();
        // LANCER LA PARTIE ICI
        mainLayout->setCurrentIndex(2); // Aller à l'écran de jeu
    });

    connect(gameMenu, &GameMenu::returnToMainMenu, this, [this]() {
        mainLayout->setCurrentIndex(0);
    });

}

MainWindow::~MainWindow()
{
    // sur une autre branche
    //Jeu::getInstance()->EndGame();
}

QWidget *MainWindow::createHomePage()
{
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    const QString text("Bienvenue sur Akropolis.cpp !"); 
    QLabel *label = new QLabel(text);

    // Stylisation à voir plus tard
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-family: Helvetica; font-size: 20px; font-weight: bold;");

    QPushButton *btnGameScreen = new QPushButton("Jouer une partie");

    layout->addWidget(label);
    layout->addWidget(btnGameScreen);

    connect(btnGameScreen, &QPushButton::clicked, this, [this]() {
        mainLayout->setCurrentIndex(1);  // 1 -> Ecran de la partie
    });

    return page;
}

QWidget *MainWindow::createStartingGamePage()
{
    QWidget *page = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(page);
    playerNumberInput = new StartMenu(page);
    
    QPushButton *btnHomeScreen = new QPushButton("Retour à l'écran titre");
    layout->addWidget(playerNumberInput);
    layout->addWidget(btnHomeScreen);
    connect(btnHomeScreen, &QPushButton::clicked, this, [this]() {
        mainLayout->setCurrentIndex(0); // 0 -> Accueil
    });
    
    return page;
}

QWidget *MainWindow::createGamePage()
{
    
    gameMenu = new GameMenu(this);
    return gameMenu;
    // Create hexagon grid widget
    //QWidget *page = new QWidget();
    //QVBoxLayout *mainLayout = new QVBoxLayout(page);
    // QVBoxLayout *btnContainer = new QVBoxLayout();


    // QPushButton *btnEndScreen = new QPushButton("Finir partie (bouton temporaire)");
    // connect(btnEndScreen, &QPushButton::clicked, this, [=]() {
    //     mainLayout->setCurrentIndex(0); // 0 -> Accueil
    // });

    // btnContainer->addWidget(btnEndScreen);
    // Création d'UNE vue de la scène graphique créée
    //HexView* view = new HexView(50, page);
    //view->setDrag(true);
    
    //mainLayout->addWidget(view);
    // view->launchDrawRecursive(temp5, QPoint(350, 0));


}

QWidget *MainWindow::createEndPage()
{
    
    QWidget *page = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(page);
    
    const QString text("Fin de la partie"); 
    QLabel *label = new QLabel(text);

    // Stylisation à voir plus tard
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-family: Helvetica; font-size: 20px; font-weight: bold;");

    QPushButton *btnHomeScreen = new QPushButton("Retour à l'écran titre");

    layout->addWidget(label);
    layout->addWidget(btnHomeScreen);

    connect(btnHomeScreen, &QPushButton::clicked, this, [=]() {
        mainLayout->setCurrentIndex(0); // 0 -> Accueil
    });

    return page;
}


// void MainWindow::onHexagonClicked()
// {
//     HexagonalButton *button = qobject_cast<HexagonalButton*>(sender());
//     if (button) {
//         QMessageBox::information(this, "Hexagon Clicked",
//             QString("Clicked: %1").arg(button->toolTip()));
//     }
// }


void MainWindow::closeEvent(QCloseEvent *event)
{
    // Handle any cleanup before closing
    Jeu::getInstance()->EndGame();
    event->accept(); // Accept the close event
}