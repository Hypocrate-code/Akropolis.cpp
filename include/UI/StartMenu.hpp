#pragma once
#include <string>
#include <QWidget>
#include <QVBoxLayout>
#include "UI/AkrPushButton.hpp"
#include "UI/AkrLabel.hpp"
#include <QLineEdit>
#include <QComboBox>
#include <array>

class StartMenu : public QWidget {
    Q_OBJECT
public:
    StartMenu(QWidget* parent = nullptr);
    ~StartMenu() override;
signals:
    void playerSelectionConfirmed(std::vector<std::string> players, uint32_t difficultyLevel, std::array<int,5>variantes);

private slots:
    void onConfirmClicked();
private:
    AkrPushButton* decBtn;
    AkrPushButton* incBtn;
    AkrLabel* countInput;

    AkrPushButton* confirmBtn;
    QVBoxLayout* layout;
    QVBoxLayout* lineEditLayout;
    
    QComboBox* dificultyComboBox;
    
    QVBoxLayout* variantesLayout;
    uint32_t playerCount;
    uint32_t dificultyLevel;
    std::vector<QWidget*> playerWidget;
    std::array<int,5> variantes;
};
