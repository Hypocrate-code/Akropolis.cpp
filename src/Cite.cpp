#include "Cite.hpp"
#include "Tuile.hpp"
#include <iostream>
#include <algorithm>
#include "Utils.hpp"
#include <bitset>
#include <unordered_set>
#include <stack>
#include <cstdint>

using namespace Utils;

void Cite::print_hex(Hexagone *hex, int x, int y, strCalc &calc)
{

    std::string colorCode = get_color_code(hex->getCouleur());
    if (!(hex->getType() == Type::Fantome && hex->getVoisinsBOT()))
    {
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
            std::string idstr = std::to_string(hex->getIndice());
            calc[y - 1].replace(x - 2, idstr.length(), idstr);

            std::string hauteurStr;
            if (hex->getVoisinsBOT() && hex->getVoisinsBOT()->getTuileParent())
                hauteurStr = std::to_string(hex->getVoisinsBOT()->getTuileParent()->get_hauteur());
            else
                hauteurStr = "??";


            calc[y + 1].replace(x - 3,  hauteurStr.length(), hauteurStr);
            return;
        }
        else
        {
            std::string topLine;
            topLine = center_string(get_short_type(hex->getType()), 7);
            
            calc[y].erase(x - 3, 7);
            calc[y].insert(x - 3, topLine);
        }

    }

    if (y >= 0 && x - 3 >= 0 && x + 4 <= calc[y].size())
    {
        // std::string midLine = center_string(colorCode + colorStr + resetCode, 5);
        std::string midLine = center_string(colorCode, 7);
        // calc[y+1].replace(x - 2, 5, midLine);
        calc[y + 1].replace(x - 3, 7, midLine);
    }
}


void CiteJoueur::afficher() const

{
    //std::cout << "Nombre d'hexagone fantome: " << hexs_fantome.size() << std::endl;
    // =================== DEBUG HEXAGONE CONNECTIONS =========================

    //std::cout << "=== DEBUG HEXAGONE CONNECTIONS ===" << std::endl;

    // if (tuiles.empty())
    // {
    //     return;
    // }

    // Debug: Print all hexagones and their neighbors
    /*
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
        */

    //affichage debug de tuiles fantomes
    /*
    if (1){
        std::cout << "\n--- Hexagones Fantomes dans la Cite ---" << std::endl;
        for (const auto& hexFan : hexs_fantome) {
            std::cout << "Hexagone Fantome ID " << hexFan->getIndice() << ": "
                      << type_to_string(hexFan->getType()) << ", "
                      << color_to_string(hexFan->getCouleur()) << std::endl;

            // Print neighbors of the fantome hexagon
            const Hexagone *neighbors[] = {
                hexFan->getVoisinsNE(),
                hexFan->getVoisinsS(),
                hexFan->getVoisinsSE(),
                hexFan->getVoisinsSO(),
                hexFan->getVoisinsN(),
                hexFan->getVoisinsNO(),
                hexFan->getVoisinsTOP(),
                hexFan->getVoisinsBOT()};

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
        std::cout << "--- Fin des Hexagones Fantomes ---\n" << std::endl;
    }
        */

    // std::cout << "Nombre de tuiles dans la cite: " << tuiles.size() << std::endl;
    // if (tuiles.size())
    //     std::cout << "Nombre d'hexagones dans la cite: " << ((tuiles.size() - 1) * 3 + 4) << std::endl;
    // std::cout << "nombre d'hexagone fantome dans la cite: " << hexs_fantome.size() << std::endl;

    // std::cout << "=== END DEBUG ===" << std::endl
    //           << std::endl;

    // =================== FIN DEBUG HEXAGONE CONNECTIONS =========================

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

    const auto &firstHexagones = tuiles[0]->get_hexagones();
    if (firstHexagones.empty())
        return;

    Hexagone *startHex = firstHexagones[0];
    std::unordered_set<Hexagone *> drawnHexagones;

    // First pass: draw only non-fantome hexagons
    draw_hex_recursive_filtered(startHex, 50, 10, calc, drawnHexagones, 0, false);

    // Second pass: draw only fantome hexagons
    drawnHexagones.clear(); // Reset to redraw fantomes
    draw_hex_recursive_filtered(startHex, 50, 10, calc, drawnHexagones, 0, true);

    for (auto &line : calc)
    {
        std::cout << colorize_line(line) << std::endl;
    }
}

