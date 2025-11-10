#include <iostream>
#include <Tuile.hpp>

int main(){
    std::cout << "c'est ²" << std::endl;

    //tests pour tuiles et hexs : 
    std::cout << "\ntest creation d'hex\n" << std::endl;
    Hexagone hex1{Type::Quartier,Couleur::Bleu}; 
    hex1.affiche(); 

    Hexagone hex2{Type::Place,Couleur::Bleu}; 
    hex2.affiche(); 
    Hexagone hex3{Type::Carriere}; 
    hex3.affiche(); 
    Tuile t1{hex1,hex2,hex3}; 
    t1.affichage(); 
     
    t1=t1.rotation(); 
    t1.affichage(); 
  
    return 0;
}