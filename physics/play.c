#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "../src/ising-2d.c"

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
                *H_prev = evolve_islat2d(lat, beta, 1,
                                global_J1, global_J2, global_h, *H_prev);
                mag = Mag(lat);
                M_sum += mag;
        }
        return fabs(M_sum/MCsteps);
}


int main() {
        islat2d lat = rand_islat2d(40, 40);
        double H_val = ising2d_H(lat, 1, 1, 0);
        double M1, M2;
        double T = 1;

        for (T=2.27; T >= 2.25; T-=0.001) {
        H_val = evolve_islat2d(lat, 1./T, 5000, global_J1, global_J2, 0.1, H_val);
        M1 = Mag(lat);

        H_val = evolve_islat2d(lat, 1./T, 5000, global_J1, global_J2, -0.1, H_val);
        M2 = Mag(lat);

        printf("%f, %f\n", T, (M1 - M2));
        };

        free_islat2d(lat);
}
