#pragma once
#include "UI/AkrLabel.hpp"
#include <QLabel>

class AkrTitle : public QLabel
{
private:
    int idFont = QFontDatabase::addApplicationFont(":assets/fonts/Doto/static/Doto-Black.ttf");
public:
    AkrTitle(const char* text = "");
    ~AkrTitle() = default;
};