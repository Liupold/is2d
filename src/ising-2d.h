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
double ising2d_del_H(islat2d l, int i, double J1, double J2, double h);
double ising2d_H(islat2d l, double J1, double J2, double h);
double evolve_islat2d(islat2d Lat, double beta, int steps,
                double j1, double j2, double h, double H_prev);
