#include "Cite.hpp"
#include <iostream>

Cite::Cite(const Tuile* tuileDeDepart){
    tuiles.push_back(tuileDeDepart);

}

void Cite::afficher() const {
    std::cout << "Nombre de tuiles dans la cite" << tuiles.size() << std::endl;
    if (tuiles.size())
        std::cout << "Nombre d'hexagones dans la cite" << ((tuiles.size()-1) *3 +4) << std::endl;
}

void Cite::QTDraw() const {

}

CiteJoueur::CiteJoueur(const Tuile* tuileDeDepart) : Cite{tuileDeDepart} 
{
    updateTuileFantome();
}


void CiteJoueur::placerTuile(const Tuile* dest) {

}
uint32_t CiteJoueur::compterPoints() const{

    return 0;
}

void CiteJoueur::updateTuileFantome(){

}
