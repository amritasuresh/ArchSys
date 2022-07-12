#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <SDL/SDL.h>
#include <X11/Xlib.h>
#include <sys/time.h>

/////  Paramètres  //////////////////////////////////////////////////////////

// taille du fenêtre à dessiner (en pixels)
#define width 800
#define height 800

// nombre maximal d'itérations
#define maxiters 8000
// coloration lisse
#define smooth 1

// zone de coordonnées: les nombres complexes seront choisis dans
// le rectangle [x-ε,x+ε] x [y-ε,y+ε] 

typedef struct { double x,y,epsilon; } target_t;
target_t target = { -0.5, 0.0, 1.5 };

/////////////////////////////////////////////////////////////////////////////

// Un rectangle_t représente une partie de l'écran, c'est à dire
// les pixels entre [x1,x2] x [y1,y2]. La fonction compute_rectangle
// (dans mandel.h) va calculer le contenu d'un tel rectangle.

typedef struct { int x1, x2, y1, y2; } rectangle_t;

#include "mandel.h"

/////////////////////////////////////////////////////////////////////////////

// Ceci est la fonction à modifier. Elle génère le contenu de l'image en
// faisant appel à compute_rectangle qui peut en calculer une partie.
// Actuellement, la fonction n'utilise qu'un seul rectangle qui recouvre
// l'écran entier.

void compute_image ()
{
	rectangle_t rect;

	rect.x1 = 0;
	rect.x2 = width;
	rect.y1 = 0;
	rect.y2 = height;
	compute_rectangle(rect);
}

/////////////////////////////////////////////////////////////////////////////

int main ()
{
	// Configuration de l'écran avec les paramètres ci-dessus
	init_screen();

	while (1)
	{
		// Calculer le contenu de l'écran entier
		start_timer();
		compute_image();
		printf("Temps de calcul : %f secondes\n",stop_timer());

		// Taper 'q' pour terminer, sinon on change de vue
		if (wait_for_keystroke()) return 0;
	}
}
