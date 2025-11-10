#pragma once
#include <string>
#include <optional>
#include <iostream>
#include <vector>



enum class Couleur {
    Vert,
    Bleu, 
    Jaune, 
    Violet,
    Rouge, 
    nulle // pour les hexagone de type carrière 
};
//affichage couleurs pour tests
std::string toString(Couleur c); 
std::ostream& operator<<(std::ostream& os, Couleur c);

enum class Type {
    Carriere,
    Quartier,
    Place
};

//affichage types pour test
std::string toString(Type t); 
std::ostream& operator<<(std::ostream& os, Type t); 
 

class Hexagone{
    public: 
        Hexagone(Type t=Type::Carriere, Couleur c=Couleur::nulle);

        // acesseurs lecture
        const Hexagone* getVoisinsNE()const{return voisins[0]; }; 
        const Hexagone* getVoisinsE()const{return voisins[1]; }; 
        const Hexagone* getVoisinsSE()const{return voisins[2]; }; 
        const Hexagone* getVoisinsSO()const{return voisins[3]; }; 
        const Hexagone* getVoisinsO()const{return voisins[4]; }; 
        const Hexagone* getVoisinsNO()const{return voisins[5]; }; 
        
       Hexagone* const* getVoisins() const { return voisins; }

       const Couleur getCouleur() const{return couleur;}
       const Type getType()const {return type; }

        //accesseurs écriture 
        void setVoisinsNE(Hexagone* hex){voisins[0]=hex; }; 
        void setVoisinsE(Hexagone* hex){voisins[1]=hex; }; 
        void setVoisinsSE(Hexagone* hex){voisins[2]=hex; }; 
        void setVoisinsSO(Hexagone* hex){voisins[3]=hex; }; 
        void setVoisinsO(Hexagone* hex){voisins[4]=hex; }; 
        void setVoisinsNO(Hexagone* hex){voisins[5]=hex;}; 

        //fonction pour l'affichage pour les tests 

        void affiche(){
            std::cout<<"type : " <<this->type<<", couleur : "<<couleur<<"\n"; 

            for (int i=0;i<=5; i++ ){
                std::cout<<"addresse voisin "<<i<<":"<<voisins[i]<<"\n"; 
            }
        }; 

    private : 
        Type type; 
        Couleur couleur; 
        Hexagone* voisins[6]; 
    

}; 



class Tuile{
    public: 
        Tuile(const Hexagone& hex1,const Hexagone& hex2,const Hexagone& hex3); 
        Tuile rotation(); 

        //affichage pour les test : 
        void affichage(){
            std::cout<<"hexagone 1 : \n"; 
            hexagones[0].affiche(); 

            std::cout<<"hexagone 2 : \n"; 
            hexagones[1].affiche(); 
            
            std::cout<<"hexagone 3 : \n"; 
            hexagones[2].affiche(); 

        }; 
    protected: 
        int position[3]; // à voir avec ce qui existe dans la librairie standart 
        //Hexagone* hexagones[2]; // peut être mettre un tableau d'hexagone directement car normalement une fois creer, les hexagones restent dans la même tuile ? 
                                // tab de pointeurs permet de simplifier pour la fonction rotation 
        std::vector<Hexagone> hexagones;

}; 

class TuileDepart: public Tuile{
    public:
        TuileDepart(); 
    private:
         
} ;     
