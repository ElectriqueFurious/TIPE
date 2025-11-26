# Projet de Simulation de Gardes

Ce projet simule le comportement de gardes et d'un joueur dans un environnement 2D. Il permet de générer des GIFs de simulations individuelles ou de lancer des lots de simulations pour analyser les performances de différents comportements de gardes.

## Compilation

Le projet se compile à l'aide du `Makefile` fourni. Les cibles principales sont expliquées dans la section suivante.

## Utilisation du Makefile

Le `Makefile` simplifie la compilation et l'exécution des simulations.

-   `make tests`: Compile le programme de simulation unique et génère l'exécutable `tests.out`.
-   `make main`: Compile le programme de simulation en lot et génère l'exécutable `main.out`.
-   `make all_t`: **Raccourci tout-en-un pour générer un GIF.** Compile et exécute la simulation de test, puis génère le fichier `out/test.gif`.
-   `make all_m`: **Raccourci tout-en-un pour l'analyse.** Lance une simulation en lot avec des paramètres par défaut, puis génère et affiche les graphiques d'analyse.
-   `make clean`: Supprime les fichiers objets, les exécutables et les fichiers de résultats (`.txt`).

## Comment générer un GIF d'une simulation

La méthode la plus simple est d'utiliser la cible `all_t` du `Makefile`, qui compile, exécute la simulation de test et génère le fichier `out/test.gif`.

Pour personnaliser cette simulation de test, vous devez modifier le fichier `tests/tests.c`.

1.  **Ouvrez `tests/tests.c`.** Les principaux paramètres se trouvent dans la fonction `test_simulate_path`.

2.  **Ajustez les paramètres des entités :**
    *   **Nombre de gardes :** Modifiez la variable `number`.
        ```c
        // dans tests/tests.c
        int number = 180; // La simulation se lancera avec 180 gardes.
        ```
    *   **Comportement des gardes :** Modifiez le deuxième argument de `new_guard_list`. La liste des comportements (1: rotation, 2: random_line, etc.) se trouve plus bas dans ce README.
        ```c
        // dans tests/tests.c
        guard_list* guards = new_guard_list(number, 5, ROTATION_SPEED, 1, 4); // Comportement 5 (mousaid).
        ```
    *   **Position d'un garde :** Pour tester une configuration précise, vous pouvez fixer la position d'un garde en décommentant et modifiant la ligne `moveto`.
        ```c
        // dans tests/tests.c
        // moveto(guards->tab[0]->pos, 50, 50); // Déplace le garde 0 en (50, 50)
        ```

3.  **Ajustez les paramètres de la simulation :**
    La fonction `simulate` est appelée vers la fin de `test_simulate_path`. Vous pouvez en modifier les arguments pour plus de personnalisation.
    `simulate(scene, guards, bot, 5, n, true, f)`
    *   **`speed` (4ème argument) :** Contrôle la durée maximale de la simulation (`durée_max = hauteur * speed`). Augmenter cette valeur donne plus de temps au joueur pour atteindre la sortie. La valeur est ici `5`.
    *   **`accuracy` (5ème argument, variable `n`) :** C'est la profondeur de l'anticipation du joueur. Ce paramètre est défini dans la fonction `main` de `tests/tests.c`.
        ```c
        // dans tests/tests.c, fonction main()
        test_simulate_path(100); // L'accuracy est ici de 100.
        ```
    *   **`export` (6ème argument) :** C'est un booléen (`true` ou `false`). Il doit être à `true` pour que le programme écrive les données de la simulation dans un fichier, nécessaire à la création du GIF.

4.  **Exécutez la commande `make all_t` :**
    ```bash
    make all_t
    ```
    Après avoir sauvegardé vos modifications dans `tests/tests.c`, cette commande relancera le processus complet.

## Comment lancer des simulations en lot

La méthode la plus simple pour lancer des simulations en lot est de passer par la cible `all_m` du `Makefile`, qui s'occupe de tout (compilation, exécution, analyse).

Pour configurer votre lot de simulations, vous pouvez directement modifier les paramètres dans le `Makefile`.

1.  **Ouvrez le fichier `Makefile`.**

2.  **Modifiez les variables de simulation** au début du fichier :
    ```makefile
    # Parameters for all_m
    K        = 20
    N        = 1
    M        = 200
    PAS      = 5
    ACCURACY = 20
    ```
    -   `K`: Le nombre de simulations à exécuter pour chaque configuration.
    -   `N`, `M`: La plage pour le nombre de gardes (de N à M).
    -   `PAS`: Le pas d'incrémentation pour le nombre de gardes.
    -   `ACCURACY`: La précision de l'algorithme du joueur (nombre de tours anticipés).

3.  **Exécutez la commande `make all_m` :**
    ```bash
    make all_m
    ```
    Cette commande lancera les simulations avec vos paramètres et générera les graphiques d'analyse correspondants.

4.  **(Optionnel) Changer le type de simulation en lot :**
    Si vous souhaitez exécuter un autre type de simulation en lot (par exemple, pour tester l'impact de la vitesse de rotation des gardes), vous pouvez modifier le fichier `main.c`.
    -   Ouvrez le fichier `main.c`.
    -   Dans la fonction `main`, vous pouvez commenter la ligne `simulate_from_to(...)` et décommenter la ligne `find_angle_rotation(...)` pour changer le type de simulation.
        ```c
        // Exemple dans main.c
        simulate_from_to(k,n,m,pas,MAX_BEHAVE,accuracy);
        // find_angle_rotation(0,20,2,k,n,m,pas,accuracy);
        ```
    -   Vous pouvez également créer votre propre fonction de simulation sur le modèle de `simulate_from_to` ou `find_angle_rotation` et l'appeler dans le `main`.
    -   Après avoir modifié `main.c`, relancez `make all_m` pour compiler et exécuter vos nouvelles simulations.

## Scripts Python d'Analyse et de Visualisation

Le dossier `out/` contient plusieurs scripts Python pour traiter et visualiser les données.

-   `main.py`: Le script principal qui prend un argument en ligne de commande :
    -   `gif`: Lit `test.txt` pour créer un GIF animé.
    -   `plot`: Lit les données dans `out/score/` pour afficher des graphiques de performance.
    Les commandes du `Makefile` (`all_t`, `all_m`) utilisent ces arguments automatiquement.
-   `sublim.py`: Bibliothèque pour la création d'images à partir des données de simulation.
-   `analyses.py`: Bibliothèque pour l'analyse statistique des scores et la génération des graphiques avec `matplotlib`.
-   `const.py`: Fichier de constantes définissant les chemins des dossiers.

Pour utiliser ces scripts, les bibliothèques `matplotlib` et `Pillow` sont nécessaires :
```bash
pip install matplotlib Pillow
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