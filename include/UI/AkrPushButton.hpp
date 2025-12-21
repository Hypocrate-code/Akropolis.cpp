#pragma once
#include <QPushButton>

class AkrPushButton : public QPushButton
{
    Q_OBJECT
public:
    AkrPushButton(QWidget *parent = nullptr);
    AkrPushButton(const QString &text, QWidget *parent = nullptr);
    AkrPushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    ~AkrPushButton();

    void updateStyle();

};