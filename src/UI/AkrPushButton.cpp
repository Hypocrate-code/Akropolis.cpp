#include "UI/AkrPushButton.hpp"
#include <QFontDatabase>

AkrPushButton::AkrPushButton(QWidget *parent)
    : QPushButton(parent)
{
    updateStyle();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setCursor(Qt::PointingHandCursor);
}

AkrPushButton::AkrPushButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)

{
    updateStyle();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setCursor(Qt::PointingHandCursor);
}
AkrPushButton::AkrPushButton(const QIcon& icon, const QString &text, QWidget *parent)
    : QPushButton(icon, text, parent)
{
    updateStyle();
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setCursor(Qt::PointingHandCursor);
}
AkrPushButton::~AkrPushButton()
{
}

void AkrPushButton::updateStyle()
{
    QString family = QFontDatabase::applicationFontFamilies(0).at(0);
    QFont font(family, 14);
    font.setWeight(QFont::Medium);
    font.setLetterSpacing(QFont::AbsoluteSpacing, 1);
    this->setFont(font);

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