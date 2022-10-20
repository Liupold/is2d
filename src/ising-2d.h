#pragma once
#include "mtwister.h"

// Technical
struct ising_2d_latice{
        int8_t* S;
        int r, c, N;
};
# define islat2d struct ising_2d_latice

void disp_islat2d(islat2d A);
islat2d rand_islat2d(int r, int c);
void islat2d_randomize(islat2d lat);
void free_islat2d(islat2d Lat);
void save_islat2d(islat2d A, char* filename);

// Physics
int ising2d_del_H(const islat2d* l, int i, int J1, int J2, double h);
double ising2d_H(const islat2d l, int J1, int J2, double h);
double evolve_islat2d(islat2d Lat, double beta, int steps,
                int j1, int j2, double h, double H_prev, MTRand r);
