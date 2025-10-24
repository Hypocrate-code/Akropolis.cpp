#pragma once
#include <vector>
#include <memory>

class Hexagone;
class Tuile;
class Joueur;

class Cite{
    public:

    //enum class Type_Cite {CITE_MULTI, CITE_ILLUTRE};

    Cite(const Tuile* tuileDeDepart);
    ~Cite() = default;

    virtual void placerTuile(const Tuile* dest) = 0;
    virtual uint32_t compterPoints() const = 0;

    void afficher() const;
    void QTDraw() const;

    protected:
    std::vector<const Tuile*> tuiles;


};

class CiteJoueur : public Cite{
    public:

    CiteJoueur(const Tuile* tuileDeDepart);
    ~CiteJoueur()=default;


    void placerTuile(const Tuile* dest) override;
    uint32_t compterPoints() const override;
    void updateTuileFantome();

    private:

    std::vector<const Tuile*> tuile_fantome;
    //Joueur* owner;

};

class CiteIllu : public Cite{
    public:

    private:

};