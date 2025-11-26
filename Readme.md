# Projet de Simulation de Gardes

Ce projet simule le comportement de gardes et d'un joueur dans un environnement 2D. Il permet de générer des GIFs de simulations individuelles ou de lancer des lots de simulations pour analyser les performances de différents comportements de gardes.

## Compilation

Le projet se compile à l'aide du `Makefile` fourni. Les cibles principales sont expliquées dans la section suivante.

## Utilisation du Makefile

Le `Makefile` simplifie la compilation et l'exécution des simulations.

-   `make tests`: Compile le programme de simulation unique et génère l'exécutable `tests.out`.
-   `make main`: Compile le programme de simulation en lot et génère l'exécutable `main.out`.
-   `make all_t`: **Raccourci pour générer un GIF.** Compile et exécute la simulation de test, puis lance le script Python pour créer `out/test.gif`.
-   `make all_m`: **Raccourci pour lancer une simulation en lot.** Compile et exécute `main.out` avec un jeu de paramètres par défaut (`20 1 200 5 20`).
-   `make clean`: Supprime les fichiers objets, les exécutables et les fichiers de résultats (`.txt`).

## Comment générer un GIF d'une simulation

La méthode la plus simple est d'utiliser la cible `all_t` du makefile.

1.  **Modifiez les paramètres de la simulation (optionnel) :**
    -   Ouvrez le fichier `tests/tests.c`.
    -   Dans la fonction `test_simulate_path`, vous pouvez changer le comportement des gardes en modifiant le 3ème argument de la fonction `new_guard_list`. Par exemple, pour utiliser le comportement "snake" (6) :
        ```c
        // Ligne 176 dans tests/tests.c
        guard_list* guards = new_guard_list(number, 6, ROTATION_SPEED, 1, 4);
        ```

2.  **Exécutez la commande `make all_t` :**
    ```bash
    make all_t
    ```
    Cette commande s'occupe de tout : compilation, exécution du programme C, et création du GIF `out/test.gif` via le script Python. Assurez-vous d'avoir `Pillow` d'installé pour Python (`pip install Pillow`).

## Comment lancer des simulations en lot

1.  **Utilisez la commande `make main` pour compiler.**
    ```bash
    make main
    ```

2.  **Exécutez le programme `main.out` avec les arguments souhaités :**
    ```bash
    ./main.out k n m pas accuracy
    ```
    -   `k` : Le nombre de simulations à exécuter pour chaque configuration.
    -   `n` et `m` : La plage du nombre de gardes à simuler (de `n` à `m`).
    -   `pas` : Le pas d'incrémentation du nombre de gardes entre `n` et `m`.
    -   `accuracy` : La "précision" du joueur (nombre de tours anticipés).

    Par exemple, pour faire 100 simulations pour 10 à 50 gardes (par pas de 10) avec une précision de 100 :
    ```bash
    ./main.out 100 10 50 10 100
    ```
    Les résultats (scores) seront enregistrés dans des fichiers texte dans le répertoire `out/score/`.

    Vous pouvez aussi utiliser `make all_m` qui exécute une simulation avec des paramètres prédéfinis.

## Scripts Python d'Analyse et de Visualisation

Le dossier `out/` contient plusieurs scripts Python pour traiter et visualiser les données générées par les simulations.

-   `main.py`: C'est le script principal à exécuter. Il peut soit générer un GIF (s'il trouve un fichier `test.txt`), soit (en décommentant les lignes appropriées) lancer l'analyse des scores et afficher des graphiques.
-   `sublim.py`: Utilisé par `main.py` pour la création d'images. Il contient les fonctions qui lisent le fichier `test.txt` et génèrent les images `.bmp` individuelles qui composent le GIF.
-   `analyses.py`: Contient toute la logique pour l'analyse des données des simulations en lot. Il lit les fichiers de score, calcule des moyennes, des taux de réussite et génère des graphiques à l'aide de `matplotlib`.
-   `const.py`: Fichier de constantes qui définit les chemins vers les dossiers de sortie utilisés par les autres scripts.

Pour l'analyse de données, la bibliothèque `matplotlib` est nécessaire. Vous pouvez l'installer via pip :
```bash
pip install matplotlib
```

## Configuration

### Le paramètre `accuracy`
Ce paramètre, passé à l'exécutable `main` ou modifié dans `tests.c`, est crucial pour l'intelligence du joueur. Il représente le **nombre de tours de jeu que le joueur peut anticiper**.

Concrètement, le programme pré-calcule les `accuracy` prochains mouvements des gardes. L'algorithme de recherche de chemin (A*) du joueur utilise ensuite ces positions futures pour trouver un chemin qui n'est pas seulement sûr maintenant, mais qui le restera aussi pour les `accuracy` prochains tours.

-   **Une `accuracy` faible** rendra le joueur "myope" : il pourrait s'engager dans un chemin qui semble sûr mais qui deviendra un piège.
-   **Une `accuracy` élevée** permettra au joueur de trouver des stratégies complexes pour éviter les gardes, mais augmentera le temps de calcul.

### Comportements des gardes

Les comportements (`behave`) sont définis dans la fonction `initializer` du fichier `entity/guard.c`. Voici la liste :

| Valeur | Comportement     |
| :----: | :--------------- |
|   1    | `rotation`       |
|   2    | `random_line`    |
|   3    | `corridor`       |
|   4    | `centred_square` |
|   5    | `mousaid`        |
|   6    | `snake`          |
| autre  | `nothing`        |

### Paramètres globaux

Vous pouvez modifier des constantes globales pour la simulation dans `values.h`, telles que :

-   `HEIGHT`, `WIDTH` : Dimensions de la zone de simulation.
-   `VISION_DISTANCE` : Distance de vision des gardes.
-   `VISION_FIELD` : Champ de vision des gardes (en degrés).
-   `ROTATION_SPEED` : Vitesse de rotation des gardes.

```