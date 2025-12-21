#pragma once
#include <QLabel>
#include <QFontDatabase>
#include <QFont>

class AkrLabel : public QLabel
{
private:
    // int idFontLight = QFontDatabase::addApplicationFont(":assets/fonts/Doto/static/Doto-Regular.ttf");
    int idFont = QFontDatabase::addApplicationFont(":assets/fonts/Jost/static/Jost-SemiBold.ttf");
public:
    AkrLabel(const char* text = "");
    ~AkrLabel() = default;
};

