#include "Tuile.hpp"
#include "Exception.hpp"
#include <iostream>
#include "Utils.hpp"

using namespace Utils;
Hexagone::Hexagone(Type t, Couleur c) : type(t), couleur(c)
{
    if ((type == Type::Carriere && couleur != Couleur::nulle) || (type != Type::Carriere && couleur == Couleur::nulle))
    {
        throw Exception("le type ne correspond pas à la couleur indiquée");
    }
    for (int i = 0; i < 8; i++)
    {
        voisins[i] = nullptr; // à la creation des hexagones, pas de voisins ne font pas encore partie des tuiles
    }
};

void Hexagone::afficherData() const
{
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
    /*Hexagone* copie[2];
    for (int i =0; i<=2; i++){
        copie[i]=hexagones[i];
    }
    hexagones[0]=copie[1];
    hexagones[1]=copie[2];
    hexagones[2]=copie[3];

    // modification des voisins des hexagones à gérer
    // position à gérer
    */

    // autre solution :
    *this = Tuile(*hexagones[1], *hexagones[2], *hexagones[0]);
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




std::array<const Hexagone*, 6> Hexagone::getVoisins3D() const{
    std::array<const Hexagone*,8> voisins8 = getVoisins();

    std::array<const Hexagone*, 6> voisins6{}; // init à nullptr

    
    std::copy(voisins8.begin(), voisins8.begin() + 6, voisins6.begin());

    const Hexagone* copie = this; 
    while(copie!=nullptr){ 
        std::array<const Hexagone*,8> voisinsNiveau = copie->getVoisins(); 
        for(int i=0; i<6; i++){
            if(voisinsNiveau[i]!=nullptr && voisins6[i]==nullptr ){
                voisins6[i]=voisinsNiveau[i]; 
            }
        }
        copie = copie->getVoisinsBOT();
    }
    return voisins6;
}


int Hexagone::getNiveau()const{
    const Hexagone*copie= this ; 
    int niveau =1;  
        while(copie->getVoisinsBOT()!=nullptr){
            niveau++; 
            copie=copie->getVoisinsBOT(); 

        }
    return niveau; 
};