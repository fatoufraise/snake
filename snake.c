#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Pour Windows
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
    #define SLEEP(ms) Sleep(ms)
// Pour Linux/Mac
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
    #define CLEAR_SCREEN "clear"
    #define SLEEP(ms) usleep(ms * 1000)
#endif


#define LARGEUR 40
#define HAUTEUR 20
#define MAX_SERPENT 800  // Taille max possible du serpent
#define VITESSE_INITIALE 150
#define VITESSE_MIN 50


#define VIDE ' '
#define MUR '#'
#define SERPENT 'O'
#define TETE '@'
#define NOURRITURE '*'


#define HAUT 1
#define BAS 2
#define GAUCHE 3
#define DROITE 4


typedef struct {
    int x;
    int y;
} Position;


Position serpent[MAX_SERPENT];
int longueurSerpent = 3;
int direction = DROITE;
int nouvelleDirection = DROITE;
Position nourriture;
int score = 0;
int jeuEnCours = 1;
int vitesse = VITESSE_INITIALE;
int niveau = 1;


void initialiserJeu();
void afficherJeu();
void deplacerSerpent();
void genererNourriture();
int verifierCollision();
void gererInput();
void afficherMenuPrincipal();
void afficherGameOver();
void afficherInstructions();
void viderBuffer();
int kbhit();

#ifndef _WIN32

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if(ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}


int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif


int main() {
    int choix;
    
    srand(time(NULL));
    
    do {
        afficherMenuPrincipal();
        scanf("%d", &choix);
        viderBuffer();
        
        switch(choix) {
            case 1:
                initialiserJeu();
                
                while (jeuEnCours) {
                    system(CLEAR_SCREEN);
                    afficherJeu();
                    gererInput();
                    deplacerSerpent();
                    
                    if (verifierCollision()) {
                        jeuEnCours = 0;
                    }
                    
                    SLEEP(vitesse);
                }
                
                afficherGameOver();
                break;
                
            case 2:
                afficherInstructions();
                break;
                
            case 3:
                printf("\nMerci d'avoir joue ! Au revoir !\n");
                break;
                
            default:
                printf("\nChoix invalide !\n");
                SLEEP(1000);
        }
        
    } while (choix != 3);
    
    return 0;
}


void initialiserJeu() {
    longueurSerpent = 3;
    direction = DROITE;
    nouvelleDirection = DROITE;
    score = 0;
    jeuEnCours = 1;
    vitesse = VITESSE_INITIALE;
    niveau = 1;
    
    
    serpent[0].x = LARGEUR / 4;
    serpent[0].y = HAUTEUR / 2;
    
    serpent[1].x = LARGEUR / 4 - 1;
    serpent[1].y = HAUTEUR / 2;
    
    serpent[2].x = LARGEUR / 4 - 2;
    serpent[2].y = HAUTEUR / 2;
    
    genererNourriture();
}


void afficherJeu() {
    
    for (int i = 0; i < LARGEUR + 2; i++) {
        printf("%c", MUR);
    }
    printf("\n");
    
    
    for (int y = 0; y < HAUTEUR; y++) {
        printf("%c", MUR);  
        
        for (int x = 0; x < LARGEUR; x++) {
            int affiche = 0;
            
            
            if (x == serpent[0].x && y == serpent[0].y) {
                printf("%c", TETE);
                affiche = 1;
            }
           
            else {
                for (int i = 1; i < longueurSerpent; i++) {
                    if (x == serpent[i].x && y == serpent[i].y) {
                        printf("%c", SERPENT);
                        affiche = 1;
                        break;
                    }
                }
            }
            
            // Vérifier si c'est la nourriture
            if (!affiche && x == nourriture.x && y == nourriture.y) {
                printf("%c", NOURRITURE);
                affiche = 1;
            }
            
            // Sinon, case vide
            if (!affiche) {
                printf("%c", VIDE);
            }
        }
        
        printf("%c\n", MUR);  // Bordure droite
    }
    
    
    for (int i = 0; i < LARGEUR + 2; i++) {
        printf("%c", MUR);
    }
    printf("\n");
    
    
    printf("\n Score: %d  |  Longueur: %d  |  Niveau: %d\n", score, longueurSerpent, niveau);
    printf(" Utilisez Z/Q/S/D ou les fleches pour jouer | P pour pause | X pour quitter\n");
}


void deplacerSerpent() {
    
    direction = nouvelleDirection;
    
    
    Position ancienneQueue = serpent[longueurSerpent - 1];
    
    
    for (int i = longueurSerpent - 1; i > 0; i--) {
        serpent[i] = serpent[i - 1];
    }
    
    // Déplacer la tête selon la direction
    switch (direction) {
        case HAUT:
            serpent[0].y--;
            break;
        case BAS:
            serpent[0].y++;
            break;
        case GAUCHE:
            serpent[0].x--;
            break;
        case DROITE:
            serpent[0].x++;
            break;
    }
    
   
    if (serpent[0].x == nourriture.x && serpent[0].y == nourriture.y) {
        longueurSerpent++;
        serpent[longueurSerpent - 1] = ancienneQueue;
        score += 10;
        
       
        if (score % 50 == 0 && vitesse > VITESSE_MIN) {
            vitesse -= 10;
            niveau++;
        }
        
        genererNourriture();
    }
}


void genererNourriture() {
    int valide;
    
    do {
        valide = 1;
        nourriture.x = rand() % LARGEUR;
        nourriture.y = rand() % HAUTEUR;
        
        // Vérifier que la nourriture n'est pas sur le serpent
        for (int i = 0; i < longueurSerpent; i++) {
            if (nourriture.x == serpent[i].x && nourriture.y == serpent[i].y) {
                valide = 0;
                break;
            }
        }
    } while (!valide);
}


