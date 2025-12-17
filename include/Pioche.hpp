#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

#include "Tuile.hpp"

class Jeu; 

class Pioche {
private:
    vector<Tuile*> tuiles;
public:
    Pioche(const Pioche&) = delete;
    void init();
    Pioche& operator=(const Pioche&) = delete;

    Pioche(const Jeu& jeu);

    Tuile* piocher();

    size_t getNbTuiles() const { return tuiles.size(); }
    bool estVide() const { return tuiles.empty(); }

    private:
    Jeu const& jeuRef;
  
};
