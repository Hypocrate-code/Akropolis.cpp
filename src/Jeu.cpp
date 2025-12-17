//
// Created by thiba on 06/11/2025.
//
#include "Pioche.hpp"
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

Jeu* Jeu::getInstance(int nbJoueur) {
  if (instance) {
    return instance;
  }
  instance = new Jeu(nbJoueur);
  return instance;
}

void Jeu::EndGame(){
  for (auto& tuile : tuilesCite) {
      delete tuile;
  }
  tuilesCite.fill(nullptr);

  for (auto& hex : hexs) {
      delete hex;
  }
  hexs.clear();

  for (auto& tuileDepart : tuilesDepart) {
      delete tuileDepart;
  }
  tuilesDepart.clear();

  for (auto& joueur : joueurs) {
      delete joueur;
  }
  joueurs.clear();

  // Libération de l'instance singleton
  delete instance;
  instance = nullptr;
}

// Fin opérations de singleton


// Constructeur créant les tuilesCité de la partie
Jeu::Jeu(int nbJoueur): mode(nbJoueur == 1 ? ModeDeJeu::Solo : ModeDeJeu::Multi), niveauDeDifficulte(0) , pioche(*this) {


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
    tuilesCite[i]->set_indice(i);
  }

  // -> Passer les tuiles à la Pioche / Chantier ??

}


