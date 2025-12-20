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
      selectedHex(nullptr), isPlacementMode(false),
      centralWidget(new QWidget(this)),
      mainLayout(new QHBoxLayout()),
      chantierWidget(new QWidget()),
      chantierLayout(new QVBoxLayout()),
      chantierLabel(new QLabel("CHANTIER")),
      hexViewChantier(new HexView(100, this)),
      chantierButtonsLayout(new QHBoxLayout()),
      citeWidget(new QWidget()),
      citeLayout(new QVBoxLayout()),
      citeLabel(new QLabel("CITÉ")),
      playerInfoLabel(new QLabel()),
      hexviewCite(new HexView(100, this)),
      controlWidget(new QWidget()),
      controlLayout(new QHBoxLayout()),
      rotateBtn(new QPushButton("Tourner")),
      confirmBtn(new QPushButton("Confirmer")),
      cancelBtn(new QPushButton("Annuler")),
      finDePartieBtn(new QPushButton("Fin Partie")),
      statusLabel(new QLabel("Sélectionnez une tuile du chantier"))
{
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
    centralWidget->setLayout(mainLayout);
    
    // ===== Configuration du panel CHANTIER (gauche) =====
    chantierLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    chantierLayout->addWidget(chantierLabel);
    
    QScrollArea *chantierScroll = new QScrollArea();
    chantierScroll->setWidget(hexViewChantier);
    chantierScroll->setWidgetResizable(true);
    chantierLayout->addWidget(chantierScroll, 1);
    
    chantierLayout->addLayout(chantierButtonsLayout);
    chantierWidget->setLayout(chantierLayout);
    chantierWidget->setMaximumWidth(400);
    
    // ===== Configuration du panel CITÉ (droite) =====
    citeLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    playerInfoLabel->setStyleSheet("font-size: 12px; color: #555;");
    
    citeLayout->addWidget(citeLabel);
    citeLayout->addWidget(playerInfoLabel);
    
    QScrollArea *citeScroll = new QScrollArea();
    citeScroll->setWidget(hexviewCite);
    citeScroll->setWidgetResizable(true);
    citeLayout->addWidget(citeScroll, 1);
    
    citeWidget->setLayout(citeLayout);
    
    // ===== Ajout des panels au layout principal =====
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
    
    // ===== Création d'une layout verticale globale =====
    QVBoxLayout *globalLayout = new QVBoxLayout();
    globalLayout->addLayout(mainLayout, 1);
    globalLayout->addWidget(statusLabel);
    globalLayout->addWidget(controlWidget);
    
    centralWidget->setLayout(globalLayout);
    
    // ===== Configuration de la fenêtre =====
    setWindowTitle("Akropolis - Partie en cours");
    resize(1400, 800);
}

void GameMenu::connectSignals()
{
    connect(rotateBtn, &QPushButton::clicked, this, &GameMenu::onRotateTuile);
    connect(confirmBtn, &QPushButton::clicked, this, &GameMenu::onConfirmPlacement);
    connect(cancelBtn, &QPushButton::clicked, this, &GameMenu::onCancelAction);
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

        hexViewChantier->clearView();
        statusLabel->setText("Chantier vide - Attente de mise à jour...");
        return;
    }
    
    // Nettoyer la scène précédente
    hexViewChantier->clearView();
    
    // Dessiner chaque tuile du chantier
    for (size_t i = 0; i < j->getChantier().size(); ++i) {
        Tuile *tuile = j->getChantier()[i];
        if (tuile) {
            const auto& hexagones = tuile->get_hexagones();
            if (!hexagones.empty()) {
                // Positionner les tuiles horizontalement
                int xOffset = 150 + static_cast<int>(i) * 120;
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
        QMessageBox::warning(this, "Erreur", "Jeu ou joueur courant non initialisé.");
        return;
    } 
    
    Cite *cite = currentPlayer->getCite();
    if (!cite){
        QMessageBox::warning(this, "Erreur", "Cité du joueur non trouvée.");
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

void GameMenu::onTuileSelected(int index)
{
    Jeu* j = Jeu::getInstance();
    if (!j || index < 0 || index >= static_cast<int>(j->getChantier().size())) {
        selectedTuile = nullptr;
        rotateBtn->setEnabled(false);
        statusLabel->setText("Sélection invalide");
        return;
    }
    
    selectedTuile = j->getChantier()[index];
    isPlacementMode = true;
    
    rotateBtn->setEnabled(true);
    confirmBtn->setEnabled(true);
    cancelBtn->setEnabled(true);
    
    statusLabel->setText(
        QString("Tuile %1 sélectionnée - Cliquez sur la cité pour placer")
            .arg(index)
    );
}

void GameMenu::onHexagonSelected(const Hexagone* hex)
{
    if (!isPlacementMode || !selectedTuile) {
        return;
    }
    
    selectedHex = hex;
    statusLabel->setText("Hexagone sélectionné - Confirmez le placement");
    
    confirmBtn->setEnabled(true);
}

void GameMenu::onConfirmPlacement()
{
    Jeu* j = Jeu::getInstance();
    Joueur* currentPlayer = j->getCurrentPlayer();
    if (!selectedTuile || !selectedHex || !currentPlayer) {
        statusLabel->setText("Erreur: sélection incomplète");
        return;
    }
    
    CiteJoueur* citeJoueur = dynamic_cast<CiteJoueur*>(currentPlayer->getCite());
    if (!citeJoueur) {
        statusLabel->setText("Erreur: cité joueur invalide");
        return;
    }
    
    // Trouver l'hexagone de référence dans la tuile sélectionnée
    const auto& hexagones = selectedTuile->get_hexagones();
    for (const Hexagone* hex : hexagones) {
        if (hex == selectedHex) {
            bool success = citeJoueur->placerTuileFromHexRef(const_cast<Hexagone*>(hex));
            
            if (success) {
                statusLabel->setText("Tuile placée avec succès!");
                
                // Retirer la tuile du chantier
                j->removeTuileFromChantier(selectedTuile);
                
                // Mettre à jour l'affichage
                selectedTuile = nullptr;
                selectedHex = nullptr;
                isPlacementMode = false;
                
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
            return;
        }
    }
}

void GameMenu::onRotateTuile()
{
    if (!selectedTuile) {
        statusLabel->setText("Aucune tuile sélectionnée");
        return;
    }
    
    selectedTuile->rotate();
    statusLabel->setText("Tuile tournée");
    
    // Redessiner le chantier pour montrer la rotation
    updateChantier();
}

void GameMenu::onCancelAction()
{
    selectedTuile = nullptr;
    selectedHex = nullptr;
    isPlacementMode = false;
    
    rotateBtn->setEnabled(false);
    confirmBtn->setEnabled(false);
    cancelBtn->setEnabled(false);
    
    statusLabel->setText("Action annulée");
}

Tuile* GameMenu::getCurrentTuile() const
{
    return selectedTuile;
}