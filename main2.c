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

#define LIFEMAX 2

int start = 0;
int gameover = 0;
//int alive = 1;

//==============PADDLE=============================================
int xpaddle = 350;
int ypaddle = 750;
int lg_paddle = 120;
int h_paddle = 26;

void drawpaddle (){
	changeColor(37, xpaddle, 233);
	//drawRect(xpaddle,ypaddle,lg_paddle,h_paddle);
	sprite(xpaddle, ypaddle,"asset/santasleigh.bmp");
}

//==============BALL===============================================

struct Ball {
	double xball;
	double yball;
	double speed_ballx;
	double speed_bally;
	//char* texture; 
};

struct Ball myball = {350,700,3.5,5.5};

void drawball() 
{
	changeColor(255,255,255);
	drawCircle(myball.xball,myball.yball,RADIUS_BALL);
	//sprite(xball,yball,"asset/boule.bmp");
}

//=============BRICKS===============================================
struct Brick {                          // structure pour brick
	int pos_x ;
	int pos_x2 ;
	int pos_y ; 
	int pos_y2 ;
	int alive ;
	char* texture1;
	char* texture2;
}; 


struct Brick tabBricks[ROW][NBBRICKS_PER_ROW];                                         //déclaration d'un tableau via une structure

int InittabBricks()                                                                    // initialisation du tableau des briques
{
	int i,j ;

	for(j=0 ; j<ROW ; j++){
		for(i=0; i < NBBRICKS_PER_ROW; i++){
			tabBricks[j][i].pos_x = ECARTX +(TAILLEBR + ECARTX)*i;
			tabBricks[j][i].pos_x2 = tabBricks[j][i].pos_x + TAILLEBR;
			tabBricks[j][i].pos_y = ECARTY + (TAILLEBR + ECARTY)*j;
			tabBricks[j][i].pos_y2 = tabBricks[j][i].pos_y + TAILLEBR;
			tabBricks[j][i].alive = 1;                                // alive = 1 pour dire que mes briques sont "en vie" --> changer par 2 ou 3 pour donner plusieurs vie aux briques
			tabBricks[j][i].texture1 = "asset/brick1.bmp";
			tabBricks[j][i].texture2 = "asset/brick2.bmp";
		}                                                                                       
	}
}

int drawBricks()                                                                      //fonction pour dessiner les briques via un tableau
{	
	int j,i;
	for (j=0; j<ROW; j++){                                               // les lignes
		for (i=0; i<NBBRICKS_PER_ROW; i++){                              // les colonnes 			
			
			if  (tabBricks[j][i].alive == 1) {
				if ((j+i)%2 == 0)
					sprite(tabBricks[j][i].pos_x,tabBricks[j][i].pos_y,tabBricks[j][i].texture1);  // pour afficher des bricks 'green gift' -- pour les briques paires
				else
					sprite(tabBricks[j][i].pos_x,tabBricks[j][i].pos_y,tabBricks[j][i].texture2);  // pour afficher des bricks 'red gift'   -- pour les briques impaires
			}
		}
	}
}

	
// ========== COLLISSION BALL =====================================



int WallCollision ()   // pour la collision avec les bords de la fenêtre de jeu
{
	if ((myball.xball <= (0 + RADIUS_BALL)) || (myball.xball >= ( WINDOW_WIDTH - RADIUS_BALL)))
		myball.speed_ballx = - myball.speed_ballx;
	
	if ((myball.yball <= (0 + RADIUS_BALL)))
		myball.speed_bally = - myball.speed_bally;
		
	if (myball.yball >= (WINDOW_HEIGHT - RADIUS_BALL) /*&& (gameover < 2)*/)     // si la ball touche le sol -> gameover
	//	myball.speed_bally = - myball.speed_bally;
	//else
		gameover = 1 ;
		
}

