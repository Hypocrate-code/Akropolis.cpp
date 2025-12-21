//
// Created by thiba on 06/11/2025.
//
#include "Pioche.hpp"
#include "Jeu.hpp"
#include <iostream>
#include "Tuile.hpp"
#include <map>
#include <array>
#include <random>
#include <chrono>
#include "Utils.hpp"
#include <ranges>
#include <algorithm>

using std::map;

// Opérations de singleton

Jeu *Jeu::instance = nullptr;

Jeu *Jeu::getInstance()
{
  if (instance)
  {
    return instance;
  }
  instance = new Jeu();
  return instance;
}

void Jeu::EndGame()
{
  for (auto &tuile : tuilesCite)
  {
    delete tuile;
  }
  tuilesCite.fill(nullptr);

  for (auto &hex : hexs)
  {
    delete hex;
  }
  hexs.clear();

  for (auto &tuileDepart : tuilesDepart)
  {
    delete tuileDepart;
  }
  tuilesDepart.clear();

  for (auto &joueur : joueurs)
  {
    delete joueur;
  }
  joueurs.clear();

  // Libération de l'instance singleton
  delete instance;
  instance = nullptr;
}

// Fin opérations de singleton

// Constructeur créant les tuilesCité de la partie
Jeu::Jeu() : mode(ModeDeJeu::Solo), niveauDeDifficulte(0), pioche(*this)
{

  // Définition des différentes quantités d'hexagones dans chaque catégorie
  // Soit 183 hors hexagones pour tuiles de départ (eux créer dans Initialiser)
  map<Type, map<Couleur, int>> totauxParCouleur;

  map<Couleur, int> nCarriere;
  nCarriere[Couleur::nulle] = 57;
  nCarriere[Couleur::Bleu] = 0;
  nCarriere[Couleur::Rouge] = 0;
  nCarriere[Couleur::Jaune] = 0;
  nCarriere[Couleur::Violet] = 0;
  nCarriere[Couleur::Vert] = 0;

  map<Couleur, int> nPlaceParCouleur;
  nPlaceParCouleur[Couleur::Bleu] = 9;
  nPlaceParCouleur[Couleur::Rouge] = 13;
  nPlaceParCouleur[Couleur::Jaune] = 13;
  nPlaceParCouleur[Couleur::Violet] = 13;
  nPlaceParCouleur[Couleur::Vert] = 13;
  nPlaceParCouleur[Couleur::nulle] = 0; // Couleur reservée au carrière

  map<Couleur, int> nQuartierParCouleur;
  nQuartierParCouleur[Couleur::Bleu] = 13;
  nQuartierParCouleur[Couleur::Rouge] = 13;
  nQuartierParCouleur[Couleur::Jaune] = 13;
  nQuartierParCouleur[Couleur::Violet] = 13;
  nQuartierParCouleur[Couleur::Vert] = 13;
  nQuartierParCouleur[Couleur::nulle] = 0; // Couleur reservée au carrière

  totauxParCouleur[Type::Carriere] = nCarriere;
  totauxParCouleur[Type::Place] = nPlaceParCouleur;
  totauxParCouleur[Type::Quartier] = nQuartierParCouleur;

  constexpr std::array<Type, 3> types{Type::Carriere, Type::Place, Type::Quartier};
  constexpr std::array<Couleur, 6> couleurs{Couleur::Rouge, Couleur::Vert, Couleur::Bleu, Couleur::Jaune, Couleur::Violet, Couleur::nulle};

  // Création des hexagones dans l'ordre
  for (auto &type : types)
  {
    if (type == Type::Carriere)
    {
      for (size_t i = 0; i < totauxParCouleur[type][Couleur::nulle]; i++)
      {
        hexs.push_back(new Hexagone(Type::Carriere, Couleur::nulle));
      }
    }
    else
    {
      for (const Couleur &couleur : couleurs)
      {
        if (couleur == Couleur::nulle)
          continue;
        for (size_t i = 0; i < totauxParCouleur[type][couleur]; i++)
        {
          hexs.push_back(new Hexagone(type, couleur));
        }
      }
    }
  }

  // Hexagones créés de manière ordonnée, place au mélange
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::ranges::shuffle(hexs.begin(), hexs.end(), std::default_random_engine(seed));

  // Création des tuilesCité
  for (size_t i = 0; i < tuilesCite.size(); i++)
  {
    const size_t j = i * 3;
    tuilesCite[i] = new Tuile(*hexs[j], *hexs[j + 1], *hexs[j + 2]);
    tuilesCite[i]->set_indice(i);
  }

  // -> Passer les tuiles à la Pioche / Chantier ??
}

