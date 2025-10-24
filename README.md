# Akropolis.cpp

A C++ version of the board game Akropolis, project for the LO21 course in UTC, France.

### Compilation
```bash
mkdir build && cd build
cmake ..
make
```



## Workflow
### Pour travailler sur une feature (exemple) :
```bash
# 1. Se mettre à jour
git checkout dev
git pull origin dev

# 2. Créer sa branche
git checkout -b feature/ton-nom-description

# 3. Travailler et commiter régulièrement
git add .
git commit -m "ajout système de placement de tuile"

# 4. Quand fini, pousser la branche
git push -u origin feature/ton-nom-description

# 5. Créer une Pull Request/Merge Request sur GitHub/GitLab