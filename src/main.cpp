//
// Created by thiba on 06/11/2025.
//
#include <iostream>
#include "Tuile.hpp"

#include "Exception.hpp"
#include "Jeu.hpp"
#include "Tuile.hpp"
#include "Cite.hpp"

int main(){
    try {
        //test rotate 
        // Hexagone hex1{Type::Place, Couleur::Rouge};
        // Hexagone hex2{Type::Place, Couleur::Bleu};
        // Hexagone hex3{Type::Place, Couleur::Jaune};
        //
        // Tuile tl1{hex2, hex1, hex3};
        // CiteJoueur c1{&tl1};
        //
        // c1.afficher();
        // tl1.rotate();
        // c1.afficher();


        std::cout << "\n=============================" << std::endl;
        std::cout << "Bienvenue sur Akropolis.cpp !" << std::endl;
        std::cout << "=============================" << std::endl << std::endl;
        Jeu* j = Jeu::getInstance();
        int nJoueur;
        std::cout << "Une partie est sur le point de commencer, combien êtes-vous :  ";
        std::cin >> nJoueur;
        j->Initialiser(nJoueur);
        j->Lancer();

        std::cout << std::endl << "Fin d'exécution d'Akropolis.cpp" << std::endl;
        return 0;

    

        // Hexagone hex1{Type::Place, Couleur::Rouge};
        // Hexagone hex2{Type::Place, Couleur::Bleu};
        // Hexagone hex3{Type::Place, Couleur::Jaune};
        //
        // Tuile tl1{hex2, hex1, hex3};
        //tl1.afficherData();

        //hex1.setVoisinsN(&hexN);
        //hex1.setVoisinsNE(&hexNE);
        //hex1.setVoisinsNO(&hexNO);
        //hex1.setVoisinsS(&hexS);
        //hex1.setVoisinsSE(&hexSE);
        //hex1.setVoisinsSO(&hexSO);
        //hex1.setVoisinsO(&hex4);
        //hex4.setVoisinsNE(&hex1);

        // CiteJoueur cite1{&tl1};
        // cite1.afficher();

    }
    catch (Exception& e) {
        std::cout << e.getInfo();
        return 1;
    }
}