// Initialisation et lancement de la partie avec un nombre de joueur indiqué
void Jeu::StartMenuC()
{
  std::vector<std::string> names;
  std::array<int, 5> variantes; 
 
  std::cout << "\n=============================" << std::endl;
  std::cout << "Bienvenue sur Akropolis.cpp !" << std::endl;
  std::cout << "=============================" << std::endl
            << std::endl;
  Jeu *j = Jeu::getInstance();

  int nJoueur;
  std::cout << "Une partie est sur le point de commencer, combien êtes-vous :  ";
  std::cin >> nJoueur;

  int index_variante =0; 
  std::cout<<"Voici les differentes variantes disponibles : "<<std::endl; 
  std::cout<<" 0. Marches. Si vos Quartiers Marchands sont adjacents à une place Marche, leurs points sont doubles"<<std::endl; 
  std::cout<<" 1. Jardins. Si vos Jardins sont adjacents à un lac (espace vide entouré), leurs points sont doublés"<<std::endl; 
  std::cout<<" 2. Casernes. Si vos Casernes ont 3 ou 4 espaces vides adjacents, leurs points sont doublés. "<<std::endl; 
  std::cout<<" 3. Habitations. Si votre groupe d'Habitations a une valeur de 10 ou plus, ses points sont doubles"<<std::endl;
  std::cout<<" 4. Temples. Si vos Temples sont placés sur un niveau superieur, leurs points sont doubles"<<std::endl;
  
  char reponse;
  std::cout<<"Voulez vous rajoutez une variante ? o/n  ";
  cin>>reponse;
  while (reponse != 'n' && reponse != 'N')
    {
      
      do{
        cout<<"Quelle variante voulez vous ajoutez ? "; 
        std::cin>>index_variante;
        if (index_variante < 0 || index_variante> 4){   //toutes les variantes compris
          std::cout << "Numero de variante invalide. Doit être compris entre 0 et 4" << std::endl;
        }
    } while (index_variante < 0 || index_variante> 4);
      variantes[index_variante] = 1; 
      cout<<"Merci. Cette variable a ete ajoute. En voulez vous d'autres ? o/n "; 
      cin>>reponse; 
  }
 
  for (int i = 0; i < nJoueur; i++)
  {

    // Accueil du joueur i+1
    std::string name;
    std::cout << std::endl
              << "Quel est ton prénom, joueur " << i + 1 << " :  ";
    std::cin >> name;
    std::cout << "Merci " << name << "." << std::endl
              << std::endl;
    names.push_back(name);
  }

  std::cout << "Vous avez choisi de jouer à " << nJoueur << " joueurs." << std::endl;
  if (nJoueur == 1)
  {
    std::cout << "Vous avez choisi de jouer en mode solo contre l'illustre architecte.\n"
              << std::endl;
    std::cout << "Choisissez le niveau de difficulté de l'illustre architecte (de 0 à 2) : \n"
              << "0. Hippodamos (niveau Facile) : tous les Quartiers d'Hippodamos sont considérés comme étant au 1er niveau.\n"
              << "1. Métagénès (niveau Moyen) : tous les Quartiers de Métagénès sont considérés comme étant au 1er niveau. Chaque Carrière de Métagénès lui rapporte 2 points supplémentaires.\n"
              << "2. Callicratès (niveau Difficile) : tous les Quartiers de Callicratès sont considérés comme étant au 2e niveau.\n";
    do
    {
      std::cin >> this->niveauDeDifficulte;

      if (this->niveauDeDifficulte < 0 || this->niveauDeDifficulte > 2)
      {
        std::cout << "Niveau de difficulté invalide. Veuillez choisir un niveau entre 0 et 2." << std::endl;
      }
    } while (this->niveauDeDifficulte < 0 || this->niveauDeDifficulte > 2);
  }

  j->InitialiserPartie(names, this->niveauDeDifficulte,variantes); // difficulté 0 par défaut en mode console
}

