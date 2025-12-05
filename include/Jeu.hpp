//
// Created by thiba on 06/11/2025.
//

#ifndef JEU_H
#define JEU_H

#include <vector>

#include "Tuile.hpp"
#include "Joueur.hpp"
#include <array>

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

    void Initialiser(const int& nbJoueur);
    void Jouer(Joueur j);
    std::array<Tuile*, 61> tuilesCite;

  protected:
    Jeu();
    static Jeu* instance;
    const ModeDeJeu mode; // A voir pendant développement mode solo, initialiser Jeu avec mode solo
    std::vector<Hexagone*> hexs;
    std::vector<TuileDepart *> tuilesDepart;
    std::vector<Joueur *> joueurs;
};

#endif //JEU_H
