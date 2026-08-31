#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"

#define NB_PROIES 100
#define NB_PREDATEURS 50
#define T_WAIT 40000
#define MAX_ITERATIONS 50

/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.01;
float p_reproduce_proie=4.0;
float p_reproduce_predateur=0.5;
int temps_repousse_herbe=-15;

int main() {
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;

    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    int initial_x = 5;
    int initial_y = 5;
    float initial_energie = 10.0;
    Animal *animal = creer_animal(initial_x, initial_y, initial_energie);
    
    //! Tester la fonction de déplacement
    // Définir une direction (ex : droite et bas)
    animal->dir[0] = 1;  // dx = 1 (droite)
    animal->dir[1] = 1;  // dy = 1 (bas)

    liste_proie = ajouter_en_tete_animal(liste_proie, animal);

    //Initialisation de l'herbe 
    int tab_herbe[SIZE_X][SIZE_Y];
    for (size_t x = 0; x < SIZE_X; x++){
        for (size_t y = 0; y < SIZE_Y; y++){
            tab_herbe[x][y] = 0; // Initialise les cases du tableau d'herbe à 0;
        }
    }

    // Ouverture du fichier pour écrire les données
    FILE *fichier = fopen("Evol_Pop.txt", "w");
    if (fichier == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier Evol_Pop.txt\n");
        return 1;
    }

    // Affichage de l'écosystème avant le mouvement
    printf("Avant mouvement:\n");
    afficher_ecosys(liste_proie, liste_predateur);

    // Affichage des coordonnées de l'animal avant le mouvement
    printf("Position de l'animal avant mouvement : (%d, %d)\n", animal->x, animal->y);

    bouger_animaux(liste_proie, p_reproduce_proie);

    // Affichage de l'écosystème après le mouvement
    printf("Après mouvement:\n");
    afficher_ecosys(liste_proie, liste_predateur);

    // Affichage des coordonnées de l'animal après le mouvement
    printf("Position de l'animal après mouvement : (%d, %d)\n", animal->x, animal->y);


    //! Tester la fonction de reproduction

    int nb_animaux_avant = compte_animal_it(liste_proie); // Nombre d'animaux avant reproduction
    printf("Nombre d'animaux avant reproduction : %d\n", nb_animaux_avant);

    reproduce(&liste_proie, p_reproduce_proie);

    int nb_animaux_apres = compte_animal_it(liste_proie);
    printf("Nombre d'animaux après reproduction : %d\n\n\n", nb_animaux_apres);

    // Vérifiez si le nombre d'animaux est bien multiplié par 2
    assert(nb_animaux_apres == nb_animaux_avant * 2); // Vérification
    
    // Libération de la mémoire
    liste_proie = liberer_liste_animaux(liste_proie);
    

    //!  simulation d'une évolution de l'écosystème
    for (int i = 0; i < NB_PROIES; i++) {
        int x = rand() % SIZE_X;           // Position aléatoire en x
        int y = rand() % SIZE_Y;           // Position aléatoire en y
        float energie = 10.0 + rand() % 10; // Énergie initiale aléatoire entre 10 et 20
        Animal *new_proie = creer_animal(x, y, energie);
        liste_proie = ajouter_en_tete_animal(liste_proie, new_proie);
    }

    // Ajouter des prédateurs à la liste
    for (int i = 0; i < NB_PREDATEURS; i++) {
        int x = rand() % SIZE_X;           // Position aléatoire en x
        int y = rand() % SIZE_Y;           // Position aléatoire en y
        float energie = 5.0 + rand() % 10; // Énergie initiale aléatoire pour le prédateur
        Animal *new_predateur = creer_animal(x, y, energie);
        liste_predateur = ajouter_en_tete_animal(liste_predateur, new_predateur);
    }

    int nbr_a = compte_animal_it(liste_proie);
    int nbr_predateurs = compte_animal_it(liste_predateur);
    int monde[SIZE_X][SIZE_Y] = {0}; // Initialisation de l'herbe à zéro (pas d'herbe initialement)

    // Boucle de simulation
    int iter = 0;
    while (iter < MAX_ITERATIONS) {
        printf("\nIteration %d\n", iter + 1);

        // Rafraîchir l'herbe dans le monde
        rafraichir_monde(monde);

        // Mettre à jour les proies
        rafraichir_proies(&liste_proie, monde, p_ch_dir ,p_reproduce_proie );

        // Mettre à jour les prédateurs
        rafraichir_predateurs(&liste_predateur, &liste_proie, p_ch_dir, p_reproduce_predateur );

        // Mettre à jour le nombre de proies
        nbr_a = compte_animal_it(liste_proie); // Mettre à jour le nombre de proies
        nbr_predateurs = compte_animal_it(liste_predateur); // Mettre à jour le nombre de prédateurs

        // Afficher l'état actuel de l'écosystème
        afficher_ecosys(liste_proie, liste_predateur);

        // Ecrire dans le fichier le nombre de proies et prédateurs
        fprintf(fichier, "%d %d %d\n", iter, nbr_a, nbr_predateurs);

        // Pause pour observer la simulation
        usleep(T_WAIT);

        // Vérifiez si les proies sont épuisées
        if (nbr_a == 0) {
            printf("Il n'y a plus de proies. Fin de la simulation.\n");
            break;
        }
        
        // Vérifiez si les prédateurs sont épuisés
        if (nbr_predateurs == 0) {
            printf("Il n'y a plus de prédateurs. Fin de la simulation.\n");
            break;
        }

        iter++;
    }

    // Libération de la mémoire
    liste_proie = liberer_liste_animaux(liste_proie);
    liste_predateur = liberer_liste_animaux(liste_predateur);

    fclose(fichier);
    
    return 0;
}