void Jeu::InitialiserPartie(const std::vector<std::string> &names, uint32_t difficultyLevel, std::array<int, 5> variantes)
{
  nombreTuilesChantier = names.size() + 2;
  niveauDeDifficulte = difficultyLevel;
  this->variantes = variantes; 

  for (size_t i = 0; i < names.size(); i++)
  {

    // Création des hexagones pour la tuile de départ du joueur i+1
    for (size_t j = 0; j < 3; j++)
    {
      hexs.push_back(new Hexagone(Type::Carriere, Couleur::nulle));
    }
    hexs.push_back(new Hexagone(Type::Place, Couleur::Bleu));

    // Création de la tuile de départ
    const size_t n = hexs.size() - 4;

    tuilesDepart.push_back(new TuileDepart(*hexs[n], *hexs[n + 1], *hexs[n + 2], *hexs[n + 3]));
    // Création du joueur
    joueurs.push_back(new Joueur(names[i].c_str(), i + 1, tuilesDepart[tuilesDepart.size() - 1]));
  }

  // creation du joueur illustre architecte pour la partie en mode solo
  if (names.size() == 1)
  {
    for (size_t j = 0; j < 3; j++)
    {
      hexs.push_back(new Hexagone(Type::Carriere, Couleur::nulle));
    }

    hexs.push_back(new Hexagone(Type::Place, Couleur::Bleu));
    // Création de la tuile de départ
    const size_t n = hexs.size() - 4;

    tuilesDepart.push_back(new TuileDepart(*hexs[n], *hexs[n + 1], *hexs[n + 2], *hexs[n + 3]));
    // Création du joueur
    joueurs.push_back(new Illu(2, tuilesDepart[tuilesDepart.size() - 1]));
  }

  // creation de la pioche en commun
  pioche.init();

  currentPlayer = joueurs[0];

  // Initialisation du chantier avec des tuiles piochées
  mettreAJourChantier();

  //tuile fantome gen
  for (auto &j : joueurs)
  {
    if (j->getNom() != "Illustre Architecte")
    {
      j->getCite()->generateAllHexFantome();
    }
  }
}


void Jeu::Lancer()
{
  bool fin = false;
  // Tant que le jeu n'est pas terminé
  while (!fin)
  {
    // Générer les hex fantômes pour chaque joueur au début de la manche
    for (auto &j : joueurs)
    {
      if (j->getNom() != "Illustre Architecte")
      {
        j->getCite()->generateAllHexFantome();
      }
    }

    // Chaque joueur joue son tour
    for (auto &j : joueurs)
    {
      if (j->getNom() != "Illustre Architecte")
      {
        tourJoueur(j); // Inclut affichage cité et placement de tuile
      }
      else
      {
        tourIllu(static_cast<Illu *>(joueurs.back()));
      }
    }
    // Si il n'y a qu'une tuile dans le chantier et que la pioche est vide --> fin de partie
    if (chantier.size() == 1 && pioche.estVide())
    {
      fin = true;
    }
    else
    {
      // Après que tous les joueurs aient joué, demander si +1 tour
      char reponse;
      std::cout << "\nVoulez-vous continuer la partie ? (o/n) : ";
      std::cin >> reponse;
      if (reponse != 'o' && reponse != 'O')
      {
        fin = true;
      }
    }
  }
  std::cout << "============== FIN DE PARTIE =====================\n";
  std::cout << "AFFICHAGE DES SCORES \n";
  for (auto &j : joueurs)
  {
    std::cout << j->getNom() << " : ";
    std::cout << j->getCite()->compterPoints(niveauDeDifficulte, variantes) << " points" << std::endl;
  }
}

