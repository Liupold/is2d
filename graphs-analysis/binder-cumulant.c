#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "ising-2d.h"

double global_J1 = 1, global_J2 = 1, global_h = 0;

double Mag(islat2d lat) {
        double val=0;
        for (int i=0; i< lat.N; i++) {
                val += lat.S[i];
        }
        return val/lat.N;
}

double binder_cumulant(islat2d lat, double beta, int MCsteps, double *H_prev) {
        double Mag_2=0, Mag_4=0, mag;
        for (int mc_step=0; mc_step < MCsteps; mc_step++) {
                *H_prev = evolve_islat2d(lat, beta, lat.N,
                                global_J1, global_J2, global_h, *H_prev);
                mag = Mag(lat);
                mag = mag * mag;

                Mag_2 += mag;
                Mag_4 += mag * mag;
        }

        Mag_2 = Mag_2/MCsteps;
        Mag_4 = Mag_4/MCsteps;
        return 1 - (Mag_4) / (3 * Mag_2 * Mag_2);
}

int main() {
        double T, Tmax = 2.7, dT = 0.05;
        islat2d lat;

        for (int k=1; k <= 10;  k++) {
        for (int r=10; r <= 60;  r+=10) {

        T = 1.8;
        int SEED = time(NULL);
        srand(SEED);
        lat = rand_islat2d(r, r);
        double H_val = ising2d_H(lat, 1, 1, 0);

        char buf[256];
        char* filename = "binder-cimulant";
        snprintf(buf, 256, "%s_%d_%d.txt", filename, lat.N, SEED);
        FILE* fptr = fopen(buf, "w");

        fprintf(stderr, "Latice:%dx%d (run-%d)\n", lat.r, lat.c, k);
        while (T < Tmax) {
                double beta = 1/T;
                H_val = evolve_islat2d(lat, beta, 10000*lat.N, global_J1, global_J2,
                                global_h, H_val);
                fprintf(fptr, "%f, %f\n", T, binder_cumulant(lat, beta, 10000, &H_val));
                fprintf(stderr, "T:%f, U:%f\n", T, binder_cumulant(lat, beta, 10000, &H_val));
                islat2d_randomize(lat);
                T += dT;
        }
        free_islat2d(lat);
        fclose(fptr);
        }}
}
