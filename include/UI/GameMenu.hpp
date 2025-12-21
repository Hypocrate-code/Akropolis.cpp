#pragma once
#include <QWidget>
#include <QMainWindow>
#include "UI/HexItem.hpp"
#include "UI/HexView.hpp"
#include "UI/Dashboard.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "UI/AkrLabel.hpp"
#include "UI/AkrTitle.hpp"
#include "UI/AkrPushButton.hpp"

class Jeu;
class Joueur;
class Tuile;

class GameMenu : public QWidget
{
    Q_OBJECT
public:
    GameMenu(QWidget *parent = nullptr);
    ~GameMenu();

    void updateDisplay();
    void updateChantier();
    void updateCite();
    void updateCiteIllu();

signals:
    void returnToMainMenu();

public slots:
    void onTuileSelected(const Hexagone* hex);
    void onHexagonSelected(const Hexagone* hex);
    void onConfirmPlacement();
    void onRotateTuile();
    void onCancelAction();
    void onContinuerTour();
    void onFinDePartie();

private:
    void setupUI();
    void connectSignals();
    Tuile* getCurrentTuile() const;

    // Core game references
    Tuile* selectedTuile;
    const Hexagone* selectedHexChantier;  // hex sélectionné du chantier
    const Hexagone* selectedHexCite;      // hex sélectionné de la cité
    bool isChantierSelectionMode;         // mode sélection d'une tuile du chantier
    bool isRotationMode;                  // mode rotation après sélection d'une tuile
    bool isCiteSelectionMode;             // mode sélection du placement dans la cité
    int tuilePrice;                       // prix de la tuile sélectionnée

    // Main layouts
    QHBoxLayout *mainLayout;

    // Uper panel - Chantier
    QBoxLayout *chantierWidget;
    QVBoxLayout *chantierLayout;
    AkrTitle *chantierLabel;
    HexView *hexViewChantier;
    QHBoxLayout *chantierButtonsLayout;

    // Bot panel - Cité
    QBoxLayout *citeWidget;
    QVBoxLayout *citeLayout;
    AkrTitle *citeLabel;
    AkrLabel *playerInfoLabel;
    HexView *hexViewCite;

    // Bottom panel - Control buttons
    QWidget *controlWidget;
    QHBoxLayout *controlLayout;
    AkrPushButton *rotateBtn;
    AkrPushButton *confirmBtn;
    AkrPushButton *cancelBtn;
    AkrPushButton *continuerBtn;
    AkrPushButton *finDePartieBtn;

    // Status bar
    AkrLabel *statusLabel;

    // Dashboard
    Dashboard *dashboard;

    uint32_t tailleHexChantier;
    uint32_t tailleHexCite;
};
