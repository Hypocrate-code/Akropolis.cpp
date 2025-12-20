#pragma once
#include <QPushButton>

class GamePushButton : public QPushButton
{
    Q_OBJECT
public:
    GamePushButton(QWidget *parent = nullptr);
    GamePushButton(const QString &text, QWidget *parent = nullptr);
    GamePushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    ~GamePushButton();

    void updateStyleSheet();

};