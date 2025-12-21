#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
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
    QLabel *titleLabel;
    QLabel *currentPlayerLabel;
    QLabel *playerScoresLabel;
    QLabel *gameStatsLabel;
    QWidget *scrollWidget;
    QVBoxLayout *scrollLayout;
};

