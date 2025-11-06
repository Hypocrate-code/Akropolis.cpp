#include "chantier.hpp"

//  Constructeur
Chantier::Chantier(int nbJoueurs)
    : nbJoueurs(nbJoueurs), tailleMax(static_cast<std::size_t>(nbJoueurs + 2)) {}

//  Remplissage depuis la pioche
void Chantier::remplirDepuisPioche(Pioche& pioche) {
    // On garde la dernière tuile, on complète jusqu'à nbJoueurs + 2
    while (tuiles.size() < tailleMax && !pioche.estVide()) {
        auto nouvelle = pioche.piocher();
        if (nouvelle) {
            tuiles.push_back(*nouvelle);
        }
    }
}

//  Achat d'une tuile
std::optional<Tuile> Chantier::acheterTuile(std::size_t index, int& pierresJoueur) {
    if (index >= tuiles.size())
        return std::nullopt;

    int cout = coutPourIndice(index);
    if (pierresJoueur < cout)
        return std::nullopt;

    pierresJoueur -= cout;
    Tuile achetee = tuiles[index];
    tuiles.erase(tuiles.begin() + static_cast<long>(index));

    return achetee;
}

// Calcul du coût
int Chantier::coutPourIndice(std::size_t index) const {
    if (index >= tuiles.size()) return -1;
    return static_cast<int>(index); // coût = position dans le chantier
}

// Accesseurs
const std::vector<Tuile>& Chantier::getTuiles() const noexcept {
    return tuiles;
}

std::size_t Chantier::taille() const noexcept {
    return tuiles.size();
}

bool Chantier::estVide() const noexcept {
    return tuiles.empty();
}
