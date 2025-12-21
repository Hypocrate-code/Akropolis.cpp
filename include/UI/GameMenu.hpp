#pragma once
#include <QWidget>
#include <QMainWindow>
#include "UI/HexItem.hpp"
#include "UI/HexView.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "UI/GamePushButton.hpp"

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
    QVBoxLayout *mainLayout;

    // Uper panel - Chantier
    QBoxLayout *chantierWidget;
    QVBoxLayout *chantierLayout;
    QLabel *chantierLabel;
    HexView *hexViewChantier;
    QHBoxLayout *chantierButtonsLayout;

    // Bot panel - Cité
    QBoxLayout *citeWidget;
    QVBoxLayout *citeLayout;
    QLabel *citeLabel;
    QLabel *playerInfoLabel;
    HexView *hexviewCite;

    // Bottom panel - Control buttons
    QWidget *controlWidget;
    QHBoxLayout *controlLayout;
    QPushButton *rotateBtn;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
    QPushButton *continuerBtn;
    QPushButton *finDePartieBtn;

    // Status bar
    QLabel *statusLabel;

    uint32_t tailleHexChantier;
    uint32_t tailleHexCite;
};
