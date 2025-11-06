#include "Exception.hpp"
#include "Pioche.hpp"
#include <cstdlib> //rand

Pioche::Pioche(const Jeu& jeu) {
    nb = jeu.getNbTuiles();
    tuiles = new const Tuile*[nb];
    for (size_t i = 0; i < nb; ++i)
        tuiles[i] = jeu.tuiles[i];
}

const Tuile& Pioche::piocher() {
    if (estVide())
        throw Exception("Pioche de tuiles vide.");

    size_t i = rand() % nb;
    const Tuile* t = tuiles[i];
    tuiles[i] = tuiles[--nb]; //remplacement pour la derniere tuile
    return *t;
}