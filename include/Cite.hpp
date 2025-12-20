#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_set>


class Tuile;
class Hexagone;
// class HexFantome;

typedef std::vector<std::string> strCalc;

class Cite {
public:

    //enum class Type_Cite {CITE_MULTI, CITE_ILLUTRE};

    Cite(const Tuile* tuileDeDepart);
    ~Cite() = default;

    virtual bool placerTuileFromHexRef(Hexagone* hex) = 0; // return true si la Tuile a été placé
    void addTuile(Tuile* t);
    void addTuile(const Tuile* t);

    virtual uint32_t compterPoints() const = 0;

    void afficher() const;
    void QTDraw() const;
    std::vector<Hexagone*> hexs_fantome;
    Hexagone* create_new_hex_fantome();
    void release_hex_fantome();

    void generateAllHexFantome();
    void updateFantomeOfTuile(const Tuile* t);

    // Dessines les hexagones recursivement, les hexagones deja dessine sont dans drawnHexagones.
    static void draw_hex_recursive_filtered(Hexagone *hex, int x, int y, strCalc &calc,
                                       std::unordered_set<Hexagone *> &drawnHexagones, 
                                       int hauteur, bool drawFantomes);

    const Tuile* getTuileDeDepart() const { return tuiles[0]; }

protected:

    std::vector<const Tuile*> tuiles;

    static void print_hex(Hexagone*hex, int x, int y, strCalc& calc);
    static void increase_calc_size_H(strCalc& calc, uint32_t size);
    static void increase_calc_size_V(strCalc& calc, uint32_t size);

    //ajoute le texte dans les hexagones(PLA pour place, CAR pour carriere...)
    static void add_hex_data(Hexagone *hex, int x, int y, strCalc& calc);

    static void updateHexSrcFromFan(Hexagone *src, Hexagone *fan);

    
};

class CiteJoueur : public Cite {
public:

    CiteJoueur(const Tuile* tuileDeDepart);
    ~CiteJoueur()=default;

    bool placerTuileFromHexRef(Hexagone* hex) override;
    uint32_t compterPoints() const override;

};

class CiteIllu : public Cite{
    public:

    private:

};