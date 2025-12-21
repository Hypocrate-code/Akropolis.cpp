#include "UI/Dashboard.hpp"
#include "Jeu.hpp"
#include "Joueur.hpp"
#include <QScrollArea>
#include <QGroupBox>
#include <QStyle>

Dashboard::Dashboard(QWidget *parent)
    : QWidget(parent),
    mainLayout(new QVBoxLayout()),
    titleLabel(new QLabel("TABLEAU DE BORD")),
    currentPlayerLabel(new QLabel()),
    playerScoresLabel(new QLabel()),
    gameStatsLabel(new QLabel()),
    scrollWidget(new QWidget()),
    scrollLayout(new QVBoxLayout())
{
    setupUI();
}

Dashboard::~Dashboard()
{
}

void Dashboard::setupUI()
{
    titleLabel->setAlignment(Qt::AlignCenter);
    
    currentPlayerLabel->setAlignment(Qt::AlignLeft);
    currentPlayerLabel->setWordWrap(true);
    
    playerScoresLabel->setAlignment(Qt::AlignLeft);
    playerScoresLabel->setWordWrap(true);
    
    gameStatsLabel->setAlignment(Qt::AlignLeft);
    gameStatsLabel->setWordWrap(true);
    
    //setStyleSheet("");
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(currentPlayerLabel);
    mainLayout->addWidget(playerScoresLabel);
    mainLayout->addWidget(gameStatsLabel);
    mainLayout->addStretch();
    
    setMaximumWidth(300);
    setMinimumWidth(200);
    
    setLayout(mainLayout);
    updateDisplay();
}

void Dashboard::updateDisplay()
{
    updatePlayerInfo();
    updateScores();
    updateGameStats();
}

void Dashboard::updatePlayerInfo()
{
    Jeu* jeu = Jeu::getInstance();
    if (!jeu || jeu->getJoueurs().empty()) {
        currentPlayerLabel->setText("Aucun joueur actif");
        return;
    }
    
    Joueur* currentPlayer = jeu->getCurrentPlayer();
    if (currentPlayer) {
        QString playerInfo = QString("<b>Joueur Actuel:</b><br>%1")
            .arg(QString::fromStdString(currentPlayer->getNom()));
        currentPlayerLabel->setText(playerInfo);
    }
}

void Dashboard::updateScores()
{
    Jeu* jeu = Jeu::getInstance();
    if (!jeu || jeu->getJoueurs().empty()) {
        playerScoresLabel->setText("Scores: Non disponibles");
        return;
    }
    
    QString scoresText = "<b>Scores:</b><br>";
    for (const auto* joueur : jeu->getJoueurs()) {
        if (joueur) {
            scoresText += QString("%1: %2<br>")
                .arg(QString::fromStdString(joueur->getNom()))
                .arg(joueur->getCite()->compterPoints(jeu->getNiveauDeDifficulte()));
        }
    }
    playerScoresLabel->setText(scoresText);
}

void Dashboard::updateGameStats()
{
    Jeu* jeu = Jeu::getInstance();
    if (!jeu) {
        gameStatsLabel->setText("Statistiques: Non disponibles");
        return;
    }
    
    QString statsText = QString("<b>Statistiques:</b><br>");
    statsText += QString("Joueurs: %1<br>").arg(jeu->getJoueurs().size());
    QString niveauDefficulte;
    switch (jeu->getNiveauDeDifficulte()) {
        case 0: niveauDefficulte = "Hippodamos"; break;
        case 1: niveauDefficulte = "Métagénès"; break;
        case 2: niveauDefficulte = "Callicratès"; break;
        default: niveauDefficulte = "Inconnu"; break;
    }
    statsText += QString("Difficulté: %1").arg(niveauDefficulte);
    gameStatsLabel->setText(statsText);
}
