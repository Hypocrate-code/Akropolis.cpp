#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

#include "Tuile.hpp"
#include "Jeu.hpp"


class Pioche {
private:
    vector<Tuile*> tuiles;
public:
    Pioche(const Pioche&) = delete;
    Pioche& operator=(const Pioche&) = delete;

    explicit Pioche(const Jeu& jeu);

    Tuile* piocher();

    size_t getNbTuiles() const { return tuiles.size(); }
    bool estVide() const { return tuiles.empty(); }
  
};
