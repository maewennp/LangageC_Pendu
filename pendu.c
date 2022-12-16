#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>

#define TAILLEMAXTAB 52 												// on déclare une constante : taille maximal du tableau de lettre déjà utilisées : on met 52 au lieu de 26 car il y a des espaces entre chaque lettre
#define TAILLEMOT 50													// constante taille maximale mot pour le mot secret et dashed word
#define TAILLEPSEUDO 50													// constante taille maximale (nb lettres) du pseudo 

char letter; 															// on déclare une variable de type charactère qui se nomme lettre. cette variable est égale à la fonction getchar qui permet de retenir le charactère 1er de ce qui est saisi
char secretWord[TAILLEMOT]; 											// on déclare un tableau de caractères qui contient le mot secret à deviner --> récupéré aléatoirement dans le fichier dico.txt
char dashedWord[TAILLEMOT];												// on déclare un tableau de caractères dashdword de taille max 'TAILLEMOT' qui permettra d'afficher le mot à deviner en tirets
char players[TAILLEPSEUDO];												// on déclare un tableau de caractères players de taille max 'TAILLE PSEUDO' qui stockera les pseudo des joueurs
char tabUsedLetters[TAILLEMAXTAB]; 										// on déclare un tableau de caractères tabUserLetters de taille max 'TAILLEMAXTAB'--> permettra de stocker les lettres déjà utilisées
int win; 																// on déclare une variable globale type entier win qui permet de savoir quand le joueur gagne
int error; 																// on déclare une variable globale type entier error qui permet de comptabiliser le nombre d'erreurs du joueur tout au long de la partie et si le nb d'erreur = 8 le jeu s'arrête
int indice =0; 															// on déclare une variable globale type entier indice' à laquelle on affecte la valeur 0. Cette variable permet de n'aviguer dans les emplacements du tableau des lettres utilisées qui sont remplis
int usedletter;															// on déclare une variable globale type entier usedletter --> lettre déjà utilisée : qui fera office de booléen
int replay =1;															// on déclare une variable globale replay à laquelle on affecte 1.

//============================ ETABLIR UN NOMBRE ALEATOIRE ============================================================================================================================================================
 
int num_aleatoire() 													//fonction pour obtenir un nombre aléatoirement --> ce nb permettra d'obtenir un mot aléatoire 
{
    srand(time(NULL)); 													// régalge du random en fonction du temps. Opérateur que l'on ajoute dans l'équation du nombre aléatoire car sinon l'aléatoire est biaisé et commence toujours pas le même nombre parmis une liste donnée
    int randomnumber = rand() % 835; 										// on affecte à la variable randomnumber la fonction rand() sur 835 nombres.
    return randomnumber;												// la fonction retourne le nombre aléatoire obtenu
}
 
// ========================== RECUPERER UN MOT ALEATOIRE DANS LE DICO =================================================================================================================================
 
void random_word() 														//procédure pour obtenir un mot aléatoire
{
	FILE* dico = NULL; 													//on déclare un pointeur qui par défaut est vide/null
    dico = fopen("dico.txt", "r"); 										// le dico est égal au fichier txt dans lequel sont listés les mots
 
    if (dico != NULL){ 													// si dico est non null --> donc si l'ouverture du dico ne renvoie pas un NULL
		
		for(int i =0; i<num_aleatoire();i++) 							//pour un indice de 0 au nombre aléatoire obtenu avec la fonction précédente
			fgets(secretWord, TAILLEMOT, dico);         				// on appelle la fonction fget avec les paramètres : le secret word qu'on récupère, la taille maximale du mot et dans quel fichier on récupère
        fclose(dico); 													// on ferme le fichier après pour éviter toute erreur possible
    }
}
// ============================ VIDER MEMOIRE TAMPON ===============================================================================================================================================

