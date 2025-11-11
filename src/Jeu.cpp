//
// Created by thiba on 06/11/2025.
//

#include "Jeu.hpp"
#include <iostream>
#include "Tuile.hpp"
#include <map>
#include <array>
#include <random>
#include <chrono>
#include "Utils.hpp"
#include <ranges>
#include <algorithm>

using std::map;

// Opérations de singleton

Jeu* Jeu::instance = nullptr;

Jeu* Jeu::getInstance() {
  if (instance) {
    return instance;
  }
  instance = new Jeu();
  return instance;
}

// Fin opérations de singleton


// Constructeur créant les tuilesCité de la partie
Jeu::Jeu() : mode{ModeDeJeu::Solo} {

  // Définition des différentes quantités d'hexagones dans chaque catégorie
  // Soit 183 hors hexagones pour tuiles de départ (eux créer dans Initialiser)
  map<Type, map<Couleur, int>> totauxParCouleur;

  map<Couleur, int> nCarriere;
  nCarriere[Couleur::nulle] = 57;
  nCarriere[Couleur::Bleu] = 0;
  nCarriere[Couleur::Rouge] = 0;
  nCarriere[Couleur::Jaune] = 0;
  nCarriere[Couleur::Violet] = 0;
  nCarriere[Couleur::Vert] = 0;

  map<Couleur, int> nPlaceParCouleur;
  nPlaceParCouleur[Couleur::Bleu] = 9;
  nPlaceParCouleur[Couleur::Rouge] = 13;
  nPlaceParCouleur[Couleur::Jaune] = 13;
  nPlaceParCouleur[Couleur::Violet] = 13;
  nPlaceParCouleur[Couleur::Vert] = 13;
  nPlaceParCouleur[Couleur::nulle] = 0; // Couleur reservée au carrière

  map<Couleur, int> nQuartierParCouleur;
  nQuartierParCouleur[Couleur::Bleu] = 13;
  nQuartierParCouleur[Couleur::Rouge] = 13;
  nQuartierParCouleur[Couleur::Jaune] = 13;
  nQuartierParCouleur[Couleur::Violet] = 13;
  nQuartierParCouleur[Couleur::Vert] = 13;
  nQuartierParCouleur[Couleur::nulle] = 0; // Couleur reservée au carrière

  totauxParCouleur[Type::Carriere] = nCarriere;
  totauxParCouleur[Type::Place] = nPlaceParCouleur;
  totauxParCouleur[Type::Quartier] = nQuartierParCouleur;

  constexpr std::array<Type, 3> types{Type::Carriere, Type::Place, Type::Quartier};
  constexpr std::array<Couleur, 6> couleurs{Couleur::Rouge, Couleur::Vert, Couleur::Bleu, Couleur::Jaune, Couleur::Violet, Couleur::nulle};

  // Création des hexagones dans l'ordre
  for (auto& type : types) {
    if (type == Type::Carriere) {
      for (size_t i = 0; i < totauxParCouleur[type][Couleur::nulle]; i++) {
        hexs.push_back(new Hexagone(Type::Carriere, Couleur::nulle));
      }
    }
    else {
      for (const Couleur& couleur : couleurs) {
        if (couleur == Couleur::nulle) continue;
        for (size_t i = 0; i < totauxParCouleur[type][couleur]; i++) {
          hexs.push_back(new Hexagone(type, couleur));
        }
      }
    }
  }

  // Hexagones créés de manière ordonnée, place au mélange
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::ranges::shuffle(hexs.begin(), hexs.end(), std::default_random_engine(seed));

  // Création des tuilesCité
  for (size_t i = 0 ; i < tuilesCite.size() ; i++) {
    const size_t j = i*3;
    tuilesCite[i] = new Tuile(*hexs[j], *hexs[j+1], *hexs[j+2]);
  }

  // -> Passer les tuiles à la Pioche / Chantier ??

}


// Initialisation et lancement de la partie avec un nombre de joueur indiqué
void Jeu::Initialiser(const int& nbJoueur) {

  std::vector<std::string> names;

  for (int i=0; i < nbJoueur ; i++) {

    // Accueil du joueur i+1
    std::string name;
    std::cout << "What's your name, player " << i+1 << " :  ";
    std::cin >> name;
    std::cout << "Thanks " << name << "." << std::endl;

    // Création des hexagones pour la tuile de départ du joueur i+1
    for (size_t j = 0; j < 3; j++) {
      hexs.push_back(new Hexagone(Type::Carriere, Couleur::nulle));
    }
    hexs.push_back(new Hexagone(Type::Place, Couleur::Bleu));

    // Création de la tuile de départ
    const size_t n = hexs.size() - 4;
    tuilesDepart.push_back(new TuileDepart(*hexs[n], *hexs[n+1], *hexs[n+2], *hexs[n+3]));

    // Création du joueur
    joueurs.push_back(new Joueur(name.c_str(), 2, tuilesDepart[tuilesDepart.size() - 1]));
  }

  for (auto& j : joueurs) {
    std::cout << "Player : " << j->getNom() << std::endl;
    std::cout << "Rocks count : " << j->getNbPierres() << std::endl;
    j->getCite()->afficher();
  }

}

// Méthodes d'affichage

void Jeu::afficherTuiles() const {
  // for(const auto& tuile : tuilesCite) {
  //   tuile->afficherData();
  // }  Print les tuiles de manière plus ordonnée :
  for (size_t i = 0 ; i < tuilesCite.size() ; i++) {
    std::cout << "Tuile " << i+1 << std::endl;
    tuilesCite[i]->afficherData();
    std::cout << std::endl;
  }
}

void Jeu::afficherHexagones() const {
  for(size_t i = 0; i < hexs.size() ; i++) {
    std::cout << "Hexagone " << i+1 << std::endl;
    hexs[i]->afficherData();
    std::cout << std::endl;
  }
}