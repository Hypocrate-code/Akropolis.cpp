//
// Created by thiba on 06/11/2025.
//

#ifndef JEU_H
#define JEU_H

#include <vector>
#include "Pioche.hpp"
#include "Tuile.hpp"
#include "Joueur.hpp"
#include <array>
#include <string>

class Pioche; 
#include "Exception.hpp" 

enum class ModeDeJeu {
  Multi,
  Solo,
};
enum class Duree {
    Courte,
    Longue
};

class Jeu {

  public:
    ~Jeu()=default;
    void EndGame();

    static Jeu* getInstance();
    Jeu(Jeu &other) = delete;
    void operator=(const Jeu&) = delete;

    void afficherTuiles() const;
    void afficherHexagones() const;


    // FONCTIONNES AFFICHAGE PARTIE EN MODE CONSOLE
    void StartMenuC();
    void tourJoueur(Joueur* joueur);
    void tourIllu(Illu* illu);
    void AffFinDePartieC();
    // FONCTIONS APPELLES PAR QT ET CONSOLE:
    void InitialiserPartie(const std::vector<std::string>& names, uint32_t difficultyLevel, std::array<int, 5>variantes={}, Duree d=Duree::Courte);
    Joueur* gagnant();
    //void setDifficultyLevel(uint32_t level) { difficultyLevel = level; }
    //uint32_t getDifficultyLevel() const { return difficultyLevel; }

    //void Jouer(Joueur j);   
    void Lancer();   
    std::array<Tuile*, 61> tuilesCite;

    const std::array<Tuile*, 61>& getTuilesCite() const {
        return tuilesCite;
    }

    void mettreAJourChantier();

    void afficherChantier() const;

    Tuile* choisirTuileDuChantier(Joueur* joueur);
    Tuile* choisirTuileDuChantier(Illu* illu); 

    inline ModeDeJeu getModeDeJeu() const {return mode; };

    uint32_t getNbPlayers() const { return joueurs.size(); }
    int getnombreTuilesPioche()const{return nombreTuilesPioche; }
    void setMaxPlayers(uint32_t n) { joueurs.reserve(n); maxPlayers = n; }
    uint32_t getMaxPlayers() const { return maxPlayers; }
    void addJoueur(Joueur* j) { joueurs.push_back(j); }
    const std::vector<Joueur*>& getJoueurs() const { return joueurs; }

    void set_niveau_difficulte(int n){
      if(0<=n && n<=2){
        niveauDeDifficulte=n; }
      else  {
          throw Exception("Niveau de difficulté invalide"); 
      
      }
    
    }
    int choisirHexagoneDeReference(Tuile* t);

    void setQtDisplay(bool qt){ QtDisplay=qt; }

  protected:
    Jeu();

    static Jeu* instance;
    ModeDeJeu mode;
    Duree duree;
    std::vector<Hexagone*> hexs;
    std::vector<TuileDepart *> tuilesDepart;
    std::vector<Joueur *> joueurs;
    uint32_t maxPlayers;
    
    Pioche pioche;  
    std::vector<Tuile*> chantier; 

    bool QtDisplay = true;
    int nombreTuilesChantier; 
    int niveauDeDifficulte;
    std::array<int, 5> variantes;
    int nombreTuilesPioche;


};

#endif //JEU_H