void EmptyBuffer()														// on créer une procédure qui videra la mémoire tampon pour éviter de biaiser les getchars
{
    int c=0;															
    while (c != '\n' && c != EOF)
        c=getchar();
}
//==================================== PSEUDO =========================================================================================================================================================
void PlayerName()														// procédure pour récupérer et stocker le nom du joueur
{
	printf("\nPlayer's name ? \n\n");									// on affiche dans la console la question "Player's name ?"
	int i = 0;															// on déclare une variable i type entier et on lui affecte la valeur 0 
	char pseudo ; 														// on déclare une variable type caractère 'pseudo' 
	pseudo = getchar();													// pseudo récupère ce qui a été tapé par le player avec la fonction getchar
	
	while (pseudo != '\n' && i < TAILLEPSEUDO){ 						//tant que le caractère tapé par le joueur n'est pas un retour à la ligne et que le nom du joueur ne dépasse pas une certaine taille
		players[i] = pseudo;											//le pseudo devient une des valeurs du tableau players => pseudo est un des charactères composant le nom du joueur.
		pseudo = getchar(); 											//pseudo est égale à ce que le joueur à taper
		i++; }															// on icrémente pour parcourir le tableau
	
	printf("\nWelcome %s\n\n",players);             					//on affiche sur la console "welcome" et le nom du joueur qu'on récupère juste avant
}


// ====================================TABLEAU TIRETS ==================================================================================================================================================

void InitDashedWord() 											    	// procédure qui permet de déclarer le tableau du dashedWord et de lui donner la taille du secretword automatiquement via une boucle while
{
	int i=0;															// on déclare une variable i --> on lui affecte 0 pour commencer au 1er caractère du tableau & pour incrémenter.
										
	while(secretWord[i] != '\0' && secretWord[i] != '\n'){				// boucle 'tant que' pour que la taille du tableau dashedWord soit égale à celle du tableau secretWord --> tant que en parcourant le tableau secretword on ne tombe pas sur une valeur null ou un retour chariot
		dashedWord[i] = '_'; 											//on remplace chaque valeur du tableau/ lettre du mot par un '_'
		win ++;															//on incrémente la variable globale win (qui part de 0) et on lui ajoute 1 pour chaque tiret qui compose le dashedword --> comme ça, le win sera modifier automatiquement en fonction du nombre de lettre dans le secretword
		i ++;															//on incrémente pour parcourir tout le tableau 
	}
	dashedWord[i] = '\0';												//en cas de replay, si le mot précédent est plus long que le mot actuel à deviner, si on ne met pas hors de la boucle que le tableau de tiret est = \0, la fin du mot précédent s'affichera après le nouveau mot tiret à deviner.
}

// ==================================== HANGEDMAN ====================================================================================================================================================
void HangedMan() 														// procédure pour dessiner le pendu dans la console 
{
	switch(error) {														// chaque cas correspond au nombre d'erreurs faites par le joueur lorsqu'il dévine le mot
		case 1: 														// si 1 erreur, voila ce qui s'affiche dans la console.
			printf("\n\n\n\n\n\n _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n");
				break;
		case 2:															// si 2 erreurs... etc...
			printf("  |\n  | \n  |\n  |\n  |\n");
			printf(" _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n");
				break; 
		case 3:
			printf("   _____\n");
			printf("  |\n  | \n  |\n  |\n  |\n");
			printf(" _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n");
				break;
		case 4:
			printf("   _____\n");
			printf("  |     |\n  | \n  |\n  |\n  |\n");
			printf(" _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n");
				break;
		case 5:
			printf("   _____\n");
			printf("  |     |\n  |     O\n  |\n  |\n  |\n");
			printf(" _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n");
				break;
		case 6:
			printf("   _____\n");
			printf("  |     |\n  |     O\n  |    /|\\\n  |\n  |\n");
			printf(" _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n");
				break;
		case 7:
			printf("   _____\n");
			printf("  |     |\n  |     O\n  |    /|\\\n  |     |\n  |\n");
			printf(" _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n");
				break;
		case 8:
			printf("   _____\n");
			printf("  |     |\n  |     O\n  |    /|\\\n  |     |\n  |    / \\\n");
			printf(" _|_\n");
			printf("|   |_____\n");
			printf("|         |\n");
			printf("|_________|\n\n");
				break;
			
		default:
	}
	
}

// ===================================== VERIFICATION DE LETTRES DANS LE MOT SECRET =====================================================================================================================

