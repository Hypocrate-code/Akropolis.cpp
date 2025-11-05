#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <vector>



enum class Couleur {
    Rouge,
    Vert,
    Bleu, 
    Jaune, 
    Violet,
    nulle // pour les hexagone de type carrière 
};
enum class Type {
    Carriere,
    Quartier,
    Place
};


// pour la classe hexagone c'est surement mieux de faire un héritage pour chaque hexagone est sa méthode de comptage de point ? 
class Hexagone{
    public: 
        Hexagone(Type t, Couleur c=Couleur::nulle);
        int CompterPoint(); 

        // acesseurs lecture
        Hexagone* getVoisinsNE(){return voisins[0]; }; 
        Hexagone* getVoisinsE(){return voisins[1]; }; 
        Hexagone* getVoisinsSE(){return voisins[2]; }; 
        Hexagone* getVoisinsSO(){return voisins[3]; }; 
        Hexagone* getVoisinsO(){return voisins[4]; }; 
        Hexagone* getVoisinsNO(){return voisins[5]; }; 
        
        Hexagone* getVoisins(){return voisins[0]; }
        //accesseurs écriture 
        void setVoisinsNE(Hexagone* hex){voisins[0]=hex; }; 
        void setVoisinsE(Hexagone* hex){voisins[1]=hex; }; 
        void setVoisinsSE(Hexagone* hex){voisins[2]=hex; }; 
        void setVoisinsSO(Hexagone* hex){voisins[3]=hex; }; 
        void setVoisinsO(Hexagone* hex){voisins[4]=hex; }; 
        void setVoisinsNO(Hexagone* hex){voisins[5]=hex;}; 



    private : 
        Type type; 
        Couleur couleur; 
        Hexagone* voisins[5]; // cotes dans l'UML, peut être mettre plutôt un vecteur qu'un tableau? 
    

}; 
class Tuile{
    public: 
        Tuile(Hexagone hex1, Hexagone hex2, Hexagone hex3); 
        Tuile rotation(); 
    protected: 
        int position[3]; // à voir avec ce qui existe dans la librairie standart 
        //Hexagone* hexagones[2]; // peut être mettre un tableau d'hexagone directement car normalement une fois creer, les hexagones restent dans la même tuile ? 
                                // tab de pointeurs permet de simplifier pour la fonction rotation 
        std::vector<Hexagone*> hexagones;

}; 

class TuileDepart: public Tuile{
    public:
        TuileDepart(); 
    private:
         
} ;     