int Jeu::choisirHexagoneDeReference(Tuile *t)
{
  int hex;
  std::cout << "\n--- Choisir l'hexagone de référence à placer sur la cité ---\n";
  for (int i = 0; i < 3; i++)
  {
    Hexagone *h = t->get_hexagones()[i];
    std::cout << "  [" << i << "] : "
              << Utils::type_to_string(h->getType())
              << " " << Utils::color_to_string(h->getCouleur())
              << std::endl;
  }
  do
  {
    std::cout << "\nVotre choix (0-2) : ";
    std::cin >> hex;

    if (hex < 0 || hex > 2)
    {
      std::cout << "Choix invalide. Veuillez choisir un numéro entre 0 et 2.\n";
    }
  } while (hex < 0 || hex > 2);

  Hexagone *hexChoisi = t->get_hexagones()[hex];
  std::cout << "\nVous avez choisi l'hexagone " << hex
            << " (" << Utils::type_to_string(hexChoisi->getType())
            << " " << Utils::color_to_string(hexChoisi->getCouleur())
            << ") !\n"
            << std::endl;

  return hex;
}

// Méthodes d'affichage

void Jeu::afficherTuiles() const
{
  // for(const auto& tuile : tuilesCite) {
  //   tuile->afficherData();
  // }  Print les tuiles de manière plus ordonnée :
  for (size_t i = 0; i < tuilesCite.size(); i++)
  {
    std::cout << "Tuile " << i + 1 << std::endl;
    tuilesCite[i]->afficherData();
    std::cout << std::endl;
  }
}

void Jeu::afficherHexagones() const
{
  for (size_t i = 0; i < hexs.size(); i++)
  {
    std::cout << "Hexagone " << i + 1 << std::endl;
    hexs[i]->afficherData();
    std::cout << std::endl;
  }
}

void Jeu::tourJoueur(Joueur *joueur)
{
  
  CiteJoueur* citeJoueur = dynamic_cast<CiteJoueur*>(joueur->getCite());
  if(!citeJoueur){
    throw Exception("Erreur : le joueur n'a pas de cité.");
    return;
  }
    // ===== Infos début du tour =====
    std::cout << "\n==========================\n\n";

    // --- Afficher les informations du joueur avant son tour ---
    std::cout << "Nom : " << joueur->getNom() << std::endl;
    std::cout << "Nombre de pierres : " << joueur->getNbPierres() << std::endl;

    // --- Premier affichage de la cité ---

    citeJoueur->afficher();
    std::cout << "Pressez Entrer pour afficher le Chantier...";

    cin.ignore();
    cin.ignore();

    // --- Affichage chantier global partagé ---
    if (chantier.size() <= 1)
    {
      mettreAJourChantier();
    }
    afficherChantier(); // Affiche le chantier actuel

    // --- Choix de la tuile et débit de pierres (pour le moment manuel) ---
    std::cout << "Pierres disponibles : " << joueur->getNbPierres() << "\n"
              << std::endl;
    Tuile *tChoisie = choisirTuileDuChantier(joueur); // Le joueur choisit une tuile
    citeJoueur->afficher();
    std::cout << "-- Tuile choisie --" << std::endl;
    tChoisie->afficherData();
    // --- Demande de rotation ---
    char reponse;
    std::cout << "\nTourner la tuile ? (o/n) : ";
    std::cin >> reponse;
    while (reponse == 'o' || reponse == 'O')
    {
      tChoisie->rotate();
      tChoisie->afficherData();
      std::cout << "\nTourner la tuile ? (o/n) : ";
      std::cin >> reponse;
    }

    citeJoueur->afficher();
    tChoisie->afficherData();

    // --- 5) Placement de la tuile ---
    int indexHexRef = choisirHexagoneDeReference(tChoisie);
    Hexagone *hexRef = tChoisie->get_hexagones()[indexHexRef];

    int res = placerTuileConsole(joueur, tChoisie, hexRef);
    while (0 == res)
      res = placerTuileConsole(joueur, tChoisie, hexRef);

    // --- Affichage final de la cité après le placement ---
    std::cout << "\nTuile correctement placée :\n";
  

  citeJoueur->afficher();

  // Ajout des pierres issues des carrières selon la tuile placée
}

