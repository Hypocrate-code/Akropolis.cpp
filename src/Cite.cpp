#include "Cite.hpp"
#include "Tuile.hpp"
#include <iostream>
#include <algorithm>
#include "Utils.hpp"
#include <bitset>

using namespace Utils;

void Cite::print_hex(Hexagone *hex, int x, int y, strCalc &calc)
{
    std::string colorCode = get_color_code(hex->getCouleur());
    if (!(hex->getType() == Type::Fantome && hex->getVoisinsBOT()))
    {
        // std::cout << hex->getType() << " " << hex->getIndice() << " at (" << x << "," << y << ") not drawn, fantome hex." << std::endl;
        // std::cout << hex->getVoisinsTOP() << " " << hex->getVoisinsBOT() << std::endl;
        // colorCode   = get_color_code(hex->getVoisinsBOT()->getCouleur());
        // return ;

        // Ensure we're within bounds
        if (y < 2 || y + 3 >= calc.size())
        {
            increase_calc_size_V(calc, 10);
        }
        if (x < 7 || x + 7 >= calc[0].size())
        {
            increase_calc_size_H(calc, 20);
        }

        // Get color code for this hexagon
        std::string resetCode = "\033[0m";

        // Draw the hexagon with proper ASCII art
        // Line 1: Top
        if (y - 2 >= 0 && x - 4 >= 0 && x + 5 <= calc[y - 2].size())
        {
            calc[y - 2].replace(x - 2, 5, "_____");
        }

        // Line 2: Top middle
        if (y - 1 >= 0 && x - 5 >= 0 && x + 6 <= calc[y - 1].size())
        {
            calc[y - 1].replace(x - 3, 7, "/     \\");
        }

        // Line 3: Middle top
        if (y >= 0 && x - 5 >= 0 && x + 6 <= calc[y].size())
        {
            calc[y].replace(x - 4, 9, "/       \\");
        }

        // Line 4: Middle bottom
        if (y + 1 >= 0 && x - 5 >= 0 && x + 6 <= calc[y + 1].size())
        {
            calc[y + 1].replace(x - 4, 9, "\\       /");
        }

        // Line 5: Bottom
        if (y + 2 >= 0 && x - 4 >= 0 && x + 5 <= calc[y + 2].size())
        {
            calc[y + 2].replace(x - 3, 7, "\\_____/");
        }
    }
    add_hex_data(hex, x, y, calc);
}

void Cite::add_hex_data(Hexagone *hex, int x, int y, strCalc &calc)
{

    std::string resetCode = "\033[0m";
    std::string typeStr = get_color_code(hex->getCouleur()) + get_short_type(hex->getType()) + resetCode;
    std::string colorStr = colorize_string(typeStr, hex->getCouleur());
    std::string colorCode = get_short_color(hex->getCouleur());

    if (y - 1 >= 0 && x - 4 >= 0 && x + 5 <= calc[y - 1].size())
    {
        // std::string topLine = center_string(colorCode + typeStr + resetCode, 3);
        if (hex->getType() == Type::Fantome)
        {
            // std::string indice = std::to_string(hex->getIndice());
            // calc[y].erase(x, indice.length());
            // calc[y].insert(x - indice.length() / 2, indice);
            std::string idstr = std::to_string(hex->getIndice());
            calc[y - 1].replace(x - 2, idstr.length(), idstr);
            return;
        }
        else
        {
            std::string topLine;

            topLine = center_string(get_short_type(hex->getType()), 7);
            calc[y].erase(x - 3, 7);
            calc[y].insert(x - 3, topLine);
        }

        // std::string final = replace_all(topLine ,get_short_type(hex->getType()), typeStr);
        // std::cout << final << std::endl;
        // calc[y].insert(x + 4, resetCode);
        // calc[y].insert(x - 3, get_color_code(hex->getCouleur()));
        // calc[y].insert(x - 3, topLine);
    }

    if (y >= 0 && x - 3 >= 0 && x + 4 <= calc[y].size())
    {
        // std::string midLine = center_string(colorCode + colorStr + resetCode, 5);
        std::string midLine = center_string(colorCode, 7);
        // calc[y+1].replace(x - 2, 5, midLine);
        calc[y + 1].replace(x - 3, 7, midLine);
    }
    // Leave line 4 (y+1) empty for clean look
}

