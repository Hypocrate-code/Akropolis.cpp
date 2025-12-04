#include "Cite.hpp"
#include <iostream>
#include <algorithm>
#include "Tuile.hpp"
#include "Utils.hpp"
#include <unordered_set>
#include <stack>
#include <cstdint>



using namespace Utils;

void Cite::print_hex(Hexagone *hex, int x, int y, strCalc &calc) const
{
    // Ensure we're within bounds
    if (y < 2 || y + 3 >= calc.size())
    {
        increase_calc_size_V(calc, 10);
    }
    if (x < 7 || x + 7 >= calc[0].size())
    {
        increase_calc_size_H(calc, 20);
    }

    // Get color code for this hexagon
    std::string colorCode = get_color_code(hex->getCouleur());
    std::string resetCode = "\033[0m";

    // Draw the hexagon with proper ASCII art
    // Line 1: Top
    if (y - 2 >= 0 && x - 4 >= 0 && x + 5 <= calc[y - 2].size())
    {
        calc[y - 2].replace(x - 2, 5, "_____");
    }

    // Line 2: Top middle
    if (y - 1 >= 0 && x - 5 >= 0 && x + 6 <= calc[y - 1].size())
    {
        calc[y - 1].replace(x - 3, 7, "/     \\");
    }

    // Line 3: Middle top
    if (y >= 0 && x - 5 >= 0 && x + 6 <= calc[y].size())
    {
        calc[y].replace(x - 4, 9, "/       \\");
    }

    // Line 4: Middle bottom
    if (y + 1 >= 0 && x - 5 >= 0 && x + 6 <= calc[y + 1].size())
    {
        calc[y + 1].replace(x - 4, 9, "\\       /");
    }

    // Line 5: Bottom
    if (y + 2 >= 0 && x - 4 >= 0 && x + 5 <= calc[y + 2].size())
    {
        calc[y + 2].replace(x - 3, 7, "\\_____/");
    }

    add_hex_data(hex, x, y, calc);
}

void Cite::add_hex_data(Hexagone *hex, int x, int y, strCalc &calc) const
{
    std::string typeStr = get_short_type(hex->getType());
    std::string colorStr = get_short_color(hex->getCouleur());
    // std::string colorCode = get_color_code(hex->getCouleur());
    // std::string resetCode = "\033[0m";

    if (y - 1 >= 0 && x - 4 >= 0 && x + 5 <= calc[y - 1].size())
    {
        // std::string topLine = center_string(colorCode + typeStr + resetCode, 3);
        std::string topLine = center_string(typeStr, 7);
        calc[y].replace(x - 3, 7, topLine);
    }

    if (y >= 0 && x - 3 >= 0 && x + 4 <= calc[y].size())
    {
        // std::string midLine = center_string(colorCode + colorStr + resetCode, 5);
        std::string midLine = center_string(colorStr, 7);
        // calc[y+1].replace(x - 2, 5, midLine);
        calc[y + 1].replace(x - 3, 7, midLine);
    }
    // Leave line 4 (y+1) empty for clean look
}

void Cite::draw_hex_recursive(Hexagone *hex, int x, int y, strCalc &calc,
                              std::unordered_set<Hexagone *> &drawnHexagones) const//, int depth) const
{
    if (!hex || drawnHexagones.count(hex))
        return;

    drawnHexagones.insert(hex);
    print_hex(hex, x, y, calc);

    // Proper hexagon grid offsets for pointy-top hexagones

    static const std::vector<std::pair<int, int>> directionOffsets = {
        {7, -2}, // NE (index 0) - largeur hexagone = 9
        {0, 4},  // S  (index 1) - hauteur hexagone = 5
        {7, 2},  // SE (index 2) - largeur hexagone = 9
        {-7, 2}, // SO (index 3) - largeur hexagone = 9
        {0, -4}, // N  (index 4) - hauteur hexagone = 5
        {-7, -2} // NO (index 5) - largeur hexagone = 9
    };

    const Hexagone *neighbors[6] = {
        hex->getVoisinsNE(), // 0: Nord-Est
        hex->getVoisinsS(),  // 1: Sud
        hex->getVoisinsSE(), // 2: Sud-Est
        hex->getVoisinsSO(), // 3: Sud-Ouest
        hex->getVoisinsN(),  // 4: Nord
        hex->getVoisinsNO()  // 5: Nord-Ouest
    };

    for (int i = 0; i < 6; ++i)
    {
        if (neighbors[i])
        {
            Hexagone *neighbor = const_cast<Hexagone *>(neighbors[i]);
            int newX = x + directionOffsets[i].first;
            int newY = y + directionOffsets[i].second;
            draw_hex_recursive(neighbor, newX, newY, calc, drawnHexagones);//, depth + 1);
        }
    }
}

void Cite::afficher() const

