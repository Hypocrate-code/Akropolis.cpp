#include "UI/mainwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QMessageBox>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*
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
    */

        // Create central widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Title
    QLabel *title = new QLabel("Hexagonal Game Board", centralWidget);
    title->setAlignment(Qt::AlignCenter);
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    mainLayout->addWidget(title);
    
    // Create hexagon grid widget
    QWidget *gridWidget = new QWidget(centralWidget);
    QGridLayout *gridLayout = new QGridLayout(gridWidget);
    gridLayout->setSpacing(2);
    gridLayout->setContentsMargins(20, 20, 20, 20);
    
    // Create hexagonal buttons with different textures
    QStringList texturePaths = {
        "../assets/tuile_habitation.png",
        "../assets/tuile_marche.png", 
        "../assets/tuile_caserne.png",
        "../assets/tuile_temple.png",
        "../assets/tuile_jardin.png",
        "../assets/cube.png"
    };
    
    // Create 4x4 grid of hexagonal buttons
    int buttonSize = 80;
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            HexagonalButton *button = new HexagonalButton(gridWidget);
            button->setSize(buttonSize);
            
            // Offset even rows for honeycomb pattern
            int actualCol = col * 2 + (row % 2);
            
            // Set texture (cycle through available textures)
            if (texturePaths.size() > 0) {
                int textureIndex = (row * 4 + col) % texturePaths.size();
                button->setTexture(texturePaths[textureIndex]);
            }
            
            // Set tooltip
            QStringList tileNames = {"Habitation", "Market", "Barracks", "Temple", "Garden", "Stone"};
            button->setToolTip(tileNames[(row * 4 + col) % tileNames.size()]);
            
            // Connect signal
            connect(button, &HexagonalButton::clicked, this, &MainWindow::onHexagonClicked);
            
            // Add to grid with offset for honeycomb effect
            gridLayout->addWidget(button, row, actualCol);
            m_hexButtons.append(button);
        }
    }
    
    // Add some empty columns for spacing
    for (int row = 0; row < 4; ++row) {
        QWidget *spacer = new QWidget(gridWidget);
        spacer->setFixedWidth(buttonSize / 2);
        gridLayout->addWidget(spacer, row, 8);
    }
    
    mainLayout->addWidget(gridWidget, 0, Qt::AlignCenter);
    setCentralWidget(centralWidget);
    
    // Set window properties
    setWindowTitle("Qt Hexagonal Button Demo");
    resize(800, 600);

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


void MainWindow::onHexagonClicked()
{
    HexagonalButton *button = qobject_cast<HexagonalButton*>(sender());
    if (button) {
        QMessageBox::information(this, "Hexagon Clicked",
            QString("Clicked: %1").arg(button->toolTip()));
    }
}