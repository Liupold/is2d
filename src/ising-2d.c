#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ising-2d.h"

void disp_islat2d(islat2d A) {
        int n = 0;
        printf("\n2d ISING LATTICE: \n");
        for (int i=0; i < A.r; i++) {
                printf("|");
                for (int j=0; j < A.c; j++) {
                        printf("%2d", A.S[n]);
                        if (j < A.c - 1) {
                                printf(",\t");
                        }
                        n++;
                }
                printf("|");
                if (i < A.r - 1) {
                        printf("\n");
                }
        }
        printf("\n\n");
};


islat2d rand_islat2d(int r, int c) {
        int N = r*c;
        int8_t* S = (int8_t*)calloc(N, sizeof(int8_t));
        for (int i=0; i<N; i++) {
                if (rand()>(RAND_MAX/2)) {
                        S[i] = 1;
                } else {
                        S[i] = -1;
                }
        }

        islat2d ret_LAT = {S, r, c, N};
        return ret_LAT;
}

void islat2d_randomize(islat2d lat) {
        for (int i=0; i<lat.N; i++) {
                if (rand()>(RAND_MAX/2)) {
                        lat.S[i] = 1;
                } else {
                        lat.S[i] = -1;
                }
        }
}

void free_islat2d(islat2d Lat) {
        free(Lat.S);
        Lat.N = 0;
        Lat.r = 0;
        Lat.c = 0;
}

void save_islat2d(islat2d A, char* filename) {
        int n = 0;

        FILE *fpr = fopen(filename, "w");

        for (int i=0; i < A.r; i++) {
                for (int j=0; j < A.c; j++) {
                        fprintf(fpr, "%2d", A.S[n]);
                        if (j < A.c - 1) {
                                fprintf(fpr, "\t");
                        }
                        n++;
                }
                if (i < A.r - 1) {
                        fprintf(fpr, "\n");
                }
        }
        fprintf(fpr, "\n");
        fclose(fpr);
}

double ising2d_del_H(islat2d l, int i, double J1, double J2, double h) {
        double ret_value = 0;
        int n, s, e, w;

        n = i - l.r;
        s = i + l.r;
        e = i + 1;
        w = i -1;

        if (i % l.c == 0) {
                w = l.c + w;
        }

        if ((i + 1) % l.c == 0) {
                e = e - l.c;
        }

        if (i < l.c) {
                n = l.N + n;
        }

        if (i + l.c >= l.N) {
                s = s - l.N;
        }

        ret_value -= J1*l.S[i] * (l.S[e] + l.S[w]);
        ret_value -= J2*l.S[i] * (l.S[n] + l.S[s]);
        ret_value -= h*l.S[i];
        return 2*ret_value;
}


double ising2d_H(islat2d l, double J1, double J2, double h) {
        double ret_value = 0;
        int n, w; // iter;

        for (int i=0; i < l.N; i++) {
                n = i - l.c;
                w = i -1;

                if (i % l.c == 0) {
                        w = l.c + w;
                }

                if (i < l.c) {
                        n = l.N + n;
                }

                ret_value -= J1*(l.S[i] * l.S[w]);
                ret_value -= J2*(l.S[i] * l.S[n]);
                ret_value -= h*l.S[i];

                // iter++;
        }
        return ret_value;
}


double evolve_islat2d(islat2d Lat, double beta, int steps,
                double j1, double j2, double h, double H_prev) {
        int i;
        double H_next, H_curr = H_prev;

        for (int iter=1; iter<=steps; iter++) {
                i = rand()%(Lat.N); // choose i
                Lat.S[i] *= -1; // try fliping
                H_next = H_curr + ising2d_del_H(Lat, i, j1, j2, h);

                if (H_next <= H_curr) { // Del E < 0
                        H_curr = H_next; // finalize the flip
                } else { // Del E > 0
                        if ((double)rand() / (double)RAND_MAX
                                        < exp(-beta * (H_next - H_curr))) {
                                H_curr = H_next; // swap with (exp(-beta * del E))
                        } else {
                                Lat.S[i] *= -1; // swap back
                        }
                }
                // printf("%5.5f\r", (double)iter / (double)steps);
        }
        return H_curr;
}

