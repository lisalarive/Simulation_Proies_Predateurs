# Simulation_Proies_Predateurs

Projet de Simulation d'Écosystème

Auteurs :

    Lisa Larive 21304884
    Emilio Morales Herrera 21206572

Objectif du Projet :

    Ce programme simule un écosystème dans lequel les proies et les prédateurs interagissent dans un environnement simplifié. Le projet modélise l'évolution de l'écosystème à travers différentes interactions, comme la reproduction, le mouvement des animaux, et la prédation. L’objectif est d'étudier l’évolution des populations de proies et de prédateurs au fil des itérations.


Structure du Projet:

    Le projet est constitué des fichiers suivants :

- `main_ecosys.c` : Contient les tests des fonctionalités principales, comme ceux des interactions (reproduction, mouvement, prédation) ainsi que la logique principale de la simulation, avec l'initialisation de l'écosystème, la gestion des proies et des prédateurs, en plus de la boucle de simulation qui fait évoluer l'écosystème.
- `main_tests.c` : Effectue des tests simples, comme l'affichage et la creátion de listes d'animaux, afin de valider certaines fonctions du système.
- `main_tests2.c` : Effectue des tests fonctionnalités spécifiques à la manipulation de listes d'animaux dans l'écosystème.
- `ecosys.h` et `ecosys.c` : Contient les définitions des structures de données, les déclarations des fonctions de gestion des animaux et d'autres éléments du modèle.
- `Evol_Pop.txt` : Fichier généré pour enregistrer l’évolution des populations de proies et de prédateurs au cours de la simulation.
- `READ.ME` : Documentation du projet.


Compilation et Exécution

  Compilation

Pour compiler le programme, assurez-vous d’être dans le répertoire racine du projet et utilisez la commande make.

        moi@pc ~ $ make clean && make all

Cela générera les fichiers exécutables ecosys, tests_ecosys et tests_ecosys2.

  Exécution

Une fois compilé, vous pouvez exécuter les différents programmes en fonction de vos besoins :

Pour exécuter main_ecosys.c (la simulation principale) :

moi@pc ~ $ ./ecosys

Pour exécuter main_tests.c (les tests simples) :

moi@pc ~ $ ./tests_ecosys

Pour exécuter main_tests2.c (tests de fonctionnalités supplémentaires) :

moi@pc ~ $ ./tests_ecosys2

La simulation enregistre l'évolution des populations de proies et de prédateurs dans le fichier Evol_Pop.txt. Vous pouvez visualiser ces résultats à l'aide de gnuplot :

moi@pc ~ $ gnuplot -p -e "plot 'Evol_Pop.txt' using 1:2 with lines title 'Proies', 'Evol_Pop.txt' using 1:3 with lines title 'Prédateurs'

    
    Description des Fonctionalités principales du main_ecosys:

Ce fichier contient la logique principale du programme ainsi que quelques tests. Voici un résumé des sections :

    Initialisation :
- Création des animaux (proies et prédateurs) avec des positions et énergies aléatoires.
- Initialisation d’un tableau représentant l'herbe dans l'environnement.

    Simulation de l'écosystème :
1. Une boucle de simulation qui se répète jusqu'à ce qu'il n'y ait plus de proies ou de prédateurs, ou jusqu'à ce que le nombre maximal d'itérations (MAX_ITERATIONS) soit atteint.
2. À chaque itération, les animaux bougent, ils se reproduient, et les interactions entre prédateurs et proies se produisent (les prédateurs chassent les proies, ce qui augmente leur énergie).
3. Le nombre de proies et de prédateurs est enregistré dans un fichier Evol_Pop.txt pour chaque itération et le résultat graphique est affiché dans le terminal.

    Mouvement et Reproduction :
- Les animaux se déplacent aléatoirement dans l'environnement selon des probabilités spécifiques.
- La reproduction des proies et des prédateurs est également gérée avec des probabilités de reproduction définies par les paramètres du programme.

    Fin de la simulation :
La simulation se termine lorsque l’une des populations (proies ou prédateurs) est vide, ou après un nombre maximal d’itérations.


    Limitations et Améliorations Possibles:
    
    Visualisation des Résultats :
Pour approfondir, on peut implémenter l'utilisation d'un graphe pour visualiser l'évolution de l'ecosystème au cours du temps :
    moi@pc ~ $ gnuplot -p -e "plot 'Evol_Pop.txt' using 1:2 with lines title 'Proies', 'Evol_Pop.txt' using 1:3 with lines title 'Prédateurs

    Incrémentation des paramètres :
L'inclusion de plus de variables dans le modèle améliorerait la précision des résultats, par exemple, les précipitations qui diminue le temps de repousse de l'herbe.

    Utilisation de Valgrind :

moi@pc ~ $ valgrind ./ecosys

moi@pc ~ $ valgrind ./tests_ecosys

moi@pc ~ $ valgrind ./tests_ecosys2


    Conclusion :
Ce projet permet de simuler un écosystème avec des proies et des prédateurs, tout en intégrant des mécanismes de reproduction, de déplacement et de prédation. Le programme permet d'observer l'évolution des populations au cours du temps et d'analyser l'impact des paramètres de simulation.
