#include "Tuile.hpp"
#include "Exception.hpp"
#include <iostream>

Hexagone::Hexagone(Type t, Couleur c = Couleur::nulle)
{
    if ((type == Type::Carriere && couleur != Couleur::nulle) || (type != Type::Carriere && couleur == Couleur::nulle))
    {
        throw Exception("le type ne correspond pas à la couleur indiquée");
    }
    for (int i = 0; i < 6; i++)
    {
        voisins[i] = nullptr; // à la creation des hexagones, pas de voisins ne font pas encore partie des tuiles
    }
};
Tuile::Tuile(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3)
{   // on considère que les hexagone existent deja au moment de la creation des tuiles
    // permet quand on creer les hexagones de les "mélanger" avant de les attribuer à des tuiles
    hexagones[0] = &hex1;
    hexagones[1] = &hex2;
    hexagones[2] = &hex3;
    // définitions des voisins --> pour l'instant on a pas mis d'hexagone fantômes
    // définition voisins hex1
    hex1.setVoisinsSO(&hex2);
    hex1.setVoisinsSE(&hex3);

    // définition voisins hex2
    hex2.setVoisinsNE(&hex1);
    hex2.setVoisinsE(&hex3);

    // définition voisins hex3
    hex2.setVoisinsNO(&hex1);
    hex2.setVoisinsO(&hex2);

    // à faire : initialiser la position (que ce soit dans dans la pioche)
};

Tuile Tuile::rotation()
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
};
TuileDepart::TuileDepart() : Tuile(Hexagone(Type::Carriere, Couleur::nulle),
                                   Hexagone(Type::Carriere, Couleur::nulle),
                                   Hexagone(Type::Carriere, Couleur::nulle))
{
    this->hexagones.push_back(new Hexagone(Type::Place, Couleur::Bleu)); // rajout du centre de la tuile

    // on gère les voisins :
    // redéfinition voisins hex1
    hexagones[0]->setVoisinsSO(hexagones[3]);
    hexagones[0]->setVoisinsSE(nullptr);

    // reéfinition voisins hex2
    hexagones[1]->setVoisinsNE(nullptr);
    hexagones[1]->setVoisinsE(hexagones[3]);

    // redéfinition voisins hex3
    hexagones[2]->setVoisinsNO(hexagones[3]);
    hexagones[2]->setVoisinsO(nullptr);

    // définition voisins hexgone/place centrale hex4
    hexagones[3]->setVoisinsO(hexagones[0]);
    hexagones[3]->setVoisinsNE(hexagones[1]);
    hexagones[3]->setVoisinsSE(hexagones[2]);
};