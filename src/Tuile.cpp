#include "Tuile.hpp"
#include "Exception.hpp"
#include <iostream>

Hexagone::Hexagone(Type t, Couleur c):type(t),couleur(c){
    if((type==Type::Carriere && couleur!=Couleur::nulle)||(type!=Type::Carriere && couleur==Couleur::nulle)){
        throw Exception("le type ne correspond pas à la couleur indiquée"); 
    }
    for(int i=0; i<6; i++){
        voisins[i]=nullptr; // à la creation des hexagones, pas de voisins ne font pas encore partie des tuiles
    }

}; 

Tuile::Tuile(const Hexagone& hex1,const Hexagone& hex2,const Hexagone& hex3):hexagones{hex1,hex2,hex3}{// on considère que les hexagone existent deja au moment de la creation des tuiles 
                                                        // permet quand on creer les hexagones de les "mélanger" avant de les attribuer à des tuiles
     
    // définitions des voisins --> pour l'instant on a pas mis d'hexagone fantômes 
    //définition voisins hex1
    hexagones[0].setVoisinsNE(nullptr);
    hexagones[0].setVoisinsE(nullptr);
    hexagones[0].setVoisinsSE(&hexagones[2]); 
    hexagones[0].setVoisinsSO(&hexagones[1]);
    hexagones[0].setVoisinsO(nullptr);
    hexagones[0].setVoisinsNO(nullptr);
    
     

    //définition voisins hex2
    hexagones[1].setVoisinsNE(&hexagones[0]);
    hexagones[1].setVoisinsE(&hexagones[2]);
    hexagones[1].setVoisinsSE(nullptr); 
    hexagones[1].setVoisinsSO(nullptr);
    hexagones[1].setVoisinsO(nullptr);
    hexagones[1].setVoisinsNO(nullptr);

    //définition voisins hex3
    hexagones[2].setVoisinsNE(nullptr);
    hexagones[2].setVoisinsE(nullptr);
    hexagones[2].setVoisinsSE(nullptr); 
    hexagones[2].setVoisinsSO(nullptr);
    hexagones[2].setVoisinsO(&hexagones[1]);
    hexagones[2].setVoisinsNO(&hexagones[0]);
   
}; 


Tuile Tuile::rotation(){
    
    *this = Tuile(hexagones[1],hexagones[2],hexagones[0]); 

    return *this; 

}; 
TuileDepart::TuileDepart():Tuile(Hexagone(Type::Carriere, Couleur::nulle), 
                Hexagone(Type::Carriere, Couleur::nulle), 
                Hexagone(Type::Carriere, Couleur::nulle)){
            hexagones.push_back(Hexagone(Type::Place,Couleur::Bleu)); // rajout du centre de la tuile 


            // on gère les voisins : 
            //redéfinition voisins hex1
            hexagones[0].setVoisinsSO(&hexagones[3]);
            hexagones[0].setVoisinsSE(nullptr);

            //reéfinition voisins hex2
            hexagones[1].setVoisinsNE(nullptr);
            hexagones[1].setVoisinsE(&hexagones[3]);

            //redéfinition voisins hex3
            hexagones[2].setVoisinsNO(&hexagones[3]);
            hexagones[2].setVoisinsO(nullptr);

            //définition voisins hexgone/place centrale hex4
            hexagones[3].setVoisinsO(&hexagones[0]);
            hexagones[3].setVoisinsNE(&hexagones[1]); 
            hexagones[3].setVoisinsSE(&hexagones[2]); 

        }; 

// pour gérer les affichages pour les tests : 
std::string toString(Couleur c) {
    switch (c) {
        case Couleur::Rouge: return "Rouge";
        case Couleur::Vert:  return "Vert";
        case Couleur::Bleu:  return "Bleu";
        case Couleur::Jaune:  return "Jaune";
        case Couleur::Violet:  return "violet";
        case Couleur::nulle :   return "";
        default: return "Inconnue";
    }
}
std::string toString(Type t) {
    switch (t) {
        case Type::Carriere: return "Carriere";
        case Type::Quartier:  return "Quartier";
        case Type::Place:  return "Place";
        default: return "Inconnu";
    }
}
std::ostream& operator<<(std::ostream& os, Couleur c) {
    return os << toString(c);
}
std::ostream& operator<<(std::ostream& os, Type t) {
    return os << toString(t);
}
