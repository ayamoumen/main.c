#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

/* Declarer les constantes */
#define LIGNES 15
#define COLONNES 15

#define HORIZONTALE    0
#define VERTICALE      1

#define NOM_DE_NAVIRES 5

#define NORTH 0
#define SOUTH 1
#define WEST  2
#define EAST  3

/* Declarer les structures */
typedef struct Stats {
    int vie;
    int toucher;
    int manquer;
} Stats;

typedef enum {
    FAUX, VRAIE
} Booleen;

typedef struct Carreaux {
    bool brume;
    char symbole;
} Carreaux;

typedef struct coordonnee {
    int ligne;
    int colonne;
} coordonnee;

typedef struct Flotte {
    char symbole;
    short longueur;
    char *nom;
} Flotte;


/*** Fonctions d aide ***/
void Aide() {
    printf("1. Le but du Jeu est de detruire tout les navires ennemis\n");

    printf("2. Pour cela vous dispose d une flotte de 10 navires : 1 cuirasse, 2 croiseurs, 3 destroyers et 4 sous marins\n");

    printf("3. En debut de jeu la disposition de vos navires et de celui de l ordinateur sont repartie aleatoirement\n");

    printf("4. La grille N2 montre les degats inflige a l adversaire et est revele par la touche 'V' appele le mode visible, la grille N1 est celle du joueur\n");

    printf("5. Le joueur tire en entrant les coordonnees d une case valide de l adversaire: 'E' si le tire tombe à l eau, 'T' si le navire a ete touche\n ");

    printf("6. Pour faire couler un navire il faut toucher l ensemble des cases qu il occupe\n");

    printf("7. Chaque joueur possede 4 fussees eclairant qui permet de revele a l adversaire une zone de 4*4 a partir du coin haut et gauche dont on choisi les coordonnees\n");

    printf("8. Cependant la zone reste visible la duree d un tour\n");

    printf("9. Les navires peuvent se deplacer a condition qu il ne soit pas couler et qu il ne se deplace que dans une direction VERTICALEe et HORIZONTALEe d une seule case\n");

    printf("10. A chaque tour le joueur peux quitter la partie et la reprendre en chargeant la derniere partie a partir du menu\n");

}

/*
 void Color
 Fonction qui permet de mettre un text en couleur
 */
void Color(int couleurDuTexte, int couleurDeFond);


/*
void Affichage
Permet l'affichage des deux tableaux comme grille de jeu
*/
void Afficher(Carreaux Player[][COLONNES], Carreaux AI[][COLONNES]) {
    int i = 0, j = 0, n = 0;

    system("cls");
    Color(11, 0);
    printf("\n\n\t       GRILLE DU JOUEUR\t\t\t\t\t\t                  GRILLE DE L'IA\n\n");
    Color(15, 0);
    Color(14, 0);

    printf("\t    0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 \t   0  1  2  3  4  5  6  7  8  9  10 11 12 13 14  \n");
    Color(15, 0);
    for (i = 0, n = 0; i < 10; ++i, n++) {
        printf("\t%d | ", n);
        for (j = 0; j < 15; ++j) {
            if (Player[i][j].brume == FAUX)
                printf("%c |", Player[i][j].symbole);
            else
                printf("%c |", '~');

        }

        printf("");
        printf("\t%d |",n);

        for (j = 0; j < 15; ++j) {
            if (AI[i][j].brume == FAUX)
                printf("%c |", AI[i][j].symbole);
            else
                printf("%c |", 254);

        }
        printf("\n");

    }
    for (i = 10, n = 10; i < 15; ++i, n++) {
        printf("\t%d| ", n);
        for (j = 0; j < 15; ++j) {
            if (Player[i][j].brume == FAUX)
                printf("%c |", Player[i][j].symbole);
            else
                printf("%c |", '~');

        }

        printf("");
        printf("\t%d|", n);

        for (j = 0; j < 15; ++j) {
            if (AI[i][j].brume == FAUX)
                printf("%c |", AI[i][j].symbole);
            else
                printf("%c |", 254);

        }
        printf("\n");

    }
}


/*
Void GrilledeJeu
Permet de mettre à jour les valeurs du tableau correspondant
*/
void GrilledeJeu(Carreaux grilledejeu[][COLONNES], coordonnee cible) {
    if (grilledejeu[cible.ligne][cible.colonne].symbole == 'E' ||
        grilledejeu[cible.ligne][cible.colonne].symbole == 'm') {
        grilledejeu[cible.ligne][cible.colonne].brume = FAUX;
    } else {
        Color(12, 0);
        grilledejeu[cible.ligne][cible.colonne].symbole = 'T';
        Color(15, 0);
        grilledejeu[cible.ligne][cible.colonne].brume = FAUX;
    }

}