void Cite::draw_hex_recursive(Hexagone *hex, int x, int y, strCalc &calc,

                              std::unordered_set<Hexagone *> &drawnHexagones, int hauteur)
{

    if (!hex || drawnHexagones.count(hex))
        return;

//
#ifdef DEBUG
    if (hauteur > 0)
    {
        std::cout << hex->getType() << " " << hex->getIndice() << " at height " << hauteur << " not drawn." << std::endl;
    }
#endif

    drawnHexagones.insert(hex);

    print_hex(hex, x, y, calc);

    // Proper hexagon grid offsets for pointy-top hexagones

    static const std::vector<std::pair<int, int>> directionOffsets = {
        {0, 4},   // S  (index 0) - hauteur hexagone = 5
        {-7, 2},  // SO (index 5) - largeur hexagone = 9
        {-7, -2}, // NO (index 4) - largeur hexagone = 9
        {0, -4},  // N  (index 3) - hauteur hexagone = 5
        {7, -2},  // NE (index 2) - largeur hexagone = 9
        {7, 2}    // SE (index 1) - largeur hexagone = 9
    };

    const Hexagone *neighbors[] = {
        hex->getVoisinsS(),  // 0: Sud
        hex->getVoisinsSO(), // 1: Sud-Ouest
        hex->getVoisinsNO(), // 2: Nord-Ouest
        hex->getVoisinsN(),  // 3: Nord
        hex->getVoisinsNE(), // 4: Nord-Est
        hex->getVoisinsSE(), // 5: Sud-Est
    };

    for (int i = 0; i < 6; ++i)
    {
        if (neighbors[i])
        {
            Hexagone *neighbor = const_cast<Hexagone *>(neighbors[i]);
            int newX = x + directionOffsets[i].first;
            int newY = y + directionOffsets[i].second;
            draw_hex_recursive(neighbor, newX, newY, calc, drawnHexagones, hauteur);
        }
    }

    const Hexagone *topNeighbor = hex->getVoisinsTOP();
    if (topNeighbor)
    {
        Hexagone *neighbor = const_cast<Hexagone *>(topNeighbor);
        draw_hex_recursive(neighbor, x, y, calc, drawnHexagones, hauteur + 1);
    }
}

void Cite::afficher() const

