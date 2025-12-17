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

    static Jeu* getInstance(int nbJoueur);
    Jeu(Jeu &other) = delete;
    void operator=(const Jeu&) = delete;

    void afficherTuiles() const;
    void afficherHexagones() const;
    ModeDeJeu getModeDeJeu() const{return mode; }; // A voir si utile, mode solo ?
    void tourJoueur(Joueur* joueur);
    void tourIllu(Illu* illu); 
    void Initialiser(const int& nbJoueur);
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

    void set_niveau_difficulte(int n){
      if(0<=n && n<=2){
        niveauDeDifficulte=n; }
      else  {
          throw Exception("Niveau de difficulté invalide"); 
      
      }
    
    }
    int choisirHexagoneDeReference(Tuile* t);

  protected:
    Jeu(int nbJoueur);
    static Jeu* instance;
    const ModeDeJeu mode; // A voir pendant développement mode solo, initialiser Jeu avec mode solo
    std::vector<Hexagone*> hexs;
    std::vector<TuileDepart *> tuilesDepart;
    std::vector<Joueur *> joueurs;
    
    Pioche pioche;  
    std::vector<Tuile*> chantier; 

    int niveauDeDifficulte; 


};

#endif //JEU_H