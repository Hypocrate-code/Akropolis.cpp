#include "UI/StartMenu.hpp"
#include "UI/AkrPushButton.hpp"
#include <QHBoxLayout>
#include "UI/AkrLabel.hpp"
#include "UI/AkrCheckBox.hpp"
#include <QComboBox>
#include <QMessageBox>
#include <iostream>

StartMenu::StartMenu(QWidget* parent)
    : QWidget(parent), layout(new QVBoxLayout(this)),
      decBtn(new AkrPushButton(" - ")),
      incBtn(new AkrPushButton(" + ")),
    confirmBtn(new AkrPushButton("Confirmer")), playerCount(1), dificultyLevel(0), variantes({}),
      lineEditLayout(new QVBoxLayout()), countInput(new AkrLabel),
    dificultyComboBox(new QComboBox()), variantesLayout(new QVBoxLayout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    layout->setAlignment(Qt::AlignTop);

    btnLayout->addStretch(2);
    btnLayout->addWidget(new AkrLabel("Nombre de joueurs"));
    btnLayout->addStretch(1);
    btnLayout->addWidget(decBtn);

    countInput->setText(QString::number(playerCount));
    countInput->setFixedWidth(50);
    btnLayout->addWidget(countInput);
    
    btnLayout->addWidget(incBtn);

    btnLayout->addStretch(2);

    incBtn->setToolTip("Maximum 4 joueurs.");

    incBtn->setFixedWidth(50);
    decBtn->setFixedWidth(50);



    // Ligne de difficulté (label + combobox)
    QWidget* difficultyRow = new QWidget();
    QHBoxLayout* diffLayout = new QHBoxLayout(difficultyRow);
    diffLayout->addStretch(2);
    diffLayout->addWidget(new AkrLabel("Niveau de difficulté"));
    diffLayout->addStretch(1);
    diffLayout->addWidget(dificultyComboBox);
    diffLayout->addStretch(2);

    dificultyComboBox->addItem("Hippodamos (niveau Facile)");
    dificultyComboBox->addItem("Métagénès (niveau Moyen)");
    dificultyComboBox->addItem("Callicratès (niveau Difficile)");


    //Selection de variantes (label + checks boxs)
    AkrLabel* var = new AkrLabel("Variantes règles : ");
    AkrCheckBox* marche = new AkrCheckBox("Variante Marchés");
    //AkrCheckBox* jardin = new AkrCheckBox("Variante jardin NON DISPO");
    AkrCheckBox* temple = new AkrCheckBox("Variante Temples");
    AkrCheckBox* habitations = new AkrCheckBox("Variante Habitations");
    AkrCheckBox* caserne = new AkrCheckBox("Variante Casernes");


    variantesLayout->addWidget(var);
    variantesLayout->addWidget(marche);
    //variantesLayout->addWidget(jardin); //NON DISPO
    variantesLayout->addWidget(temple);
    variantesLayout->addWidget(habitations);
    variantesLayout->addWidget(caserne);

    connect(dificultyComboBox, &QComboBox::currentIndexChanged, this, [this](int index) {
        dificultyLevel = static_cast<uint32_t>(index);
    });

    dificultyComboBox->setToolTip(
        "Choisissez le niveau de difficulté de l'illustre architecte \n"
        "- Hippodamos (niveau Facile) : tous les Quartiers d’Hippodamos sont considérés comme étant au 1er niveau.\n"
        "- Métagénès (niveau Moyen) : tous les Quartiers de Métagénès sont considérés comme étant au 1er niveau. "
        "Chaque Carrière de Métagénès lui rapporte 2 points supplémentaires.\n"
        "- Callicratès (niveau Difficile) : tous les Quartiers de Callicratès sont considérés comme étant au 2e niveau."
    );

    connect(decBtn, &AkrPushButton::clicked, this, [this, difficultyRow]() {
        if (playerCount > 1) {
            playerCount--;
            delete playerWidget.back();
            playerWidget.pop_back();
        }
        countInput->setText(QString::number(playerCount));
        if (playerCount == 1) {
            difficultyRow->setVisible(true);
        }
    });

    connect(incBtn, &AkrPushButton::clicked, this, [this, difficultyRow]() {
        if (playerCount >= 4) {
            return;
        }
        playerCount++;
        if (playerCount >= 2) {
            difficultyRow->setVisible(false);
        }

        QWidget* container = new QWidget();

        QHBoxLayout* newPlayerLine = new QHBoxLayout(container);

        AkrLabel* label = new AkrLabel(("Joueur " + std::to_string(playerCount)).c_str());
        newPlayerLine->addStretch(4);
        newPlayerLine->addWidget(label);
        newPlayerLine->addStretch(1);

        
        QLineEdit* newPlayerEdit = new QLineEdit();
        
        newPlayerEdit->setPlaceholderText("Entrez un pseudo");
        playerWidget.push_back(container);
        countInput->setText(QString::number(playerCount));
        
        newPlayerLine->addWidget(newPlayerEdit);
        newPlayerLine->addStretch(4);

        lineEditLayout->addWidget(container);

    });

    // connect(countInput, &QLineEdit::textChanged, this, [this](const QString& text) {
    //     bool ok;
    //     uint32_t val = text.toUInt(&ok);
    //     if (val > 4) {
    //         val = 4;
    //         countInput->setText(QString::number(val));
    //     }
    //     if (val < 1) {
    //         val = 1;
    //         countInput->setText(QString::number(val));
    //     }
    //     if (ok) {
    //         while (playerCount < val) {
    //             incBtn->click();
    //         }
    //         while (playerCount > val) {
    //             decBtn->click();
    //         }
    //     }
    // });

    //connexion des check box avec variantes
    connect(marche, &QCheckBox::toggled, this, [this](bool checked) {
        variantes[0] = checked ? 1 : 0;
    });

    //variante jardin non dispo
    //connect(jardin, &QCheckBox::toggled, this, [this](bool checked) {
    //    variantes[1] = checked ? 1 : 0;
    //});
    connect(temple, &QCheckBox::toggled, this, [this](bool checked) {
        variantes[2] = checked ? 1 : 0;
    });
    connect(habitations, &QCheckBox::toggled, this, [this](bool checked) {
        variantes[3] = checked ? 1 : 0;
    });

    connect(caserne, &QCheckBox::toggled, this, [this](bool checked) {
        variantes[4] = checked ? 1 : 0;
    });



    connect(confirmBtn, &AkrPushButton::clicked, this, &StartMenu::onConfirmClicked);



    QWidget* container = new QWidget();

    QHBoxLayout* newPlayerLine = new QHBoxLayout(container);

    AkrLabel* label = new AkrLabel(("Joueur " + std::to_string(playerCount)).c_str());
    newPlayerLine->addStretch(4);
    newPlayerLine->addWidget(label);
    newPlayerLine->addStretch(1);
    
    QLineEdit* firstPlayerEdit = new QLineEdit();
    
    firstPlayerEdit->setPlaceholderText("Entrez un pseudo");
    playerWidget.push_back(container);
    
    newPlayerLine->addWidget(firstPlayerEdit);
    newPlayerLine->addStretch(4);

    lineEditLayout->addWidget(container);

    layout->addLayout(btnLayout);
    layout->addStretch();
    layout->addLayout(lineEditLayout);
    layout->addStretch();

    // Ajoute la ligne difficulté (label + combobox), choix des variantes et le bouton Confirmer
    layout->addWidget(difficultyRow);
    
    QHBoxLayout *centeringVariantes = new QHBoxLayout();
    centeringVariantes->addStretch();
    centeringVariantes->addLayout(variantesLayout);
    centeringVariantes->addStretch();

    layout->addLayout(centeringVariantes);
        
    QHBoxLayout* confirmBtnLayout = new QHBoxLayout();
    
    confirmBtn->setMinimumWidth(244);
    confirmBtnLayout->addStretch(1);
    confirmBtnLayout->addWidget(confirmBtn, 2);
    confirmBtnLayout->addStretch(1);
    layout->addLayout(confirmBtnLayout);
    
    // Afficher la difficulté uniquement quand playerCount == 1
    difficultyRow->setVisible(playerCount == 1);



    setLayout(layout);
}
StartMenu::~StartMenu() = default;



void StartMenu::onConfirmClicked()
{

    if (playerWidget.size() != playerCount) {
        //popup
        QMessageBox::warning(this, "Erreur", "Le nombre de joueurs ne correspond pas au nombre de champs de texte.");
        return;
    }
    std::vector<std::string> players;
    for (const auto& element : playerWidget) {
        QLineEdit* lineEdit = element->findChild<QLineEdit*>();
        if (lineEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les noms des joueurs.");
            return;
        }
        players.push_back(lineEdit->text().toStdString());
    }
    
    emit playerSelectionConfirmed(players, dificultyLevel, variantes);
}


