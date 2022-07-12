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

// d'autres valeurs à essayer
//	= { 0.001643, -0.82246, 0.01 };
//	= { 0.001643721971153, -0.822467633298876, 0.000000001};
//	= { 0.001643721971153, -0.822467633298876, 0.00000000002};
//	= { -0.1529, 1.0397, 0.0002 };
//	= { -0.77568377, 0.13646737, 0.0000001 };
//	= { -1.54368901269109, 0, 0.0000001 };
//	= { -0.10126, 0.95423, 0.0002 };

/////////////////////////////////////////////////////////////////////////////

// Un rectangle_t représente une partie de l'écran, c'est à dire
// les pixels entre [x1,x2] x [y1,y2]. La fonction compute_rectangle
// (dans mandel.h) va calculer le contenu d'un tel rectangle.

typedef struct { int x1, x2, y1, y2; } rectangle_t;

#include "mandel.h"

/////////////////////////////////////////////////////////////////////////////

// Simple emballage autour de compute_rectangle.
void* thread (void *ptr)
{
	rectangle_t rect = *(rectangle_t*)ptr;
	compute_rectangle(rect);
	return NULL;
}

#define NUMTHREADS 64

// On découpe l'écran dans NUMTHREADS morceaux, chaque thread en calcule un.

void compute_image ()
{
	int i;
	pthread_t threads[NUMTHREADS];
	rectangle_t rect[NUMTHREADS];

	// générer les threads, chacun avec son morceau
	for (i = 0; i < NUMTHREADS; i++)
	{
		rect[i].x1 = width*i/NUMTHREADS;
		rect[i].x2 = width*(i+1)/NUMTHREADS;
		rect[i].y1 = 0;
		rect[i].y2 = height;
		pthread_create(threads+i,NULL,thread,rect+i);
	}

	// attendre que tout le fin ait fini son travail
	for (i = 0; i < NUMTHREADS; i++)
		pthread_join(threads[i],NULL);
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
