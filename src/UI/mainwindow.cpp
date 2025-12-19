#include "UI/mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>
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



    //stackedWidget->addWidget(playerNumberInput); // 4 
    //stackedWidget->addWidget(playerNameInput);// 5

    //stackedWidget->addWidget(playerNameInputScreen);
    //stackedWidget->addWidget(playerCountInputScreen);

    setCentralWidget(stackedWidget);
    
    // Connect playerNumberInput vers playerNameInput
    /*connect(playerNumberInput, &PlayerCountPage::playerCountConfirmed, this, [this](uint32_t count) {
        Jeu::getInstance()->setMaxPlayers(count);
        playerNameInput->setPlayerCount(count);
        stackedWidget->setCurrentWidget(playerNameInput);
    });

    // Connect playerNameInput vers gameScreen
    connect(playerNameInput, &PlayerNamePage::playerNamesConfirmed, this, [this](const std::vector<std::string>& names) {
        Jeu* jeu = Jeu::getInstance();
        jeu->createPlayers(names);
        
        std::cout << "Partie démarrée avec les joueurs : ";
        for (const auto& name : names) {
            std::cout << name << " ";
        }
        std::cout << std::endl;
        stackedWidget->setCurrentIndex(2); // Aller à l'écran de jeu
    });
    */



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
    // Create hexagon grid widget
    QWidget *page = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(page);
    QVBoxLayout *btnContainer = new QVBoxLayout();
    QGridLayout *gridLayout = new QGridLayout();

    QPushButton *btnEndScreen = new QPushButton("Finir partie (bouton temporaire)");
    btnContainer->addWidget(btnEndScreen);
    mainLayout->addLayout(btnContainer);
    mainLayout->addLayout(gridLayout);

    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    
    // Create hexagonal buttons with different textures
    QStringList texturePaths = {
        "../assets/tuile_habitation.png",
        "../assets/tuile_marche.png", 
        "../assets/tuile_caserne.png",
        "../assets/tuile_temple.png",
        "../assets/tuile_jardin.png",
        "../assets/cube.png"
    };
    QStringList tileNames = {"Habitation", "Marché", "Quartier", "Temple", "Jardin", "Carrière"};
    
    // Create 4x4 grid of hexagonal buttons
    int buttonSize = 80;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            HexagonalButton *button = new HexagonalButton(page);
            button->setSize(buttonSize);
            
            // Offset even rows for honeycomb pattern
            int actualCol = col * 2 + (row % 2);
            
            // Set texture (cycle through available textures)
            if (texturePaths.size() > 0) {
                int textureIndex = (row * 4 + col) % texturePaths.size();
                button->setTexture(texturePaths[textureIndex]);
            }
            
            // Set tooltip
            button->setToolTip(tileNames[(row * 4 + col) % tileNames.size()]);
            
            // Connect signal
            connect(button, &HexagonalButton::clicked, this, &MainWindow::onHexagonClicked);
            
            // Add to grid with offset for honeycomb effect
            gridLayout->addWidget(button, row, actualCol);
            m_hexButtons.append(button);
        }
    }

    connect(btnEndScreen, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(3);
    });

    return page;
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


void MainWindow::onHexagonClicked()
{
    HexagonalButton *button = qobject_cast<HexagonalButton*>(sender());
    if (button) {
        QMessageBox::information(this, "Hexagon Clicked",
            QString("Clicked: %1").arg(button->toolTip()));
    }
}
