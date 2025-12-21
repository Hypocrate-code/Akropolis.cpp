#include "UI/AkrCheckBox.hpp"


AkrCheckBox::AkrCheckBox(QString t) : QCheckBox(t)
{
    setCursor(Qt::PointingHandCursor);
    QString family = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont font(family, 14);
    font.setLetterSpacing(QFont::AbsoluteSpacing, .5);
    font.setWeight(QFont::Normal);
    this->setFont(font);
    this->setStyleSheet(R"(
        QCheckBox {
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 12px;
            height: 12px;
            border: 2px solid rgba(9, 37, 84, 1);
            border-radius: 3px;
        }
        QCheckBox::indicator:unchecked {
            background-color: white;
        }
        QCheckBox::indicator:checked {
            background-color: rgba(155, 189, 248, 1);
        }
    )");
}

AkrCheckBox::~AkrCheckBox()
{
}
