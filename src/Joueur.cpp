#include "Joueur.hpp"
#include "Exception.hpp"
#include "Tuile.hpp"
#include <iostream>


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

void Joueur::MaJPierres(){
    int nlles_pierres = 0; 

    if(!cite->getTuiles().empty()){
        const Tuile*  tuile = cite->getTuiles().back(); 
        auto hexagones = tuile->get_hexagones(); 
        for(auto& hex : hexagones ){
            nlles_pierres += hex->gainPierre(); 
        }
        if(nlles_pierres!=0){
            setNbPierre(getNbPierres()+nlles_pierres); 
        }
    
    }
}; 
    
    
    
