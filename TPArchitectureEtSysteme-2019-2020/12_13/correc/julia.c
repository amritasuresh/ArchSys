#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <complex.h>

#define THREADS 6

int _orbit_len = 400;
double complex _c = -0.4 - 0.6 * I;

double complex inverse(double complex w) {
    double rho  = sqrt(cabs(w - _c));
    double rwmc = creal(w - _c);
    double vtheta = atan(cimag(w - _c) / rwmc);
    if (rwmc <= 0) {
        vtheta = vtheta + M_PI;
    }
    double theta = vtheta / 2;
    if (random() % 2) { theta = theta + M_PI; }
    return rho * cexp(I * theta);
}

void orbit(complex double w0) {
    double complex w = w0;
    for (int i = 0; i < _orbit_len; i++) {
        printf("%lf %lf\n", creal(w), cimag(w));
        w = inverse(w);
    }
    pthread_exit(NULL);
}

double randf(double min, double max) {
    return min + ((float) rand() / (float) (RAND_MAX)) * (max - min);
}

int main() {
    pthread_t orbit_th[THREADS];
    for (int i = 0; i < THREADS; i++) {
        int w = randf(-1, 1) + I * randf(-1, 1);
        pthread_create(orbit_th + i, NULL, (void *(*)(void*)) orbit, &w);
    }
    for (int i = 0; i < THREADS; i++)
        pthread_join(orbit_th[i], NULL);
    exit(0);
}
