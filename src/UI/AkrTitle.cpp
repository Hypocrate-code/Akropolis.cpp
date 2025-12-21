#include "UI/AkrTitle.hpp"

AkrTitle::AkrTitle(const char* text) : QLabel(QString(text))
{
    if (idFont != -1) {
        QString family = QFontDatabase::applicationFontFamilies(idFont).at(0);
        QFont font(family, 22);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 1);
        this->setFont(font);
    }
    else {
        this->setStyleSheet("font-family: Helvetica; font-size: 18px; font-weight: bold;");
    }
    // Stylisation à voir plus tard
    this->setAlignment(Qt::AlignCenter);

}
