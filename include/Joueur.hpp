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
    void setNbPierre(uint32_t nouveau_nb_pierre){nbPierres=nouveau_nb_pierre; }
    Cite* getCite() const { return cite; }


    protected:
        std::string nom;
        uint32_t nbPierres;

        Cite* cite;
};

class Illu : public Joueur{
public :
    Illu(uint32_t nb_p, const Tuile* tdD); 
private : 
    
}; 