void CheckLetters()														// procédure pour checker si la lettre est dans le mot secretword + remplacer dans le tableau tiret si la lettre est bonne
{
	int i=0; 															// décaration de la variable i qui permet d'incrémenter, à laquelle on affecte la valeur 0.
	int wincondi ;														// on déclare une variable qui va nous permettre de comparer la variable win avant et après la boucle tant que : on l'appelle wincondi
	wincondi = win;														// tout d'abord, la variable wincondi est égale à la variable win
	
	while(secretWord[i] != '\0') {										// tant qu'on ne dépasse pas la taille du secretword ...
		
			if (secretWord[i] == letter) {								//condition : si une des lettres du secretword est égale à la lettre suggérée par le joueur
				dashedWord[i] = letter; 								// alors le tiret au même emplacement de la lettre du secretword sera remplacé par la lettre suggérée
				win --; }												// on décrémente la variable globale win pour à la fin, arriver à 0 = victoire (si pas plus de 8 erreurs).									
			i++; 
		}																// on incrémente pour parcourir tout le tableau
		
	if (wincondi == win){ 												// si la variable win est inchangée, qu'elle est toujours égale à la variable wincondi --> car la lettre suggérée par le joueur n'est pas dans le secretword
		printf("WRONG!\n"); 											// on affiche dans la console le message 'WRONG !'
		error ++; 														// et on incrémente le nombre d'erreur faites par le joueur 
		HangedMan();													// on affiche dans la console le hangedman (en fontion du nombre d'erreur)
	}	
	printf("*************************************************************\n\nWord to guess : "); 			// on affiche à nouveau dans la console "Word to guess :"
	printf("%s\n",dashedWord); 											// on affiche dans la console le mot à tirets qu'il faut deviner avec (ou non) les lettres trouvées par le joueur
}

//================================= VERIFICATION SI LETTRE DEJA UTILISEE ================================================================================================================================

void InittabUsedLetters() 												// j'initialise le tableau des lettres utilisées 
{
	for(int i=0; i< TAILLEMAXTAB; i++){									// boucle parcourant le tableau de 0 à la taille maximum du tableau : nb de lettres dans l'alphabet x 2 : 52
		tabUsedLetters[i] = ' '; 										// donc ici j'initialise le tableau usedletters en mettant des espaces dans toutes les "cases" du tableau. Le but est que lors de
	}																	//    l'affichage des lettres déjà utilisées dans la console, il y ait un espace entre chaque lettre pour que l'affichage soit plus propre
}


void CheckSuggestion() 													//procédure qui va permettre de checker si les lettres proposées par le joueur ont déjà été proposées ou non
{
	usedletter = 0; 													// on a définit la variable en global et on lui affecte la valeur 0. Cette variable se modifie en 1 lorsqu'une lettre est trouvée dans le tableau.
	for( int i =0; i < indice; i++){ 									// boucle pour parcourir le tableau tant que l'indice i ne dépasse pas l'indice 'indice'
		if (letter == tabUsedLetters[i]){								// si la lettre se retrouve dans le tableau des lettres déjà utilisées 
			usedletter = 1;												// la variable usedletter change de valeur à 1. 
			i++; 														// on icrémente l'indice i une fois de plus pour parcourir les cases paires (j'aurais dû mettre i+2 au lieu de i++ dans le for
		}
	}
	printf("Here's the letters already used : %s\n", tabUsedLetters);   // affiche après chaque tentative toutes les lettres déjà jouées par le joueur
}

void AddLetter() 														// procédure pour ajouter la lettre jouée dans le tableau des lettres utilisées
{	
	if(indice %2 == 0) 													//si l'indice 'indice' est pair (l'indice 'indice' représente les "cases" du tableau qui sont remplies
		tabUsedLetters[indice] = letter;								//donc dans les emplacements paires, on ajoute la lettre qui a été jouée.
	indice = indice + 2;												// on incrémente 2 par 2 --> pour ajouter dans les emplacements paires
}																		// ==>> le but est d'ajouter une lettre dans le tableau des lettres utilisées, puis laisser un espace, puis une lettre etc... pour que lors de l'affichage dans la console ce soit plus propre

