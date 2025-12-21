#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include "UI/AkrLabel.hpp"
#include "UI/AkrTitle.hpp"
#include <QList>

class Joueur;

class Dashboard : public QWidget
{
    Q_OBJECT
public:
    Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    void updateDisplay();
    void updatePlayerInfo();
    void updateScores();
    void updateGameStats();

private:
    void setupUI();

    QVBoxLayout *mainLayout;
    AkrTitle *titleLabel;
    AkrLabel *currentPlayerLabel;
    AkrLabel *playerScoresLabel;
    AkrLabel *gameStatsLabel;
    QWidget *scrollWidget;
    QVBoxLayout *scrollLayout;
};

