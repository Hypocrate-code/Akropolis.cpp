#pragma once
#include <vector>
#include <array>
#include <ostream>
#include <cstdint>

#include "Utils.hpp"

class Cite;

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

    //peut etre utiliser le destructeur pour suprimmer les laisons avec les autres hexagones pour ne pas avoir 
    ~Hexagone();

    // acesseurs lecture

    const Hexagone *getVoisinsS() const { return voisins[0]; };
    const Hexagone *getVoisinsSO() const { return voisins[1]; };
    const Hexagone *getVoisinsNO() const { return voisins[2]; };
    const Hexagone *getVoisinsN() const { return voisins[3]; };
    const Hexagone *getVoisinsNE() const { return voisins[4]; };
    const Hexagone *getVoisinsSE() const { return voisins[5]; };
    const Hexagone *getVoisinsTOP() const { return voisins[6]; };
    const Hexagone *getVoisinsBOT() const { return voisins[7]; };
    
    Hexagone *getVoisinIndice(int i) const {
        return (i >= 0 && i < static_cast<int>(voisins.size())) ? voisins[i] : nullptr;
    };

    uint8_t getVoisinsNonFantomeBin() const;


    const std::array<Hexagone*,8>& getVoisins() const { return voisins; }

    // accesseurs écriture

    inline void setVoisinsS(Hexagone *hex) { this->setVoisinIndice(0, hex);};
    inline void setVoisinsSO(Hexagone *hex) { this->setVoisinIndice(1, hex);};
    inline void setVoisinsNO(Hexagone *hex) { this->setVoisinIndice(2, hex);};
    inline void setVoisinsN(Hexagone *hex) { this->setVoisinIndice(3, hex);};
    inline void setVoisinsNE(Hexagone *hex) { this->setVoisinIndice(4, hex);};
    inline void setVoisinsSE(Hexagone *hex) { this->setVoisinIndice(5, hex);};
    inline void setVoisinsTOP(Hexagone *hex) { this->setVoisinIndice(6, hex);};
    inline void setVoisinsBOT(Hexagone *hex) { this->setVoisinIndice(7, hex);};

    inline void setCouleur(Couleur c) { couleur = c; };
    inline void setType(Type t) { type = t; };

    
    inline void setVoisinIndice(int i, Hexagone *hex) {
        if (i < 0 || i >= int(voisins.size())) return;
        voisins[i] = hex;
        if (hex) {
            int opp = Utils::opposite_index(i);
            if (opp >= 0 && opp < int(hex->voisins.size())) {
                hex->voisins[opp] = this;
            }
        }
    };

    inline void setVoisins(Hexagone* hexs) { voisins.fill(hexs); }

    inline void setTuileParent(Tuile *tl) { parent = tl; }
    //inline const Tuile *getTuileParent() const { return parent; }
    inline Tuile *getTuileParent() const { return parent; }

    Type getType() const { return type; }
    Couleur getCouleur() const { return couleur; }

    void afficherData() const;

    int getIndice() const {return indice;}
    void setIndice(int i) {indice = i;}

    void removeConnection(const Hexagone* hex); //eleve la connection avec les voisins qui on la valeur the hex

    std::array<Hexagone*, 6> getVoisins3D() const;
    int getNiveau()const;

    std::array<int, 8> getVoisinsList() const;

    int gainPierre()const; 
protected:
    Type type;
    Couleur couleur;
    std::array<Hexagone*, 8> voisins;
    Tuile *parent;
    int indice = 0;
};


class Tuile
{
public:
    Tuile(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3);
    Tuile* rotate();

    inline const std::vector<Hexagone *> &get_hexagones() const { return hexagones; }

    void reset_hex_links();

    void afficherData() const;

    void set_indice(const int i) { indice = i; }
    void set_cite(Cite* c) { cite = c; }

    uint32_t get_hauteur() const { return hauteur;}
    void set_hauteur(uint32_t h) {hauteur = h;}

    Cite* cite;

    std::array<int, 8> getVoisinsHex(int indexHex) const;


protected:
    std::vector<Hexagone *> hexagones;
    int indice;
    uint32_t hauteur;
};

 class TuileDepart : public Tuile
{
 public:
    TuileDepart(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3, Hexagone &hexCentre);

 private:
 };

std::ostream& operator<<(std::ostream& os, Couleur c);
std::ostream& operator<<(std::ostream& os, Type t);