void Cite::draw_hex_recursive_filtered(Hexagone *hex, int x, int y, strCalc &calc,
                                       std::unordered_set<Hexagone *> &drawnHexagones, 
                                       int hauteur, bool drawFantomes)
{
    if (!hex || drawnHexagones.count(hex))
        return;

    // Skip based on filter
    bool isFantome = (hex->getType() == Type::Fantome);
    if (isFantome != drawFantomes)
    {
        // Still mark as visited to avoid infinite loops, but don't draw
        drawnHexagones.insert(hex);
        
        // Continue traversal through all neighbors
        static const std::vector<std::pair<int, int>> directionOffsets = {
            {0, 4},   {-7, 2},  {-7, -2}, {0, -4},  {7, -2},  {7, 2}
        };

        const Hexagone *neighbors[] = {
            hex->getVoisinsS(),  hex->getVoisinsSO(), hex->getVoisinsNO(),
            hex->getVoisinsN(),  hex->getVoisinsNE(), hex->getVoisinsSE(),
            hex->getVoisinsTOP()
        };

        for (int i = 0; i < 7; ++i)
        {
            if (neighbors[i])
            {
                Hexagone *neighbor = const_cast<Hexagone *>(neighbors[i]);
                int newX = (i < 6) ? x + directionOffsets[i].first : x;
                int newY = (i < 6) ? y + directionOffsets[i].second : y;
                draw_hex_recursive_filtered(neighbor, newX, newY, calc, drawnHexagones, hauteur, drawFantomes);
            }
        }
        return;
    }

    drawnHexagones.insert(hex);
    print_hex(hex, x, y, calc);

    static const std::vector<std::pair<int, int>> directionOffsets = {
        {0, 4},   {-7, 2},  {-7, -2}, {0, -4},  {7, -2},  {7, 2}
    };

    const Hexagone *neighbors[] = {
        hex->getVoisinsS(),  hex->getVoisinsSO(), hex->getVoisinsNO(),
        hex->getVoisinsN(),  hex->getVoisinsNE(), hex->getVoisinsSE(),
        hex->getVoisinsTOP()
    };

    for (int i = 0; i < 7; ++i)
    {
        if (neighbors[i])
        {
            Hexagone *neighbor = const_cast<Hexagone *>(neighbors[i]);
            int newX = (i < 6) ? x + directionOffsets[i].first : x;
            int newY = (i < 6) ? y + directionOffsets[i].second : y;
            draw_hex_recursive_filtered(neighbor, newX, newY, calc, drawnHexagones, hauteur, drawFantomes);
        }
    }
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

void CiteJoueur::addTuile(Tuile *t)
{
    tuiles.push_back(t);
    t->cite = this;

    release_hex_fantome();
    generateAllHexFantome();
}

CiteJoueur::CiteJoueur(const Tuile *tuileDeDepart) : Cite{tuileDeDepart}
{
    // generateAllHexFantome();
}

bool CiteJoueur::placerTuileFromHexRef(Hexagone *hex, Hexagone *hexFantome)
{
    // Version Qt : hex fantôme directement fourni
    Tuile *owner = hex->getTuileParent();
    if (!owner || !hexFantome)
    {
        return false;
    }

    uint32_t FanVoisinsBin = hexFantome->getVoisinsNonFantomeBin();
    uint32_t SrcVoisinsBin = hex->getVoisinsNonFantomeBin();

    uint32_t res = FanVoisinsBin & SrcVoisinsBin;

    if (res != 0) // AND condition
    {
        return false;
    }

    // Variables pour l'algorithme
    Hexagone *hex1 = nullptr;
    Hexagone *hex1Fan = nullptr;
    Hexagone *hex1Bot = nullptr;
    uint32_t hex1DirFromHex0 = -1;

    Hexagone *hex2 = nullptr;
    Hexagone *hex2Fan = nullptr;
    Hexagone *hex2Bot = nullptr;
    uint32_t hex2DirFromHex0 = -1;

    for (int i = 0; i < 8; i++)
    {
        if (SrcVoisinsBin & (0b10000000 >> i))
        {
            if (!hex1)
            {
                hex1 = hex->getVoisinIndice(i);
                hex1Fan = hexFantome->getVoisinIndice(i);
                if (hexFantome && hexFantome->getVoisinsBOT())
                    hex1Bot = hexFantome->getVoisinsBOT()->getVoisinIndice(i);
                if (hex1Fan && hex1Fan->getType() != Type::Fantome)
                    hex1Fan = 0;
                hex1DirFromHex0 = i;
            }
            else
            {
                hex2 = hex->getVoisinIndice(i);
                hex2Fan = hexFantome->getVoisinIndice(i);
                if (hexFantome && hexFantome->getVoisinsBOT())
                    hex2Bot = hexFantome->getVoisinsBOT()->getVoisinIndice(i);
                if (hex2Fan && hex2Fan->getType() != Type::Fantome)
                    hex2Fan = 0;
                hex2DirFromHex0 = i;
            }
        }
    }

    if (!hexFantome->getVoisinsBOT()) // on est sur la couche 0.
    {
        updateHexSrcFromFan(hex, hexFantome);
        if (hex1Fan)
            updateHexSrcFromFan(hex1, hex1Fan);
        if (hex2Fan)
            updateHexSrcFromFan(hex2, hex2Fan);
    }
    else
    { // on est pas sur la couche 0
        if ((hex1Bot && hex1Bot->getType() == Type::Fantome) || (hex2Bot && hex2Bot->getType() == Type::Fantome))
        { //  ici on passe par les hexgaone du desous, car au dessus il y a surement de l'air.
            std::cout << "Impossible de placer la Tuile ici. Un ou les hexagones du dessous nexiste pas" << std::endl;
            return 0;
        }
        else
        {
            if (!hex1Bot->getVoisinsTOP() || !hex2Bot->getVoisinsTOP())
            {
                std::cout << "gros problem dans lalgo, jai rien compris" << std::endl;
                return 0;
            }

            if (hex1Bot->getTuileParent() == hex2Bot->getTuileParent() && hex1Bot->getTuileParent() == hexFantome->getVoisinsBOT()->getTuileParent())
            {
                std::cout << "Impossible de placer la Tuile ici. Les 3 hexagones du dessous appartiennent a la meme tuile." << std::endl;
                return 0;
            }

            updateHexSrcFromFan(hex, hexFantome);
            updateHexSrcFromFan(hex1, hex1Bot->getVoisinIndice(6)); // get TOP
            updateHexSrcFromFan(hex2, hex2Bot->getVoisinIndice(6));
            hex1->getTuileParent()->set_hauteur(
                1 + hex1Bot->getTuileParent()->get_hauteur()); // update la hauteur de la tuile

            if (hex1Fan)
                hex1Fan->setVoisins(nullptr);

            if (hex2Fan)
                hex2Fan->setVoisins(nullptr);

            hexFantome->setVoisins(nullptr);
        }
    }

    addTuile(owner);
    return 1;

    // updateFantomeOfTuile(tl);
}

uint32_t CiteJoueur::compterPoints( int niveau_difficulte, std::array<int,5>variantes) const
{
    uint32_t nb_place_bleue = 0;
    uint32_t nb_place_rouge = 0;
    uint32_t nb_place_verte = 0;
    uint32_t nb_place_violet = 0;
    uint32_t nb_place_jaune = 0;

    uint32_t points_bleu = 0;
    uint32_t points_rouge = 0;
    uint32_t points_vert = 0;
    uint32_t points_violet = 0;
    uint32_t points_jaune = 0;

    std::vector<const Hexagone *> habitations_visitees{};
    std::vector<uint32_t> points_hab{};

    // parcours des hexagones
    const Hexagone *start = tuiles.back()->get_hexagones().back();
    if (!start)
        return 0;

    std::unordered_set<const Hexagone *> visited;
    std::stack<const Hexagone *> pile;

    pile.push(start);

    while (!pile.empty())
    {
        const Hexagone *h = pile.top();
        pile.pop();
        if (visited.count(h))
            continue;
        visited.insert(h);

        // Comptage des points :
        // si la tuile est recouverte --> ne vaut pas de points
        if (h->getVoisinsTOP() == nullptr || h->getVoisinsTOP()->getType() == Type::Fantome)
        {
            // on determine le niveau de l'hex
            uint32_t niveau = h->getNiveau();

            // place, on incrémente le nombre de place de cette couleur
            if (h->getType() == Type::Place)
            {
                Couleur hc = h->getCouleur();
                switch (hc)
                {
                case Couleur::Bleu:
                    nb_place_bleue++;
                    break;
                case Couleur::Jaune:
                    nb_place_jaune++;
                    break;
                case Couleur::Rouge:
                    nb_place_rouge++;
                    break;
                case Couleur::Vert:
                    nb_place_verte++;
                    break;
                case Couleur::Violet:
                    nb_place_violet++;
                    break;
                }
            }
            // quartier
            if (h->getType() == Type::Quartier)
            {

                // calcul point marché: + 1 points si n'est pas entouré d'autres marchés
                if (h->getCouleur() == Couleur::Jaune)
                {
                    const std::array<Hexagone *, 6> voisins = h->getVoisins3D();
                    bool cond = true;
                    int var = 1; 
                    for (int i = 0; i < 6; i++)
                    {

                        if (voisins[i] != nullptr)
                        {
                            if (voisins[i]->getCouleur() == Couleur::Jaune && voisins[i]->getType() == Type::Quartier)
                            {
                                cond = false;
                            }
                        }
                    }
                    //variante si on a une place jaune, on double les points
                     if(variantes[0]==1){
                         for (int i = 0; i < 6; i++){
                                if (voisins[i] != nullptr){
                                    if (voisins[i]->getCouleur() == Couleur::Jaune && voisins[i]->getType() == Type::Place){
                                        var=2; 
                                }
                            }
                        }
                    }
                    if (cond == true) points_jaune += 1 * niveau * var;
                    
                   
                        
                }
                // calcul points jardins : +1 pt pour chaque jardin
                if (h->getCouleur() == Couleur::Vert)
                {
                    points_vert += 1 * niveau;
                }
                // calcul temples : +1 si est entièrement entouré
                if (h->getCouleur() == Couleur::Violet)
                {

                    const std::array<Hexagone *, 6> &voisins = h->getVoisins3D();
                    int var =1; 

                    // on vérifie si entierment entouré au niveau 0
                    bool cond = true;
                    for (int i = 0; i < 6; i++)
                    {
                        if (voisins[i] != nullptr)
                        {
                            if (voisins[i]->getType() == Type::Fantome)
                                cond = false;
                        }
                        else
                        {
                            cond = false;
                        }
                    }
                    if(variantes[2]==1) var = niveau; 
                    if (cond)
                        points_violet += 1 * niveau * var;
                }
                // calcul habitation: on doit calculer les groupes d'habitations
                if (h->getCouleur() == Couleur::Bleu)
                {
                    // si on a deja visite le groupe --> on passe
                    if (std::find(habitations_visitees.begin(), habitations_visitees.end(), h) == habitations_visitees.end())
                    {
                        std::stack<const Hexagone *> habitations_a_visiter;
                        uint32_t points_bleu_h = 1;
                        habitations_a_visiter.push(h);
                        while (!habitations_a_visiter.empty())
                        {
                            const Hexagone *hab = habitations_a_visiter.top();
                            habitations_a_visiter.pop();
                            const std::array<Hexagone *, 6> voisins = hab->getVoisins3D();
                            for (int i = 0; i < 6; i++)
                            {
                                if (voisins[i] != nullptr)
                                {
                                    if (voisins[i]->getCouleur() == Couleur::Bleu && voisins[i]->getType() == Type::Quartier && std::find(habitations_visitees.begin(), habitations_visitees.end(), voisins[i]) == habitations_visitees.end() && voisins[i]->getVoisinsTOP() == nullptr)
                                    {
                                        habitations_a_visiter.push(voisins[i]);
                                        points_bleu_h++;
                                    }
                                }
                            }
                            habitations_visitees.push_back(hab);
                        }
                        points_hab.push_back(points_bleu_h);
                    }
                    // algo de recherche d'habitations voisines à faire
                    // on parcourt les voisins de h,
                    // puis les voisins des voisins de h ect jusqu'à ce qu'il n'y est plus d'habitations dans les voisins
                    // à ch nouvelle habitation visitée, on l'ajoute dans le vecteur + on incrémente nbre de points
                    // et on parcours les voisins de cette habitations  qui n'ont pas encore été visités
                }

                // calcul caserne : +1 si l'hexagone est en périphérie de la cité
                if (h->getCouleur() == Couleur::Rouge)
                {
                    bool cond = false;
                    const std::array<Hexagone *, 6> &voisins = h->getVoisins3D();
                    int var = 1; 
                    for (int i = 0; i < 6; i++)
                    {
                        if (voisins[i] != nullptr)
                        {
                            if (voisins[i]->getType() == Type::Fantome)
                                cond = true;
                        }
                        else
                        {
                            cond = true;
                        }
                    }
                    if(variantes[4]==1){
                        int cases_vides = 0; 
                        for (int i = 0; i < 6; i++){
                            if (voisins[i] != nullptr){
                                if (voisins[i]->getType() == Type::Fantome)
                                    cases_vides++; 
                            }
                        else
                        {
                            cases_vides++; 
                        }

                    }
                    if(cases_vides>=3){
                        var = 2; 
                    }
                    }
                    if (cond) points_rouge += 1 * niveau * var;
                }
            }
        }
        // Ajouter les voisins
        for (const Hexagone *v : h->getVoisins())
        {
            if (v != nullptr && !visited.count(v))
                pile.push(v);
        }
    }

    // On prend le max des suites de cité pour avoir le nbre de points correspondants
    if (!habitations_visitees.empty())
    {
        points_bleu = *std::max_element(points_hab.begin(), points_hab.end());
        if(variantes[3]==1){
            if(points_bleu>=10){
                points_bleu=points_bleu*2; 
            }
        }
    }

    //std::cout << "printing points : " << points_bleu * nb_place_bleue * 1 << " . " << points_jaune * nb_place_jaune * 2 << " . " << points_rouge * nb_place_rouge * 2 << " . " << points_vert * nb_place_verte * 3 << " . " << points_violet * nb_place_violet * 2 << std::endl;

    uint32_t total = points_bleu * nb_place_bleue * 1 + points_jaune * nb_place_jaune * 2 + points_rouge * nb_place_rouge * 2 + points_vert * nb_place_verte * 3 + points_violet * nb_place_violet * 2;
    return total;
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
        //std::cout << "nombre de tuile dans la cite : " << tuiles.size() << std::endl;
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
            if (t->get_hauteur() == 0 && i == 7) // skip BOT
                continue;

            if (voisins[i] == nullptr) //si voisin est null alors il faut mettre un hex fantome
            {

                //before creating a new fantome, check if one already exists in that direction
                Hexagone* voisinHex = nullptr;

                if (hexagones[j]->getVoisinsBOT() && hexagones[j]->getVoisinsBOT()->getVoisinIndice(i) )
                {
                     voisinHex = hexagones[j]->getVoisinsBOT()->getVoisinIndice(i)->getVoisinIndice(6); // get TOP of BOT voisin
                     if (voisinHex && voisinHex->getType() == Type::Fantome){
                         hexagones[j]->setVoisinIndice(i, voisinHex);
                            voisinHex->setVoisinIndice(opposite_index(int(i)), hexagones[j]);


                            continue;
                     }

                    // std::cout << "Reused existing fantome for hex indice " << hexag
                }
                
                //for (Hexagone* hexFan : hexs_fantome)
                //{
                //    Hexagone* voisinFan = hexFan->getVoisinIndice(Utils::opposite_index(int(i)));
                //    if (voisinFan == hexagones[j])
                //    {
                //        // Link both ways
                //        hexagones[j]->setVoisinIndice(i, hexFan);
                //        hexFan->setVoisinIndice(Utils::opposite_index(int(i)), hexagones[j]);
                //        fantomeExists = true;
                //        break;
                //    }
               // }

                Hexagone *newHex = create_new_hex_fantome();

                // Link both ways
                hexagones[j]->setVoisinIndice(i, newHex);
                newHex->setVoisinIndice(opposite_index(int(i)), hexagones[j]);

                // For TOP we stop here (no ring traversal with gauche/droite)
                if (i == 6)
                {

                    // std::cout << "Created TOP fantome for hex indice " << hexagones[j]->getIndice() << std::endl;
                    // hexagones[j]->afficherData();
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
            else{

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
}; 
uint32_t CiteIllu::compterPoints( int niveau_difficulte, std::array<int,5> variantes) const {

    uint32_t nb_carriere = 0;

    uint32_t nb_place_bleue = 0, nb_place_rouge = 0, nb_place_verte = 0, nb_place_violet = 0, nb_place_jaune = 0;
    uint32_t points_bleu = 0, points_rouge = 0, points_vert = 0, points_violet = 0, points_jaune = 0;

    for (size_t i = 0; i < tuiles.size(); ++i) {
        const auto& hexas = tuiles[i]->get_hexagones();
        for (size_t j = 0; j < hexas.size(); ++j) {
            Hexagone* h = hexas[j];
            if (!h) continue; 

            switch (h->getType()) {
                case Type::Carriere:
                    nb_carriere++;
                    break;

                case Type::Place:
                    switch (h->getCouleur()) {
                        case Couleur::Bleu:   nb_place_bleue++; break;
                        case Couleur::Jaune:  nb_place_jaune++; break;
                        case Couleur::Rouge:  nb_place_rouge++; break;
                        case Couleur::Violet: nb_place_violet++; break;
                        case Couleur::Vert:   nb_place_verte++; break;
                    }
                    break;

                case Type::Quartier:
                    switch (h->getCouleur()) {
                        case Couleur::Bleu:   points_bleu++; break;
                        case Couleur::Jaune:  points_jaune++; break;
                        case Couleur::Rouge:  points_rouge++; break;
                        case Couleur::Violet: points_violet++; break;
                        case Couleur::Vert:   points_vert++; break;
                    }
                    break;
            }
        }
    }
    
    uint32_t total =
          points_bleu   * nb_place_bleue   * 1
        + points_jaune  * nb_place_jaune   * 2
        + points_rouge  * nb_place_rouge   * 2
        + points_vert   * nb_place_verte   * 3
        + points_violet * nb_place_violet  * 2;

    switch (niveau_difficulte) {
        case 0: return total;
        case 1: return total + 2 * nb_carriere;
        case 2: return total * 2;
        default: return total; 
    }
}
void CiteIllu::afficher()const{
    std::cout << "\n======= Tuile Illu =======\n\n";
    for (size_t i = 0; i < tuiles.size(); ++i) {
        std::cout << "Tuile " << i << " : ";
        tuiles[i]->afficherData();
    }
}

void Cite::updateHexSrcFromFan(Hexagone *src, Hexagone *fan)
{
    uint8_t FanVoisinsBinl = fan->getVoisinsNonFantomeBin();
    for (int i = 0; i < 8; ++i)
    {
        if (FanVoisinsBinl & (0b10000000 >> i))
        {
            //std::cout << "index voisin a connecter: " << i << "\n";
            // si bit == 1 alors il faut faire la connection
            Hexagone *voisinFantome = fan->getVoisinIndice(i);
            src->setVoisinIndice(i, voisinFantome);
            voisinFantome->setVoisinIndice(Utils::opposite_index(i), src);
        }
    }
};

