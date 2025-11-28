#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <vector>
#include <array>

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
    Place,
    Fantome
};

class Tuile;
class Hexagone
{
public:
    Hexagone(Type t, Couleur c = Couleur::nulle);
    // int CompterPoint();

    // acesseurs lecture
    const Hexagone *getVoisinsNE() const { return voisins[0]; };
    const Hexagone *getVoisinsS() const { return voisins[1]; };
    const Hexagone *getVoisinsSE() const { return voisins[2]; };
    const Hexagone *getVoisinsSO() const { return voisins[3]; };
    const Hexagone *getVoisinsN() const { return voisins[4]; };
    const Hexagone *getVoisinsNO() const { return voisins[5]; };
    const Hexagone *getVoisinsTOP() const { return voisins[6]; };
    const Hexagone *getVoisinsBOT() const { return voisins[7]; };

    const std::array<const Hexagone*,8>& getVoisins() const { return voisins; }
    const std::array<const Hexagone*, 6>& getVoisins3D() const; 


    int getNiveau()const; 

    // accesseurs écriture
    inline void setVoisinsNE(const Hexagone *hex) { voisins[0] = hex; };
    inline void setVoisinsS(const Hexagone *hex) { voisins[1] = hex; };
    inline void setVoisinsSE(const Hexagone *hex) { voisins[2] = hex; };
    inline void setVoisinsSO(const Hexagone *hex) { voisins[3] = hex; };
    inline void setVoisinsN(const Hexagone *hex) { voisins[4] = hex; };
    inline void setVoisinsNO(const Hexagone *hex) { voisins[5] = hex; };
    inline void setVoisinsTOP(const Hexagone *hex) { voisins[6] = hex; };
    inline void setVoisinsBOT(const Hexagone *hex) { voisins[7] = hex; };

    inline void setVoisins(const Hexagone* hexs) { voisins.fill(hexs); }

    inline void setTuileParent(Tuile *tl) { parent = tl; }
    inline const Tuile *getTuileParent() const { return parent; }

    Type getType() const { return type; }
    Couleur getCouleur() const { return couleur; }

    void afficherData() const;

private:
    Type type;
    Couleur couleur;
    std::array<const Hexagone*, 8> voisins;
    const Tuile *parent;
};

class Tuile
{
public:
    Tuile(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3);
    Tuile* rotate();

    inline const std::vector<Hexagone *> &get_hexagones() const { return hexagones; }

    void reset_hex_links();

    void afficherData() const;

protected:
    std::vector<Hexagone *> hexagones;
    
};

 class TuileDepart : public Tuile
{
 public:
    TuileDepart(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3, Hexagone &hexCentre);

 private:
 };

std::ostream& operator<<(std::ostream& os, Couleur c);
std::ostream& operator<<(std::ostream& os, Type t); 
 