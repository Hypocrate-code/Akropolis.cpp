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

enum class ModeDeJeu {
  Multi,
  Solo,
  PasCommence
};
class Jeu {

  public:
    ~Jeu()=default;

    static Jeu* getInstance();
    Jeu(Jeu &other) = delete;
    void operator=(const Jeu&) = delete;

    void afficherTuiles() const;
    void afficherHexagones() const;
    void tourJoueur(Joueur* joueur);
    void Initialiser(const int& nbJoueur);

    void createPlayers(const std::vector<std::string>& names);


    //void Jouer(Joueur j);   
    void Lancer();   
    std::array<Tuile*, 61> tuilesCite;

    const std::array<Tuile*, 61>& getTuilesCite() const {
        return tuilesCite;
    }

    void mettreAJourChantier();

    void afficherChantier() const;

    Tuile* choisirTuileDuChantier();

    ModeDeJeu getModeDeJeu() const; // A voir si utile, mode solo ?
    void setGameMode(ModeDeJeu mdj) { mode = mdj; }

    uint32_t getNbPlayers() const { return joueurs.size(); }
    void setMaxPlayers(uint32_t n) { joueurs.reserve(n); maxPlayers = n; }
    uint32_t getMaxPlayers() const { return maxPlayers; }
    void addJoueur(Joueur* j) { joueurs.push_back(j); }
    const std::vector<Joueur*>& getJoueurs() const { return joueurs; }


  protected:
    Jeu();
    static Jeu* instance;
    ModeDeJeu mode; // A voir pendant développement mode solo, initialiser Jeu avec mode solo
    std::vector<Hexagone*> hexs;
    std::vector<TuileDepart *> tuilesDepart;
    std::vector<Joueur *> joueurs;
    uint32_t maxPlayers;
    
    Pioche* pioche = nullptr;  
    std::vector<Tuile*> chantier; 

    bool QtDisplay = true;
};

#endif //JEU_H