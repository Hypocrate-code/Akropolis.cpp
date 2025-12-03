#include "TuilesFantomes.hpp"
#include "Exception.hpp"
#include <unordered_map>


namespace HexagoneFantome {
// Donne la direction opposée
static const std::array<int, 6> oppose = {
    3, // NE (0) -> SO (3)
    4, // S  (1) -> N  (4)
    5, // SE (2) -> NO (5)
    0, // SO (3) -> NE (0)
    1, // N  (4) -> S  (1)
    2  // NO (5) -> SE (2)
};

// Vérifie si un hexagone n’a pas déjà un vrai voisin
bool GenerateurFantome::emplacementLibre(const Hexagone* hex) {

    const auto& voisins = hex->getVoisins();

    for (int i = 0; i < 6; i++) {
        const Hexagone* v = voisins[i];

        if (v != nullptr && v->getType() != Type::Fantome)
            return false;
    }

    return true;
}

// Crée un hexagone fantôme dans une direction donnée
Hexagone* GenerateurFantome::creerFantomeVoisin(Hexagone* centre, int direction) {

    auto* ghost = new Hexagone(Type::Fantome);

    // connecter centre -> ghost
    centre->setVoisins(ghost + direction);

    // connecter ghost -> centre
    ghost->setVoisins(centre + oppose[direction]);

    return ghost;
}

// Génère tous les hexagones fantômes autour de la cité
std::vector<Hexagone*> GenerateurFantome::genererAutour(const std::vector<const Tuile*>& tuilesExistantes) {

    std::vector<Hexagone*> resultat;

    // Pour éviter les doublons
    std::unordered_set<const Hexagone*> dejaCree;

    for (auto* tuile : tuilesExistantes) {
        for (Hexagone* hex : tuile->get_hexagones()) {

            const auto& voisins = hex->getVoisins();

            // Vérifie que cet hexagone peut accueillir des fantômes
            if (!emplacementLibre(hex))
                continue;

            for (int dir = 0; dir < 6; dir++) {

                if (voisins[dir] == nullptr) {

                    // éviter de créer plusieurs fois le même fantôme
                    if (dejaCree.count(hex) == 0) {
                        Hexagone* ghost = creerFantomeVoisin(hex, dir);
                        resultat.push_back(ghost);
                    }
                }
            }

            dejaCree.insert(hex);
        }
    }

    return resultat;
}

}
