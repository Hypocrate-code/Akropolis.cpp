#include "UI/AkrLabel.hpp"

AkrLabel::AkrLabel(const char* text) : QLabel(QString(text))
{
    if (idFont != -1) {
        QString family = QFontDatabase::applicationFontFamilies(idFont).at(0);
        QFont font(family, 16);
        font.setLetterSpacing(QFont::AbsoluteSpacing, .5);
        this->setFont(font);
    }
    else {
        this->setStyleSheet("font-family: Helvetica; font-size: 16px; font-weight: bold;");
    }
    // Stylisation à voir plus tard
    this->setAlignment(Qt::AlignCenter);

}