void Jeu::tourIllu(Illu *illu)
{
  // ===== Infos début du tour =====
  std::cout << "\n==========================\n\n";

  // --- Afficher les informations du joueur avant son tour ---
  std::cout << "Nom : " << illu->getNom() << std::endl;
  std::cout << "Nombre de pierres : " << illu->getNbPierres() << std::endl;
  if (chantier.empty())
  {
    mettreAJourChantier();
  }

  afficherChantier();

  CiteIllu* citeIllu = dynamic_cast<CiteIllu*>(illu->getCite());
  if(!citeIllu){
    throw Exception("Erreur : l'illustre architecte n'a pas de cité.");
    return;
  }

  Tuile* t = choisirTuileDuChantier(illu);
  if (t) {
    int price = getTuilePrice(t);
    if (price < 0) price = 0;

    // Si pas assez de pierres, il prend la tuile gratuite (index 0) comme fallback
    if (illu->getNbPierres() < price && !chantier.empty()) {
      t = chantier.front();
      price = 0;
    }

    // Déduire les pierres puis retirer la tuile du chantier
    if (illu->getNbPierres() >= price) {
      illu->setNbPierre(illu->getNbPierres() - price);
    }
    removeTuileFromChantier(t);

    // Ajouter la tuile à la cité de l'Illustre
      citeIllu->afficher();  
    citeIllu->addTuile(t);

    std::cout << "Score : " << citeIllu->compterPoints(niveauDeDifficulte);
  } else {
    std::cout << "Erreur lors du choix de la tuile par l'illustre architecte.\n";
    return;
  }
}

// piocher
//  === Met à jour le chantier global si moins de 5 tuiles ===
void Jeu::mettreAJourChantier()
{
  while (chantier.size() < nombreTuilesChantier && !pioche.estVide())
  {
    chantier.push_back(pioche.piocher());
  }
}

// === Affiche le chantier actuel ===
void Jeu::afficherChantier() const
{
  // std::cout << "\n======= Chantier =======\n\n";
  // for (size_t i = 0; i < chantier.size(); ++i) {
  //      std::cout << "Tuile " << i << " : ";
  //        chantier[i]->afficherData();
  //}

  std::cout << "======= Chantier =======\n\n";
  std::cout << "Nombre de tuiles dans le chantier : " << chantier.size() << "\n\n";
  // draw the tuiles on 1 line
  strCalc calc = strCalc(10, std::string(chantier.size() * 15, ' '));
  for (size_t i = 0; i < chantier.size(); ++i)
  {
    std::unordered_set<Hexagone *> drawnHexagones;
    Cite::draw_hex_recursive_filtered(chantier[i]->get_hexagones()[0], 6 + i * 20, 2, calc, drawnHexagones, 0, false);
  }
  std::cout << "Tuile indices : \n      ";
  for (size_t i = 0; i < chantier.size(); ++i)
  {
    std::cout << "[" << i << "]" << std::string(17, ' ');
  }
  std::cout << std::endl;
  for (auto &line : calc)
  {
    std::cout << Utils::colorize_line(line) << std::endl;
  }
}

