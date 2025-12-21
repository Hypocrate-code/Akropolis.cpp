#include "UI/AkrCheckBox.hpp"


AkrCheckBox::AkrCheckBox(QString t) : QCheckBox(t)
{
    if (idFont != -1) {
        QString family = QFontDatabase::applicationFontFamilies(idFont).at(0);
        QFont font(family, 14);
        font.setLetterSpacing(QFont::AbsoluteSpacing, .5);
        this->setFont(font);
    }
    else {
        this->setStyleSheet("font-family: Helvetica; font-size: 14px;");
    }
    // Stylisation à voir plus tard

    this->setStyleSheet(R"(
        QCheckBox {
            spacing: 8px;
        }
        QCheckBox::indicator {
            width: 10px;
            height: 10px;
        }
        QCheckBox::indicator:unchecked {
            border: 2px solid rgba(36, 32, 112, 1);
            background-color: white;
            border-radius: 3px;
        }
        QCheckBox::indicator:checked {
            border: 2px solid #214ff3ff;
            background-color: #2196F3;
            border-radius: 3px;
        }
        QCheckBox::indicator:hover {
            border: 2px solid #2196F3;
        }
    )");
}

AkrCheckBox::~AkrCheckBox()
{
}
