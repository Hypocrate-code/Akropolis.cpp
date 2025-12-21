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
  PasCommence
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
    void tourJoueur(Joueur* joueur);
    void tourIllu(Illu* illu); 


    // FONCTIONNES AFFICHAGE PARTIE EN MODE CONSOLE
    void StartMenuC();


    // FONCTIONS APPELLES PAR QT ET CONSOLE:
    void InitialiserPartie(const std::vector<std::string>& names, uint32_t difficultyLevel);

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

    inline ModeDeJeu getModeDeJeu() const {return mode; }; // A voir si utile, mode solo ?
    //void setGameMode(ModeDeJeu mdj) { mode = mdj; }
    
    uint32_t getNbPlayers() const { return joueurs.size(); }
    void setMaxPlayers(uint32_t n) { joueurs.reserve(n); maxPlayers = n; }
    uint32_t getMaxPlayers() const { return maxPlayers; }
    void addJoueur(Joueur* j) { joueurs.push_back(j); }
    const std::vector<Joueur*>& getJoueurs() const { return joueurs; }
    
    int getNiveauDeDifficulte() const { return niveauDeDifficulte; }
    void set_niveau_difficulte(int n){
      if(0<=n && n<=2){
        niveauDeDifficulte=n; }
      else  {
          throw Exception("Niveau de difficulté invalide"); 
      
      }
    
    }


    Joueur* getCurrentPlayer() const { return currentPlayer; }

    const std::vector<Tuile*>& getChantier() const { return chantier; }

    int choisirHexagoneDeReference(Tuile* t);

    void setQtDisplay(bool qt){ QtDisplay=qt; }

    void removeTuileFromChantier(Tuile* t){
      auto it = std::find(chantier.begin(), chantier.end(), t);
      if(it != chantier.end()){
        chantier.erase(it); 
      }
    }

    // Système de placement de tuile
    // Retourne le coût en pierre de la tuile sélectionnée
    int getTuilePrice(Tuile* tuile) const;
    
    // Placement console : demande l'hexagone fantôme puis délègue au placement commun
    bool placerTuileConsole(Joueur* joueur, Tuile* tuile, Hexagone* hexChantier);

    // Gère le placement complet: placement + déduction pierres + retrait du chantier
    // Retourne true si succès, false sinon
    bool executerPlacementTuile(Joueur* joueur, Tuile* tuile, Hexagone* hexChantier, Hexagone* hexCite);

    // Gestion des tours pour Qt
    void nextPlayer();  // Passe au joueur suivant
    void executerTourIllu();  // Exécute le tour de l'Illustre Architecte (mode Qt)
    bool isGameOver() const;  // Vérifie si la partie est terminée
    Joueur* getWinner() const;  // Retourne le joueur avec le plus de points

  protected:
    Jeu();

    static Jeu* instance;
    ModeDeJeu mode; // A voir pendant développement mode solo, initialiser Jeu avec mode solo
    std::vector<Hexagone*> hexs;
    std::vector<TuileDepart *> tuilesDepart;
    std::vector<Joueur *> joueurs;
    Joueur* currentPlayer;


    uint32_t maxPlayers;
    
    Pioche pioche;  
    std::vector<Tuile*> chantier; 

    bool QtDisplay = true;
    int nombreTuilesChantier; 
    int niveauDeDifficulte; 


};

#endif //JEU_H