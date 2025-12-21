#include "UI/AkrLabel.hpp"

AkrLabel::AkrLabel(const char* text) : QLabel(QString(text))
{
    QString family = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont font(family, 14);
    font.setWeight(QFont::Normal);
    font.setLetterSpacing(QFont::AbsoluteSpacing, .5);
    this->setFont(font);

    // Stylisation à voir plus tard
    this->setAlignment(Qt::AlignCenter);

}