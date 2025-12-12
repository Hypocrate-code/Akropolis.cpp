#include "Tuile.hpp"
#include "Cite.hpp"
#include "Exception.hpp"
#include <iostream>

#include "Jeu.hpp"
#include "Utils.hpp"

using namespace Utils;
Hexagone::Hexagone(Type t, Couleur c) : type(t), couleur(c)
{
    if ((type == Type::Carriere && couleur != Couleur::nulle) || (type != Type::Carriere && type != Type::Fantome && couleur == Couleur::nulle))
    {
        throw Exception("le type ne correspond pas à la couleur indiquée");
    }
    if(type == Type::Fantome){
        couleur = Couleur::nulle;
    }
    for (int i = 0; i < 8; i++)
    {
        voisins[i] = nullptr;
    }
};

void Hexagone::afficherData() const
{
    //????? what ?????
    if (this == nullptr) {
        // std::cout << "nullptr" << std::endl;
        return;
    }
    std::cout << "  Type: " << type_to_string(getType())
              << ", Couleur: " << color_to_string(getCouleur()) << std::endl;

    // Afficher les voisins
    std::cout << "  Voisins: ";
    std::cout << "NE:" << (getVoisinsNE() ? "O" : "X");
    std::cout << " S:" << (getVoisinsS() ? "O" : "X");
    std::cout << " SE:" << (getVoisinsSE() ? "O" : "X");
    std::cout << " SO:" << (getVoisinsSO() ? "O" : "X");
    std::cout << " N:" << (getVoisinsN() ? "O" : "X");
    std::cout << " NO:" << (getVoisinsNO() ? "O" : "X");
    std::cout << " TOP:" << (getVoisinsTOP() ? "O" : "X");
    std::cout << " BOT:" << (getVoisinsBOT() ? "O" : "X");
    std::cout << std::endl;


}

Tuile::Tuile(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3) : hauteur(0)
{
    hexagones.push_back(&hex1);
    hexagones.push_back(&hex2);
    hexagones.push_back(&hex3);

    hex1.setVoisinsSE(&hex2);
    hex1.setVoisinsS(&hex3);

    hex2.setVoisinsNO(&hex1);
    hex2.setVoisinsSO(&hex3);

    hex3.setVoisinsN(&hex1);
    hex3.setVoisinsNE(&hex2);

    hex1.setTuileParent(this);
    hex2.setTuileParent(this);
    hex3.setTuileParent(this);
};

void Tuile::afficherData() const
{
    std::cout << "=== DONNEES DE LA TUILE ===" << std::endl;

    std::cout << "Nombre d'hexagones: " << hexagones.size() << std::endl;


    {
    std::cout << "                           " << std::endl;
    strCalc calc = strCalc(10, std::string(50, ' '));
    std::unordered_set<Hexagone*> drawnHexagones;
    Cite::draw_hex_recursive(hexagones[0], 10, 6, calc, drawnHexagones, 0);

    for (auto &line : calc)
    {
        std::cout << colorize_line(line) << std::endl;
    }
    
}

    std::cout << "============================" << std::endl;
}


Tuile *Tuile::rotate()
{
    /*permet la rotation de la tuile en invertissant les hexagones */
    /*Hexagone* copie[2];
    for (int i =0; i<=2; i++){
        copie[i]=hexagones[i];
    }
    hexagones[0]=copie[1];
    hexagones[1]=copie[2];
    hexagones[2]=copie[3];

    // modification des voisins des hexagones à gérer
    // position à gérer
    */

    //tmp sol
    typedef struct tmpStruct {
        Type type;
        Couleur couleur;
        int indice;
    } tmpStrct;

    tmpStrct Hex0Data ={ hexagones[0]->getType(), hexagones[0]->getCouleur(), hexagones[0]->getIndice() };
    tmpStrct Hex1Data ={ hexagones[1]->getType(), hexagones[1]->getCouleur(), hexagones[1]->getIndice() };
    tmpStrct Hex2Data ={ hexagones[2]->getType(), hexagones[2]->getCouleur(), hexagones[2]->getIndice() };

    hexagones[0]->setIndice(Hex1Data.indice);
    hexagones[0]->setType(Hex1Data.type);
    hexagones[0]->setCouleur(Hex1Data.couleur);

    hexagones[1]->setIndice(Hex2Data.indice);
    hexagones[1]->setType(Hex2Data.type);
    hexagones[1]->setCouleur(Hex2Data.couleur);

    hexagones[2]->setIndice(Hex0Data.indice);
    hexagones[2]->setType(Hex0Data.type);
    hexagones[2]->setCouleur(Hex0Data.couleur);
    return this;

};

