#include "TuileFantome.hpp"
#include "Exception.hpp"


TuileFantome::TuileFantome(Hexagone &h1, Hexagone &h2, Hexagone &h3)
    : Tuile(h1, h2, h3)
{
    // on force le type fantôme
    for (auto* h : hexagones) {
        h->setVoisins(nullptr);
    }
}


// vérifie si un hexagone n’a pas déjà une vraie tuile autour
bool GenerateurFantome::emplacementLibre(const Hexagone* hex) {
    for (auto v : hex->getVoisins()) {
        if (v != nullptr && v->getType() != Type::Fantome)
            return false;
    }
    return true;
}

// génère une tuile fantôme orientée autour d'un hexagone
TuileFantome* GenerateurFantome::creerFantomeAutour(const Hexagone* centre) {

    auto* a = new Hexagone(Type::Fantome);
    auto* b = new Hexagone(Type::Fantome);
    auto* c = new Hexagone(Type::Fantome);

    // liaisons minimales fantômes
    a->setVoisinsS(b);
    b->setVoisinsNE(c);
    c->setVoisinsNO(a);

    return new TuileFantome(*a, *b, *c);
}

// Génère toutes les tuiles fantômes autour des tuiles existantes
std::vector<TuileFantome*> GenerateurFantome::genererAutour(const std::vector<const Tuile*>& tuilesExistantes) {

    std::vector<TuileFantome*> resultat;

    for (auto* t : tuilesExistantes) {
        for (auto* hex : t->get_hexagones()) {

            // pour chaque voisin vide → on génère une tuile fantôme
            for (auto v : hex->getVoisins()) {
                if (v == nullptr && emplacementLibre(hex)) {
                    resultat.push_back(creerFantomeAutour(hex));
                }
            }
        }
    }

    return resultat;
}