//===================================== PLAY =============================================================================================================================================================
void Play()  												 			// procédure play --> le jeu
{
		printf("\nPlease, suggest a letter :\n"); 						// on affiche dans la console "proposez une lettre :" 
		letter = getchar();  											// la variable lettre est égale à la fonction getchar qui permet de retenir le 1er caractère de ce qui est saisi
		printf("\nYou entered : "); 									// on affiche dans la console "vous avez saisi :" 
		putchar(letter);												// cette fonction putchar est le printf lié à la fonction getchar
		printf("\n");													// printf de retour à la ligne pour que la lettre saisi ne s'affiche pas sur la même ligne que le nom d'utilisateur dans la console	
		if ((letter >= 97) && (letter <= 122)){							// condition : on check la lettre seulement si le caractère fait parti de l'alphabet
			CheckSuggestion(); 											// on appelle la procédure CheckSuggestion pour voir si la lettre a déjà été utilisée ou non
				if (usedletter == 0){ 									// si elle n'a pas été utilisée (qu'elle n'est pas présente dans le tableau des lettres utilisées)
					CheckLetters();										// on appelle la fonction (procédure) pour checker si la lettre tapée par le joueur fait parti du mot. si oui remplacer le tiret par la lettre
					AddLetter();} 										// on ajoute la lettre au tableau des lettres déjà utilisées.
				else
				printf("\nLetter already entered !\n");					// si la lettre fait déjà partie du tableau des lettres déjà utilisées : on renvoit ce message
		}
		else 															// sinon, si ça ne fait pas parti de l'alphabet (des lettres en mminuscules)...
			printf("\nThis is not a letter !\n"); 						// on affiche dans la console " ce n'est pas une lettre !" 
		
		EmptyBuffer(); 													// on vide la mémoire tampon (pour éviter que l'ordinateur, lorsque le joueur tape sur la touche entrée pour soumettre une lettre, pense que c'est un autre caractère tapé par le joueur.
}

//=================================== END OF THE GAME ==================================================================================================================================================

void EndGame()
{
	if(win == 0)																				// si la variable win redescent à 0
		printf("\nBRAVO %s ! You found the word to guess ! It was : %s\n",players,secretWord);  // alors on affiche dans la console le message de gagne
	else if(error == 8)	 																		// si le joueur réaliser 8 erreurs 
		printf("\nSorry %s... You lost ! The word to guess was : %s\n",players,secretWord); 	// alors on affiche dans la console le message de perte.
}

//=================================== REPLAY ? ===========================================================================================================================================================

int Replay() 															// fonction replay pour proposer au joueur de rejouer ou non à la fin de la partie
{
	printf("\n Try again ?  (y/n) \n");									// on affiche dans la console try again yes or no ?
	char response = getchar(); 											//on déclare une variable "response" qui sera égale au caractère tapé par le joueur
	EmptyBuffer(); 														// on vide la mémoire tampon
	while(response != 'y' && response != 'n') {							// tant que la réponse est différente de y et n
		printf("\nI didn't undertand : y or n ?\n"); 					// on affiche I didn't understand.... dans la console
		response = getchar();											// le jeu attend un caractère / une lettre --> le jeu demandera si y ou n le joueur veut rejouer tant qu'il n'aura pas tapé un 'y' ou un 'n'
		EmptyBuffer(); 													// on vide la mémoire tampon
	}
	if(response == 'y')													// si le joueur tape un y
		return 1;														// la fonction replay retourne un 1 --> et la boucle du jeu se réalise tant que replay = 1
	else if (response == 'n') {											// si le joueur tape un 'n'
		system("sl");													// petite fonction bonus téléchargé dans le terminal pour afficher un petit train
		return 0;														// la fonction retourne un 0 --> le jeu s'arrête
	}
}

//==================================== MAIN ==============================================================================================================================================================

int main() 
{  
	while (replay == 1){
		win = 0; 														// on affecte la valeur 0 à la variable win --> celle ci se modifiera en fonction du nombre de lettres du mot à deviner
		error = 0;														// on affecte la valeur 0 à la variable error --> pour que le joueur commence la partie avec 0 erreur au début
	
		PlayerName();  													// on appelle la procédure PlayerName pour que, avant de commencer le jeu, le joueur doive saisir son pseudo.
		random_word();													// le programme choisit un mot aléatoirement dans le fichier dico.txt
		printf("Here's the word to guess :\n"); 						// on affiche dans la console le mot à deviner mais en tiret
		InitDashedWord();												// initialise le tableau à tirets --> pour donner le mot à tiret à faire deviner
		InittabUsedLetters();											// initialise le tableau des lettres déjà utilisées en le remplissant d'espaces avant de lancer le jeu
		printf("%s\n",dashedWord);  									// on affiche dans la console le mot à tirets

		while ((win != 0) && (error < 8)){				   				// on boucle la procédure 'play' tant que win est différent de 0 et error est inférieur à 8 (8 est le max de tentatives échouées possibles)
			Play(); 													// on appelle la procédure Play qui continuera tant que je joueur n'a pas gagné ou perdu
		}
		EndGame(); 														// on appelle la procédure EndGame pour les conditions de fin du jeu
		replay = Replay();												//la variable replay est égale au return de la fonction Replay	
	}
}









