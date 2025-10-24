#pragma once
#include "Cite.hpp"
#include <string>
#include <memory>

class Tuile;
class Joueur{
    public:

    Joueur(const char* nom, uint32_t nbPierres, const Tuile* tuileDeDepart);
    ~Joueur() = default;

    private:
        std::string nom;
        uint32_t nbPierres;

        std::shared_ptr<CiteJoueur> cite;
};