#ifndef TUILESFANTOMES_H_INCLUDED
#define TUILESFANTOMES_H_INCLUDED

#pragma once
#include "Tuile.hpp"
#include <vector>

class TuileFantome : public Tuile {
public:
    TuileFantome(Hexagone &h1, Hexagone &h2, Hexagone &h3);
};

class GenerateurFantome {
public:
    static std::vector<TuileFantome*> genererAutour(const std::vector<const Tuile*>& tuilesExistantes);

private:
    static bool emplacementLibre(const Hexagone* hex);
    static TuileFantome* creerFantomeAutour(const Hexagone* centre);
};



#endif // TUILESFANTOMES_H_INCLUDED
