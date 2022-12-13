#include <SDL2/SDL.h>
#include <unistd.h>
#include "function.h"

#define WINDOW_WIDTH 675
#define WINDOW_HEIGHT 820
#define FPS 60

#define RADIUS_BALL 10

#define TAILLEBR 32
#define ECARTX 5
#define ECARTY 5
#define ROW 7
#define NBBRICKS_PER_ROW 18

int start = 0;
int gameover = 0;
int alive = 1;

//==============PADDLE=============================================
int xpaddle = 350;
int ypaddle = 750;
int lg_paddle = 75;
int h_paddle = 15;

void drawpaddle (){
	changeColor(37, xpaddle, 233);
	drawRect(xpaddle,ypaddle,lg_paddle,h_paddle);
}

//==============BALL===============================================

int xball = 350;
int yball = 700;
double speed_ballx = 3.3;
double speed_bally = 5.2;

void drawball() {
	changeColor(253, xball, yball);
	drawCircle(xball,yball,RADIUS_BALL);
	}
	

//=============BRICKS===============================================

int tabBricks[ROW][NBBRICKS_PER_ROW];

int InittabBricks()                                                                    // initialisation du tableau des briques
{
	int i,j ;

	for(j=0 ; j<ROW ; j++){
		for(i=0; i < NBBRICKS_PER_ROW; i++){
			tabBricks[j][i] = alive;                                // avant c'était le 1 --> maintenant la variable "alive" pour dire que mes briques sont "en vie"
		}                                                                                       
	}
}

int drawBricks()                                                                      //fonction pour dessiner les briques via un tableau
{
	int x,y; 
	
	for (int j=0; j<ROW; j++){                                               // les lignes
		for (int i=0; i<NBBRICKS_PER_ROW; i++){                              // les colonnes 
			x = ECARTX +(TAILLEBR + ECARTX)*i;
			y = ECARTY + (TAILLEBR + ECARTY)*j;
			
			if  (tabBricks[j][i] == 1) {
				changeColor(250,133,40);
				if ((i==8 || i==9) && (j==0))                                                             // couleur queue de citrouille
					changeColor(9,82,40);
				if ((i==4 || i ==5 || i ==12 || i ==13) &&(j==1))                                         // couleur yeux
					changeColor(109,7,45);
				if ((i==5 || i==12) && (j==2))                                                            // couleur yeux
					changeColor(109,7,45);
				if ((i==3 || i==6 || i==8 || i==10 || i==13) && (j==4))                                   // couleur bouche
					changeColor(109,7,45);
				if ((i==4 || i==5 || i==7 || i==9 || i==11 || i==12) && (j==5))                           // couleur bouche
					changeColor(109,7,45);
					
			drawSquare(x,y,TAILLEBR);
			}	
		}
	}
}

	
// ========== COLLISSION BALL =====================================



int WallCollision ()   // pour la collision avec les bords de la fenêtre de jeu
{
	if ((xball == (0 + RADIUS_BALL)) || (xball >= ( WINDOW_WIDTH - RADIUS_BALL)))
		speed_ballx = - speed_ballx;
	
	if ((yball == (0 + RADIUS_BALL)) /*|| (yball >= (WINDOW_HEIGHT - RADIUS_BALL))*/)
		speed_bally = - speed_bally;
		
	if (yball >= (WINDOW_HEIGHT - RADIUS_BALL))
		gameover = 1;
		
}

int PaddleCollision ()  // pour la collision avec le paddle -- avec des zones d'impact et des rebonds différents en fonction de ces zones
{
	if ((yball >= (ypaddle - RADIUS_BALL)) && (xball >= xpaddle) && (xball <= (xpaddle + (lg_paddle/3))) && (yball <= (ypaddle + RADIUS_BALL + h_paddle))) { // rebond à gauche
		
		speed_bally = - speed_bally;	
			
		if (speed_ballx == 0)
			speed_ballx = speed_ballx + 2;
			else
			speed_ballx = abs(speed_ballx);
	}
	
	if ((yball >= (ypaddle - RADIUS_BALL)) && (xball > xpaddle + (lg_paddle/3)) && (xball < (xpaddle + (lg_paddle/2))) && (yball <= (ypaddle + RADIUS_BALL + h_paddle))) { //rebond au milieu
		
		speed_bally = - speed_bally;
		speed_ballx = 0 ;
	}
	
	if ((yball >= (ypaddle - RADIUS_BALL)) && (xball >= xpaddle + (lg_paddle/2)) && (xball <= (xpaddle + lg_paddle + RADIUS_BALL)) && (yball <= (ypaddle + RADIUS_BALL + h_paddle))) { //rebond à droite
		
		speed_bally = - speed_bally;
		
		if (speed_ballx == 0)
			speed_ballx = speed_ballx - 2;
			else
		speed_ballx = -abs(speed_ballx);
	}	
}


