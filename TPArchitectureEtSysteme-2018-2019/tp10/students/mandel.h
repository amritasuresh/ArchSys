// Normalement vous n'aurez pas besoin de modifier cette partie du programme.

double x_min, x_max, y_min, y_max;
double ratio_x, ratio_y;

SDL_Surface *screen;

/////  Calcul Mandelbrot  ///////////////////////////////////////////////////

// Calculer la valeur "Mandelbrot" pour la coordonnée (cx,cy).
// Valeur renvoyée :
//   -1 : (cx,cy) appartient à l'ensemble Mandelbrot (dessinée en noir)
//  >=0 : nombre d'itérations pour sortir du cercle
//		en mode lisse: un réel, sinon un entier

double test_mandel (double cx, double cy)
{
	double barrier = (smooth? 65536 : 4);
	double x = 0, xt;
	double y = 0, yt;
	int i = 0;
  
	// On itère pour (cx,cy) au plus maxiters fois.
	for(i = 0;  (x*x + y*y <= barrier) && i < maxiters; i++)
	{
		xt = x*x - y*y + cx;
		yt = 2*x*y + cy;
		x = xt;
		y = yt;
	}

	// Si l'itération n'est pas sorti du cercle après le nombre
	// maximal d'itérations on considère que (cx,cy) appartient
	// à l'ensemble Mandelbrot.
	if (i == maxiters) return -1.0;

	// Mode non lisse : on renvoie le nombre d'itérations.
	if (!smooth) return i;

	// Mode lisse : on prend en compte combien l'itération précédente
	// était déjà proche de sortir du cercle.
	double size = x*x + y*y;
	double smoothed = log(log(size)/2.0/log(256))/log(2);
	if (smoothed > 1.0) smoothed = 1.0;
	return ((double)i)-smoothed;
}

/////  Calcul de couleur pour un pixel  //////////////////////////////////////

// trouver la coordonnée correspondant à (x,y)
void find_complex (int x, int y, double *re, double *im)
{
	*re = x_min + x/ratio_x;
	*im = y_min + y/ratio_y;
}

// les couleurs à utiliser (autre que noir)
unsigned char palette[16][3] = {
	{66, 30, 15}, {25, 7, 26}, {9, 1, 47}, {4, 4, 73},
	{0, 7, 100}, {12, 44, 138}, {24, 82, 177}, {57, 125, 209},
	{134, 181, 229}, {211, 236, 248}, {241, 233, 191}, {248, 201, 95},
	{255, 170, 0}, {204, 128, 0}, {153, 87, 0}, {106, 52, 3} };

// interpoler entre deux couleurs de la palette
int mix (unsigned char c1, unsigned char c2, double r)
{
	double v1 = ((double)c1)*(1.0-r);
	double v2 = ((double)c2)*r;
	int result = (int)(v1+v2);
	return result;
}

// Calculer la couleur d'un pixel donné
void compute_pixel (int x, int y)
{
	// calculer la valeur Mandelbrot du pixel (x,y)
	double xvalue, yvalue, result;
	find_complex(x,y,&xvalue,&yvalue);
	result = test_mandel(xvalue,yvalue);

	// Convertir le résultat vers une couleur
	unsigned char color[3] = { 0,0,0 };	// défaut : noir

	if (result >= 0.0)
	{
		double base = floor(result);
		double rest = result - base;
		int i1 = (int)base % 16;
		int i2 = (int)(base+1) % 16;
		color[0] = mix(palette[i1][0],palette[i2][0],rest);
		color[1] = mix(palette[i1][1],palette[i2][1],rest);
		color[2] = mix(palette[i1][2],palette[i2][2],rest);
	}

	// Mettre à jour le pixel (en mémoire, pas encore sur l'écran)
	Uint32 pixel = SDL_MapRGB(screen->format,color[0],color[1],color[2]);
	Uint32 *pixels = (Uint32*)screen->pixels; 
	pixels[(y*screen->w) + x] = pixel;
}

/////  Calculer tous les pixels d'un rectangle sur l'écran  //////////////////

void compute_rectangle (rectangle_t r)
{
	int x,y;

	// calculer tous
	for (x = r.x1; x < r.x2; x++)
		for (y = r.y1; y < r.y2; y++)
			compute_pixel(x,y);

	// mettre à jour cette partie de l'écran
	SDL_UpdateRect(screen, r.x1, r.y1, r.x2-r.x1, r.y2-r.y1);
}

/////  Gestion de vues  /////////////////////////////////////////////////////

typedef struct tstack_t { target_t target; struct tstack_t *next; } tstack_t;

tstack_t *stack = NULL;

void target_push ()
{
	tstack_t *st = malloc(sizeof(tstack_t));
	st->target = target;
	st->next = stack;
	stack = st;
}

void target_pop ()
{
	if (stack == NULL) return;
	target = stack->target;
	tstack_t *tmp = stack->next;
	free(stack);
	stack = tmp;
}

void setup_target ()
{
	// configurer la zone ciblée
	x_min = target.x - target.epsilon;
	x_max = target.x + target.epsilon;
	y_min = target.y - target.epsilon;
	y_max = target.y + target.epsilon;
	ratio_x = width / (x_max - x_min);
	ratio_y = height / (y_max - y_min);
	printf("cible = (%lf,%lf,%lf)\n",target.x,target.y,target.epsilon);

	// effacer l'écran
	bzero(screen->pixels,width*height*sizeof(Uint32));
	SDL_Flip(screen);
}

/////  Opérations avec SDL  /////////////////////////////////////////////////

void error (char *msg)
{
	fputs(msg,stderr);
	fprintf(stderr,": %s\n",SDL_GetError());
	exit(1);
}

void init_screen ()
{
        XInitThreads();
        if (SDL_Init(SDL_INIT_VIDEO) < 0) error("Unable to init SDL");
        atexit(SDL_Quit);

	screen = SDL_SetVideoMode(width,height,32,SDL_SWSURFACE);
	if (!screen) error("Unable to set video");
        SDL_WM_SetCaption("Mandelbrot set: taper 'q' pour terminer, "
			"click gauche = zoom, click droite = unzoom",NULL);

	setup_target();
}

// renvoie 1 si l'utiliateur tape 'q', 0 s'il touche un bouton de la souris
int wait_for_keystroke ()
{
	SDL_Event event;
	while (1)
	{
		if (!SDL_WaitEvent(&event)) error("Unable to get event");
		if (event.type == SDL_KEYDOWN)
		{
			SDL_keysym key = event.key.keysym;
			if (key.sym == SDLK_q) return 1;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			SDL_MouseButtonEvent me = event.button;
			if (me.button == SDL_BUTTON_LEFT)
			{
				target_push();
				find_complex(me.x,me.y,&target.x,&target.y);
				target.epsilon *= 0.3;
				setup_target();
				return 0;
			}
			else if (me.button == SDL_BUTTON_RIGHT)
			{
				target_pop();
				setup_target();
				return 0;
			}
		}
	}
}

/////  Opérations horloge  //////////////////////////////////////////////////

struct timeval tv;

void start_timer ()
{
	gettimeofday(&tv,NULL);
}

double stop_timer ()
{
	struct timeval stop;
	gettimeofday(&stop,NULL);
	double seconds = stop.tv_sec - tv.tv_sec;
	double micros = stop.tv_usec - tv.tv_usec;
	
	return seconds + (micros/1000000.0);
}
