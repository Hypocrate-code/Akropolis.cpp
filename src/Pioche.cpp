#include "Exception.hpp"
#include "Pioche.hpp"
#include "Jeu.hpp"
#include <cstdlib> //rand
#include <ctime>

Pioche::Pioche(const Jeu& jeu) : jeuRef(jeu) {}

void Pioche::init() {
    // Cette méthode peut être utilisée pour réinitialiser ou mélanger la pioche si nécessaire.
    // Actuellement, elle ne fait rien car la pioche est initialisée dans le constructeur.

    static bool seedInit = false;
    if (!seedInit) {
        srand(static_cast<unsigned>(time(nullptr)));
        seedInit = true;
    }

    for (auto t : jeuRef.getTuilesCite()) {
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



std::vector<Tuile*> creerChantier(Pioche& pioche) {
    std::vector<Tuile*> chantier;
    while (chantier.size() < 5 && !pioche.estVide()) {
        Tuile* t = pioche.piocher();
        chantier.push_back(t);
    }
    return chantier; // devuelve las tuiles sacadas
}

void afficherChantier(const std::vector<Tuile*>& chantier) {
    std::cout << "=== Chantier ===\n";
    for (auto t : chantier) {
        t->afficherData();
    }
}


Tuile* choisirTuileChantier(std::vector<Tuile*>& chantier) {
    std::cout << "=== Chantier complet ===\n";
    for (size_t i = 0; i < chantier.size(); ++i) {
        std::cout << i << " : ";
        chantier[i]->afficherData();
    }


    size_t choix = 0;
    do {
        std::cout << "Choisissez une tuile par son numéro (0-" << chantier.size()-1 << ") : ";
        std::cin >> choix;
    } while (choix >= chantier.size());

    Tuile* tuileChoisie = chantier[choix];
    chantier.erase(chantier.begin() + choix);

    std::cout << "=== Chantier après choix ===\n";
    for (size_t i = 0; i < chantier.size(); ++i) {
        std::cout << i << " : ";
        chantier[i]->afficherData();
    }

    return tuileChoisie;
}