{

    // #ifdef DEBUG
    std::cout << "=== DEBUG HEXAGONE CONNECTIONS ===" << std::endl;

    if (tuiles.empty())
    {
        return;
    }

    // Debug: Print all hexagones and their neighbors
    for (size_t i = 0; i < tuiles.size(); ++i)
    {
        std::cout << "Tuile " << i << ":" << std::endl;
        const auto &hexagones = tuiles[i]->get_hexagones();
        for (size_t j = 0; j < hexagones.size(); ++j)
        {
            std::cout << "  Hexagone " << j << " (" << type_to_string(hexagones[j]->getType())
                      << ", " << color_to_string(hexagones[j]->getCouleur()) << "):" << std::endl;

            // Print all neighbors
            const Hexagone *neighbors[] = {
                hexagones[j]->getVoisinsNE(),
                hexagones[j]->getVoisinsS(),
                hexagones[j]->getVoisinsSE(),
                hexagones[j]->getVoisinsSO(),
                hexagones[j]->getVoisinsN(),
                hexagones[j]->getVoisinsNO(),
                hexagones[j]->getVoisinsTOP(),
                hexagones[j]->getVoisinsBOT()};

            const char *directionNames[] = {"NE", "S", "SE", "SO", "N", "NO", "TOP", "BOT"};

            for (int k = 0; k < 8; ++k)
            {
                if (neighbors[k])
                {
                    std::cout << "    " << directionNames[k] << " -> "
                              << type_to_string(neighbors[k]->getType()) << ", "
                              << color_to_string(neighbors[k]->getCouleur()) << std::endl;
                }
                else
                {
                    std::cout << "    " << directionNames[k] << " -> NULL" << std::endl;
                }
            }
        }
    }

    std::cout << "=== END DEBUG ===" << std::endl
              << std::endl;
    // #endif

    std::cout << "Nombre de tuiles dans la cite: " << tuiles.size() << std::endl;
    if (tuiles.size())
        std::cout << "Nombre d'hexagones dans la cite: " << ((tuiles.size() - 1) * 3 + 4) << std::endl;
    std::cout << "nombre de tuile fantome dans la cite: " << hexs_fantome.size() << std::endl;

    // Initialize canvas
    strCalc calc = {
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             "};

    // Start with the first hexagon of the first tuile
    const auto &firstHexagones = tuiles[0]->get_hexagones();
    if (firstHexagones.empty())
        return;

    Hexagone *startHex = firstHexagones[0];
    std::unordered_set<Hexagone *> drawnHexagones;

    draw_hex_recursive(startHex, 50, 10, calc, drawnHexagones, 0);

    for (auto &line : calc)
    {
        std::cout << colorize_line(line) << std::endl;
    }
    std::cout << "Total hexagones drawn: " << drawnHexagones.size() << std::endl;
}

void Cite::increase_calc_size_H(strCalc &calc, uint32_t size)
{
    for (auto &line : calc)
    {
        line = line + std::string(size, ' ');
    }
}

void Cite::increase_calc_size_V(strCalc &calc, uint32_t size)
{
    strCalc c(size, std::string(calc[0].size(), ' '));
    calc.insert(calc.end(), c.begin(), c.end());
}

Cite::Cite(const Tuile *tuileDeDepart)
{
    this->addTuile(tuileDeDepart);
}

void Cite::addTuile(const Tuile *t)
{
    tuiles.push_back(t);
}

void Cite::addTuile(Tuile *t)
{
    tuiles.push_back(t);
    t->cite = this;
    // remove the old fantomes then update..
    //...
    // updateFantomeOfTuile(t);
    release_hex_fantome();
    generateAllHexFantome();
}

CiteJoueur::CiteJoueur(const Tuile *tuileDeDepart) : Cite{tuileDeDepart}
{
    // generateAllHexFantome();
}

void CiteJoueur::placerTuileFromHexRef(Hexagone *hex)
{
    std::cout << "\n\n/======== DEBUG PLACER TUILE =========/\n";
    std::cout << "Tu vas placer la tuile avec le hexagone de reference : "
              << hex->getType() << " "
              << hex->getCouleur() << "\n";
    Tuile *owner = hex->getTuileParent();
    if (!owner)
    {
        std::cout << "Erreur: L'hexagone de reference n'appartient a aucune tuile.\n";
        exit(1);
        return;
    }
    hex->afficherData();
    // owner->rotate();
    // hex->afficherData();

    int id;
    bool idValide = false;

    while (!idValide)
    {
        std::cout << "Entrez le numero d'hexagone ou vous voulez placer la tuile : ";
        std::cin >> id;

        for (auto *hex : hexs_fantome)
        {
            if (hex->getIndice() == id)
            {
                idValide = true;
                break;
            }
        }

        if (!idValide)
        {
            std::cout << "ID invalide. Veuillez entrer un numero correct.\n";
        }
    }
    std::cout << "ID: " << id << "\n";

    auto IthexFantome = std::find_if(hexs_fantome.begin(), hexs_fantome.end(),
                                     [id](Hexagone *h)
                                     { return h->getIndice() == id; });
    if (IthexFantome == hexs_fantome.end())
    {
        std::cout << "Erreur: Aucun hexagone avec l'ID " << id << " trouve.\n";
        return;
    }

    Hexagone *hexFantome = *IthexFantome;
    std::cout << "Hexagone fantome selectionne : "
              << hexFantome->getType() << " "
              << hexFantome->getCouleur() << "\n";

    hexFantome->getVoisinsList();
    uint32_t FanVoisinsBin = hexFantome->getVoisinsNonFantomeBin();
    uint32_t SrcVoisinsBin = hex->getVoisinsNonFantomeBin();

    uint32_t res = FanVoisinsBin & SrcVoisinsBin;

    if (res != 0)
    {
        std::cout << "Impossible de placer la tuile ici\n";
        return;
    }

    // etablir les connextions entre les hexagones de la tuile et les hexagones fantomes

    std::cout << "Voisins binaires de l'hexagone fantome: " << std::bitset<8>(FanVoisinsBin) << "\n";
    std::cout << "Voisins binaires de l'hexagone src: " << std::bitset<8>(SrcVoisinsBin) << "\n";

    // Voisins binaires de l'hexagone fantome: 00100000
    // Voisins binaires de l'hexagone dest: 10000100

    // for each bits equal to 1 in fantome

    typedef struct tmpStruct
    {
        Type type;
        Couleur couleur;
        int indice;
    } tmpStrct;

    std::cout << "Details de l'hexagone fantome:\n";
    hexFantome->afficherData();

    for (int i = 0; i < 8; ++i)
    {
        if (FanVoisinsBin & (0b10000000 >> i))
        {
            std::cout << "index voisin a connecter: " << i << "\n";
            // si bit == 1 alors il faut faire la connection
            Hexagone *voisinFantome = hexFantome->getVoisinIndice(i);

            hex->setVoisinIndice(i, voisinFantome);

            // voisinFantome->setVoisinIndice(i, nullptr); // on deconnecte le fantome de son voisinage

            // Hexagone* voisinSrc = hex->getVoisinIndice(i);
            // if (voisinSrc && voisinFantome) {
            //  Connecter l'hexagone de la tuile a l'hexagone fantome
            // voisinSrc->setVoisinIndice(opposite_index(i), voisinFantome);

            //                std::cout << "Connecte l'hexagone src indice " << voisinSrc->getIndice()
            //                        << " a l'hexagone fantome indice " << voisinFantome->getIndice() << "\n";
            //        }
        }
    }

    std::cout << "Details de l'hexagone src apres placement:\n";
    // hex->afficherData();

    addTuile(owner);

    // placerSurID(id, tl);

    // updateFantomeOfTuile(tl);
}

void CiteJoueur::placerSurID(int id, const Tuile *t)
{
    Hexagone *hexagoneFantome = nullptr;
    for (auto *h : hexs_fantome)
    {
        if (h->getIndice() == id)
        {
            hexagoneFantome = h;
            break;
        }
    }

    std::cout << "=== Voisins de l'hexagone fantome selectionne (id=" << id << ") ===\n";
    hexagoneFantome->getVoisinsList();

    std::cout << "=== Voisins du premier hexagone de la tuile a placer ===\n";
    t->getVoisinsHex(0);

    // --- REPETIR EL TEST MANUAL EXISTENTE ---
    const Tuile *depart = tuiles[0];

    depart->get_hexagones()[0]->setVoisinsNE(t->get_hexagones()[0]);
    depart->get_hexagones()[0]->setVoisinsSE(t->get_hexagones()[2]);
    depart->get_hexagones()[3]->setVoisinsNE(t->get_hexagones()[2]);
    depart->get_hexagones()[2]->setVoisinsN(t->get_hexagones()[2]);

    // Agregar la tuile a la cité
    addTuile(t);

    std::cout << "Tuile place manuellement avec placerSurID().\n";
}

uint32_t CiteJoueur::compterPoints() const
{

    return 0;
}

Hexagone *Cite::create_new_hex_fantome()
{
    hexs_fantome.push_back(new Hexagone(Type::Fantome, Couleur::nulle));
    hexs_fantome.back()->setIndice(int(hexs_fantome.size()));
    return hexs_fantome.back();
}

void Cite::generateAllHexFantome()
{
    for (const Tuile *t : tuiles)
    {
        std::cout << "nombre de tuile dans la cite : " << tuiles.size() << std::endl;
        updateFantomeOfTuile(t);
    }
}

void Cite::updateFantomeOfTuile(const Tuile *t)
{
    const auto &hexagones = t->get_hexagones();
    for (size_t j = 0; j < hexagones.size(); j++)
    {
        const std::array<Hexagone *, 8> voisins = hexagones[j]->getVoisins();
        for (size_t i = 0; i < voisins.size(); i++)
        {
            if (i == 7) // skip BOT
                continue;

            if (voisins[i] == nullptr)
            {
                Hexagone *newHex = create_new_hex_fantome();

                // Link both ways
                hexagones[j]->setVoisinIndice(i, newHex);
                newHex->setVoisinIndice(opposite_index(int(i)), hexagones[j]);

                // For TOP we stop here (no ring traversal with gauche/droite)
                if (i == 6)
                {

                    // std::cout << "Created TOP fantome for hex indice " << hexagones[j]->getIndice() << std::endl;
                    hexagones[j]->afficherData();
                    continue;
                }

                // PARCOURS AUTOUR DE L'HEX FANTOME PAR LA GAUCHE POUR CHERCHER LES LIAISONS
                int k = indiceDeGauche(i);
                Hexagone *voisinDeGauche = const_cast<Hexagone *>(hexagones[j]->getVoisinIndice(k));
                while (voisinDeGauche != nullptr)
                {
                    k = indiceDeDroite(indiceDeDroite(k));
                    voisinDeGauche->setVoisinIndice(k, newHex);
                    k = indiceDeGauche(k);
                    voisinDeGauche = const_cast<Hexagone *>(voisinDeGauche->getVoisinIndice(k));
                }

                // PARCOURS AUTOUR DE L'HEX FANTOME PAR LA DROITE POUR CHERCHER LES LIAISONS
                k = indiceDeDroite(i);
                Hexagone *voisinDeDroite = const_cast<Hexagone *>(hexagones[j]->getVoisinIndice(k));
                while (voisinDeDroite != nullptr)
                {
                    k = indiceDeGauche(indiceDeGauche(k));
                    voisinDeDroite->setVoisinIndice(k, newHex);
                    k = indiceDeDroite(k);
                    voisinDeDroite = const_cast<Hexagone *>(voisinDeDroite->getVoisinIndice(k));
                }
            }
        }
    }
}

void Cite::release_hex_fantome()
{
    for (Hexagone *hex : hexs_fantome)
    {
        delete hex;
    }
    hexs_fantome.clear();
}
