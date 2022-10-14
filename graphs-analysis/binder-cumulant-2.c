#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

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

void one_seed_loop(int ith) {
        double T, Tmax = 2.4, dT = 0.02;
        islat2d lat;

        int SEED = time(NULL) + ith;
        srand(SEED);

        for (int r=10; r <= 60;  r+=10) {
        T = 2;
        lat = rand_islat2d(r, r);
        double H_val = ising2d_H(lat, 1, 1, 0);

        char buf[256];
        char* filename = "binder-cimulant-2";
        snprintf(buf, 256, "%s_%d_%d.txt", filename, lat.N, SEED);
        FILE* fptr = fopen(buf, "w");

        //fprintf(stderr, "Latice:%dx%d (run-%d)\n", lat.r, lat.c, ith);
        while (T < Tmax) {
                double beta = 1/T;
                H_val = evolve_islat2d(lat, beta, 10000*lat.N, global_J1, global_J2,
                                global_h, H_val);
                fprintf(fptr, "%f, %f\n", T, binder_cumulant(lat, beta, 10000, &H_val));
                //fprintf(stderr, "T:%f, U:%f\n", T, binder_cumulant(lat, beta, 10000, &H_val));
                islat2d_randomize(lat);
                T += dT;
        }
        free_islat2d(lat);
        fclose(fptr);
        fprintf(stderr, "Latice:%dx%d (run-%d) [DONE!]\n", lat.r, lat.c, ith);
        }
}

int main() {
        int id = 1;
        int workers = 7;
        int num_seed_avg = 14;

        for (int i=1; i < num_seed_avg; i++) {
                if (id == 0) {
                        one_seed_loop(i-1);
                        break;
                } else {
                        id = fork();
                }

                if (i % workers == 0) {
                        while(wait(NULL) != -1|| errno != ECHILD);
                }
        }
}
