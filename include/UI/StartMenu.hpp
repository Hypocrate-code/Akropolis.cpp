#pragma once
#include <string>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "UI/GamePushButton.hpp"
#include <QLineEdit>
#include <QComboBox>

class StartMenu : public QWidget {
    Q_OBJECT
public:
    StartMenu(QWidget* parent = nullptr);
    ~StartMenu() override;
signals:
    void playerCountConfirmed(std::vector<std::string> players );

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
    
    
    uint32_t playerCount;
    uint32_t dificultyLevel;
    std::vector<QLineEdit*> playerButtons;
};