#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <vector>

enum class Couleur
{
    Rouge,
    Vert,
    Bleu,
    Jaune,
    Violet,
    nulle // pour les hexagone de type carrière
};
enum class Type
{
    Carriere,
    Quartier,
    Place
};

// pour la classe hexagone c'est surement mieux de faire un héritage pour chaque hexagone est sa méthode de comptage de point ?
class Hexagone
{
public:
    Hexagone(Type t, Couleur c = Couleur::nulle);
    // int CompterPoint();

    // acesseurs lecture
    const Hexagone *getVoisinsNE() const { return voisins[0]; };
    const Hexagone *getVoisinsE() const { return voisins[1]; };
    const Hexagone *getVoisinsSE() const { return voisins[2]; };
    const Hexagone *getVoisinsSO() const { return voisins[3]; };
    const Hexagone *getVoisinsO() const { return voisins[4]; };
    const Hexagone *getVoisinsNO() const { return voisins[5]; };

    Hexagone *getVoisins() const { return (Hexagone *)voisins; }
    // accesseurs écriture
    void setVoisinsNE(const Hexagone *hex) { voisins[0] = hex; };
    void setVoisinsE(const Hexagone *hex) { voisins[1] = hex; };
    void setVoisinsSE(const Hexagone *hex) { voisins[2] = hex; };
    void setVoisinsSO(const Hexagone *hex) { voisins[3] = hex; };
    void setVoisinsO(const Hexagone *hex) { voisins[4] = hex; };
    void setVoisinsNO(const Hexagone *hex) { voisins[5] = hex; };

private:
    Type type;
    Couleur couleur;
    const Hexagone *voisins[5]; // cotes dans l'UML, peut être mettre plutôt un vecteur qu'un tableau?
};

class Tuile
{
public:
    Tuile(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3);
    Tuile rotation();

protected:
    int position[3]; // à voir avec ce qui existe dans la librairie standart
    // Hexagone* hexagones[2]; // peut être mettre un tableau d'hexagone directement car normalement une fois creer, les hexagones restent dans la même tuile ?
    //  tab de pointeurs permet de simplifier pour la fonction rotation
    std::vector<Hexagone *> hexagones;
};

class TuileDepart : public Tuile
{
public:
    TuileDepart();

private:
};