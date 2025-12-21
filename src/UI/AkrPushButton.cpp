
#include "UI/AkrPushButton.hpp"

AkrPushButton::AkrPushButton(QWidget *parent)
    : QPushButton(parent)
{
    updateStyle();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

AkrPushButton::AkrPushButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)

{
    updateStyle();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}
AkrPushButton::AkrPushButton(const QIcon& icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{
    updateStyle();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}
AkrPushButton::~AkrPushButton()
{
}

void AkrPushButton::updateStyle()
{
    if (idFont != -1) {
        QString family = QFontDatabase::applicationFontFamilies(idFont).at(0);
        QFont font(family, 14);
        font.setLetterSpacing(QFont::AbsoluteSpacing, 0);
        this->setFont(font);
    }
    else {
        this->setStyleSheet("font-family: Helvetica; font-size: 14px; font-weight: bold;");
    }

    QString style = R"(
        QPushButton {
            background-color: rgba(9, 37, 84, 1);
            border: 1px solid whitesmoke;
            color: whitesmoke;
            padding: 10px 15px;
            text-align: center;
            text-decoration: none;
            font-size: 18px;
            border-radius: 8px;
        }
        QPushButton:hover {
            background-color: rgba(3, 22, 57, 1);
        }
        QPushButton:pressed {
            background-color: rgba(2, 12, 29, 1);
        }
    )";
    setStyleSheet(style);
}