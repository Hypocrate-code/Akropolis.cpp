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

class GameMenu : public QMainWindow
{
    Q_OBJECT
public:
    GameMenu(QWidget *parent = nullptr);
    ~GameMenu();

    void updateDisplay();
    void updateChantier();
    void updateCite();

public slots:
    void onTuileSelected(int index);
    void onHexagonSelected(const Hexagone* hex);
    void onConfirmPlacement();
    void onRotateTuile();
    void onCancelAction();

private:
    void setupUI();
    void connectSignals();
    Tuile* getCurrentTuile() const;

    // Core game references
    Tuile* selectedTuile;
    const Hexagone* selectedHex;
    bool isPlacementMode;

    // Main layouts
    QWidget* centralWidget;
    QHBoxLayout *mainLayout;

    // Left panel - Chantier
    QWidget *chantierWidget;
    QVBoxLayout *chantierLayout;
    QLabel *chantierLabel;
    HexView *hexViewChantier;
    QHBoxLayout *chantierButtonsLayout;

    // Right panel - Cité
    QWidget *citeWidget;
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
    QPushButton *finDePartieBtn;

    // Status bar
    QLabel *statusLabel;
};
