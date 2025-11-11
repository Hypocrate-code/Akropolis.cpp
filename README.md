# Akropolis.cpp

Une version C++ du jeu de plateau Akropolis, projet réalisé pour le cours LO21 à l'Université de Technologie de Compiègne, France.

- <a href="#workflow">Travailler en groupe</a>
- <a href="#compilation">Compilation du projet</a>

---
## <a id="workflow">Travailler en groupe</a>

Chacun.e développe sa feature sur sa branche. Bien sûr, les features sont le plus souvent liées, donc il est important de dev avec la dernière version de dev, et certains fichiers d'autres branches en développement (voir "Travailler sur le projet")

#### Quelques règles importantes

- Lire les commentaires faits sur votre code dans votre PR : votre branche n'est pas merged sur dev tant que le code n'est pas complet et fonctionnel.
- Testez votre code (voir <a>Compilation</a>).
- Ne pas commit des fichiers encore en développement sur d'autres branches : quand vous pousserez votre branche, ces fichiers non terminés seront donc dupliqués, et ce n'est pas votre code, même si le fonctionnement du vôtre en dépend.
- Ne pas commit et push votre CMakeLists.txt
- Bien sélectionner <u>dev <- votre-branch</u> au lieu du <u>main <- votre-branch</u> par défaut : les commits qui seront proposés d'être merged en seront pas les mêmes si vous passez à côté de ça.

#### Nommer sa branche

- Lorsque vous implémentez une **nouvelle** fonctionnalité, vous devez appeler votre banche `feature/nom-de-la-feature`.
- Si votre branch a été **merged sur dev** (votre code est normalement complet), mais que vous avez notifié un bug a corriger, faites une branche `fix/nom-du-fix`
- Si devez faire des corrections sur votre branche qui **n'a pas encore été merged**, faites les directement dessus : pas besoin de nouvelle branche.

#### Travailler sur le projet
```bash
# 1. Se mettre à jour et vérifier d'avoir tout enregistré
git pull origin dev
git fetch --all
git status


# 2. Aller sur la branceh de la dernière version de dev
git switch dev

# 3. Créer sa branche à partir de dev
git branch <nom-de-branche> # Voir partie ci-dessus

# 4. Aller sur cette branche
git switch <nom-de-branche>

# 4.bis Vérifier la branche actuelle
git branch -a 
# Une liste de branche apparaît, en rouge celles en ligne, en vert celle.s en local sur votre pc. 
# La branche sur laquelle vous êtes possède un * devant. 

# 5. Si vous avez besoin de fichiers en développement sur d'autres branches.
git checkout remotes/origin/feature/en-developpement -- chemin/vers/fichier
# Remplacez par la branche ciblée. (Voir git branch -a)
# Une copie locale du fichier est créée sur votre ordi, avec laquelle vous pouvez dev votre feature.

# 6. Une fois votre code fait, restore les fichiers empruntés et commit les ajouts.
git restore --staged chemin/vers/fichier-emprunté1
git restore --staged chemin/vers/fichier-emprunté2
git add chemin/vers/fichier # seulement vos fichiers
git status # Important : vérifiez (en vert) les fichiers que vous allez commit afin de ne rien push d'inopportun.
git restore --staged chemin/vers/votre-fichier # Si vous avez ajouté un fichier que vous ne voulez pas commit.
git commit -m "Ajout structure Pioche"

# 7. Pousser la branche
git push -u origin <nom-de-branche>

# 8. Créer une Pull Request sur GitHub

```

---

## <a id="compilation">Compilation du projet</a>

Tout d'abord, plusieurs outils sont requis.
Pour vérifier que vous les avez, rentrez les commandes suivantes dans un terminal :

- `gcc --version`
- `make --version`
- `cmake --version`

Dans une WSL ou sur MacOS, normalement vous avez déjà gcc, make et CMake d'installés.
Si vous voulez vraiment utiliser le terminal de windows, plein de tutoriels sont en ligne sur comment installer ces exécutables.

#### Compilateur GCC

Un compilateur prend des fichiers C ou C++, crée des fichier objets .o et les link ensemble avec les headers.

On utilise communément <a href="https://en.wikipedia.org/wiki/GNU_Compiler_Collection">GCC (GNU Compiler Collection)</a>.

Une commande utilisant gcc ressemble à ça : `gcc main.c Tuile.c -o Akropolis`, le `-o` pour output signifie que l'exécutable créé sera `Akropolis`.

Or, pour ne pas passer tous les fichiers du projet en argument de la commande, on utilise un MakeFile.

#### Make

Make est un autre outil, permet d'automatiser la compilation, à partir d'un fichier Makefile contenant les commandes qu'il doit faire, il les éxecute dans l'ordre.
Mais c'est toujours galère parce qu'il écrire dans le Makefile toutes les commandes qu'il faut faire.
Donc on automatise ça avec CMake

#### CMake (Create Makefile)

Permet donc d'automatiser la création du Makefile.
A partir d'un CMakeLists.txt contenant des informations sur quels fichiers source prendre, quels fichiers header prendre, et des options liées au compilateur, il crée un Makefile.
Une fois ce Makefile créé, on a plus besoin d'utiliser CMake tant qu'on a pas de nouveaux fichiers .cpp.

Créez donc le fichier CMakeLists.txt avec une config qui marche pour notre projet, je propose celui-ci :

```cmake
cmake_minimum_required(VERSION 3.16)
project(Akropolis)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Fichiers sources
file(GLOB SOURCES "src/*.cpp")

# Créer l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link
# find_package(QT...)
# target_link_libraries(${PROJECT_NAME}) ...
```

#### Ce que ça donne sur un IDE

Pour compiler et lancer le projet depuis votre IDE, il doit être capable de faire les étapes précédentes, avec le fichier CMakeLists.txt dans le projet.
Cela correspond exactement aux lignes de commande qui suivent.

#### Ce que ça donne en lignes de commande

Avec le CMakeLists.txt créé, vous pouvez rentrer ceci en lignes de commande :

```bash

# 0. Aller dans le dossier du projet
cd chemin/vers/racine/projet

# 1. Créer un dossier dans lequel seront stockés l'exécutable, et les fichiers propres au build.
mkdir build

# 2. Aller dans ce nouveau dossier.
cd build

# 3. Utilisez CMake avec le CMakeLists.txt du projet.
cmake ..
# Le Makefile est alors créé à votre chemin actuel (build/).

# 4. Build le projet.
make
# Make va utiliser le Makefile pour créer un exécutable.

# 5. Lancer l'exécutable.
./Akropolis

```