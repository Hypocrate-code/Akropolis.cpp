#pragma once
#include <string>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "UI/GamePushButton.hpp"
#include <QLineEdit>
#include <QComboBox>
#include <array>
#include "Jeu.hpp"



class StartMenu : public QWidget {
    Q_OBJECT
public:
    StartMenu(QWidget* parent = nullptr);
    ~StartMenu() override;
signals:
    void playerSelectionConfirmed(std::vector<std::string> players, uint32_t difficultyLevel, std::array<int,5>variantes, Duree d);

private slots:
    void onConfirmClicked();
private:
    QPushButton* decBtn;
    QPushButton* incBtn;
    QLineEdit* countInput;

    GamePushButton* confirmBtn;
    QVBoxLayout* layout;
    QVBoxLayout* lineEditLayout;
    
    QComboBox* dificultyComboBox;
    
    QVBoxLayout* variantesLayout;

    QHBoxLayout* longueurLayout;
    uint32_t playerCount;
    uint32_t dificultyLevel;
    std::array<int,5> variantes;
    Duree duree;
    std::vector<QLineEdit*> playerButtons;
};
