#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"

#define TEMPS_REPOUSSE_HERBE -5  // Valeur négative représentant le temps de repousse de l'herbe 
/* PARTIE 1 */

/* Fonction de création d'un animal avec ses coordonnées, son énergie et sa direction aléatoire */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal)); // Allocation mémoire pour un nouvel animal
  assert(na); // Vérifie que l'allocation s'est bien passée
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1; // Direction aléatoire en x (-1, 0, ou 1)
  na->dir[1] = rand() % 3 - 1; // Direction aléatoire en y (-1, 0, ou 1)
  na->suivant = NULL; // Initialise le pointeur suivant à NULL
  return na;
}

/* Ajoute un animal au début d'une liste chaînée d'animaux */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal); // Vérifie que l'animal est non NULL
  assert(!animal->suivant); // Vérifie que l'animal n'est pas déjà dans une liste
  animal->suivant = liste; // Pointe le nouvel animal vers le premier élément de la liste
  return animal; // Retourne le nouvel animal en tête de liste
}

/* Ajoute un nouvel animal aux coordonnées données dans la liste d'animaux spécifiée */
void ajouter_animal(int x, int y, float energie, Animal **liste_animal) {
  assert(liste_animal != NULL);
  assert(x >= 0 && x < SIZE_X);  
  assert(y >= 0 && y < SIZE_Y);
  
  Animal* new_animal = creer_animal(x, y, energie); // Crée un nouvel animal
  *liste_animal = ajouter_en_tete_animal(*liste_animal, new_animal); // Ajoute l'animal en tête de la liste
}

/* Retire un animal spécifique de la liste et libère sa mémoire */
void enlever_animal(Animal **liste, Animal *animal) {
  assert(liste != NULL);

  Animal *tmp = *liste;
  Animal *prev = NULL;

  // Vérifie si l'animal est en tête de liste
  if (tmp == animal) {
    *liste = tmp->suivant;  // Met à jour la tête de liste
    free(tmp);               // Libère la mémoire de l'animal supprimé
    return;
  }

  // Parcourt la liste pour trouver l'animal et le retirer
  while (tmp != NULL) {
    if (tmp == animal) {
      if (prev != NULL) {
        prev->suivant = tmp->suivant; // Lie l'élément précédent au suivant de l'animal
      }
      free(tmp); // Libère la mémoire de l'animal
      return;
    }
    prev = tmp; // Déplace `prev` vers l'élément courant
    tmp = tmp->suivant; // Passe à l'élément suivant dans la liste
  }
}

/* Libère toute la liste d'animaux et retourne NULL */
Animal* liberer_liste_animaux(Animal *liste) {
   Animal* tmp;
   while (liste != NULL) {
       tmp = liste->suivant; // Sauvegarde l'élément suivant
       assert(liste != NULL);
       free(liste); // Libère la mémoire du courant
       liste = tmp; // Passe au suivant
   }
   return NULL; // Retourne NULL pour signaler que la liste est vide
}

/* Compte récursivement le nombre d'animaux dans une liste */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Compte itérativement le nombre d'animaux dans une liste */
unsigned int compte_animal_it(Animal *la) {
  int cpt = 0;
  while (la) {
    ++cpt;
    la = la->suivant;
  }
  return cpt;
}

/* Affiche l'écosystème sous forme de grille avec les proies et prédateurs représentés par des symboles */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa = NULL;

  /* Initialise la grille de l'écosystème à des espaces vides */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j] = ' ';
    }
  }

  /* Ajoute les proies représentées par '*' */
  pa = liste_proie;
  while (pa) {
    assert(pa->x >= 0 && pa->x < SIZE_X);  
    assert(pa->y >= 0 && pa->y < SIZE_Y);
    ecosys[pa->x][pa->y] = '*';
    pa = pa->suivant;
  }

  /* Ajoute les prédateurs représentés par 'O' ou '@' si une proie est présente */
  pa = liste_predateur;
  while (pa) {
    assert(pa->x >= 0 && pa->x < SIZE_X); 
    assert(pa->y >= 0 && pa->y < SIZE_Y);
    if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { // Proie et prédateur dans la même case
      ecosys[pa->x][pa->y] = '@';
    } else {
      ecosys[pa->x][pa->y] = 'O';
    }
    pa = pa->suivant;
  }

  /* Affiche la grille avec les bordures */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");

  int nbproie = compte_animal_it(liste_proie); // Compte le nombre de proies
  int nbpred = compte_animal_it(liste_predateur); // Compte le nombre de prédateurs
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}

