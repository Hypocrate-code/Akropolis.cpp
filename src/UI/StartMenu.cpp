#include "UI/StartMenu.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#include <QCheckBox>
#include <iostream>

StartMenu::StartMenu(QWidget* parent)
    : QWidget(parent), layout(new QVBoxLayout(this)),
      decBtn(new QPushButton("Enlever joueurs")),
      incBtn(new QPushButton("Ajouter joueurs")),
    confirmBtn(new GamePushButton("Confirmer")), playerCount(1), dificultyLevel(0),variantes({}),
      lineEditLayout(new QVBoxLayout()), countInput(new QLineEdit()),
    dificultyComboBox(new QComboBox()), variantesLayout(new QVBoxLayout), longueurLayout(new QHBoxLayout)
{
    QHBoxLayout* btnLayout = new QHBoxLayout();

    layout->setAlignment(Qt::AlignTop);

    btnLayout->addWidget(new QLabel("Nombre de joueurs :"));
    btnLayout->addWidget(decBtn);
    btnLayout->addWidget(countInput);

    countInput->setText(QString::number(playerCount));
    countInput->setFixedWidth(20);

    btnLayout->addWidget(incBtn);

    incBtn->setToolTip("Maximum 4 joueurs.");

    incBtn->setFixedWidth(120);
    decBtn->setFixedWidth(120);



    // Ligne de difficulté (label + combobox)
    QWidget* difficultyRow = new QWidget();
    QHBoxLayout* diffLayout = new QHBoxLayout(difficultyRow);
    diffLayout->addWidget(new QLabel("Niveau de difficulté :"));
    diffLayout->addWidget(dificultyComboBox);

    dificultyComboBox->addItem("Hippodamos (niveau Facile)");
    dificultyComboBox->addItem("Métagénès (niveau Moyen)");
    dificultyComboBox->addItem("Callicratès (niveau Difficile)");


    //Selection de variantes (label + checks boxs)
    QLabel* var = new QLabel("Variantes règles : ");
    QCheckBox* marche = new QCheckBox("Variante marché");
    //QCheckBox* jardin = new QCheckBox("Variante jardin NON DISPO");
    QCheckBox* temple = new QCheckBox("Variante temple");
    QCheckBox* habitations = new QCheckBox("Variante habitations");
    QCheckBox* caserne = new QCheckBox("Variante caserne");




    variantesLayout->addWidget(var);
    variantesLayout->addWidget(marche);
    //variantesLayout->addWidget(jardin); //NON DISPO
    variantesLayout->addWidget(temple);
    variantesLayout->addWidget(habitations);
    variantesLayout->addWidget(caserne);

    //Selection partie longue
    QLabel* longueur = new QLabel("Voulez vous faire une partie longue ? ");
    QCheckBox* longue = new QCheckBox("oui");
    longueurLayout->addWidget(longueur);
    longueurLayout->addWidget(longue);


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

    connect(decBtn, &QPushButton::clicked, this, [this, difficultyRow]() {
        if (playerCount > 1) {
            playerCount--;
            delete playerButtons.back();
            playerButtons.pop_back();
        }
        countInput->setText(QString::number(playerCount));
        if (playerCount == 1) {
            difficultyRow->setVisible(true);
        }
    });

    connect(incBtn, &QPushButton::clicked, this, [this, difficultyRow]() {
        if (playerCount >= 4) {
            return;
        }
        playerCount++;
        if (playerCount >= 2) {
            difficultyRow->setVisible(false);
        }
        QLineEdit* newPlayerEdit = new QLineEdit();
        newPlayerEdit->setPlaceholderText("Nom Joueur " + QString::number(playerCount));
        playerButtons.push_back(newPlayerEdit);
        lineEditLayout->addWidget(newPlayerEdit);

        countInput->setText(QString::number(playerCount));
    });

    connect(countInput, &QLineEdit::textChanged, this, [this](const QString& text) {
        bool ok;
        uint32_t val = text.toUInt(&ok);
        if (val > 4) {
            val = 4;
            countInput->setText(QString::number(val));
        }
        if (val < 1) {
            val = 1;
            countInput->setText(QString::number(val));
        }
        if (ok) {
            while (playerCount < val) {
                incBtn->click();
            }
            while (playerCount > val) {
                decBtn->click();
            }
        }
    });

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

    connect(longue, &QCheckBox::toggled, this, [this](bool checked) {
        if(checked){
            duree = Duree::Longue;
        }else{
            duree = Duree::Courte;
        }
    });



    connect(confirmBtn, &QPushButton::clicked, this, &StartMenu::onConfirmClicked);

    // Champ du premier joueur (min = 1)
    QLineEdit* firstPlayerEdit = new QLineEdit();
    firstPlayerEdit->setPlaceholderText("Nom Joueur 1");
    playerButtons.push_back(firstPlayerEdit);
    lineEditLayout->addWidget(firstPlayerEdit);

    layout->addLayout(btnLayout);
    layout->addLayout(lineEditLayout);

    // Ajoute la ligne difficulté (label + combobox), choix des variantes, choix duree et le bouton Confirmer
    layout->addWidget(difficultyRow);
    layout->addLayout(variantesLayout);
    layout->addLayout(longueurLayout);
    layout->addWidget(confirmBtn);


    // Afficher la difficulté uniquement quand playerCount == 1
    difficultyRow->setVisible(playerCount == 1);



    setLayout(layout);
}
StartMenu::~StartMenu() = default;



void StartMenu::onConfirmClicked()
{
    std::cout << "Confirm clicked with " << playerCount << " players." << std::endl;
    if (playerButtons.size() != playerCount) {
        //popup
        QMessageBox::warning(this, "Erreur", "Le nombre de joueurs ne correspond pas au nombre de champs de texte.");
        return;
    }
    std::vector<std::string> players;
    for (const auto& lineEdit : playerButtons) {
        if (lineEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les noms des joueurs.");
            return;
        }
        players.push_back(lineEdit->text().toStdString());
    }
    
    emit playerSelectionConfirmed(players, dificultyLevel, variantes, duree);
}


