#include "Joueur.hpp"
#include "Exception.hpp"

Joueur::Joueur(const char* nom, uint32_t nbPierres, const Tuile* tuileDeDepart) :
    nom{nom}, nbPierres{nbPierres}, cite{nullptr} {
    if (!tuileDeDepart)
        throw Exception("Tuile De Depart manquante");
    else
        cite = new CiteJoueur(tuileDeDepart);
}
Illu::Illu(uint32_t nb_p, const Tuile* tdD)
    : Joueur("Illustre Architecte", nb_p, tdD)   
{

    delete cite;                 
    cite = new CiteIllu(tdD);
}