{
    std::cout << "=== DEBUG HEXAGONE CONNECTIONS ===" << std::endl;

    if (tuiles.empty())
        return;

    // Debug: Print all hexagones and their neighbors
    for (size_t i = 0; i < tuiles.size(); ++i)
    {
        std::cout << "Tuile " << i << ":" << std::endl;
        const auto &hexagones = tuiles[i]->get_hexagones();
        for (size_t j = 0; j < hexagones.size(); ++j)
        {
            std::cout << "  Hexagone " << j << " (" << type_to_string(hexagones[j]->getType())
                      << ", " << color_to_string(hexagones[j]->getCouleur()) << "):" << std::endl;

            // Print all neighbors
            const Hexagone *neighbors[6] = {
                hexagones[j]->getVoisinsNE(),
                hexagones[j]->getVoisinsS(),
                hexagones[j]->getVoisinsSE(),
                hexagones[j]->getVoisinsSO(),
                hexagones[j]->getVoisinsN(),
                hexagones[j]->getVoisinsNO()};

            const char *directionNames[6] = {"NE", "S", "SE", "SO", "N", "NO"};

            for (int k = 0; k < 6; ++k)
            {
                if (neighbors[k])
                {
                    std::cout << "    " << directionNames[k] << " -> "
                              << type_to_string(neighbors[k]->getType()) << ", "
                              << color_to_string(neighbors[k]->getCouleur()) << std::endl;
                }
                else
                {
                    std::cout << "    " << directionNames[k] << " -> NULL" << std::endl;
                }
            }
        }
    }

    std::cout << "=== END DEBUG ===" << std::endl;

    std::cout << "Nombre de tuiles dans la cite: " << tuiles.size() << std::endl;
    if (tuiles.size())
        std::cout << "Nombre d'hexagones dans la cite: " << ((tuiles.size() - 1) * 3 + 4) << std::endl;

    // Initialize canvas
    static strCalc calc = {
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             ",
        "                                                                             "};

    // Start with the first hexagon of the first tuile
    const auto &firstHexagones = tuiles[0]->get_hexagones();
    if (firstHexagones.empty())
        return;

    Hexagone *startHex = firstHexagones[0];
    std::unordered_set<Hexagone *> drawnHexagones;

    draw_hex_recursive(startHex, 50, 10, calc, drawnHexagones);
    
    for (auto &line : calc)
    {
        std::cout << colorize_line(line) << std::endl;
    }
    std::cout << "Total hexagones drawn: " << drawnHexagones.size() << std::endl;
}

void Cite::increase_calc_size_H(strCalc &calc, uint32_t size) const
{
    for (auto &line : calc)
    {
        line = line + std::string(size, ' ');
    }
}

void Cite::increase_calc_size_V(strCalc &calc, uint32_t size) const
{
    strCalc c(size, std::string(calc[0].size(), ' '));
    calc.insert(calc.end(), c.begin(), c.end());
}

Cite::Cite(const Tuile *tuileDeDepart)
{
    tuiles.push_back(tuileDeDepart);
}

CiteJoueur::CiteJoueur(const Tuile *tuileDeDepart) : Cite{tuileDeDepart}
{
    updateTuileFantome();
}