int BricksCollision()       //pour la collision avec les bricks / qui réutilise le tableau de la fonction des Bricks
{
	int x,y ;
	
	for (int j=0; j<ROW; j++){                                       
		for (int i=0; i<=NBBRICKS_PER_ROW; i++){      
		x = ECARTX +(TAILLEBR + ECARTX)*i;
		y = ECARTY + (TAILLEBR+ECARTY)*j;
		
				if ((yball <= (y + TAILLEBR  + RADIUS_BALL)) && (yball >= (y - RADIUS_BALL))&& (xball >= x) && (xball <= (x + TAILLEBR))){
					
					if  (tabBricks[j][i] == 1) {
						speed_bally = - speed_bally;
						tabBricks[j][i] = 0;}
				}
		}
	}
}



int Collision ()                                                    // collision générale de la balle
{
	WallCollision() ;
	PaddleCollision() ;
	BricksCollision();
}

// ========= MOUVEMENT BALL =========================================

int MoveBall ()    // pour faire bouger la balle et quelle rebondisse quand elle collisionne qlq chose
{
	xball = (xball - speed_ballx);
	yball = (yball - speed_bally); 
	Collision();
}


//==============GAME==============================================

void drawGame(){
    /* Ici je dessine mon jeu
     * exemple position x, y modifiés dans KeyPressed() et utilisés pour
     * pouvoir deplacer la figure à chaque boucle de gameLoop()
     */
    clear();
    drawBricks();
    drawpaddle() ;
    drawball();
    actualize();
    usleep(1000000 / FPS); // 60 images par seconde | 1000000 = 1 seconde
}
void KeyPressed(SDL_Keycode touche){
    /** @brief event.key.keysym.sym renvoi la touche appuyé
     *
     */
    switch (touche) {
        case SDLK_LEFT:                        // faire bouger le paddle à gauche quand j'appuie sur la touche left
            printf("LEFT\n");
            if (xpaddle > 0)
				xpaddle = xpaddle - 20;
            break;
        case SDLK_RIGHT:                    // faire bouger le paddle à droite quand j'appuie sur la touche right
            printf("RIGHT\n");
            if (xpaddle < (WINDOW_WIDTH - lg_paddle))
				xpaddle = xpaddle + 20;
            break;
        case SDLK_ESCAPE:
            freeAndTerminate();
            break;
        default:
            break;
    }
}
void joyButtonPressed(){
}

void gameLoop() {
    int programLaunched = 1;
	while (programLaunched == 1) {
        // Boucle pour garder le programme ouvert
        // lorsque programLaunched est different de 1
        // on sort de la boucle, donc de la fonction
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // boucle pour la gestion d'évenement
            switch (event.type) {
                /** event.type renvoi le type d'evenement qui se passe
                 * (si on appuie sur une touche, clique de souris...)
                 * en fonction du type d'évènement on à alors
                 * différentes information accessibles
                 * voir doc pour plus d'event https://wiki.libsdl.org/SDL_EventType
                 */
                case SDL_QUIT:
                    // quand on clique sur fermer la fénêtre en haut à droite
                    programLaunched = 0;
                    break;
                case SDL_MOUSEBUTTONUP:
					start = 1;
                    /* clique de la souris
                     * 
                     * event.motion.y | event.motion.x pour les positions de la souris
                     */
                    printf("position de la souris x : %d , y : %d\n", event.motion.x, event.motion.y);
                    break;
                case SDL_KEYDOWN:
                    KeyPressed(event.key.keysym.sym);
                    break;
                case SDL_JOYBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        /* coder ici pour que le code s'execute après chaque évenement
         * exemple dessiner un carré avec position int x, int y qu'on peut
         * deplacer lorsqu'on appuie sur une touche
         */
         if (gameover == 0) 
			drawGame();
			if (start == 1) 
				MoveBall();
		if (gameover == 1) {
			clear();
			sprite(157.5,290,"asset/gameover.bmp");    // arrête le jeu et affiche une image game over quand la balle touche le sol
			actualize();
		}
					
   }
}

int main(){
    /** @description 3 fonctions dans le main qui permettent de créer l'application et la maintenir ouverte :
     *  init(...) : initialiser la SDL/ fenêtre
     *  gameLoop() : boucle de jeu dans laquelle l'application reste ouverte
     *  freeAndTerminate() : quitte le programme proprement
     */
    init(WINDOW_WIDTH, WINDOW_HEIGHT);
    InittabBricks ();
    gameLoop();
    printf("Fin du programme\n");
    freeAndTerminate();
}



