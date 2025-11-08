#include "Joueur.hpp"
#include "Exception.hpp"
#include <iostream>
#include "Tuile.hpp"

int main(){
    try{

    //Joueur a{"A", 3, nullptr};

    Hexagone hex1{Type::Quartier, Couleur::Rouge};
    Hexagone hex2{Type::Carriere, Couleur::nulle};
    Hexagone hex3{Type::Place, Couleur::Jaune};
    
    Hexagone hexN{Type::Place, Couleur::Vert};
    Hexagone hexS{Type::Place, Couleur::Jaune};
    Hexagone hexNE{Type::Place, Couleur::Violet};
    Hexagone hexNO{Type::Place, Couleur::Violet};
    Hexagone hexSE{Type::Place, Couleur::Vert};
    Hexagone hexSO{Type::Place, Couleur::Violet};

    Tuile tl1{hex1, hex2, hex3};
    tl1.afficherData();

    hex1.setVoisinsN(&hexN);
    hex1.setVoisinsNE(&hexNE);
    hex1.setVoisinsNO(&hexNO);
    hex1.setVoisinsS(&hexS);
    hex1.setVoisinsSE(&hexSE);
    hex1.setVoisinsSO(&hexSO);
    //hex1.setVoisinsO(&hex4);
    //hex4.setVoisinsNE(&hex1);
    
    CiteJoueur cite1{&tl1};

    cite1.afficher();

    }catch (Exception e) {
        std::cout << e.getInfo() << std::endl;
    }

    return 0;
}