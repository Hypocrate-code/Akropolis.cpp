#include "UI/GameMenu.hpp"
#include "Jeu.hpp"
#include "Joueur.hpp"
#include "Tuile.hpp"
#include "Cite.hpp"
#include <QScrollArea>
#include <QGroupBox>
#include <QSpinBox>
#include <QComboBox>
#include <iostream>
#include <QMessageBox>

GameMenu::GameMenu(QWidget *parent)
    : QMainWindow(parent), selectedTuile(nullptr),
      tailleHexChantier(77), tailleHexCite(77),
      selectedHexChantier(nullptr), selectedHexCite(nullptr),
      isChantierSelectionMode(true),
      isRotationMode(false),
      isCiteSelectionMode(false),
      tuilePrice(0),
      centralWidget(new QWidget(this)),
      mainLayout(new QVBoxLayout()),
      chantierWidget(new QWidget()),
      chantierLayout(new QVBoxLayout()),
      chantierLabel(new QLabel("CHANTIER")),
      chantierButtonsLayout(new QHBoxLayout()),
      citeWidget(new QWidget()),
      citeLayout(new QVBoxLayout()),
      citeLabel(new QLabel("CITÉ")),
      playerInfoLabel(new QLabel()),
      controlWidget(new QWidget()),
      controlLayout(new QHBoxLayout()),
      rotateBtn(new QPushButton("Tourner")),
      confirmBtn(new QPushButton("Confirmer")),
      cancelBtn(new QPushButton("Annuler")),
      continuerBtn(new QPushButton("Continuer")),
      finDePartieBtn(new QPushButton("Fin Partie")),
      statusLabel(new QLabel("Sélectionnez une tuile du chantier"))
{
      hexViewChantier = new HexView(tailleHexChantier, this);
      hexviewCite = new HexView(tailleHexCite, this);

    setupUI();
    connectSignals();
    
    // Mettre à jour l'affichage initial
    updateDisplay();
}

GameMenu::~GameMenu()
{
}

