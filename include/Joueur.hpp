#pragma once
#include "Cite.hpp"
#include <string>
#include <memory>

class Tuile;
class Joueur{
    public:

    Joueur(const char* nom, uint32_t nbPierres, const Tuile* tuileDeDepart);
    ~Joueur() = default;

    const std::string& getNom() const { return nom; }
    uint32_t getNbPierres() const { return nbPierres; }
    CiteJoueur* getCite() const { return cite; }


    private:
        std::string nom;
        uint32_t nbPierres;

        CiteJoueur* cite;
};