int verifierCollision() {
    // Collision avec les murs
    if (serpent[0].x < 0 || serpent[0].x >= LARGEUR ||
        serpent[0].y < 0 || serpent[0].y >= HAUTEUR) {
        return 1;
    }
    
    // Collision avec soi-même
    for (int i = 1; i < longueurSerpent; i++) {
        if (serpent[0].x == serpent[i].x && serpent[0].y == serpent[i].y) {
            return 1;
        }
    }
    
    return 0;
}

// Gérer les entrées clavier
void gererInput() {
    if (kbhit()) {
        char touche = getch();
        
        // Convertir en minuscule
        if (touche >= 'A' && touche <= 'Z') {
            touche = touche + 32;
        }
        
        // Gérer les touches fléchées sous Windows
        #ifdef _WIN32
        if (touche == -32 || touche == 0) {
            touche = getch();
            switch(touche) {
                case 72: touche = 'z'; break;  // Flèche haut
                case 80: touche = 's'; break;  // Flèche bas
                case 75: touche = 'q'; break;  // Flèche gauche
                case 77: touche = 'd'; break;  // Flèche droite
            }
        }
        #endif
        
        switch (touche) {
            case 'z':  // Haut
                if (direction != BAS) {
                    nouvelleDirection = HAUT;
                }
                break;
                
            case 's':  // Bas
                if (direction != HAUT) {
                    nouvelleDirection = BAS;
                }
                break;
                
            case 'q':  // Gauche
                if (direction != DROITE) {
                    nouvelleDirection = GAUCHE;
                }
                break;
                
            case 'd':  // Droite
                if (direction != GAUCHE) {
                    nouvelleDirection = DROITE;
                }
                break;
                
            case 'p':  // Pause
                printf("\nJeu en pause. Appuyez sur une touche pour continuer...");
                getch();
                break;
                
            case 'x':  // Quitter
                jeuEnCours = 0;
                break;
        }
    }
}


void afficherMenuPrincipal() {
    system(CLEAR_SCREEN);
    printf("\n");
    printf("  ====================================\n");
    printf("  |                                  |\n");
    printf("  |        JEU DU SNAKE EN C         |\n");
    printf("  |                                  |\n");
    printf("  ====================================\n");
    printf("\n");
    printf("           @@@@@@@@@@@@\n");
    printf("          @            @\n");
    printf("         @   O O O O    @\n");
    printf("          @            @\n");
    printf("           @@@@@@@@@@@@\n");
    printf("\n");
    printf("  ====================================\n");
    printf("\n");
    printf("       1. Jouer\n");
    printf("       2. Instructions\n");
    printf("       3. Quitter\n");
    printf("\n");
    printf("  ====================================\n");
    printf("\n");
    printf("  Votre choix : ");
}


void afficherGameOver() {
    system(CLEAR_SCREEN);
    printf("\n");
    printf("  ====================================\n");
    printf("  |                                  |\n");
    printf("  |          GAME OVER !             |\n");
    printf("  |                                  |\n");
    printf("  ====================================\n");
    printf("\n");
    printf("       Score final : %d\n", score);
    printf("       Longueur du serpent : %d\n", longueurSerpent);
    printf("       Niveau atteint : %d\n", niveau);
    printf("\n");
    
   
    if (score >= 200) {
        printf("       Excellent ! Vous etes un pro !\n");
    } else if (score >= 100) {
        printf("       Tres bien joue !\n");
    } else if (score >= 50) {
        printf("       Pas mal du tout !\n");
    } else {
        printf("       Continue a t'entrainer !\n");
    }
    
    printf("\n");
    printf("  ====================================\n");
    printf("\n");
    printf("  Appuyez sur Entree pour revenir au menu...");
    viderBuffer();
    getchar();
}


void afficherInstructions() {
    system(CLEAR_SCREEN);
    printf("\n");
    printf("  ====================================\n");
    printf("  |          INSTRUCTIONS            |\n");
    printf("  ====================================\n");
    printf("\n");
    printf("  BUT DU JEU :\n");
    printf("  ------------\n");
    printf("  Mangez la nourriture (*) pour grandir\n");
    printf("  et marquer des points !\n");
    printf("\n");
    printf("  CONTROLES :\n");
    printf("  -----------\n");
    printf("  Z ou Fleche Haut    : Aller en haut\n");
    printf("  S ou Fleche Bas     : Aller en bas\n");
    printf("  Q ou Fleche Gauche  : Aller a gauche\n");
    printf("  D ou Fleche Droite  : Aller a droite\n");
    printf("  P                   : Pause\n");
    printf("  X                   : Quitter\n");
    printf("\n");
    printf("  REGLES :\n");
    printf("  --------\n");
    printf("  - Ne touchez pas les murs (#)\n");
    printf("  - Ne vous mordez pas vous-meme\n");
    printf("  - Le jeu accelere tous les 50 points\n");
    printf("  - Chaque nourriture = 10 points\n");
    printf("\n");
    printf("  SYMBOLES :\n");
    printf("  ----------\n");
    printf("  @  : Tete du serpent\n");
    printf("  O  : Corps du serpent\n");
    printf("  *  : Nourriture\n");
    printf("  #  : Mur\n");
    printf("\n");
    printf("  ====================================\n");
    printf("\n");
    printf("  Appuyez sur Entree pour revenir au menu...");
    getchar();
}


void viderBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}