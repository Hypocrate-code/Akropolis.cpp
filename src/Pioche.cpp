#include "Exception.hpp"
#include "Pioche.hpp"
#include <cstdlib> //rand
#include <ctime>



Pioche::Pioche(const Jeu& jeu) {
    // Initialise la graine du générateur aléatoire une seule fois
    // pour que l'ordre des tuiles soit différent à chaque partie.
    static bool seedInit = false;
    if (!seedInit) {
        srand(static_cast<unsigned>(time(nullptr)));
        seedInit = true;
    }

    for (auto t : jeu.getTuilesCite()) {
        if (t != nullptr)
            tuiles.push_back(t);
    }
}


Tuile* Pioche::piocher() {
    if (tuiles.empty())
        throw Exception("Pas de tuiles disponibles.");
    //tuile aleatoire
    size_t i = rand() % tuiles.size();  
    Tuile* t = tuiles[i];               
    
    tuiles[i] = tuiles.back();
    tuiles.pop_back();  

    return t;
}