// === Permet au joueur de choisir une tuile dans le chantier ===
Tuile *Jeu::choisirTuileDuChantier(Joueur *joueur)
{
  if (chantier.empty())
  {
    std::cout << "Le chantier est vide.\n";
    return nullptr;
  }

  // Affichage pour vérification
  // afficherChantier();

  size_t choix;
  do
  {
    std::cout << "Choisissez une tuile par son numéro (0-" << chantier.size() - 1 << ") : ";
    std::cin >> choix;
    if (choix >= chantier.size())
    {
      std::cout << "Choix invalide. Veuillez réessayer.\n";
    }
    else if (choix > joueur->getNbPierres())
    {
      std::cout << "Vous n'avez pas assez de pierres pour cette tuile. Veuillez réessayer.\n";
    }
  } while (choix >= chantier.size() || choix > joueur->getNbPierres());

  // Ne pas débiter ni retirer la tuile ici: cela se fera
  // uniquement après un placement réussi via executerPlacementTuile.
  Tuile *t = chantier[choix];
  
  
  // Affichage du chantier restant pour vérification
  /*
  std::cout << "=== Chantier après choix ===\n";
  for (size_t i = 0; i < chantier.size(); ++i) {
      std::cout << i << " : ";
      chantier[i]->afficherData();
  }*/

  return t;
}
Tuile *Jeu::choisirTuileDuChantier(Illu *illu)
{
  if (chantier.empty())
  {
    std::cout << "Le chantier est vide.\n";
    return nullptr;
  }
  // permet de retourner le choix de l'illustre architecte et de lui retirer les pierre que cela lui a couté
  // rappel regle : l'illu archi prend la tuile avec au moins une place la moins chère du chantier
  // s'il n'a pas assez d'argent ou que aucune tuile n'a de place, il prend la tuile gratuite du chantier
  // c'est à dire celle à la position 0;
  bool tuile_ok = false;
  bool place = false;
  int index = 0;
  for (auto tuile_chantier : chantier)
  {
    for (auto hex : chantier[index]->get_hexagones())
    {
      if (hex->getType() == Type::Place)
      {
        if (illu->getNbPierres() >= index)
        { // vérifier juste que l'index correspond bien au prix
          illu->setNbPierre(illu->getNbPierres() - index);
          cout << "L'illustre architect a choisi la tuile " << index << "du chantier \n";
          auto t = tuile_chantier;
          chantier.erase(chantier.begin() + index);
          return t;
        }
        else
        {
          // cas où l'illArchi n'a pas assez de pierre pour s'acheter de place
          cout << "L'illustre architect a choisi la tuile 0 du chantier \n";
          auto t = chantier[0];
          return t;
        }
      }
    }
  }
  // cas ou y n'y a pas de place
  cout << "L'illustre architect a choisi la tuile 0 du chantier \n";
  auto t = chantier[0];
  return t;
}

int Jeu::getTuilePrice(Tuile* tuile) const
{
  // Chercher l'index de la tuile dans le chantier
  for (size_t i = 0; i < chantier.size(); ++i) {
    if (chantier[i] == tuile) {
      return static_cast<int>(i);  // Le coût = la position dans le chantier
    }
  }
  return -1;  // Tuile non trouvée
}

bool Jeu::placerTuileConsole(Joueur* joueur, Tuile* tuile, Hexagone* hexChantier)
{
  if (!joueur || !tuile || !hexChantier) {
    return false;
  }

  CiteJoueur* citeJoueur = dynamic_cast<CiteJoueur*>(joueur->getCite());
  if (!citeJoueur) {
    return false;
  }

  int id = -1;
  bool idValide = false;

  std::cout << "Tu vas placer la tuile avec le hexagone de reference : "
            << hexChantier->getType() << " "
            << hexChantier->getCouleur() << "\n";

  while (!idValide)
  {
    std::cout << "Entrez le numero d'hexagone ou vous voulez placer la tuile : ";
    std::cin >> id;

    for (auto *hexFan : citeJoueur->hexs_fantome)
    {
      if (hexFan->getIndice() == id)
      {
        idValide = true;
        break;
      }
    }

    if (!idValide)
    {
      std::cout << "ID invalide. Veuillez entrer un numero correct.\n";
    }
  }

  auto IthexFantome = std::find_if(citeJoueur->hexs_fantome.begin(), citeJoueur->hexs_fantome.end(),
                                   [id](Hexagone *h)
                                   { return h->getIndice() == id; });
  if (IthexFantome == citeJoueur->hexs_fantome.end())
  {
    std::cout << "Erreur: Aucun hexagone avec l'ID " << id << " trouve.\n";
    return false;
  }

  Hexagone* hexFantome = *IthexFantome;
  return executerPlacementTuile(joueur, tuile, hexChantier, hexFantome);
}

