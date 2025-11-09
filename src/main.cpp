//
// Created by thiba on 06/11/2025.
//
#include <iostream>

#include "Exception.hpp"
#include "Jeu.hpp"
#include "Tuile.hpp"
#include "Cite.hpp"

int main(){
    try {
        std::cout << "Start of program." << std::endl << std::endl;
        Jeu* j = Jeu::getInstance();
        int nJoueur;
        std::cout << "=============================" << std::endl;
        std::cout << "Welcome to Akropolis.cpp !" << std::endl;
        std::cout << "=============================" << std::endl << std::endl;
        std::cout << "A game is about to launch, how many are you :  ";
        std::cin >> nJoueur;
        j->Initialiser(nJoueur);
        std::cout << std::endl << "Program correctly ended." << std::endl;
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
        std::cout << "Test222";
        std::cout << e.getInfo();
        return 1;
    }
}