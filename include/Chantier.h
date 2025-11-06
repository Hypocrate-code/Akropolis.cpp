#pragma once

#include "Tuile.hpp"
#include "Pioche.hpp"
#include <vector>
#include <optional>
#include <iostream>

class Chantier {
public:
    // Constructeur : crée un chantier avec un nombre de joueurs
    explicit Chantier(int nbJoueurs);

    // Remplir le chantier depuis la pioche (en gardant la dernière tuile)
    void remplirDepuisPioche(Pioche& pioche);

    // Acheter une tuile selon sa position et le nombre de pierres du joueur
    std::optional<Tuile> acheterTuile(std::size_t index, int& pierresJoueur);

    // Coût d'une tuile selon sa position (index) (0 gratuite, 1 → 1 pierre, etc.)
    int coutPourIndice(std::size_t index) const;

    // Accès lecture seule
    const std::vector<Tuile>& getTuiles() const noexcept;
    std::size_t taille() const noexcept;
    bool estVide() const noexcept;

private:
    std::vector<Tuile> tuiles;   // tuiles présentes dans le chantier
    int nbJoueurs;               // nombre de joueurs
    std::size_t tailleMax;       // = nbJoueurs + 2
};

