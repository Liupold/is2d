#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <errno.h>
#include <sys/wait.h>
#include "ising-2d.h"

double global_J1 = 1, global_J2 = 1, global_h = 0;

void Align(islat2d lat) {
        for (int i=0; i< lat.N; i++) {
                lat.S[i] = 1;
        }
}

double Mag(islat2d lat) {
        double val=0;
        for (int i=0; i< lat.N; i++) {
                val += lat.S[i];
        }
        return val/lat.N;
}

double chi(islat2d lat, double beta, int MCsteps, double *H_prev) {
        double mag, M_sum=0, M_sq_sum=0;
        for (int mc_step=0; mc_step < MCsteps; mc_step++) {
                *H_prev = evolve_islat2d(lat, beta, lat.N,
                                global_J1, global_J2, global_h, *H_prev);
                mag = Mag(lat);
                M_sq_sum += pow(mag, 2);
                M_sum += mag;
        }
        return beta*(M_sq_sum/MCsteps - pow(M_sum/MCsteps, 2));
}

void make_chi_graph(char* filename, int id) {
        islat2d lat = rand_islat2d(250, 250);
        Align(lat);
        char buf[256];
        double H_val = ising2d_H(lat, 1, 1, 0);
        double T = 2, Tmax = 3, dT = 0.1;
        double beta = 1/T;
        srand(time(NULL)+id);
        printf("id: %d (start)\n", id);

        printf("Stabilizing...");
        fflush(stdout);

        snprintf(buf, 256, "%s_%d.txt", filename, id);
        FILE* fpr = fopen(buf, "w");
        if (fpr == NULL) {
                return;
        }
        H_val = evolve_islat2d(lat, beta, 10000*lat.N, global_J1, global_J2,
                        global_h, H_val);
        printf("(done)\n");

        while (T < Tmax) {
                beta = 1/T;
                H_val = evolve_islat2d(lat, beta, 1000*lat.N, global_J1, global_J2,
                                global_h, H_val);
                fprintf(fpr, "%f, %f\n", T, chi(lat, beta, 1000, &H_val));
                printf("%f\n", T);
                T += dT;
        }
        fclose(fpr);
        free_islat2d(lat);
        printf("id: %d (end)\n", id);
}

//int main(int argc, char** argv) {
int main() {
        int id = 1;
        int num_seed_avg = 20;
        int workers = 4;

        for (int i=1; i < num_seed_avg; i++) {
                if (id == 0) {
                        make_chi_graph("raw-data/chi-graph/chi_graph_", i-1);
                        break;
                } else {
                        id = fork();
                }

                if (i % workers == 0) {
                        while(wait(NULL) != -1|| errno != ECHILD);
                }
        }
}
