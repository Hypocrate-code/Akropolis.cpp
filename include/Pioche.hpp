#pragma once
#include <iostream>
#include <string>

class Tuile;
//exemple à mettre à jour 
class Jeu {
    size_t nbTuiles;      
public:
    size_t getNbTuiles() const { return nbTuiles; }
    Tuile** tuiles;
};

class Pioche {
    const Tuile** tuiles;
    size_t nb; 
public:
    Pioche(const Pioche&) = delete;
    Pioche& operator=(const Pioche&) = delete;

    explicit Pioche(const Jeu& jeu);

    const Tuile& piocher();

    size_t getNbTuiles() const { return nb; }
    bool estVide() const { return nb == 0; }

    ~Pioche() { delete[] tuiles; }
};