/* Efface l'écran en utilisant un code ANSI */
void clear_screen() {
  printf("\x1b[2J\x1b[1;1H"); 
}

/* Fait bouger les animaux avec une probabilité donnée de changement de direction */
void bouger_animaux(Animal *la, int p_ch_dir) {
  Animal* tmp = la;
  while (tmp) {
    if (rand()/(float) RAND_MAX < p_ch_dir) {
      tmp->dir[0] = rand() % 3 - 1; // Nouvelle direction en x
      tmp->dir[1] = rand() % 3 - 1; // Nouvelle direction en y
    }
    tmp->x = (tmp->x + tmp->dir[0] + SIZE_X) % SIZE_X; // Déplace l'animal dans la grille en x
    tmp->y = (tmp->y + tmp->dir[1] + SIZE_Y) % SIZE_Y; // Déplace l'animal en y
    tmp = tmp->suivant;
  }
}

/* Gère la reproduction des animaux avec une probabilité donnée */
void reproduce(Animal **liste_animal, float p_reproduce) {
  Animal *tmp = liste_animal ? *liste_animal : NULL;
  while (tmp) {
    if (rand() / (float)RAND_MAX < p_reproduce) {
      ajouter_animal(tmp->x, tmp->y, tmp->energie / 2, liste_animal); // Crée un nouvel animal
      tmp->energie /= 2; // Réduit l'énergie du parent
    }
    tmp = tmp->suivant;
  }
}

/* Met à jour la liste des proies : déplacement, perte d'énergie, et reproduction */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y], float p_ch_dir, float p_reproduce_proie) {
  Animal *current = *liste_proie;

  while (current != NULL) {
    bouger_animaux(current, p_ch_dir); // Déplace la proie

    if (current->x >= 0 && current->x < SIZE_X && current->y >= 0 && current->y < SIZE_Y) {
    // Si la proie est sur une case avec de l'herbe
      if (monde[current->x][current->y] > 0) {
          current->energie += monde[current->x][current->y]; // Gagne de l'énergie
          monde[current->x][current->y] = TEMPS_REPOUSSE_HERBE; // L'herbe est mangée et commence à repousser
      }
    }

    if (current->energie > 0) { // Vérifie si l'énergie est positive avant de la diminuer
      current->energie -= 1;
    }

    if (current->energie <= 0) { // Supprime la proie si l'énergie est épuisée
      Animal *to_remove = current;
      current = current->suivant;
      enlever_animal(liste_proie, to_remove);
    } else {
      current = current->suivant; // Passe à la proie suivante
    }
  }

  reproduce(liste_proie, p_reproduce_proie); // Gère la reproduction des proies
}

/* Recherche et retourne un animal aux coordonnées (x, y) */
Animal *animal_en_XY(Animal *l, int x, int y) {
  Animal *current = l;

  while (current != NULL) {
    if (current->x == x && current->y == y) {
      return current;
    }
    current = current->suivant;
  }  

  return NULL;
} 

/* Met à jour la liste des prédateurs : déplacement, chasse de proies, perte d'énergie, et reproduction */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie, float p_ch_dir, float p_reproduce_predateur) {
  Animal *current_predateur = *liste_predateur;

  while (current_predateur != NULL) {
    bouger_animaux(current_predateur, p_ch_dir); // Déplace le prédateur

    if (current_predateur->energie > 0) { // Diminue l'énergie du prédateur
      current_predateur->energie -= 1;
    }

    if (current_predateur->energie <= 0) { // Supprime le prédateur si l'énergie est épuisée
      Animal *to_remove = current_predateur;
      current_predateur = current_predateur->suivant;
      enlever_animal(liste_predateur, to_remove);
      continue;
    }

    // Recherche de proie dans la même position
    Animal *current_proie = *liste_proie;
    while (current_proie != NULL) {
      if (current_proie->x == current_predateur->x && current_proie->y == current_predateur->y) {
        current_predateur->energie += current_proie->energie; // Prend l'énergie de la proie
        Animal *to_remove = current_proie;
        current_proie = current_proie->suivant;
        enlever_animal(liste_proie, to_remove); // Supprime la proie
        break;
      }
      current_proie = current_proie->suivant;
    }

    current_predateur = current_predateur->suivant; 
  }

  reproduce(liste_predateur, p_reproduce_predateur); // Gère la reproduction des prédateurs
}

/* Met à jour l'état du monde */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]) {
   for (size_t x = 0; x < SIZE_X; x++){
      assert(x<SIZE_X);
    for (size_t y = 0; y < SIZE_Y; y++){
      assert(y<SIZE_Y);
      monde[x][y]++;
    }
  }
}
