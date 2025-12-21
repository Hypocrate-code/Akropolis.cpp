
#include "UI/GamePushButton.hpp"

GamePushButton::GamePushButton(QWidget *parent)
    : QPushButton(parent)
{
    updateStyleSheet();
}

GamePushButton::GamePushButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)

{
    updateStyleSheet();
}
GamePushButton::GamePushButton(const QIcon& icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{
    updateStyleSheet();
}
GamePushButton::~GamePushButton()
{
}

void GamePushButton::updateStyleSheet()
{
    QString style = R"(
        QPushButton {
            background-color: #4CAF50;
            border: none;
            color: white;
            padding: 15px 32px;
            text-align: center;
            text-decoration: none;
            font-size: 16px;
            margin: 4px 2px;
            border-radius: 12px;
        }
        QPushButton:hover {
            background-color: #45a049;
        }
        QPushButton:pressed {
            background-color: #3e8e41;
        }
    )";


    setStyleSheet(style);
}