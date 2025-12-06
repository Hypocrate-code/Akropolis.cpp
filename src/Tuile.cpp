#include "Tuile.hpp"
#include "Cite.hpp"
#include "Exception.hpp"
#include <iostream>

#include "Jeu.hpp"
#include "Utils.hpp"

using namespace Utils;
Hexagone::Hexagone(Type t, Couleur c) : type(t), couleur(c)
{
    if ((type == Type::Carriere && couleur != Couleur::nulle) || (type != Type::Carriere && type != Type::Fantome && couleur == Couleur::nulle))
    {
        throw Exception("le type ne correspond pas à la couleur indiquée");
    }
    if(type == Type::Fantome){
        couleur = Couleur::nulle;
    }
    for (int i = 0; i < 8; i++)
    {
        voisins[i] = nullptr;
    }
};

void Hexagone::afficherData() const
{
    if (this == nullptr) {
        // std::cout << "nullptr" << std::endl;
        return;
    }
    std::cout << "  Type: " << type_to_string(getType())
              << ", Couleur: " << color_to_string(getCouleur()) << std::endl;

    // Afficher les voisins
    std::cout << "  Voisins: ";
    std::cout << "NE:" << (getVoisinsNE() ? "O" : "X");
    std::cout << " S:" << (getVoisinsS() ? "O" : "X");
    std::cout << " SE:" << (getVoisinsSE() ? "O" : "X");
    std::cout << " SO:" << (getVoisinsSO() ? "O" : "X");
    std::cout << " N:" << (getVoisinsN() ? "O" : "X");
    std::cout << " NO:" << (getVoisinsNO() ? "O" : "X");
    std::cout << " TOP:" << (getVoisinsTOP() ? "O" : "X");
    std::cout << " BOT:" << (getVoisinsBOT() ? "O" : "X");
    std::cout << std::endl;
}

Tuile::Tuile(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3)
{
    hexagones.push_back(&hex1);
    hexagones.push_back(&hex2);
    hexagones.push_back(&hex3);

    hex1.setVoisinsSE(&hex2);
    hex1.setVoisinsS(&hex3);

    hex2.setVoisinsNO(&hex1);
    hex2.setVoisinsSO(&hex3);

    hex3.setVoisinsN(&hex1);
    hex3.setVoisinsNE(&hex2);
};

void Tuile::afficherData() const
{
    std::cout << "=== DONNEES DE LA TUILE ===" << std::endl;

    std::cout << "Nombre d'hexagones: " << hexagones.size() << std::endl;

    for (size_t i = 0; i < hexagones.size(); ++i)
    {
        std::cout << "Hexagone " << i + 1 << ":" << std::endl;
        if (hexagones[i])
            hexagones[i]->afficherData();
    }

    std::cout << "============================" << std::endl;
}


Tuile *Tuile::rotate()
{
    /*permet la rotation de la tuile en invertissant les hexagones */

    //enregistre les voisins des hex de la tuile 
     const std::array<Hexagone*,8>& voisins_0=this->get_hexagones()[0]->getVoisins(); 
     const std::array<Hexagone*,8>& voisins_1=this->get_hexagones()[1]->getVoisins(); 
     const std::array<Hexagone*,8>& voisins_2=this->get_hexagones()[2]->getVoisins(); 

    //suppresion de tous les voisins des hex
    this->reset_hex_links(); 

    // rotation des hexagones
    std::swap(hexagones[0], hexagones[2]); 
    std::swap(hexagones[1], hexagones[2]);

    
    // ch hex prend les voisins ext de l'hex apres lui
     for (int i=0; i<=0; i++ ){
        this->hexagones[0]->setVoisinIndice(i,voisins_2[i]); 
     }
     for (int i=0; i<=0; i++ ){
        this->hexagones[1]->setVoisinIndice(i,voisins_0[i]); 
     }
     for (int i=0; i<=0; i++ ){
        this->hexagones[2]->setVoisinIndice(i,voisins_1[i]); 
     }



     // on met les bons voisins au centre de la tuile 
     hexagones[0]->setVoisinsS(hexagones[2]); 
     hexagones[2]->setVoisinsN(hexagones[0]); 

     hexagones[0]->setVoisinsSE(hexagones[1]); 
     hexagones[1]->setVoisinsNO(hexagones[0]); 

     hexagones[2]->setVoisinsNE(hexagones[1]); 
     hexagones[1]->setVoisinsSO(hexagones[2]); 
     
    return this;
};

void Tuile::reset_hex_links()
{
    for (auto &hex : hexagones)
    {
        hex->setVoisins(nullptr);
    }
}

TuileDepart::TuileDepart(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3, Hexagone &hexCentre) : Tuile(hex1, hex2, hex3)
{

    this->hexagones.push_back(&hexCentre); // rajout du centre de la tuile
    reset_hex_links();
    // On gère les voisins :

    // Redéfinition voisins hex1
    hexagones[0]->setVoisinsS(hexagones[3]);

    // Redéfinition voisins hex2
    hexagones[1]->setVoisinsNE(hexagones[3]);

    // Redéfinition voisins hex3
    hexagones[2]->setVoisinsNO(hexagones[3]);

    // Définition voisins hexgone/place centrale hex4
    hexagones[3]->setVoisinsN(hexagones[0]);
    hexagones[3]->setVoisinsSO(hexagones[1]);
    hexagones[3]->setVoisinsSE(hexagones[2]);
    hexagones[3]->setVoisinsNE(nullptr);

};

std::ostream& operator<<(std::ostream& os, Couleur c) {
    return os << Utils::color_to_string(c);
}
std::ostream& operator<<(std::ostream& os, Type t) {
    return os << type_to_string(t);
}


Hexagone::~Hexagone(){
    for (auto& voisin : voisins) {
        if (voisin){
            voisin->removeConnection(this);
            voisin = nullptr;
        }
    } 
}

void Hexagone::removeConnection(const Hexagone* hex) {
    for (auto& voisin : voisins){
        if (voisin == hex)
            voisin = nullptr;
    }
}

