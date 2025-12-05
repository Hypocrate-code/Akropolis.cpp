#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>

class Tuile;
class Hexagone;
class HexFantome;

typedef std::vector<std::string> strCalc;

class Cite {
public:

    //enum class Type_Cite {CITE_MULTI, CITE_ILLUTRE};

    Cite(const Tuile* tuileDeDepart);
    ~Cite() = default;

    virtual void placerTuile(const Tuile* dest) = 0;
    virtual uint32_t compterPoints() const = 0;

    void afficher() const;
    void QTDraw() const;
    std::vector<HexFantome*> hexs_fantome;
    HexFantome* get_hex_fantome();
protected:

    std::vector<const Tuile*> tuiles;

    void print_hex(Hexagone*hex, int x, int y, strCalc& calc) const;
    void increase_calc_size_H(strCalc& calc, uint32_t size) const;
    void increase_calc_size_V(strCalc& calc, uint32_t size) const;

    //calc est un tableau de chaîne de caractères sur lequel on dessine les hexagones, il est passé en paramètre des fonctions pour être modifié

    //ajoute le texte dans les hexagones(PLA pour place, CAR pour carriere...)
    void add_hex_data(Hexagone *hex, int x, int y, strCalc& calc) const;

    // Dessines les hexagones recursivement, les hexagones deja dessine sont dans drawnHexagones.
    void draw_hex_recursive(Hexagone* hex, int x, int y, strCalc& calc, std::unordered_set<Hexagone*>& drawnHexagones) const; //, int depth) const;

    void addTuile(const Tuile* t);
};

class CiteJoueur : public Cite {
public:

    CiteJoueur(const Tuile* tuileDeDepart);
    ~CiteJoueur()=default;

    void placerTuile(const Tuile* dest) override;
    uint32_t compterPoints() const override;
    void updateTuileFantome();

};

class CiteIllu : public Cite{
    public:

    private:

};