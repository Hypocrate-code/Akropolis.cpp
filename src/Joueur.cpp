#include "Joueur.hpp"
#include "Exception.hpp"

Joueur::Joueur(const char* nom, uint32_t nbPierres, const Tuile* tuileDeDepart) :
    nom{nom}, nbPierres{nbPierres}, cite{nullptr} {
    if (!tuileDeDepart)
        throw Exception("Tuile De Depart manquante");
    else
        cite = std::make_shared<CiteJoueur>(tuileDeDepart);
}
