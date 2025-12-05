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
void Tuile::set_hex_fantome()
{
    for (size_t j = 0; j < hexagones.size() ; j++)
    {
        std::array<const Hexagone*, 8> voisins = hexagones[j]->getVoisins();
        for (size_t i = 0; i < voisins.size() - 2; i++) { // Skip top et bottom
            if (voisins[i] == nullptr) {
                // cite->afficher();
                Hexagone* newHex = cite->get_hex_fantome();
                // std::cout << "indice i : " << i << std::endl;
                hexagones[j]->setVoisinIndice(i, newHex);

                // PARCOURS AUTOUR DE L'HEX FANTOME PAR LA GAUCHE POUR CHERCHER LES LIAISONS
                int k = indiceDeGauche(i);
                Hexagone* voisinDeGauche = const_cast<Hexagone*>(hexagones[j]->getVoisinIndice(k));
                while (voisinDeGauche != nullptr) {
                    // std::cout << "pas nullptr : " << std::endl;
                    // voisinDeGauche->afficherData();
                    k = indiceDeDroite(indiceDeDroite(k));
                    voisinDeGauche->setVoisinIndice(k, newHex);
                    k = indiceDeGauche(k);
                    voisinDeGauche = const_cast<Hexagone*>(voisinDeGauche->getVoisinIndice(k));
                }


                // PARCOURS AUTOUR DE L'HEX FANTOME PAR LA DROITE POUR CHERCHER LES LIAISONS
                k = indiceDeDroite(i);
                Hexagone* voisinDeDroite = const_cast<Hexagone*>(hexagones[j]->getVoisinIndice(k));
                while (voisinDeDroite != nullptr) {
                    // std::cout << "pas nullptr : " << std::endl;
                    // voisinDeDroite->afficherData();
                    k = indiceDeGauche(indiceDeGauche(k));
                    voisinDeDroite->setVoisinIndice(k, newHex);
                    k = indiceDeDroite(k);
                    voisinDeDroite = const_cast<Hexagone*>(voisinDeDroite->getVoisinIndice(k));
                }

                // std::cout << "Fin des voisins de l'hex fantome : " << newHex->getIndice() << std::endl;

            }
        }
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