int PaddleCollision ()  // pour la collision avec le paddle -- avec des zones d'impact et des rebonds différents en fonction de ces zones
{
	if ((myball.yball >= (ypaddle - RADIUS_BALL)) && (myball.xball >= xpaddle) && (myball.xball <= (xpaddle + (lg_paddle/5))) && (myball.yball <= (ypaddle + RADIUS_BALL + h_paddle))) { // rebond à extrémité gauche
		
		myball.speed_bally = - myball.speed_bally;	
			
		if (myball.speed_ballx == 0)
			myball.speed_ballx = myball.speed_ballx + 2;
			else
			myball.speed_ballx = abs(myball.speed_ballx);
	}
	
		if ((myball.yball >= (ypaddle - RADIUS_BALL)) && (myball.xball >= (lg_paddle/5)) && (myball.xball <= (xpaddle + ((lg_paddle/5)*2))) && (myball.yball <= (ypaddle + RADIUS_BALL + h_paddle))) { // rebond à gauche
		
		myball.speed_bally = - myball.speed_bally;	
			int BricksCollision()       //pour la collision avec les bricks / qui réutilise le tableau de la fonction des Bricks
{	
	for (int j=0; j<ROW; j++){                                       
		for (int i=0; i<=NBBRICKS_PER_ROW; i++){      
		
			if ((myball.yball <= (tabBricks[j][i].pos_y2 + RADIUS_BALL)) && (myball.yball >= (tabBricks[j][i].pos_y - RADIUS_BALL))&& (myball.xball >= tabBricks[j][i].pos_x) && (myball.xball <= tabBricks[j][i].pos_x2)){
					
				if  (tabBricks[j][i].alive == 1) {
						myball.speed_bally = - myball.speed_bally;
						tabBricks[j][i].alive--;
				}
			}
		}
	}
}
		if (myball.speed_ballx == 0)
			myball.speed_ballx = myball.speed_ballx + 2;
			else
			myball.speed_ballx = abs(myball.speed_ballx);
	}
	
	if ((myball.yball >= (ypaddle - RADIUS_BALL)) && (myball.xball > xpaddle + 48) && (myball.xball < (xpaddle + 72)) && (myball.yball <= (ypaddle + RADIUS_BALL + h_paddle))) { //rebond au milieu
		
		myball.speed_bally = - myball.speed_bally;
		myball.speed_ballx = 0 ;
	}
	
	if ((myball.yball >= (ypaddle - RADIUS_BALL)) && (myball.xball >= xpaddle + 72) && (myball.xball <= (xpaddle + lg_paddle + RADIUS_BALL)) && (myball.yball <= (ypaddle + RADIUS_BALL + h_paddle))) { //rebond à droite
		
		myball.speed_bally = - myball.speed_bally;
		
		if (myball.speed_ballx == 0)
			myball.speed_ballx = myball.speed_ballx - 2;
			else
		myball.speed_ballx = -abs(myball.speed_ballx);
	}	
}

int BricksCollision()       //pour la collision avec les bricks / qui réutilise le tableau de la fonction des Bricks
{	
	for (int j=0; j<ROW; j++){                                       
		for (int i=0; i<=NBBRICKS_PER_ROW; i++){      
		
			if ((myball.yball <= (tabBricks[j][i].pos_y2 + RADIUS_BALL)) && (myball.yball >= (tabBricks[j][i].pos_y - RADIUS_BALL))&& (myball.xball >= tabBricks[j][i].pos_x) && (myball.xball <= tabBricks[j][i].pos_x2)){
					
				if  (tabBricks[j][i].alive == 1) {
						myball.speed_bally = - myball.speed_bally;
						tabBricks[j][i].alive--;
				}
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
	myball.xball = (myball.xball - myball.speed_ballx);
	myball.yball = (myball.yball - myball.speed_bally); 
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
				xpaddle = xpaddle - 10;
            break;
        case SDLK_RIGHT:                    // faire bouger le paddle à droite quand j'appuie sur la touche right
            printf("RIGHT\n");
            if (xpaddle < (WINDOW_WIDTH - lg_paddle))
				xpaddle = xpaddle + 10;
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
			sprite(157.5,185,"asset/gameover.bmp");    // arrête le jeu et affiche une image game over quand la balle touche le sol
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