// Initialisation et lancement de la partie avec un nombre de joueur indiqué
void Jeu::Initialiser(const int& nbJoueur) { 

  std::vector<std::string> names;

  for (int i=0; i < nbJoueur ; i++) {

    // Accueil du joueur i+1
    std::string name;
    std::cout << std::endl << "Quel est ton prénom, joueur " << i+1 << " :  ";
    std::cin >> name;
    std::cout << "Merci " << name << "." << std::endl << std::endl;

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

  // creation du joueur illustre architecte pour la partie en mode solo
  if(nbJoueur==1){
    for (size_t j = 0; j < 3; j++) {
      hexs.push_back(new Hexagone(Type::Carriere, Couleur::nulle));
    }
  
  hexs.push_back(new Hexagone(Type::Place, Couleur::Bleu));
    // Création de la tuile de départ
    const size_t n = hexs.size() - 4;

    tuilesDepart.push_back(new TuileDepart(*hexs[n], *hexs[n+1], *hexs[n+2], *hexs[n+3]));
    // Création du joueur
    joueurs.push_back(new Illu( 2, tuilesDepart[tuilesDepart.size() - 1]));
  }
  
  //creation de la pioche en commun
  pioche.init();
}

void Jeu::Lancer() {
    bool fin = false;
    // Tant que le jeu n'est pas terminé
    while (!fin) {
        // Générer les hex fantômes pour chaque joueur au début de la manche
        for (auto& j : joueurs) {
          if(j->getNom()!="Illustre Architecte"){
            j->getCite()->generateAllHexFantome();
          }
            
        }
        // Chaque joueur joue son tour
        for (auto& j : joueurs) {
          if(j->getNom()!="Illustre Architecte"){
            tourJoueur(j); // Inclut affichage cité et placement de tuile
          }else{
            tourIllu(static_cast<Illu*>(joueurs.back())); 
          }
        }

        // Après que tous les joueurs aient joué, demander si +1 tour
        char reponse;
        std::cout << "\nVoulez-vous continuer la partie ? (o/n) : ";
        std::cin >> reponse;
        if (reponse != 'o' && reponse != 'O') {
            fin = true;
        }
    }
    for (auto& j : joueurs) {
        std::cout << j->getCite()->compterPoints(niveauDeDifficulte) << std::endl;
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


void Jeu::tourJoueur(Joueur* joueur) {

    // ===== Infos début du tour =====
    std::cout << "\n==========================\n\n";

    // --- Afficher les informations du joueur avant son tour ---
    std::cout << "Nom : " << joueur->getNom() << std::endl;
    std::cout << "Nombre de pierres : " << joueur->getNbPierres() << std::endl;

    // --- Premier affichage de la cité ---
    joueur->getCite()->afficher();

    std::cout << "Pressez Entrer pour afficher le Chantier...";

    cin.ignore();
    cin.ignore();

  // --- Affichage chantier global partagé ---
    if (chantier.empty()) {            
      mettreAJourChantier();         
    }
    afficherChantier();                        // Affiche le chantier actuel

    // --- Choix de la tuile et débit de pierres (pour le moment manuel) ---
    std::cout << "Pierres disponibles : " << joueur->getNbPierres() << "\n" << std::endl;
    Tuile* tChoisie = choisirTuileDuChantier(joueur); // Le joueur choisit une tuile
    joueur->getCite()->afficher();
    std::cout << "-- Tuile choisie --" << std::endl;
    tChoisie->afficherData();
    // --- Demande de rotation ---
    char reponse;
    std::cout << "\nTourner la tuile ? (o/n) : ";
    std::cin >> reponse;
    while (reponse == 'o' || reponse == 'O') {
      tChoisie->rotate();
      tChoisie->afficherData();
      std::cout << "\nTourner la tuile ? (o/n) : ";
      std::cin >> reponse;
    }

    joueur->getCite()->afficher();
    tChoisie->afficherData();
    // --- 5) Placement de la tuile ---
    Hexagone* hex0 = tChoisie->get_hexagones()[0]; //Le Hexagone 0
    //On va travailler avec l'hexagone 0 de la tuile choisie.
    int res = joueur->getCite()->placerTuileFromHexRef(hex0);
    while (0 == res) res = joueur->getCite()->placerTuileFromHexRef(hex0);

    // --- Affichage final de la cité après le placement ---
    std::cout << "\nTuile correctement placée :\n";
    joueur->getCite()->afficher();

}
void Jeu::tourIllu(Illu* illu){
  // ===== Infos début du tour =====
    std::cout << "\n==========================\n\n";

    // --- Afficher les informations du joueur avant son tour ---
  std::cout << "Nom : " << illu->getNom() << std::endl;
  std::cout << "Nombre de pierres : " << illu->getNbPierres() << std::endl;
  if (chantier.empty()) {            
      mettreAJourChantier();         
    }

  afficherChantier();  

  illu->getCite()->addTuile(choisirTuileDuChantier(illu)); 

  //affichage des tuiles apres son tour 
  illu->getCite()->afficher(); 
  //afficher score après l'ajout de la nouvelle tuile 
  std::cout<<"Score : "<<illu->getCite()->compterPoints(niveauDeDifficulte); 
  

}

//piocher 
// === Met à jour le chantier global si moins de 5 tuiles ===
void Jeu::mettreAJourChantier() {
    while (chantier.size() < 5 && !pioche.estVide()) {
        chantier.push_back(pioche.piocher());
    }
}

// === Affiche le chantier actuel ===
void Jeu::afficherChantier() const {
    //std::cout << "\n======= Chantier =======\n\n";
    //for (size_t i = 0; i < chantier.size(); ++i) {
  //      std::cout << "Tuile " << i << " : ";
//        chantier[i]->afficherData();
    //}

    std::cout << "======= Chantier =======\n\n";
    //draw the tuiles on 1 line
    strCalc calc = strCalc(10, std::string(chantier.size() * 15, ' '));
    for (size_t i = 0; i < chantier.size(); ++i) {
        std::unordered_set<Hexagone*> drawnHexagones;
        Cite::draw_hex_recursive_filtered(chantier[i]->get_hexagones()[0], 6 + i * 20, 2, calc, drawnHexagones, 0, false);
    }
    std::cout << "Tuile indices : \n      ";
    for (size_t i = 0; i < chantier.size(); ++i) {
        std::cout << "[" << i << "]" << std::string(17, ' ');
    }
    std::cout << std::endl;
    for (auto &line : calc) {
        std::cout << Utils::colorize_line(line) << std::endl;
    }

}

// === Permet au joueur de choisir une tuile dans le chantier ===
Tuile* Jeu::choisirTuileDuChantier(Joueur* joueur) {
    if (chantier.empty()) {
        std::cout << "Le chantier est vide.\n";
        return nullptr;
    }

    // Affichage pour vérification
    //afficherChantier();

    size_t choix;
    do {
        std::cout << "Choisissez une tuile par son numéro (0-" << chantier.size()-1 << ") : ";
        std::cin >> choix;
        if (choix >= chantier.size()) {
            std::cout << "Choix invalide. Veuillez réessayer.\n";
        } else if (choix > joueur->getNbPierres()) {
            std::cout << "Vous n'avez pas assez de pierres pour cette tuile. Veuillez réessayer.\n";
        }
    } while (choix >= chantier.size() || choix>joueur->getNbPierres());
    joueur->setNbPierre(joueur->getNbPierres()-choix); 
    if(this->getModeDeJeu()==ModeDeJeu::Solo){
      // l'illustre architecte récupere les pierres 
      this->joueurs.back()->setNbPierre(this->joueurs.back()->getNbPierres()+ choix); 
    }

    Tuile* t = chantier[choix];
    chantier.erase(chantier.begin() + choix);

    // Affichage du chantier restant pour vérification
    /*
    std::cout << "=== Chantier après choix ===\n";
    for (size_t i = 0; i < chantier.size(); ++i) {
        std::cout << i << " : ";
        chantier[i]->afficherData();
    }*/

    return t;
}
Tuile* Jeu::choisirTuileDuChantier(Illu* illu){
  if (chantier.empty()) {
        std::cout << "Le chantier est vide.\n";
        return nullptr;
    }
  // permet de retourner le choix de l'illustre architecte et de lui retirer les pierre que cela lui a couté
  // rappel regle : l'illu archi prend la tuile avec au moins une place la moins chère du chantier 
  // s'il n'a pas assez d'argent ou que aucune tuile n'a de place, il prend la tuile gratuite du chantier 
  //c'est à dire celle à la position 0; 
  bool tuile_ok = false; 
  bool place = false; 
  int index=0; 
for(auto tuile_chantier:chantier){
    for(auto hex:chantier[index]->get_hexagones()){
      if(hex->getType()==Type::Place){
        if(illu->getNbPierres()>=index){//vérifier juste que l'index correspond bien au prix
          illu->setNbPierre(illu->getNbPierres()-index); 
          cout<<"L'illustre architect a choisi la tuile "<<index<<"du chantier \n"; 
          auto t=tuile_chantier; 
          chantier.erase(chantier.begin() + index);
          return t; }else{
          //cas où l'illArchi n'a pas assez de pierre pour s'acheter de place
          cout<<"L'illustre architect a choisi la tuile 0 du chantier \n"; 

          auto t = chantier[0]; 
          chantier.erase(chantier.begin());
          return t; 
          
        }
      }
    }
  }
  // cas ou y n'y a pas de place 
  cout<<"L'illustre architect a choisi la tuile 0 du chantier \n"; 
  auto t = chantier[0]; 
  chantier.erase(chantier.begin());
  return t;  
}