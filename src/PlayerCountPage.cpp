
#include "UI/PlayerCountPage.hpp"

PlayerCountPage::PlayerCountPage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    lineEdit = new QLineEdit;
    lineEdit->setObjectName("playerNumberLineEdit");
    lineEdit->setPlaceholderText("Enter number of players");
    QPushButton *confirmBtn = new QPushButton("Confirm");
    confirmBtn->setObjectName("confirmPlayerNumberBtn");
    layout->addWidget(lineEdit);
    layout->addWidget(confirmBtn);

    connect(confirmBtn, &QPushButton::clicked, this, &PlayerCountPage::onConfirmClicked);
    
    setLayout(layout);
}

void PlayerCountPage::onConfirmClicked()
{
    bool ok;
    uint32_t count = lineEdit->text().toUInt(&ok);

    if (!ok || count < 2 || count > 4)
    {

        QMessageBox::warning(this, "Entrée invalide",
                             "Entrez un nombre entre 2 et 4");
            std::cout << "Invalid player count input: " << lineEdit->text().toStdString() << std::endl;
        return;
    }
    emit playerCountConfirmed(count);
}
