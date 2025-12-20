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
    resize(500, 500);
    
    stackedWidget = new QStackedWidget(this);  // attribut stackedWidget initialisé

    QWidget *homeScreen = createHomePage();
    QWidget *startGameScreen = createStartingGamePage();
    QWidget *gameScreen = createGamePage();
    QWidget *endScreen = createEndPage();

    stackedWidget->addWidget(homeScreen);
    stackedWidget->addWidget(startGameScreen);
    stackedWidget->addWidget(gameScreen);
    stackedWidget->addWidget(endScreen);

    setCentralWidget(stackedWidget);
    
    // Connect startMenu vers gameScreen
    connect(static_cast<StartMenu*>(startGameScreen), &StartMenu::playerSelectionConfirmed, this, [this](std::vector<std::string> names, uint32_t difficultyLevel) {
        Jeu::getInstance()->InitialiserPartie(names, difficultyLevel);
        //jeu->setDifficultyLevel(difficultyLevel);
        
        gameMenu->updateDisplay();
        // LANCER LA PARTIE ICI
        stackedWidget->setCurrentIndex(2); // Aller à l'écran de jeu
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
        stackedWidget->setCurrentIndex(1);  // 1 -> Ecran de la partie
    });

    return page;
}

QWidget *MainWindow::createStartingGamePage()
{
    
    playerNumberInput = new StartMenu(this);
    QVBoxLayout *layout = new QVBoxLayout(playerNumberInput);

    QPushButton *btnHomeScreen = new QPushButton("Retour à l'écran titre");
    layout->addWidget(btnHomeScreen);
    // layout->addWidget(btnEndScreen);

    connect(btnHomeScreen, &QPushButton::clicked, this, [this]() {
        stackedWidget->setCurrentIndex(0); // 0 -> Accueil
    });

    // connect(btnGameScreen, &QPushButton::clicked, this, [=]() {
    //     stackedWidget->setCurrentIndex(2);  // 2 -> Ecran de la partie
    // });

    return playerNumberInput;
}

QWidget *MainWindow::createGamePage()
{
    
    gameMenu = new GameMenu(this);
    
    QVBoxLayout *layout = new QVBoxLayout(gameMenu);
    // Create hexagon grid widget
    //QWidget *page = new QWidget();
    //QVBoxLayout *mainLayout = new QVBoxLayout(page);
    QVBoxLayout *btnContainer = new QVBoxLayout();


    QPushButton *btnEndScreen = new QPushButton("Finir partie (bouton temporaire)");
    connect(btnEndScreen, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(0); // 0 -> Accueil
    });

    btnContainer->addWidget(btnEndScreen);

    layout->addLayout(btnContainer);


    // Création d'UNE vue de la scène graphique créée
    //HexView* view = new HexView(50, page);
    //view->setDrag(true);
    
    //mainLayout->addWidget(view);
    // view->launchDrawRecursive(temp5, QPoint(350, 0));


    return gameMenu;
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
        stackedWidget->setCurrentIndex(0); // 0 -> Accueil
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