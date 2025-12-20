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
    
    // ===== Configuration du panel CHANTIER (en haut) =====
    chantierLayout->addWidget(chantierLabel);
    
    hexViewChantier->setMinimumSize(1200, 300);
    hexViewChantier->setMaximumHeight(350);
    chantierLayout->addWidget(hexViewChantier, 1);
    
    chantierLayout->addLayout(chantierButtonsLayout);
    chantierWidget->setLayout(chantierLayout);
    
    // ===== Configuration du panel CITÉ (en bas) =====
    
    citeLayout->addWidget(citeLabel);
    citeLayout->addWidget(playerInfoLabel);
    
    hexviewCite->setMinimumSize(1200, 400);
    citeLayout->addWidget(hexviewCite, 1);
    
    citeWidget->setLayout(citeLayout);
    
    // ===== Ajout des panels au layout principal (vertical: chantier en haut, cité en bas) =====
    mainLayout->addWidget(chantierWidget, 0);
    mainLayout->addWidget(citeWidget, 1);
    
    // ===== Configuration des boutons de contrôle =====
    rotateBtn->setEnabled(false);
    confirmBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    
    controlLayout->addWidget(rotateBtn);
    controlLayout->addWidget(confirmBtn);
    controlLayout->addWidget(cancelBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(finDePartieBtn);
    
    controlWidget->setLayout(controlLayout);
    
    // ===== Style pour le status label =====
    statusLabel->setStyleSheet("background-color: #f0f0f0; padding: 8px; font-size: 13px; border-top: 1px solid #ccc;");
    statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    statusLabel->setMinimumHeight(35);
    
    // ===== Création d'une layout verticale globale =====
    QVBoxLayout *globalLayout = new QVBoxLayout();
    globalLayout->addLayout(mainLayout, 1);
    globalLayout->addWidget(statusLabel);
    globalLayout->addWidget(controlWidget);
    globalLayout->setSpacing(5);
    globalLayout->setContentsMargins(5, 5, 5, 5);
    
    centralWidget->setLayout(globalLayout);
    
    // ===== Configuration de la fenêtre =====
    setWindowTitle("Akropolis - Partie en cours");
    resize(1400, 900);
}

void GameMenu::connectSignals()
{
    connect(rotateBtn, &QPushButton::clicked, this, &GameMenu::onRotateTuile);
    connect(confirmBtn, &QPushButton::clicked, this, &GameMenu::onConfirmPlacement);
    connect(cancelBtn, &QPushButton::clicked, this, &GameMenu::onCancelAction);

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
    updateChantier();
    updateCite();
    Jeu*j = Jeu::getInstance();
    if (j && j->getJoueurs().size() > 0) {
        Joueur* currentPlayer = j->getCurrentPlayer();
        
        if (currentPlayer) {
            uint32_t points = currentPlayer->getCite()->compterPoints(j->getNiveauDeDifficulte());
            playerInfoLabel->setText(
                QString("Joueur: %1 | Pierres: %2 | Points: %3")
                    .arg(QString::fromStdString(currentPlayer->getNom()))
                    .arg(currentPlayer->getNbPierres())
                    .arg(points)
            );
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
    
    // Vérifier si le chantier est vide
    if (j->getChantier().empty()) {
        j->mettreAJourChantier();
        statusLabel->setText("Chantier vide - Attente de mise à jour...");
    }
    
    // Nettoyer la scène précédente
    hexViewChantier->clearView();
    
    // Dessiner chaque tuile du chantier
    for (uint32_t i = 0; i < j->getChantier().size(); ++i) {
        Tuile *tuile = j->getChantier()[i];
        if (tuile) {
            const auto& hexagones = tuile->get_hexagones();
            if (!hexagones.empty()) {
                // Positionner les tuiles horizontalement
                uint32_t xOffset = i * tailleHexChantier*4;
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
    
    // Nettoyer la scène précédente
    hexviewCite->clearView();
    
    // Récupérer la première tuile comme point de départ
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

void GameMenu::onTuileSelected(const Hexagone* hex)
{
    if (!isChantierSelectionMode || !hex) {
        return;
    }
    
    Jeu* j = Jeu::getInstance();
    Joueur* currentPlayer = j->getCurrentPlayer();
    
    if (!j || !currentPlayer) {
        statusLabel->setText("Erreur: Jeu ou joueur invalide");
        return;
    }
    
    // Récupérer directement la tuile parente de l'hexagone
    Tuile* tuile = hex->getTuileParent();
    
    if (!tuile) {
        statusLabel->setText("Erreur: Tuile parente non trouvée");
        return;
    }
    
    selectedTuile = tuile;
    selectedHexChantier = hex;
    tuilePrice = j->getTuilePrice(tuile);
    
    // Vérifier si le joueur a assez de pierres
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
    
    // Passer en mode rotation/sélection de la cité
    isChantierSelectionMode = false;
    isRotationMode = true;
    isCiteSelectionMode = true;  // Permettre aussi la sélection d'un hex de cité directement
    
    rotateBtn->setEnabled(true);
    confirmBtn->setEnabled(false);  // Sera activé quand un hex de cité sera sélectionné
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
        QMessageBox::information(this, "Placement réussi", "La tuile a été placée avec succès dans votre cité.");
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
        
        updateDisplay();
        
        // Mettre à jour le chantier si nécessaire
        if (j->getChantier().size() < 1) {
            j->mettreAJourChantier();
            updateChantier();
        }
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