void Tuile::reset_hex_links()
{
    for (auto &hex : hexagones)
    {
        hex->setVoisins(nullptr);
    }
}

TuileDepart::TuileDepart(Hexagone &hex1, Hexagone &hex2, Hexagone &hex3, Hexagone &hexCentre) : Tuile(hex1, hex2, hex3)
{

    this->hexagones.push_back(&hexCentre); // rajout du centre de la tuile
    reset_hex_links();
    // On gère les voisins :

    // Redéfinition voisins hex1
    hexagones[0]->setVoisinsS(hexagones[3]);

    // Redéfinition voisins hex2
    hexagones[1]->setVoisinsNE(hexagones[3]);

    // Redéfinition voisins hex3
    hexagones[2]->setVoisinsNO(hexagones[3]);

    // Définition voisins hexgone/place centrale hex4
    hexagones[3]->setVoisinsN(hexagones[0]);
    hexagones[3]->setVoisinsSO(hexagones[1]);
    hexagones[3]->setVoisinsSE(hexagones[2]);
    hexagones[3]->setVoisinsNE(nullptr);

};

std::ostream& operator<<(std::ostream& os, Couleur c) {
    return os << Utils::color_to_string(c);
}
std::ostream& operator<<(std::ostream& os, Type t) {
    return os << type_to_string(t);
}


Hexagone::~Hexagone(){
    for (auto& voisin : voisins) {
        if (voisin){
            voisin->removeConnection(this);
            voisin = nullptr;
        }
    } 
}

void Hexagone::removeConnection(const Hexagone* hex) {
    for (auto& voisin : voisins){
        if (voisin == hex)
            voisin = nullptr;
    }
}

std::array<int, 8> Tuile::getVoisinsHex(int indexHex) const {
    std::array<int, 8> voisinsBin = {0,0,0,0,0,0,0,0};

    if (indexHex < 0 || indexHex >= hexagones.size()) {
        std::cerr << "Erreur: indexHex hors limites." << std::endl;
        return voisinsBin;
    }

    const auto& voisinsHex = hexagones[indexHex]->getVoisins();

    for (size_t i = 0; i < voisinsHex.size(); ++i) {
        voisinsBin[i] = (voisinsHex[i] != nullptr ? 1 : 0);
    }

    // DEBUG
    std::cout << "=== DEBUG Tuile::getVoisinsHex pour Hexagone " << indexHex << " ===" << std::endl;
    static const std::array<std::string, 8> noms = {"S","SE","NE","N","NO","SO","TOP","BOT"};
    for (size_t i = 0; i < voisinsBin.size(); ++i) {
        std::cout << noms[i] << " => " << voisinsBin[i] << std::endl;
    }
    std::cout << "===================================" << std::endl;

    return voisinsBin;
}

std::array<int, 8> Hexagone::getVoisinsList() const {
    // nombres para debug
    static const std::array<std::string, 8> noms = {"S", "SO", "NO", "N", "NE", "SE", "TOP", "BOT"};

    const auto& ptrs = Hexagone::getVoisins(); // getVoisins de la base
    std::array<int, 8> voisinsBin; // array de 1/0

    // Solo llenar el array binario
    for (int i = 0; i < 8; ++i) {
        voisinsBin[i] = (ptrs[i] != nullptr ? 1 : 0);
    }

    // Mostrar solo el binario (opcional, si quieres debug)
    std::cout << "=== DEBUG HexFantome::voisinsBin (1/0) ===" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << noms[i] << " => " << voisinsBin[i] << std::endl;
    }
    std::cout << "===================================" << std::endl;

    return voisinsBin;
}

uint8_t Hexagone::getVoisinsNonFantomeBin() const {
    uint32_t bin = 0;
    for (size_t i = 0; i < voisins.size(); ++i) {
        bin = bin<<1;
        if (voisins[i] != nullptr && voisins[i]->getType() != Type::Fantome) {
            bin |= 1;
        }
    }
    return bin;
}