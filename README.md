# Akropolis.cpp

Une version en C++ du jeu de plateau Akropolis, projet réalisé pour le cours LO21 à l'Université de Technologie de Compiègne, France.

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

# Instructions de Build

## Prérequis
- **Qt** installé sur votre système
- **Compilateur C++20** (GCC/Clang pour MinGW, ou MSVC pour Visual Studio)

### Étape 1: Configurer le chemin Qt
Ouvrez `CMakeLists.txt` et modifiez la ligne 12 avec votre chemin d'installation Qt :
```cmake
set(DEFAULT_QT_PATH "C:/Qt/6.9.3/mingw_64")  # <-- Remplacez par VOTRE chemin
```

**Trouver votre chemin Qt :**
- Windows : Regardez dans `C:\Qt\` (ex: `C:\Qt\6.9.3\msvc2022_64` ou `C:\Qt\6.9.3\mingw_64`)

### Étape 2: Build avec MinGW
```bash
# Nettoyer le dossier build s'il existe déjà
rm -rf build
mkdir build
cd build 

# Configurer avec MinGW
cmake .. -G "MinGW Makefiles"

# Compiler
cmake --build . -j8
```

### Étape 3: Exécuter le programme
```bash
./Akropolis.exe
```

### Résumé : 

```bash
# Après avoir modifié du code :
cd build
cmake --build . -j8    # Recompile rapidement
./Akropolis.exe        # Teste le programme

# Si vous changez CMakeLists.txt :
rm -rf build
cmake .. -G "MinGW Makefiles"
cmake --build . -j8

```

## Edit par Thib si vous avez des erreurs à la compilation avec Qt

Personnellement, à l'utilisation de la commande :
```bash
cmake .. -G "MinGW Makefiles"
```
J'ai eu l'erreur suivante : **"Could not create named generator MinGW Makefiles"**

En exécutant `cmake --help` à la section **Generators** je n'avais rien qui ressemblait à MinGW

2 options sont possibles :
- soit vous avez déjà mingw et il n'est juste pas correctement link a votre PATH
- soit vous n'avez pas mingw d'installé

---

Pour la première option, aller à la racine de votre disque dur (pour moi C:/), vous devriez y voir le dossier "mingw64/bin".

(Si ce n'est pas le cas, ou que vous n'avez qu'un dossier semblable à MinGW, se référer à la deuxième option.)

Ouvrez un Terminal windows et tapez "mingw32-make --version" : si vous avez un retour positif, vous devez l'ajouter au PATH de votre WSL (si vous utilisez une WSL) avec `export PATH=/chemin/bin:$PATH`.

---

Pour la seconde option (qui était mon cas), vous n'avez pas les exécutables mingw attendus. Il faut donc les installer avec msys2, et c'est un peu chelou.

Allez sur https://www.msys2.org/, installez l'exécutable et complétez son installation. Une fois fait, vous avez tout un tas de msys2 d'installés, vous devez lancer MSYS2 MSYS et taper la commande `pacman -Syu`. Une fois son déroulement terminé, fermez MSYS2 MSYS.

Lancez à présent MSYS2 MinGW64. Une fois dedans, tapez la commande `pacman -S mingw-w64-x86_64-toolchain`.

Vous devez à la suite de son exécution avoir maintenant le dossier C:/msys64/mingw64/bin.

Ajoutez ce dossier au PATH windows, et vous pourrez exécuter `mingw32-make --version` (même si vous avez une machine x64 !!).

A présent vous pouvez reprendre le fil des commandes pour compiler le projet : cmake a à présent accès à MinGW et son Makefile generator.

GG à vous. Merci d'avoir lu !