/*
void Grille
	Objectif :
		vérifie les cardinaux s'ils sont dans le tableau
*/
void Grille(Booleen cardinaux[], int lier, int direction) {
    switch (direction) {
        case NORTH:
            if (lier < 0)
                cardinaux[0] = FAUX;
            else
                cardinaux[0] = VRAIE;
            break;

        case SOUTH:
            if (lier > 15)
                cardinaux[1] = FAUX;
            else
                cardinaux[1] = VRAIE;
            break;

        case WEST:
            if (lier < 0)
                cardinaux[2] = FAUX;
            else
                cardinaux[2] = VRAIE;
            break;

        case EAST:
            if (lier > 15)
                cardinaux[3] = FAUX;
            else
                cardinaux[3] = VRAIE;
            break;
    }
}


/*
	Vérification du bateau
 Permet de vérifier si un navire a été coulé.
*/
Booleen
VerificationBateau(short BateauaVoile[][NOM_DE_NAVIRES], short player, char naviresymbole, Carreaux Player[][COLONNES],
                   Carreaux AI[][COLONNES]) {
    Booleen couler = FAUX;

    if (naviresymbole == 'C') {

        if (--BateauaVoile[player][0] == 0) {
            system("cls");
            Afficher(Player, AI);
            printf("\n\tRAPPORT DE BATAILLE :\n");
            if (player == 0) {
                printf("\n\t> Le transporteur de l'IA est coulé !\n");
            } else {
                printf("\n\t> Le transporteur du joueur a coulé !\n");
            }


            couler = VRAIE;
            printf("\n\tAppuyez sur n'importe quelle touche pour continuer...");
            system("pause > nulle");
        }
    } else if (naviresymbole == 'R') {
        if (--BateauaVoile[player][2] == 0) {
            system("cls");
            Afficher(Player, AI);
            printf("\n\tRAPPORT DE BATAILLE :\n");
            if (player == 0) {
                printf("\n\t> Le croiseur de l'IA a été coulé !\n");
            } else {
                printf("\n\t> Le croiseur du joueur a coulé !\n");
            }

            couler = VRAIE;
            printf("\n\tAppuyez sur n'importe quelle touche pour continuer...");
            system("pause > nulle");
        }
    } else if (naviresymbole == 'S') {
        if (--BateauaVoile[player][3] == 0) {
            system("cls");
            Afficher(Player, AI);
            printf("\n\tRAPPORT DE BATAILLE ::\n");
            if (player == 0) {
                printf("\n\t> Le sous-marin de l'IA a coulé !\n");
            } else {
                printf("\n\t> Le sous-marin du joueur a coulé !\n");
            }

            couler = VRAIE;
            printf("\n\tAppuyez sur n'importe quelle touche pour continuer...");
            system("pause > nulle");
        }
    } else if (naviresymbole == 'D') {
        if (--BateauaVoile[player][4] == 0) {
            system("cls");
            Afficher(Player, AI);
            printf("\n\tRAPPORT DE BATAILLE :\n");
            if (player == 0) {
                printf("\n\t> Le Destroyer de l'IA a coulé !\n");
            } else {
                printf("\n\t> Le Destroyer du joueur a coulé !\n");
            }

            couler = VRAIE;
            printf("\n\tAppuyez sur n'importe quelle touche pour continuer...");
            system("pause > nulle");
        }
    }

    return couler;

}


/*
	short VerfierleTir
 Il renvoie 0 si manquant, 1 si touchant, -1 si invalide
*/
short VerifierleTir(Carreaux grilledejeu[][COLONNES], coordonnee cible) {
    int toucher = -2;

    if ((grilledejeu[cible.ligne][cible.colonne].symbole == 'E' &&
         grilledejeu[cible.ligne][cible.colonne].brume == VRAIE) ||
        (grilledejeu[cible.ligne][cible.colonne].symbole == 'm' &&
         grilledejeu[cible.ligne][cible.colonne].brume == VRAIE)) {
        toucher = 0;
    }

    if (grilledejeu[cible.ligne][cible.colonne].symbole == 'C' ||
        grilledejeu[cible.ligne][cible.colonne].symbole == 'R' ||
        grilledejeu[cible.ligne][cible.colonne].symbole == 'S' ||
        grilledejeu[cible.ligne][cible.colonne].symbole == 'D') {
        toucher = 1;
    }

    if (grilledejeu[cible.ligne][cible.colonne].symbole == 'T' ||
        (grilledejeu[cible.ligne][cible.colonne].brume == FAUX &&
         grilledejeu[cible.ligne][cible.colonne].symbole == 'E') ||
        (grilledejeu[cible.ligne][cible.colonne].symbole == 'm' &&
         grilledejeu[cible.ligne][cible.colonne].brume == FAUX)) {
        toucher = -1;
    }

    return toucher;
}


