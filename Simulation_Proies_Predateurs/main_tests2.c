#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"
#include <string.h>

#define NUM_PROIES 20
#define NUM_PREDATEURS 20

void ecrire_ecosysteme(const char *nom_fichier, Animal *liste_proie, Animal *liste_predateur) {
    
    FILE *f = fopen(nom_fichier, "w");
    
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture du fichier pour l'écriture");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "Proies\n");
    Animal *tmp = liste_proie;
    while (tmp) {
        fprintf(f, "%d %d %.2f\n", tmp->x, tmp->y, tmp->energie);
        tmp = tmp->suivant;
    }

    fprintf(f, "Prédateurs\n");
    tmp = liste_predateur;
    while (tmp) {
        fprintf(f, "%d %d %.2f\n", tmp->x, tmp->y, tmp->energie);
        tmp = tmp->suivant;
    }

    fclose(f);
}

    
void lire_ecosysteme(const char *nom_fichier, Animal **liste_proie, Animal **liste_predateur) {

    FILE *f = fopen(nom_fichier, "r");
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture du fichier pour la lecture");
        exit(EXIT_FAILURE);
    }

    char buffer[100]; // Buffer pour chaque ligne
    int x, y;
    float energie;
    Animal *new_animal = NULL;
    char type[20] = "";  // Initialise le type à une chaîne vide
    
    while (fgets(buffer, sizeof(buffer), f)) {
        if (strcmp(buffer, "Proies\n") == 0 || strcmp(buffer, "Prédateurs\n") == 0) {
            strcpy(type, buffer); // Conserve le type
            continue; // Passe à la prochaine ligne
        }

        if (sscanf(buffer, "%d %d %f", &x, &y, &energie) == 3) {
            new_animal = creer_animal(x, y, energie);

            if (strcmp(type, "Proies\n") == 0) {
                *liste_proie = ajouter_en_tete_animal(*liste_proie, new_animal);
            } else if (strcmp(type, "Prédateurs\n") == 0) {
                *liste_predateur = ajouter_en_tete_animal(*liste_predateur, new_animal);
            }
        } else {
            fprintf(stderr, "Format invalide trouvé dans le fichier : %s\n", buffer);
            free(new_animal); // Libérer la mémoire si le format est invalide
        }
    }

    fclose(f);
}





int main() {
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;

    srand(time(NULL));

    for (int i = 0; i < NUM_PROIES; i++) {
        int x = rand() % SIZE_X;
        int y = rand() % SIZE_Y;
        float energie = (float)(rand() % 100) / 10.0;  // Énergie entre 0.0 et 10.0
        ajouter_animal(x, y, energie, &liste_proie);
    }

    for (int i = 0; i < NUM_PREDATEURS; i++) {
        int x = rand() % SIZE_X;
        int y = rand() % SIZE_Y;
        float energie = (float)(rand() % 100) / 10.0;  // Énergie entre 0.0 et 10.0
        ajouter_animal(x, y, energie, &liste_predateur);
    }


    unsigned int nb_proies = compte_animal_it(liste_proie);
    unsigned int nb_predateurs = compte_animal_it(liste_predateur);

    printf("\nNombre initiale de proies : %u\n", nb_proies);
    printf("Nombre initiale de prédateurs : %u\n\n", nb_predateurs);


    ecrire_ecosysteme("ecosysteme.txt", liste_proie, liste_predateur);

    afficher_ecosys(liste_proie, liste_predateur);

      // Suppression aléatoire de proies
    printf("\nSuppression aléatoire de proies\n");
    for (int i = 0; i < 20; i++) {  // Parcours 20 fois
        int x_a_enlever = rand() % SIZE_X;
        int y_a_enlever = rand() % SIZE_Y;
        Animal *animal_a_enlever = animal_en_XY(liste_proie, x_a_enlever, y_a_enlever);

        if (animal_a_enlever != NULL) {
            enlever_animal(&liste_proie, animal_a_enlever);
            printf("Proie supprimée aux coordonnées (%d, %d)\n", x_a_enlever, y_a_enlever);
        } else {
            printf("Aucune proie trouvée aux coordonnées (%d, %d)\n", x_a_enlever, y_a_enlever);
        }
    }

    // Suppression aléatoire de prédateurs
    printf("\nSuppression aléatoire de prédateurs\n");
    for (int i = 0; i < 20; i++) {  // Parcours 20 fois
        int x_a_enlever = rand() % SIZE_X;
        int y_a_enlever = rand() % SIZE_Y;
        Animal *animal_a_enlever = animal_en_XY(liste_predateur, x_a_enlever, y_a_enlever);

        if (animal_a_enlever != NULL) {
            enlever_animal(&liste_predateur, animal_a_enlever);
            printf("Prédateur supprimé aux coordonnées (%d, %d)\n", x_a_enlever, y_a_enlever);
        } else {
            printf("Aucun prédateur trouvé aux coordonnées (%d, %d)\n", x_a_enlever, y_a_enlever);
        }
    }

    afficher_ecosys(liste_proie, liste_predateur);


    liste_proie = liberer_liste_animaux(liste_proie);
    liste_predateur = liberer_liste_animaux(liste_predateur);

    lire_ecosysteme("ecosysteme.txt", &liste_proie, &liste_predateur);

    afficher_ecosys(liste_proie, liste_predateur);

    liste_proie = liberer_liste_animaux(liste_proie);
    liste_predateur = liberer_liste_animaux(liste_predateur);
    
    return 0;
}

