#include "Cite.hpp"
#include "Tuile.hpp"
#include <iostream>
#include <algorithm>
#include "Utils.hpp"

using namespace Utils;

void Cite::print_hex(Hexagone *hex, int x, int y, strCalc &calc) const
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
    std::string colorCode = get_color_code(hex->getCouleur());
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

    add_hex_data(hex, x, y, calc);
}

void Cite::add_hex_data(Hexagone *hex, int x, int y, strCalc &calc) const
{
    std::string resetCode = "\033[0m";
    std::string typeStr = get_color_code(hex->getCouleur()) + get_short_type(hex->getType()) + resetCode;
    std::string colorStr = colorize_string(typeStr, hex->getCouleur());
    std::string colorCode = get_short_color(hex->getCouleur());

    if (y - 1 >= 0 && x - 4 >= 0 && x + 5 <= calc[y - 1].size())
    {
        // std::string topLine = center_string(colorCode + typeStr + resetCode, 3);
        std::string topLine;

        if (hex->getType() == Type::Fantome) {
            std::string indice = std::to_string(hex->getIndice());
            calc[y].erase(x, indice.length());
            calc[y].insert(x - indice.length()/2, indice);
        }
        else {
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
                              std::unordered_set<Hexagone *> &drawnHexagones) const//, int depth) const
{
    if (!hex || drawnHexagones.count(hex))
        return;

    drawnHexagones.insert(hex);
    print_hex(hex, x, y, calc);

    // Proper hexagon grid offsets for pointy-top hexagones

    static const std::vector<std::pair<int, int>> directionOffsets = {
        {0, 4},  // S  (index 0) - hauteur hexagone = 5
        {7, 2},  // SE (index 1) - largeur hexagone = 9
        {7, -2}, // NE (index 2) - largeur hexagone = 9
        {0, -4}, // N  (index 3) - hauteur hexagone = 5
        {-7, -2}, // NO (index 4) - largeur hexagone = 9
        {-7, 2} // SO (index 5) - largeur hexagone = 9
    };

    const Hexagone *neighbors[6] = {
        hex->getVoisinsS(),  // 0: Sud
        hex->getVoisinsSE(), // 1: Sud-Est
        hex->getVoisinsNE(), // 2: Nord-Est
        hex->getVoisinsN(),  // 3: Nord
        hex->getVoisinsNO(),  // 4: Nord-Ouest
        hex->getVoisinsSO() // 5: Sud-Ouest
    };

    for (int i = 0; i < 6; ++i)
    {
        if (neighbors[i])
        {
            Hexagone *neighbor = const_cast<Hexagone *>(neighbors[i]);
            int newX = x + directionOffsets[i].first;
            int newY = y + directionOffsets[i].second;
            draw_hex_recursive(neighbor, newX, newY, calc, drawnHexagones);//, depth + 1);
        }
    }
}

void Cite::afficher() const

{
    std::cout << "=== DEBUG HEXAGONE CONNECTIONS ===" << std::endl;

    if (tuiles.empty()) {return;}
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
            const Hexagone *neighbors[6] = {
                hexagones[j]->getVoisinsNE(),
                hexagones[j]->getVoisinsS(),
                hexagones[j]->getVoisinsSE(),
                hexagones[j]->getVoisinsSO(),
                hexagones[j]->getVoisinsN(),
                hexagones[j]->getVoisinsNO()};

            const char *directionNames[6] = {"NE", "S", "SE", "SO", "N", "NO"};

            for (int k = 0; k < 6; ++k)
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

    std::cout << "=== END DEBUG ===" << std::endl;

    std::cout << "Nombre de tuiles dans la cite: " << tuiles.size() << std::endl;
    if (tuiles.size())
        std::cout << "Nombre d'hexagones dans la cite: " << ((tuiles.size() - 1) * 3 + 4) << std::endl;

    // Initialize canvas
    static strCalc calc = {
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

    draw_hex_recursive(startHex, 50, 10, calc, drawnHexagones);
    
    for (auto &line : calc)
    {
        std::cout << colorize_line(line) << std::endl;
    }
    std::cout << "Total hexagones drawn: " << drawnHexagones.size() << std::endl;
}

void Cite::increase_calc_size_H(strCalc &calc, uint32_t size) const
{
    for (auto &line : calc)
    {
        line = line + std::string(size, ' ');
    }
}

void Cite::increase_calc_size_V(strCalc &calc, uint32_t size) const
{
    strCalc c(size, std::string(calc[0].size(), ' '));
    calc.insert(calc.end(), c.begin(), c.end());
}

Cite::Cite(const Tuile *tuileDeDepart)
{
    this->addTuile(tuileDeDepart);
}


void Cite::addTuile(const Tuile* t)
{
    tuiles.push_back(t);
    const_cast<Tuile*>(t)->cite = this;
}

CiteJoueur::CiteJoueur(const Tuile *tuileDeDepart) : Cite{tuileDeDepart}
{
    updateTuileFantome();
}

void CiteJoueur::placerTuile(const Tuile *dest)
{
}
uint32_t CiteJoueur::compterPoints() const
{

    return 0;
}

HexFantome* Cite::get_hex_fantome() {
    HexFantome* newHex = new HexFantome(hexs_fantome.size());
    hexs_fantome.push_back(newHex);
    return newHex;
}

void CiteJoueur::updateTuileFantome()
{
}