/*
	coordonnee obtenirunecible
			obtenir X et Y coordonnée
*/
coordonnee obtenirunecible(Carreaux Player[][COLONNES], Carreaux AI[][COLONNES], int player) {
    coordonnee cible;

    bool verifier = VRAIE;
    do {
        fflush(stdin);
        system("cls");
        Afficher(Player, AI);
        if (player == 0)
            printf("\n\tTOUR DU JOUEUR\n");
        else
            printf("\n\tTOUR DE L'IA\n");
        printf("\n\tEntrez X coordonnee : ");
        scanf("%d", &cible.colonne);
        if (cible.colonne >= 0 && cible.colonne < COLONNES) {
            verifier = FAUX;
        } else {
            printf("\n\tEntrée invalide... Veuillez essayer à nouveau...\n\tAppuyez sur n'importe quelle 'touche' pour continuer...");
            system("pause > nulle");
        }

    } while (verifier);

    verifier = VRAIE;


    do {
        fflush(stdin);
        system("cls");
        Afficher(Player, AI);
        if (player == 0)
            printf("\n\tTOUR DU JOUEUR\n");
        else
            printf("\n\tTOUR DE L'IA\n");
        printf("\n\tEntrez X coordonnee : %d", cible.colonne);
        printf("\n\tEntrez Y coordonnee : ");
        scanf("%d", &cible.ligne);
        if (cible.ligne >= 0 && cible.ligne < LIGNES) {
            verifier = FAUX;
        } else {
            printf("\n\tEntree invalide... Veuillez essayer a nouveau..\n\tAppuyez sur n'importe quelle 'touche' pour continuer...");
            system("pause > nulle");
        }
    } while (verifier);


    return cible;
}


/*
	int Aleatoire
Elle renvoie 0, ou 1 (utilisé n'importe où)
*/
int Aleatoire(int minimum, int maximum) {
    if (minimum == 0)
        return rand() % ++maximum;

    if (minimum > 0)
        return rand() % ++maximum + minimum;

    return 0;
}


/*
	coordonnee generatePosition
 Elle génère aléatoirement une position
*/
coordonnee generatePosition(int direction, int longueur) {
    coordonnee position;

    if (direction == HORIZONTALE) {
        position.ligne = Aleatoire(0, LIGNES);
        position.colonne = Aleatoire(0, COLONNES - longueur);
    } else { /* VERTICALE */
        position.ligne = Aleatoire(0, LIGNES - longueur);
        position.colonne = Aleatoire(0, COLONNES);
    }

    return position;
}


/*
	Booleen LocalisationValide
 Elle vérifie si le poste est valide
*/
Booleen
LocalisationValide(Carreaux grilledejeu[][COLONNES], coordonnee position, int direction, int longueur, int player) {
    int i = longueur - 1;
    Booleen Validite = VRAIE;

    for (i = 0; Validite && i < longueur; i++) {
        if (direction == HORIZONTALE) {
            if (player == 1) {
                if (grilledejeu[position.ligne][position.colonne + i].symbole != 'E' &&
                    (position.colonne + i) < COLONNES)

                    Validite = FAUX;

            } else {
                if (grilledejeu[position.ligne][position.colonne + i].symbole != 'm' &&
                    (position.colonne + i) < COLONNES)
                    Validite = FAUX;
            }

        } else { /* VERTICALE */

            if (player == 1) {
                if (grilledejeu[position.ligne + i][position.colonne].symbole != 'E' && (position.ligne + i) < LIGNES)
                    Validite = FAUX;

            } else {
                if (grilledejeu[position.ligne + i][position.colonne].symbole != 'm' && (position.ligne + i) < LIGNES)
                    Validite = FAUX;
            }


        }
    }

    return Validite;
}


/*
	void creationbateaux
Cette fonction place le navire sur le tableau
*/
void creationbateaux(Carreaux grilledejeu[][COLONNES], Flotte bateau, coordonnee position, int direction, int player) {
    int i = bateau.longueur - 1;

    for (i = 0; i < bateau.longueur; i++) {
        if (direction == HORIZONTALE) {
            if (player == 1) {
                grilledejeu[position.ligne][position.colonne + i].symbole = bateau.symbole;
            } else {
                grilledejeu[position.ligne][position.colonne + i].brume = FAUX;
                grilledejeu[position.ligne][position.colonne + i].symbole = bateau.symbole;
            }

        } else /* VERTICALE */
        {
            if (player == 1) {
                grilledejeu[position.ligne + i][position.colonne].symbole = bateau.symbole;
            } else {
                grilledejeu[position.ligne + i][position.colonne].brume = FAUX;
                grilledejeu[position.ligne + i][position.colonne].symbole = bateau.symbole;
            }

        }
    }
}


