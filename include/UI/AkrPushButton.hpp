#pragma once
#include <QPushButton>
#include <QFontDatabase>

class AkrPushButton : public QPushButton
{
    Q_OBJECT
public:
    AkrPushButton(QWidget *parent = nullptr);
    AkrPushButton(const QString &text, QWidget *parent = nullptr);
    AkrPushButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    ~AkrPushButton();

    void updateStyle();

private:
    int idFont = QFontDatabase::addApplicationFont(":assets/fonts/Jost/static/Jost-SemiBold.ttf");
};