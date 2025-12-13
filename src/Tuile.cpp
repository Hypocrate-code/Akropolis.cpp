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
    strCalc calc = strCalc(6, std::string(50, ' '));
    std::unordered_set<Hexagone*> drawnHexagones;
    Cite::draw_hex_recursive_filtered(hexagones[0], 10, 6, calc, drawnHexagones, 0, false);
    for (auto &line : calc)
    {
        std::cout << colorize_line(line) << std::endl;
    }
}

Tuile *Tuile::rotate(){
    // Tableau des rotations : chaque ligne contient 3 indices de direction
    // [0] : direction de hex1 -> hex2
    // [1] : direction de hex1 -> hex3  
    // [2] : direction de hex2 -> hex3
    const int rotations[6][3] = {
        {5, 0, 1},  // rotation 1 (initiale) : hex1→hex2(SE), hex1→hex3(S),  hex2→hex3(SO)
        {0, 1, 2},  // rotation 2 :            hex1→hex2(S),  hex1→hex3(SO), hex2→hex3(NO)
        {1, 2, 3},  // rotation 3 :            hex1→hex2(SO), hex1→hex3(NO), hex2→hex3(N)
        {2, 3, 4},  // rotation 4 :            hex1→hex2(NO), hex1→hex3(N),  hex2→hex3(NE)
        {3, 4, 5},  // rotation 5 :            hex1→hex2(N),  hex1→hex3(NE), hex2→hex3(SE)
        {4, 5, 0}   // rotation 6 :            hex1→hex2(NE), hex1→hex3(SE), hex2→hex3(S)
    };
    // Déterminer la rotation actuelle en regardant où est hex2 par rapport à hex1
    int rotation_actuelle = 0;
    for (int i = 0; i < 6; i++) {
        if (hexagones[0]->getVoisinIndice(rotations[i][0]) == hexagones[1]) {
            rotation_actuelle = i;
            break;
        }
    }
    // Déconnecter tous les voisins 
    this->reset_hex_links(); 
    // Passer à la rotation suivante
    int nouvelle_rotation = (rotation_actuelle + 1) % 6; 
    // Établir les nouvelles connexions 
    hexagones[0]->setVoisinIndice(rotations[nouvelle_rotation][0], hexagones[1]);
    hexagones[0]->setVoisinIndice(rotations[nouvelle_rotation][1], hexagones[2]);
    hexagones[1]->setVoisinIndice(rotations[nouvelle_rotation][2], hexagones[2]);
    
    return this;
}


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

    hexagones[3]->setTuileParent(this);

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
    // std::cout << "=== DEBUG HexFantome::voisinsBin (1/0) ===" << std::endl;
    // for (int i = 0; i < 8; ++i) {
    //     std::cout << noms[i] << " => " << voisinsBin[i] << std::endl;
    // }
    // std::cout << "===================================" << std::endl;

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

std::array<Hexagone*, 6> Hexagone::getVoisins3D() const{
    std::array<Hexagone*,8> voisins8 = getVoisins();

    std::array<Hexagone*, 6> voisins6{}; // init à nullptr


    std::copy(voisins8.begin(), voisins8.begin() + 6, voisins6.begin());

    const Hexagone* copie = this; 
    while(copie!=nullptr){ 
        std::array<Hexagone*,8> voisinsNiveau = copie->getVoisins(); 
        for(int i=0; i<6; i++){
            if(voisinsNiveau[i]!=nullptr && voisins6[i]==nullptr ){
                voisins6[i]=voisinsNiveau[i]; 
            }
        }
        copie = copie->getVoisinsBOT();
    }
    return voisins6;
}

int Hexagone::getNiveau()const{
    const Hexagone*copie= this ; 
    int niveau =1;  
        while(copie->getVoisinsBOT()!=nullptr){
            niveau++; 
            copie=copie->getVoisinsBOT(); 

        }
    return niveau; 
};