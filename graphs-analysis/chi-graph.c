#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "ising-2d.h"

double global_J1 = 1, global_J2 = 1, global_h = 0;




double Mag(islat2d lat) {
        double val=0;
        for (int i=0; i< lat.N; i++) {
                val += lat.S[i];
        }
        return val/lat.N;
}

double M_avg(islat2d lat, double beta, int MCsteps, double *H_prev) {
        double mag, M_sum=0;
        for (int mc_step=0; mc_step < MCsteps; mc_step++) {
                *H_prev = evolve_islat2d(lat, beta, lat.N,
                                global_J1, global_J2, global_h, *H_prev);
                mag = Mag(lat);
                M_sum += mag;
        }
        return (M_sum/MCsteps);
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


int main() {
        islat2d lat = rand_islat2d(100, 100);
        double H_val = ising2d_H(lat, 1, 1, 0);
        double T = 2, Tmax = 3, dT = 0.05;
        double beta = 1/T;
        H_val = evolve_islat2d(lat, beta, 10000*lat.N, global_J1, global_J2,
                        global_h, H_val);

        while (T < Tmax) {
                beta = 1/T;
                H_val = evolve_islat2d(lat, beta, 1000*lat.N, global_J1, global_J2,
                                global_h, H_val);
                printf("%f, %f\n", T, chi(lat, beta, 1000, &H_val));
                fprintf(stderr, "T: %f\n", T);
                T += dT;
        }
        free_islat2d(lat);
}