void CiteJoueur::placerTuile(const Tuile *dest)
{
}
uint32_t CiteJoueur::compterPoints() const{
    uint32_t nb_place_bleue=0; 
    uint32_t nb_place_rouge=0;
    uint32_t nb_place_verte=0;
    uint32_t nb_place_violet=0;
    uint32_t nb_place_jaune=0;

    uint32_t points_bleu=0; 
    uint32_t points_rouge=0;
    uint32_t points_vert=0;
    uint32_t points_violet=0;
    uint32_t points_jaune=0; 

 
    std::vector<const Hexagone*> habitations_visitees{}; 
    std::vector<uint32_t> points_hab{} ; 
    

    
    // parcours des hexagones 
    const Hexagone* start=tuiles.back()->get_hexagones().back(); 
    if (!start) return 0;

    std::unordered_set<const Hexagone*> visited;
    std::stack<const Hexagone*> pile;

    pile.push(start);


    while (!pile.empty()) {
        const Hexagone* h = pile.top();
        pile.pop();
        if (visited.count(h)) continue;
        visited.insert(h);

        // Comptage des points : 
        // si la tuile est recouverte --> ne vaut pas de points 
        if(h->getVoisinsTOP()==nullptr || h->getVoisinsTOP()->getType()==Type::Fantome ){
            // on determine le niveau de l'hex
            uint32_t niveau=h->getNiveau(); 
        
        
            //place, on incrémente le nombre de place de cette couleur  
            if (h->getType()==Type::Place){
                Couleur hc = h->getCouleur();
                switch (hc)
                {
                case Couleur::Bleu:
                    nb_place_bleue++;
                    break;
                case Couleur::Jaune:
                    nb_place_jaune++;
                    break;
                case Couleur::Rouge:
                    nb_place_rouge++;
                    break;
                case Couleur::Vert:
                    nb_place_verte++;
                    break;
                case Couleur::Violet:
                    nb_place_violet++;
                    break;
                }
            }

            //quartier 
            if (h->getType()==Type::Quartier){
         
                // calcul point marché: + 1 points si n'est pas entouré d'autres marchés 
                if(h->getCouleur()==Couleur::Jaune){ 
                    const std::array<const Hexagone*, 6> voisins = h->getVoisins3D(); 
                    bool cond=true; 
                    for (int i=0; i<6; i++){

                        if(voisins[i]!=nullptr){
                             if (voisins[i]->getCouleur()==Couleur::Jaune && voisins[i]->getType()==Type::Quartier){
                            cond=false;
                            }
                        }
                       
                        }
                        if (cond==true) points_jaune+=1*niveau; 
                        
                        
                }

                //calcul points jardins : +1 pt pour chaque jardin 
                if (h->getCouleur()==Couleur::Vert){
                            points_vert+=1*niveau; 
                }

                //calcul temples : +1 si est entièrement entouré 
                if (h->getCouleur()==Couleur::Violet){

                    const std::array<const Hexagone*, 6>& voisins = h->getVoisins3D(); 

                    //on vérifie si entierment entouré au niveau 0  
                    bool cond=true;
                    for (int i=0; i<6; i++){
                        if(voisins[i]!=nullptr){
                            if(voisins[i]->getType()==Type::Fantome) cond=false; 
                        }else{
                            cond=false; 
                        }
                        
                    }
                    if(cond)points_violet+=1*niveau; 

                }

                //calcul habitation: on doit calculer les groupes d'habitations 
                if (h->getCouleur()==Couleur::Bleu){
                    // si on a deja visite le groupe --> on passe
                    if (std::find(habitations_visitees.begin(), habitations_visitees.end(), h) == habitations_visitees.end()){
                            std::stack<const Hexagone*> habitations_a_visiter; 
                            uint32_t points_bleu_h=1; 
                            habitations_a_visiter.push(h);
                            while(!habitations_a_visiter.empty()){
                                const Hexagone* hab = habitations_a_visiter.top();
                                habitations_a_visiter.pop();
                                const std::array<const Hexagone*, 6> voisins = hab->getVoisins3D();
                                for(int i=0; i<6; i++){
                                    if(voisins[i]!=nullptr){
                                        if(voisins[i]->getCouleur()==Couleur::Bleu && voisins[i]->getType()==Type::Quartier && std::find(habitations_visitees.begin(), habitations_visitees.end(), voisins[i]) == habitations_visitees.end()&&voisins[i]->getVoisinsTOP()==nullptr){
                                            habitations_a_visiter.push(voisins[i]); 
                                            points_bleu_h ++; 
                                        }
                                    }
                                }
                            habitations_visitees.push_back(hab); 
                            }
                        points_hab.push_back(points_bleu_h); 
                        }
                    
                        // algo de recherche d'habitations voisines à faire 
                        // on parcourt les voisins de h, 
                        // puis les voisins des voisins de h ect jusqu'à ce qu'il n'y est plus d'habitations dans les voisins
                        // à ch nouvelle habitation visitée, on l'ajoute dans le vecteur + on incrémente nbre de points 
                        // et on parcours les voisins de cette habitations  qui n'ont pas encore été visités 
                    }

            // calcul caserne : +1 si l'hexagone est en périphérie de la cité
                if(h->getCouleur()==Couleur::Rouge){
                    bool cond=false; 
                    const std::array<const Hexagone*, 6>& voisins = h->getVoisins3D(); 
                    
                        
                    for (int i=0; i<6; i++){
                        if(voisins[i]!=nullptr){
                            if(voisins[i]->getType()==Type::Fantome) cond=true;
                        }else{
                            cond=true; 
                        }
                         
                    }
                    if(cond)points_rouge+=1*niveau; 
                }
                        
            }
        }

        
        // Ajouter les voisins
        for (const Hexagone* v : h->getVoisins()) {
            if (v != nullptr && !visited.count(v))
                pile.push(v);
        }
    }


    // on prend le max des suites de cité pour avoir le nbre de points correspondants 
    if (!habitations_visitees.empty()) {
        points_bleu = *std::max_element(points_hab.begin(), points_hab.end());
    }
    
    std::cout << "printing points : " << points_bleu*nb_place_bleue*1 << " . " <<
                    points_jaune *nb_place_jaune*2 << " . " <<
                    points_rouge *nb_place_rouge*2 <<" . " <<
                    points_vert*nb_place_verte*3 <<" . " <<
                    points_violet*nb_place_violet*2 << std::endl;
       
    uint32_t total=points_bleu*nb_place_bleue*1
                    +points_jaune *nb_place_jaune*2
                    +points_rouge *nb_place_rouge*2
                    +points_vert*nb_place_verte*3
                    +points_violet*nb_place_violet*2
                    ; 
    return total;
}




void CiteJoueur::updateTuileFantome(){}