/*
	void generateposition
 Fonction MACRO pour générer les positions aléatoires
*/
void generateposition(Carreaux grilledejeu[][COLONNES], Flotte bateau[], int player) {
    coordonnee position;
    int direction = -1;
    int i = 0;

    for (i = 0; i < NOM_DE_NAVIRES; i++) {
        while (VRAIE) {
            direction = Aleatoire(0, 1); /* 0 -> HORIZONTALE, 1 -> VERTICALE */
            position = generatePosition(direction, bateau[i].longueur);

            if (LocalisationValide(grilledejeu, position, direction, bateau[i].longueur, player))
                break;
        }

        creationbateaux(grilledejeu, bateau[i], position, direction, player);
    }
}


/***** FONCTION PRINCIPALE *****/

int main()// ACD: analyse chronologique descendante : au debut on presente le jeu et ce qui permet de faire
{

    srand((unsigned int) time(NULL));//on initialise - on a bien apporté la bibliotehque time
    int option;
    bool optionchar = VRAIE;

    do {
        system("cls");
        Color(1, 15);
        printf(" ____        _        _ _ _                               _      \n");
        printf("| __ )  __ _| |_ __ _(_) | | ___   _ __   __ ___   ____ _| | ___ \n");
        printf("|  _ \\ / _` | __/ _` | | | |/ _ \\ | '_ \\ / _` \\ \\ / / _` | |/ _ \\\n");
        printf("| |_) | (_| | || (_| | | | |  __/ | | | | (_| |\\ V / (_| | |  __/\n");
        printf("|____/ \\__,_|\\__\\__,_|_|_|_|\\___| |_| |_|\\__,_| \\_/ \\__,_|_|\\___|\n\n");
        printf("        _     _     _ "  "__-=-//__  __\\\\-=-__"  " _     _     _        \n");
        printf(".-.,.-'`(,.-'`(,.-'`(,"  "\\_______/"  ".."  "\\_______/" ",)`'-.,)`'-.,)`'-.,.-.\n\n");

        printf("                       MENU DU JEU :                       \n");

        printf("        VEUILLEZ CONFIRMER VOTRE CHOIX PAR ENTREE          \n");
        printf("            1-DEMARRER UNE NOUVELLE PARTIE                  \n");
        printf("            2-PARAMETRE ET REGLE DU JEU                     \n");
        printf("            3-PERSONNES QUI ONT FAIT LE PROJET              \n");
        printf("            4-QUITTER LE JEU                                \n");
        printf("            5-Sauvegarder                                   \n");

        Color(1, 0);
        printf("\n\tMode d'Entree\n\tEntrer ici : ");
        Color(15, 0);
        scanf("%d", &option);



        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if (option ==
            1)//ACD : l'option 1 permet de JOUER,on affiche la grille, on affiche la structure des bateaux, le placement des bateaux, on initialse la grille au fur et a mesure de la partie,
        {

            // initialisation des grilles
            Carreaux Player[LIGNES][COLONNES]; // Grille de Jeu du Joueur
            Carreaux AI[LIGNES][COLONNES];     // Grille de Jeu de l'IA

            int v = 0, j = 0;//on initialise les entiers

            for (v = 0; v < LIGNES; ++v)//l'entier v correspond a l'incrementation de la lignes
            {
                for (j = 0; j < COLONNES; ++j)//l'entier v correspond a l'incrementation de la lignes
                {

                    Player[v][j].brume = VRAIE;//la grille du joeur est rempli de ~
                    AI[v][j].brume = VRAIE;//la grille du joeur est rempli de brume

                    Player[v][j].symbole = 'm';//lorsque l'AI rate sa cible on identifie son tir par un m pour manquer
                    AI[v][j].symbole = 'E';//lorsque le joueur rate sa cible on identifie son tir par un E pour Eau
                }
            }


            Flotte /*ACD on definit les bateaux,les coordonnes */
            bateau[NOM_DE_NAVIRES] = {{'C', 1, "Transporteur"},
                                      {'R', 2, "Croiseur"},
                                      {'S', 4, "Sous marin"},
                                      {'D', 3, "Destroyer"}};

            generateposition(Player, bateau, 0);
            generateposition(AI, bateau, 1);
            Afficher(Player, AI);

            coordonnee cible;             /* Valeur x, y d'une cible */
            coordonnee cibleTemp;         /* x, y coordonnée temporaire  */
            coordonnee cibleOrigin;       /* Valeur x, y de la cible originale */
            coordonnee cibleAI;           /* Valeur x, y des cibles à l'aide de la technique IA */

            Booleen modeChasse = VRAIE;                     /* mode de sélection aléatoire d'une cible */
            Booleen modeDescente = FAUX;                    /* mode lorsqu'il y a un bateau toucher */
            Booleen nager = VRAIE;                      /* inverser les valeurs de Booleen */
            Booleen cardinaux[4] = {VRAIE, VRAIE, VRAIE, VRAIE}; /* représente les 4 cardinaux, N, S, O, E */
            Booleen bateaucouler = FAUX;                    /* si un bateau a coulé */
            Booleen Ressourcejoueur = FAUX;

            short player = 0;               /* 0 -> joueur1, 1 -> joueur2 */
            short tir = 0;             /* conserver la valeur temporelle si le bateau a été tiré */
            int option = 0;             /* possibilité pour le joueur de placer le bateau manuellement ou aléatoirement */
            int north = 0,             /* détient un changement de valeur en allant vers le nord */
            south = 0,             /* détient un changement de valeur en allant vers le sud */
            east = 0,             /* détient un changement de valeur en allant vers le est */
            west = 0;             /* détient un changement de valeur en allant vers le ouest */
            int i = 0, compteur = 1;             /* i et compteur sont utilise comme compteurs */

            char naviresymbole = '\0';       /* valeur temporaire pour sauvegarder le symbole de caractère du bateau */

            short BateauaVoile[2][NOM_DE_NAVIRES] = {{1, 2, 4, 3},
                                                     {1, 2, 4, 3}};  /* traces des parties du bateau détruites */

            Stats Humain = {17, 0, 0};
            Stats IntArtificielle = {17, 0, 0};

            int compteurTours = 1;

            while (VRAIE) {
                player = 0;
                /////////TOUR DU JOUEUR///////////////
                Color(11, 0);
                printf("\n\n\tTOUR DU JOUEUR :");
                Color(15, 0);

                do {
                    cible = obtenirunecible(Player, AI, 0);
                    tir = VerifierleTir(AI, cible);

                    if (tir == -1) {
                        printf("\n\tRAPPORT DE BATAILLE:\n");
                        printf("\n\tEssayez une autre cible\n\tAppuyez sur n'importe quelle touche pour continuer");
                        system("pause > nulle");
                    }

                } while (tir == -1);

                naviresymbole = AI[cible.ligne][cible.colonne].symbole;

                if (tir == 1)/*ACD lorsque l'on touche un navire, on affiche la fonction AFFICHER */
                { /* toucher */
                    IntArtificielle.vie--;
                    Humain.toucher++;
                    system("cls");
                    Afficher(Player, AI);
                    printf("\n\tRAPPORT DE BATAILLE :\n");
                    printf("\n\t>coordonnee: %d, %d est touche!\n\n\tAppuyez sur n'importe quelle touche pour continuer...",
                           cible.colonne, cible.ligne);
                    system("pause > nulle");


                    /* verifier si le bateau a coulé */
                    Ressourcejoueur = VerificationBateau(BateauaVoile, player, naviresymbole, Player, AI);

                } else/*ACD si on touche pas alors on a manquer notre tire et on affche la fonction Afficher */
                {        /* manquer */
                    Humain.manquer++;
                    system("cls");
                    Afficher(Player, AI);
                    printf("\n\tRAPPORT DE BATAILLE :\n");
                    printf("\n\t>coordonnee %d, %d est manque!\n\n\tAppuyez sur n'importe quelle touche pour continuer...",
                           cible.colonne, cible.ligne);
                    system("pause > nulle");

                }

                GrilledeJeu(AI, cible);
                Afficher(Player, AI);
                printf("\n\tTOUR DE L'IA\n\n\tL'IA est en train de saisir les coordonnees...\n\t");
                printf("PRESSER une touche pour continuer....");
                system("pause > nulle");


                /**** MISE EN ŒUVRE DE L'INTELLIGENCE ARTIFICIELLE *****/
                //////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////
                player = 1;

                if (bateaucouler) {
                    bateaucouler = FAUX;//si le bateau coule est equivalent a FAUX car bateaucouler est un boolean
                    modeDescente = FAUX;//modedescente = FAUX est équivalent lorsque le bateau est toucher
                    modeChasse = VRAIE;//modeChase = VRAIE équivalent à la sélection aléatoire d'une cible
                }


                if (modeDescente)//ACD _lorsque le bateau est toucher
                {
                    /* valeur retenue pour cibleAI */
                    cible = cibleAI;

                    do {

                        if (cardinaux[NORTH]) {  /* NORD */
                            cible.ligne = north;
                        } else if (cardinaux[SOUTH]) {  /* SUD */
                            cible.ligne = south;
                        } else if (cardinaux[WEST]) {  /* OUEST */
                            cible.colonne = west;
                        } else if (cardinaux[EAST]) {  /* EST */
                            cible.colonne = east;
                        } else if (!cardinaux[NORTH] && !cardinaux[SOUTH] &&
                                   !cardinaux[WEST] && !cardinaux[EAST] &&
                                   !bateaucouler) {


                            /* réinitialise la cible à la valeur originale de la cible */
                            cible = cibleOrigin;
                            cibleTemp = cible;

                            /**
                             * compteur s'incrémente de un, lorsque la boucle recommence et
                             * tous les cardinaux sont toujours FAUX. Cela garantit que nous vérifions
                             * une cellule sur les cellules adjacentes
                             */
                            north = cible.ligne - compteur;
                            cibleTemp.ligne = north;

                            /* les vérificateurs si la valeur NORD de la cible est un toucher ou est manque */
                            if (VerifierleTir(Player, cibleTemp) != -1 && north >= 0) {
                                cardinaux[NORTH] = VRAIE;
                            }

                            cibleTemp = cible;
                            south = cible.ligne + compteur;
                            cibleTemp.ligne = south;

                            /* les vérificateurs si la valeur SUD de la cible est un toucher ou est manque */
                            if (VerifierleTir(Player, cibleTemp) != -1 && south <= 15) {
                                cardinaux[SOUTH] = VRAIE;
                            }

                            cibleTemp = cible;
                            west = cible.colonne - compteur;
                            cibleTemp.colonne = west;

                            /* les vérificateurs si la valeur OUEST de la cible est un toucher ou est manque */
                            if (VerifierleTir(Player, cibleTemp) != -1 && west >= 0) {
                                cardinaux[WEST] = VRAIE;
                            }

                            cibleTemp = cible;
                            east = cible.colonne + compteur;
                            cibleTemp.colonne = east;

                            /* les vérificateurs si la valeur EST de la cible est un toucher ou un manquer */
                            if (VerifierleTir(Player, cibleTemp) != -1 && east <= 15) {
                                cardinaux[EAST] = VRAIE;
                            }

                            /**
                             * incrémente le compteur à chaque cycle, sert de complément à la façon dont
                             * plusieurs cellules à vérifier à partir de la cellule cible
                             */
                            compteur++;

                        } else {
                            /* si toutes les conditions ne sont pas remplies, il revient en mode chasse */
                            modeDescente = FAUX;
                            modeChasse = VRAIE;
                            break;
                        }

                        /* vérificateurs si le tir est un toucher ou est manque */
                        tir = VerifierleTir(Player, cible);

                    } while (tir == -1 && modeDescente == VRAIE);

                    /**
                     * cette boucle inverse les valeurs des cardinaux lorsque cela n'est pas nécessaire
                     * retire le cardinal de la pile pour donner des instructions au vérificateur
                     */
                    if (tir == 1 && modeChasse ==
                                    FAUX)/*si le tir vaut 1 ,et, dans cette boucle la sélection d'une cible n'est pas aléatoire */
                    {
                        for (i = 0; i < 4; i++) {
                            if (nager == FAUX)/* inverser  PAS les valeurs de Booleen */
                                cardinaux[i] = FAUX;

                            if (cardinaux[i] == nager)
                                nager = FAUX;
                        }
                    } else {
                        for (i = 0; i < 4; i++) {
                            if (nager == VRAIE && cardinaux[i] != FAUX) {
                                cardinaux[i] = FAUX;
                                break;
                            }
                        }
                    }

                    /* réinitialise la valeur de nager */
                    nager = VRAIE;
                }


                if (modeChasse)//ACD_mode de sélection aléatoire d'une cible si modeChasse=VRAIE
                {

                    /* réinitialise les valeurs importantes */
                    compteur = 1;
                    nager = VRAIE;
                    for (i = 0; i < 4; i++)
                        cardinaux[i] = VRAIE;

                    /* boucle qui obtient un numéro aléatoire d'une cellule comme cible */
                    do {
                        cible.ligne = Aleatoire(0, 15);
                        cible.colonne = Aleatoire(0, 15);

                        /* vérifier la cible si c'est une casse manque ou toucher */
                        tir = VerifierleTir(Player, cible);
                    } while (tir == -1);

                    /* s'il s'agit d'une touche, elle sauvegarde la valeur de la cible originale */
                    if (tir == 1)
                        cibleOrigin = cible;
                }

                if (tir == 1) {

                    /**
                     * si tous les cardinaux sont faux et qu'aucun bateau n'a été coulé,
                     * réinitialise la cible à partir de la valeur de la cible originale
                     */
                    if (!cardinaux[NORTH] && !cardinaux[SOUTH] &&
                        !cardinaux[WEST] && !cardinaux[EAST] &&
                        !bateaucouler) {
                        cible = cibleOrigin;
                    }
                    /* le mode chasse est FAUX pendant un mode cible */
                    modeChasse = FAUX;
                    modeDescente = VRAIE;
                    cibleAI = cible;

                    /**
                     * obtenir les valeurs des cellules adjacentes et s'assurer que
                     * qu'il est à l'intérieur de la grille de jeu aux poles
                     * NORD, SUD, OUEST ET EST
                     */
                    if (cardinaux[NORTH] == VRAIE) {  /* NORD */
                        north = (cibleAI.ligne - 1);
                        Grille(cardinaux, north, NORTH);
                        cibleTemp = cible;
                        cibleTemp.ligne = north;
                        if (VerifierleTir(Player, cibleTemp) == -1)
                            cardinaux[NORTH] = FAUX;
                    }

                    if (cardinaux[SOUTH] == VRAIE) {  /* SUD */
                        south = cibleAI.ligne + 1;
                        Grille(cardinaux, south, SOUTH);
                        cibleTemp = cible;
                        cibleTemp.ligne = south;
                        if (VerifierleTir(Player, cibleTemp) == -1)
                            cardinaux[SOUTH] = FAUX;
                    }

                    if (cardinaux[WEST] == VRAIE) {   /* OUEST */
                        west = cibleAI.colonne - 1;
                        Grille(cardinaux, west, WEST);
                        cibleTemp = cible;
                        cibleTemp.colonne = west;
                        if (VerifierleTir(Player, cibleTemp) == -1)
                            cardinaux[WEST] = FAUX;
                    }

                    if (cardinaux[EAST] == VRAIE) {   /* EST */
                        east = cibleAI.colonne + 1;
                        Grille(cardinaux, east, EAST);
                        cibleTemp = cible;
                        cibleTemp.colonne = east;
                        if (VerifierleTir(Player, cibleTemp) == -1)
                            cardinaux[EAST] = FAUX;
                    }
                }

                /* sauvegarde le caractère de la cellule ciblée */
                naviresymbole = Player[cible.ligne][cible.colonne].symbole;


                if (tir == 1) { /* toucher */
                    Humain.vie--;
                    IntArtificielle.toucher++;
                    system("cls");
                    Afficher(Player, AI);
                    printf("\n\tRAPPORT DE BATAILLLE :\n");
                    printf("\n\t>coordonnee: %d, %d est touche!\n\n\tAppuyez sur n'importe quelle touche pour continuer...",
                           cible.colonne, cible.ligne);
                    system("pause > nulle");


                    /* vérifier si le bateau a coulé */
                    bateaucouler = VerificationBateau(BateauaVoile, player, naviresymbole, Player, AI);


                } else {        /* manquer */
                    IntArtificielle.manquer++;//combien de fois l'IA a ete toucher, la boucle s'incrémente
                    system("cls");
                    Afficher(Player, AI);
                    printf("\n\tRAPPORT DE BATAILLE :\n");
                    printf("\n\t>coordonnee %d, %d est manque!\n\n\tAppuyez sur n'importe quelle touche pour continuer...",
                           cible.colonne, cible.ligne);
                    system("pause > nulle");

                }

                GrilledeJeu(Player, cible);
                Afficher(Player, AI);

                double HA = Humain.toucher / compteurTours;//on affiche
                double AIA = IntArtificielle.toucher / compteurTours;

                if (Humain.vie == 0 && IntArtificielle.vie == 0)//ACD_le jeu s'arrete car la bataille est nulle
                {
                    printf("\n\n\tLE JOUEUR ET L'IA N'ONT PLUS DE BATEAUX");
                    printf("\n\n\tLA BATAILLE EST NULLE");
                    printf("\n\n\tSTATS DE BATAILLE :\n\n\t");
                    printf("NOMBRE DE TOURS : %d\n\t", compteurTours);
                    printf("JOUEUR:\t\t\tIA:\n\n\t");
                    printf("touche: %d\t\t\ttouche: %d\n\t", Humain.toucher, IntArtificielle.toucher);
                    printf("manque: %d\t\t\tmanque: %d\n\t", Humain.manquer, IntArtificielle.manquer);
                    system("pause > nulle");
                    break;
                } else if (Humain.vie == 0)//ACD_le jeu s'arrete car l'IA a gagne
                {
                    printf("\n\n\tLE JOUEUR N'A PLUS DE BATEAUX\n");
                    printf("\n\tL'IA A GAGNE");
                    printf("\n\n\tSTATS DE BATAILLE :\n\n\t");
                    printf("NOMBRE DE TOURS : %d\n\t", compteurTours);
                    printf("JOUEUR:\t\t\tIA:\n\n\t");
                    printf("touche: %d\t\t\ttouche: %d\n\t", Humain.toucher, IntArtificielle.toucher);
                    printf("manque: %d\t\t\tmanque: %d\n\t", Humain.manquer, IntArtificielle.manquer);
                    system("pause > nulle");
                    break;
                } else if (IntArtificielle.vie ==
                           0)//ACD_le jeu s'arrete car l'IA a gagne,on appelle les differentes fonctions qui permettent d'affiche que le joueur a gagne
                {

                    printf("\n\n\tL'IA A PLUS DE BATEAUX\n");
                    printf("\n\tLE JOUEUR A GAGNE");
                    printf("\n\n\tSTATS DE BATAILLE :\n\n\t");
                    printf("NOMBRE DE TOURS : %d\n\t", compteurTours);
                    printf("JOUEUR:\t\t\tIA:\n\n\t");
                    printf("touche: %d\t\t\ttouche: %d\n\t", Humain.toucher, IntArtificielle.toucher);
                    printf("manque: %d\t\t\tmanque: %d\n\t", Humain.manquer, IntArtificielle.manquer);
                    system("pause > nulle");
                    break;

                }

                compteurTours++;

            }

        }
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (option == 2)//ACD_description de la fonction AIDE qui affiche les parametres du jeux
        {
            system("cls");
            printf("\n\n\t");

            printf("\t\t\tRegles du jeux:\n\n");

            printf("1. Le but du Jeu est de detruire tout les navires ennemis\n");

            printf("2. Pour cela vous dispose d une flotte de 10 navires : 1 cuirasse, 2 croiseurs, 3 destroyers et 4 sous marins\n");

            printf("3. En debut de jeu la disposition de vos navires et de celui de l ordinateur sont repartie aleatoirement\n");

            printf("4. La grille N2 montre les debat inflige a l adversaire et est revele par la touche 'V' appele le mode visible, la grille N1 est celle du joueur\n");

            printf("5. Le joueur tire en entrant les coordonnees d une case valide de l adversaire: 'E' si le tire tombe à l eau, 'T' si le navire a ete touche\n ");

            printf("6. Pour faire couler un navire il faut toucher l ensemble des cases qu il occupe\n");

            printf("7. Chaque joueur possede 4 fussees eclairant qui permet de revele a l adversaire une zone de 4*4 a partir du coin haut et gauche dont on choisi les coordonnees\n");

            printf("8. Cependant la zone reste visible la duree d un tour\n");

            printf("9. Les navires peuvent se deplacer a condition qu il ne soit pas couler et qu il ne se deplace que dans une direction VERTICALEe et HORIZONTALEe d une seule case\n");

            printf("10. A chaque tour le joueur peux quitter la partie et la reprendre en chargeant la derniere partie a partir du menu\n");

            printf("\t\t       Pour retourner au menu,appuie sur une touche");

            system("\t\tpause > nulle");
        } else if (option ==
                   3)//ACD_ description: cette option permet d'afficher les noms des personnes qui ont contribues aux projets
        {
            system("cls");
            printf("\n\n\tDeveloppeurs:");
            Color(12, 0);
            printf("\n\n\tDelphine Lereau\t\t\tAya\t\t\tNicolas\t\t\tDelphine Lereau");
            printf("\n\n\tProfesseurs:");
            Color(12, 0);
            printf("\n\n\tSegado Jean-Pierre\t\t\tRAVAUT Frederic");
            Color(15, 0);

            printf("\n\n\n\t\tAppuyez sur n'importe quelle touche pour revenir au menu principal");
            system("pause > nulle");
        } else if (option == 4)//ACD_description de la fonction QUITTER
        {
            system("cls");
            Color(12, 0);
            printf("\n\n\t\tMERCI D'AVOIR JOUER   :D\n\n\n\n\t\t");
            Color(15, 0);
            system("pause > nulle");

        } else if (option ==
                   5)//ACD_description de la fonction sauvegarder, on enregistre le fichie, si il n'y a pas de fichier alors on affiche fichier inexistant sinon on enregistre.
        {
            system("cls");
            {
                int j;
                int i;
                int lig;
                int col;

                FILE *score;                                 /*Score est de type FICHIER*/
                score = fopen("A:\\score.txt", "w");    /*j'ouvre le FICHIER*/
                if (score == NULL)                          /*Je verifie si le fichier existe*/
                {
                    printf("Fichier inexistant: score.txt\n");
                } else {
                    for (int i = 1; i <= lig; i++)                      /*J'enregistre ma solution avec les bateaux*/
                    {
                        for (int j = 1; j <= col; j++) {
                            fprintf(score, "%c");
                        }
                        fprintf(score, "\n");
                    }

                    for (i = 1; i <= lig; i++)                      /*J'enregistre ma grille de jeux*/
                    {
                        for (j = 1; j <= col; j++) {
                            fprintf(score, "%c");
                        }
                        fprintf(score, "\n");
                    }
                    fclose(score);                             /*Je ferme le fichier Score.txt*/
                    printf("Partie sauvegardee avec succes!\n");
                }
                system("PAUSE");

            }
            system("pause > nulle");
        } else {
            printf("Entrée invalide Veuillez réessayer.\n\n\t\tAppuyez sur n'importe quelle touche pour continuer..");
            system("pause > nulle");
        }

    } while (optionchar);

    return 0;
}

void Color(int couleurDuTexte, int couleurDeFond) // ACD_fonction d'affichage de couleurs
{
    HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H, couleurDeFond * 16 + couleurDuTexte);
}