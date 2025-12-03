#ifndef TUILESFANTOMES_H_INCLUDED
#define TUILESFANTOMES_H_INCLUDED

#pragma once
#include "Tuile.hpp"
#include <vector>
#include <unordered_set>

namespace HexagoneFantome{
class GenerateurFantome {
public:
    // Renvoie une liste de tous les hexagones fantômes créés
    static std::vector<Hexagone*> genererAutour(const std::vector<const Tuile*>& tuilesExistantes);

private:
    static bool emplacementLibre(const Hexagone* hex);
    static Hexagone* creerFantomeVoisin(Hexagone* centre, int direction);
};

}
#endif // TUILESFANTOMES_H_INCLUDED
