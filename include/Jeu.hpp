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
    ModeDeJeu getModeDeJeu() const; // A voir si utile, mode solo ?
    void tourJoueur(Joueur* joueur);
    void Initialiser(const int& nbJoueur);
    //void Jouer(Joueur j);   
    void Lancer();   
    std::array<Tuile*, 61> tuilesCite;

    const std::array<Tuile*, 61>& getTuilesCite() const {
        return tuilesCite;
    }

    void mettreAJourChantier();

    void afficherChantier() const;

    Tuile* choisirTuileDuChantier();


  protected:
    Jeu();
    static Jeu* instance;
    const ModeDeJeu mode; // A voir pendant développement mode solo, initialiser Jeu avec mode solo
    std::vector<Hexagone*> hexs;
    std::vector<TuileDepart *> tuilesDepart;
    std::vector<Joueur *> joueurs;
    
    Pioche* pioche = nullptr;  
    std::vector<Tuile*> chantier; 
};

#endif //JEU_H