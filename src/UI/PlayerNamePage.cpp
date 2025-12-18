
#include "UI/PlayerNamePage.hpp"
#include <QMessageBox>
#include "UI/GamePushButton.hpp"

PlayerNamePage::PlayerNamePage(QWidget* parent)
    : QWidget(parent), layout(new QVBoxLayout(this))
{

    GamePushButton* confirmBtn = new GamePushButton("Confirm");
    layout->addWidget(confirmBtn);
    connect(confirmBtn, &QPushButton::clicked, this, &PlayerNamePage::onConfirmClicked);

    setLayout(layout);

}

void PlayerNamePage::onConfirmClicked()
{
    std::vector<std::string_view> names;
    for (const auto& edit : edits)
    {
        if (edit->text().isEmpty())
        {
            QMessageBox::warning(this, "Entrée invalide",
                                 "Veuillez entrer un nom pour chaque joueur.");
            names.clear();
            return;
        }
        names.push_back(edit->text().toStdString());
    }
    emit playerNamesConfirmed(names);
}

void PlayerNamePage::setPlayerCount(int count)
{
    QLayoutItem *item;
    while ((item = layout->takeAt(0)))
    {
        delete item->widget();
        delete item;
    }
    edits.clear();

    for (int i = 0; i < count; ++i)
    {
        auto *e = new QLineEdit;
        e->setPlaceholderText("Player " + QString::number(i + 1));
        layout->addWidget(e);
        edits.push_back(e);
    }
}