void GameMenu::setupUI()
{
    setCentralWidget(centralWidget);
    
    // Configuration du panel CHANTIER
    chantierLayout->addWidget(chantierLabel);
    
    hexViewChantier->setMinimumSize(1200, 300);
    hexViewChantier->setMaximumHeight(350);
    chantierLayout->addWidget(hexViewChantier, 1);
    
    chantierLayout->addLayout(chantierButtonsLayout);
    chantierWidget->setLayout(chantierLayout);
    
    // Configuration du panel CITÉ
    
    citeLayout->addWidget(citeLabel);
    citeLayout->addWidget(playerInfoLabel);
    
    hexviewCite->setMinimumSize(1200, 400);
    citeLayout->addWidget(hexviewCite, 1);
    
    citeWidget->setLayout(citeLayout);
    
    // Ajout des panels au layout principal
    mainLayout->addWidget(chantierWidget, 0);
    mainLayout->addWidget(citeWidget, 1);
    
    // Configuration des boutons de contrôle
    rotateBtn->setEnabled(false);
    confirmBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    continuerBtn->setVisible(false);
    
    controlLayout->addWidget(rotateBtn);
    controlLayout->addWidget(confirmBtn);
    controlLayout->addWidget(cancelBtn);
    controlLayout->addWidget(continuerBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(finDePartieBtn);
    
    controlWidget->setLayout(controlLayout);
    
    // Style pour le status label
    statusLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; font-size: 13px; border-top: 1px solid #ccc;");
    statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    statusLabel->setMinimumHeight(35);
    
    // Layout global
    QVBoxLayout *globalLayout = new QVBoxLayout();
    globalLayout->addLayout(mainLayout, 1);
    globalLayout->addWidget(statusLabel);
    globalLayout->addWidget(controlWidget);
    globalLayout->setSpacing(5);
    globalLayout->setContentsMargins(5, 5, 5, 5);
    
    centralWidget->setLayout(globalLayout);
    
    // Configuration de la fenêtre
    setWindowTitle("Akropolis - Partie en cours");
    resize(1400, 900);
}

void GameMenu::connectSignals()
{
    connect(rotateBtn, &QPushButton::clicked, this, &GameMenu::onRotateTuile);
    connect(confirmBtn, &QPushButton::clicked, this, &GameMenu::onConfirmPlacement);
    connect(cancelBtn, &QPushButton::clicked, this, &GameMenu::onCancelAction);
    connect(continuerBtn, &QPushButton::clicked, this, &GameMenu::onContinuerTour);
    connect(finDePartieBtn, &QPushButton::clicked, this, &GameMenu::onFinDePartie);

    connect(hexviewCite, &HexView::hexagonClicked, this, &GameMenu::onHexagonSelected);
    
    // Clic sur un hex du chantier pour sélectionner une tuile
    connect(hexViewChantier, &HexView::hexagonClicked, this, [this](const Hexagone* hex) {
        qDebug() << "Chantier hexagon clicked";
        if (isChantierSelectionMode && hex) {
            onTuileSelected(hex);
        }
    });
}

void GameMenu::updateDisplay()
{
    Jeu*j = Jeu::getInstance();
    if (!j || j->getJoueurs().empty()) {
        return;
    }

    Joueur* currentPlayer = j->getCurrentPlayer();
    if (!currentPlayer) {
        return;
    }

    // Vérifier si c'est le tour de l'Illustre Architecte
    bool isIlluTurn = (currentPlayer->getNom() == "Illustre Architecte");

    if (isIlluTurn) {
        // Mode affichage Illustre Architecte
        rotateBtn->setVisible(false);
        confirmBtn->setVisible(false);
        cancelBtn->setVisible(false);
        continuerBtn->setVisible(true);
        continuerBtn->setEnabled(true);
        
        hexViewChantier->setEnabled(false);
        hexviewCite->setEnabled(false);
        
        // Exécuter le tour de l'Illustre
        j->executerTourIllu();
        
        updateChantier();
        updateCiteIllu();
        
        uint32_t points = currentPlayer->getCite()->compterPoints(j->getNiveauDeDifficulte());
        playerInfoLabel->setText(
            QString("Tour de l'Illustre Architecte | Pierres: %1 | Points: %2")
                .arg(currentPlayer->getNbPierres())
                .arg(points)
        );
        
        statusLabel->setText("L'Illustre Architecte a joué son tour - Cliquez sur Continuer");
    } else {
        // Mode joueur normal
        rotateBtn->setVisible(true);
        confirmBtn->setVisible(true);
        cancelBtn->setVisible(true);
        continuerBtn->setVisible(false);
        
        hexViewChantier->setEnabled(true);
        hexviewCite->setEnabled(true);
        
        updateChantier();
        updateCite();
        
        uint32_t points = currentPlayer->getCite()->compterPoints(j->getNiveauDeDifficulte());
        playerInfoLabel->setText(
            QString("Tour de: %1 | Pierres: %2 | Points: %3")
                .arg(QString::fromStdString(currentPlayer->getNom()))
                .arg(currentPlayer->getNbPierres())
                .arg(points)
        );
        
        if (isChantierSelectionMode) {
            statusLabel->setText("Sélectionnez une tuile du chantier");
        }
    }
}

void GameMenu::updateChantier()
{
    Jeu* j = Jeu::getInstance();
    if (!j) {
        statusLabel->setText("Jeu non initialisé");
        return;
    }
    
    if (j->getChantier().empty()) {
        j->mettreAJourChantier();
        statusLabel->setText("Chantier vide - Attente de mise à jour...");
    }
    
    hexViewChantier->clearView();
    
    uint32_t numTuiles = j->getChantier().size();
    uint32_t totalWidth = numTuiles * tailleHexChantier * 4;
    uint32_t viewportWidth = hexViewChantier->width();
    int startX = (viewportWidth > totalWidth) ? (viewportWidth - totalWidth) / 2 : 50;
    
    for (uint32_t i = 0; i < numTuiles; ++i) {
        Tuile *tuile = j->getChantier()[i];
        if (tuile) {
            const auto& hexagones = tuile->get_hexagones();
            if (!hexagones.empty()) {
                uint32_t xOffset = startX + i * tailleHexChantier * 4;
                hexViewChantier->launchDrawRecursive(hexagones[0], QPoint(xOffset, 100));
            }
        }
    }
    
    statusLabel->setText(
        QString("Chantier: %1 tuile(s) disponible(s)")
            .arg(static_cast<int>(j->getChantier().size()))
    );
}

void GameMenu::updateCite()
{
    Jeu* j = Jeu::getInstance();
    Joueur* currentPlayer = j->getCurrentPlayer();
    if (!j || !currentPlayer){
        statusLabel->setText("Jeu ou joueur non initialisé");
        return;
    } 
    
    Cite *cite = currentPlayer->getCite();
    if (!cite){
        statusLabel->setText("Cité du joueur non trouvée.");
        return;
    }
    
    hexviewCite->clearView();
    
    std::vector<const Tuile*> tuiles = cite->getTuiles();
    if (tuiles.empty()) {
        statusLabel->setText("Cité vide");
        return;
    }
   
    const Tuile *firstTuile = tuiles[0];
    if (firstTuile) {
        const auto& hexagones = firstTuile->get_hexagones();
        if (!hexagones.empty()) {
            hexviewCite->launchDrawRecursive(
                hexagones[0], 
                QPoint(400, 200)
            );
        }
    }
}

void GameMenu::updateCiteIllu()
{
    Jeu* j = Jeu::getInstance();
    Joueur* currentPlayer = j->getCurrentPlayer();
    if (!j || !currentPlayer) {
        return;
    }
    
    Cite* cite = currentPlayer->getCite();
    if (!cite) {
        return;
    }
    
    hexviewCite->clearView();
    
    std::vector<const Tuile*> tuiles = cite->getTuiles();
    if (tuiles.empty()) {
        return;
    }
    
    uint32_t numTuiles = tuiles.size();
    uint32_t totalWidth = numTuiles * tailleHexCite * 4;
    uint32_t viewportWidth = hexviewCite->width();
    int startX = (viewportWidth > totalWidth) ? (viewportWidth - totalWidth) / 2 : 50;
    
    for (size_t i = 0; i < tuiles.size(); ++i) {
        const Tuile* tuile = tuiles[i];
        if (tuile) {
            const auto& hexagones = tuile->get_hexagones();
            if (!hexagones.empty()) {
                uint32_t xOffset = startX + i * tailleHexCite * 4;
                hexviewCite->launchDrawRecursive(hexagones[0], QPoint(xOffset, 200));
            }
        }
    }
}

void GameMenu::onTuileSelected(const Hexagone* hex)
{
    if (!hex) {
        return;
    }
    
    Jeu* j = Jeu::getInstance();
    Joueur* currentPlayer = j->getCurrentPlayer();
    
    if (!j || !currentPlayer) {
        statusLabel->setText("Erreur: Jeu ou joueur invalide");
        return;
    }
    
    Tuile* tuile = hex->getTuileParent();
    
    if (!tuile) {
        statusLabel->setText("Erreur: Tuile parente non trouvée");
        return;
    }
    
    selectedTuile = tuile;
    selectedHexChantier = hex;
    tuilePrice = j->getTuilePrice(tuile);
    
    if (currentPlayer->getNbPierres() < tuilePrice) {
        statusLabel->setText(
            QString("Pas assez de pierres! Prix: %1, Vous avez: %2")
                .arg(tuilePrice)
                .arg(currentPlayer->getNbPierres())
        );
        selectedTuile = nullptr;
        selectedHexChantier = nullptr;
        return;
    }
    
    isRotationMode = true;
    isCiteSelectionMode = true;
    
    rotateBtn->setEnabled(true);
    confirmBtn->setEnabled(false);
    cancelBtn->setEnabled(true);
    
    statusLabel->setText(
        QString("Tuile sélectionnée (Prix: %1 pierre(s)) - Tournez si nécessaire, puis sélectionnez un hex de la cité")
            .arg(tuilePrice)
    );
}

void GameMenu::onHexagonSelected(const Hexagone* hex)
{
    if (!hex) {
        return;
    }
    
    // Mode sélection d'un hex de la cité pour le placement
    if (isCiteSelectionMode && selectedTuile && selectedHexChantier) {
        // Vérifier que c'est un hex fantôme
        if (hex->getType() != Type::Fantome) {
            statusLabel->setText("Erreur: Cet hexagone n'est pas un hex fantôme, c'est un " +
                QString::fromStdString(
                    (hex->getType() == Type::Carriere) ? "Carrière" :
                    (hex->getType() == Type::Quartier) ? "Quartier" :
                    (hex->getType() == Type::Place) ? "Place" : "Inconnu"
                )+ "lhex du dessus est " + QString::fromStdString(Utils::type_to_string(hex->getVoisinsTOP()->getType())) 
            );
            return;
        }
        
        selectedHexCite = hex;
        statusLabel->setText("Hex de cité sélectionné - Confirmez le placement");
        confirmBtn->setEnabled(true);
        return;
    }
}

void GameMenu::onConfirmPlacement()
{
    Jeu* j = Jeu::getInstance();
    Joueur* currentPlayer = j->getCurrentPlayer();
    
    if (!j || !currentPlayer) {
        statusLabel->setText("Erreur: Jeu ou joueur invalide");
        return;
    }
    
    // Vérifier que nous avons sélectionné une tuile et un hex de placement
    if (!selectedTuile || !selectedHexChantier || !selectedHexCite) {
        statusLabel->setText("Erreur: sélection incomplète");
        return;
    }
    
    // Déléguer tout au Jeu (placement + déduction pierres + retrait chantier)
    bool success = j->executerPlacementTuile(
        currentPlayer,
        selectedTuile,
        const_cast<Hexagone*>(selectedHexChantier),
        const_cast<Hexagone*>(selectedHexCite)
    );
    
    if (success) {
        statusLabel->setText("Tuile placée avec succès!");
        
        // Réinitialiser les modes
        selectedTuile = nullptr;
        selectedHexChantier = nullptr;
        selectedHexCite = nullptr;
        isChantierSelectionMode = true;
        isRotationMode = false;
        isCiteSelectionMode = false;
        tuilePrice = 0;
        
        rotateBtn->setEnabled(false);
        confirmBtn->setEnabled(false);
        cancelBtn->setEnabled(false);
        
        // Vérifier si la partie est terminée
        if (j->isGameOver()) {
            // Afficher le gagnant
            Joueur* winner = j->getWinner();
            QString message = "Partie terminée!\n\nRésultats:\n\n";
            
            for (const auto& joueur : j->getJoueurs()) {
                if (joueur->getNom() != "Illustre Architecte") {
                    uint32_t points = joueur->getCite()->compterPoints(j->getNiveauDeDifficulte());
                    message += QString("%1: %2 points\n")
                        .arg(QString::fromStdString(joueur->getNom()))
                        .arg(points);
                }
            }
            
            if (winner) {
                message += QString("\nGagnant: %1")
                    .arg(QString::fromStdString(winner->getNom()));
            }
            
            QMessageBox::information(this, "Fin de partie", message);
            
            // Désactiver tous les contrôles
            rotateBtn->setEnabled(false);
            confirmBtn->setEnabled(false);
            cancelBtn->setEnabled(false);
            hexViewChantier->setEnabled(false);
            hexviewCite->setEnabled(false);
            
            return;
        }
        
        // Passer au joueur suivant
        j->nextPlayer();
        
        // Mettre à jour le chantier si nécessaire
        if (j->getChantier().size() < 1) {
            j->mettreAJourChantier();
        }
        
        updateDisplay();
        
        QMessageBox::information(this, "Tour terminé", 
            QString("Tuile placée avec succès!\n\nC'est maintenant au tour de: %1")
                .arg(QString::fromStdString(j->getCurrentPlayer()->getNom())));
        
    } else {
        statusLabel->setText("Placement invalide - Vérifiez les connexions");
    }
}

void GameMenu::onRotateTuile()
{
    if (!selectedTuile || !isRotationMode) {
        statusLabel->setText("Impossible de tourner - Aucune tuile en mode rotation");
        return;
    }
    
    selectedTuile->rotate();
    statusLabel->setText("Tuile tournée - Sélectionnez un hex de la cité pour placer");
    
    // Rester en mode sélection de cité
    isCiteSelectionMode = true;
    rotateBtn->setEnabled(true);  // Permettre de tourner à nouveau si nécessaire
    
    // Redessiner le chantier pour montrer la rotation
    updateChantier();
}

void GameMenu::onCancelAction()
{
    selectedTuile = nullptr;
    selectedHexChantier = nullptr;
    selectedHexCite = nullptr;
    isChantierSelectionMode = true;
    isRotationMode = false;
    isCiteSelectionMode = false;
    tuilePrice = 0;
    
    rotateBtn->setEnabled(false);
    confirmBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    
    statusLabel->setText("Action annulée - Sélectionnez une tuile du chantier");
    updateChantier();
}

Tuile* GameMenu::getCurrentTuile() const
{
    return selectedTuile;
}

void GameMenu::onContinuerTour()
{
    Jeu* j = Jeu::getInstance();
    if (!j) {
        return;
    }
    
    // Vérifier si la partie est terminée
    if (j->isGameOver()) {
        // Afficher le gagnant
        Joueur* winner = j->getWinner();
        QString message = "Partie terminée!\n\nRésultats:\n\n";
        
        for (const auto& joueur : j->getJoueurs()) {
            uint32_t points = joueur->getCite()->compterPoints(j->getNiveauDeDifficulte());
            message += QString("%1: %2 points\n")
                .arg(QString::fromStdString(joueur->getNom()))
                .arg(points);
        }
        
        if (winner) {
            message += QString("\nGagnant: %1")
                .arg(QString::fromStdString(winner->getNom()));
        }
        
        QMessageBox::information(this, "Fin de partie", message);
        
        // Désactiver tous les contrôles
        rotateBtn->setEnabled(false);
        confirmBtn->setEnabled(false);
        cancelBtn->setEnabled(false);
        continuerBtn->setEnabled(false);
        hexViewChantier->setEnabled(false);
        hexviewCite->setEnabled(false);
        
        return;
    }
    
    // Passer au joueur suivant
    j->nextPlayer();
    
    // Mettre à jour le chantier si nécessaire
    if (j->getChantier().size() < 1) {
        j->mettreAJourChantier();
    }
    
    updateDisplay();
}

void GameMenu::onFinDePartie()
{
    Jeu* j = Jeu::getInstance();
    if (!j) {
        return;
    }
    
    // Demander confirmation
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "Fin de partie", 
        "Êtes-vous sûr de vouloir terminer la partie maintenant ?",
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply != QMessageBox::Yes) {
        return;
    }
    
    // Afficher les résultats finaux
    QString message = "Partie terminée!\n\nRésultats finaux:\n\n";
    
    Joueur* winner = nullptr;
    uint32_t maxPoints = 0;
    
    for (const auto& joueur : j->getJoueurs()) {
        if (joueur->getNom() != "Illustre Architecte") {
            uint32_t points = joueur->getCite()->compterPoints(j->getNiveauDeDifficulte());
            message += QString("%1: %2 points\n")
                .arg(QString::fromStdString(joueur->getNom()))
                .arg(points);
            
            if (points > maxPoints) {
                maxPoints = points;
                winner = joueur;
            }
        }
    }
    
    if (winner) {
        message += QString("\nGagnant: %1 avec %2 points!")
            .arg(QString::fromStdString(winner->getNom()))
            .arg(maxPoints);
    }
    
    QMessageBox::information(this, "Résultats finaux", message);
    
    // Désactiver tous les contrôles
    rotateBtn->setEnabled(false);
    confirmBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    finDePartieBtn->setEnabled(false);
    hexViewChantier->setEnabled(false);
    hexviewCite->setEnabled(false);
    
    statusLabel->setText("Partie terminée - Merci d'avoir joué!");
}