bool Jeu::executerPlacementTuile(Joueur* joueur, Tuile* tuile, Hexagone* hexChantier, Hexagone* hexCite)
{
  // Vérifications préalables
  if (!joueur || !tuile || !hexChantier || !hexCite) {
    return false;
  }

  // Obtenir le coût de la tuile
  int price = getTuilePrice(tuile);
  if (price < 0) {
    return false;
  }

  // Vérifier que le joueur a assez de pierres
  if (joueur->getNbPierres() < price) {
    return false;
  }

  CiteJoueur* citeJoueur = dynamic_cast<CiteJoueur*>(joueur->getCite());
  if (!citeJoueur) {
    return false;
  }

  // Effectuer le placement (hexCite doit être fourni)
  bool success = citeJoueur->placerTuileFromHexRef(hexChantier, hexCite);


  if (success) {
    // Le placement a réussi -> déduire les pierres et retirer la tuile du chantier
    joueur->setNbPierre(joueur->getNbPierres() - price);
    joueur->MaJPierres();
    // En mode Solo, l'Illustre Architecte récupère les pierres payées
    if (this->getModeDeJeu() == ModeDeJeu::Solo && !joueurs.empty()) {
      joueurs.back()->setNbPierre(joueurs.back()->getNbPierres() + price);
    }
    removeTuileFromChantier(tuile);
  }

  return success;
}

// === Passe au joueur suivant ===
void Jeu::nextPlayer()
{
  if (joueurs.empty()) {
    return;
  }

  // Trouver l'index du joueur actuel
  size_t currentIndex = 0;
  for (size_t i = 0; i < joueurs.size(); ++i) {
    if (joueurs[i] == currentPlayer) {
      currentIndex = i;
      break;
    }
  }

  // Passer au joueur suivant
  size_t nextIndex = (currentIndex + 1) % joueurs.size();
  currentPlayer = joueurs[nextIndex];
  
  // Générer les hexagones fantômes pour le nouveau joueur (sauf Illustre)
  if (currentPlayer->getNom() != "Illustre Architecte") {
    currentPlayer->getCite()->generateAllHexFantome();
  }
}

// === Exécute le tour de l'Illustre Architecte (pour Qt) ===
void Jeu::executerTourIllu()
{
  // S'assurer qu'il y a des tuiles dans le chantier
  if (chantier.empty()) {
    mettreAJourChantier();
  }

  if (chantier.empty()) {
    return;
  }

  // L'Illustre choisit la tuile avec au moins une place la moins chère
  Tuile* chosenTuile = nullptr;
  int chosenIndex = -1;

  for (size_t i = 0; i < chantier.size(); ++i) {
    Tuile* t = chantier[i];
    bool hasPlace = false;
    
    for (auto* hex : t->get_hexagones()) {
      if (hex->getType() == Type::Place) {
        hasPlace = true;
        break;
      }
    }

    if (hasPlace && currentPlayer->getNbPierres() >= static_cast<int>(i)) {
      chosenTuile = t;
      chosenIndex = static_cast<int>(i);
      break;
    }
  }

  // Si pas de tuile avec place abordable, prendre la première (gratuite)
  if (!chosenTuile && !chantier.empty()) {
    chosenTuile = chantier[0];
    chosenIndex = 0;
  }

  if (chosenTuile) {
    // Déduire les pierres
    if (chosenIndex > 0) {
      currentPlayer->setNbPierre(currentPlayer->getNbPierres() - chosenIndex);
    }

    // Retirer la tuile du chantier
    removeTuileFromChantier(chosenTuile);

    // Ajouter la tuile à la cité de l'Illustre
    CiteIllu* citeIllu = dynamic_cast<CiteIllu*>(currentPlayer->getCite());
    if (citeIllu) {
      citeIllu->addTuile(chosenTuile);
    }

    // Mettre à jour les pierres de l'Illustre
    currentPlayer->MaJPierres();
  }

}

// === Vérifie si la partie est terminée ===
bool Jeu::isGameOver() const
{
  // La partie se termine quand il ne reste qu'une tuile et que la pioche est vide
  return (chantier.size() <= 1 && pioche.estVide());
}

// === Retourne le joueur avec le plus de points ===
Joueur* Jeu::getWinner() const
{
  if (joueurs.empty()) {
    return nullptr;
  }

  Joueur* winner = joueurs[0];
  uint32_t maxPoints = winner->getCite()->compterPoints(niveauDeDifficulte);

  for (size_t i = 1; i < joueurs.size(); ++i) {
    uint32_t points = joueurs[i]->getCite()->compterPoints(niveauDeDifficulte);
    if (points > maxPoints) {
      maxPoints = points;
      winner = joueurs[i];
    }
  }

  return winner;
}