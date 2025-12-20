//
// Created by thiba on 06/11/2025.
//
#include <iostream>
#include "Tuile.hpp"

#include "Exception.hpp"
#include "Jeu.hpp"
#include <QApplication>
#include <QLabel>

#include "UI/mainwindow.h"

#include <QLocale>
#include <QTranslator>
#include <execution>




int main( int argc, char *argv[] ){
    try {

        if (argc > 1 && std::string(argv[1]) == "--no-gui") {
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
        }
        else {


        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        
        return a.exec();
        }
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
        int nbJoueur;
        std::cout << "Une partie est sur le point de commencer, combien êtes-vous :  ";
        std::cin >> nbJoueur;
        Jeu* j = Jeu::getInstance(nbJoueur);
        j->Initialiser(nbJoueur);
        if(j->getModeDeJeu()==ModeDeJeu::Solo){
            std::cout<<"Choisissez le niveau de difficulté de l'illustre architecte (de 0 à 2) : \n"
                    <<"0. Hippodamos (niveau Facile) : tous les Quartiers d’Hippodamos sont considérés comme étant au 1er niveau.\n"
                    <<"1. Métagénès (niveau Moyen) : tous les Quartiers de Métagénès sont considérés comme étant au 1er niveau. Chaque Carrière de Métagénès lui rapporte 2 points supplémentaires.\n"
                    <<"2. Callicratès (niveau Difficile) : tous les Quartiers de Callicratès sont considérés comme étant au 2e niveau.\n"; 
            int niveau_difficulte=0; 
            cin>>niveau_difficulte; 
            j->set_niveau_difficulte(niveau_difficulte); 
            }
            

      
        j->Lancer();

        std::cout << std::endl << "Fin d'exécution d'Akropolis.cpp" << std::endl;
        j->EndGame();
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