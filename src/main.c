#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include<GL/glut.h>
#include "ising-2d.h"

islat2d lat;
double global_H, global_J1, global_J2,
       global_h, global_T, global_beta, global_frame_time,
       global_dT;

int global_dstep, global_dF, global_SEED;

int global_width = 500, global_height = 500,
    global_row = 100, global_col = 100;

void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
        int n = 0;

        // Up: Green, Down: Blue
        for (int j = 0; j < lat.r; j ++) {
        for (int i = 0; i < lat.c; i ++) {
                if (lat.S[n] == 1) {
                        glColor3f(0.0, 1.0, 0);
                } else {
                        glColor3f(0.0, 0.0, 1.0);
                }
                glVertex2i(i, j);
                n++;
        }}
        glEnd();
        glFlush();
}

void display(int id) {
        struct timespec start, end;
        double dt = global_frame_time;

        clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        global_H = evolve_islat2d(lat, global_beta, global_dstep,
                        global_J1, global_J2, global_h, global_H);
        if ((id == global_dF) && (global_dT != 0)) {
                global_T += global_dT; global_beta = (1/global_T);
                printf("Temp: %f\n\r", global_T);
                id = 0;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        render();
        double delta_ms = ((end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000);
        if (delta_ms > global_frame_time) {
                printf("Warning: Unable to maintain fps. [Reducing fps (%f)", 1000/global_frame_time);
                global_frame_time *= 2;
                global_dF *= 2;
                printf("-> (%f)]\n", 1000/global_frame_time);
        } else {
                dt -= delta_ms;
        }
        glutTimerFunc(dt, display, id+1);
}

void help() {
        printf("is2d [OPTIONS] ...\n");
        printf("\nSpin Up: Green, Spin Down: Blue\nSimulates ising model on a 2d grid. with help of C & OpenGL\n");
        printf("\nWriten By: Rohn Chatterjee  (https://github.com/liupold)\n\n");

        printf("-T <Float>\tSet simulation temperature. [default: 1]\n");
        printf("-j1 <Float>\tInteraction strength (left-right) [default: 1]\n");
        printf("-j2 <Float>\tInteraction strength (up-down) [default: 1]\n");
        printf("-h <Float>\tExternal Magnetic field interaction [default: 0]\n");
        printf("-dS <Integer>\tNumber of steps per frame [default: 100000]\n");
        printf("-dT <Float>\tChange of temperature per dF [default: 0]\n");
        printf("-dF <Integer>\tFrame Spacing [default: 120]\n");
        printf("-fps <Float>\tFrames per second [default: 60]\n");
        printf("-W <Integer>\tWidth of the window [default: 500]\n");
        printf("-H <Integer>\tHeight of the window [default: 500]\n");
        printf("-R <Integer>\tNumber of row in lattice [default: 100]\n");
        printf("-C <Integer>\tNumber of column in lattice [default: 100]\n");
        printf("-Seed <Integer>\tRandom Seed [default: Unix epoc]\n");
}


void config_load(int argc, char** argv) {
        for (int i=0; i < argc; i++) {
                if (strcmp(argv[i], "-j1") == 0){global_J1 = atof(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-j2") == 0){global_J2 = atof(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-h") == 0){global_h = atof(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-dS") == 0){global_dstep = atoi(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-T") == 0){global_T = atof(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-dT") == 0){global_dT = atof(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-dF") == 0){global_dF = atoi(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-fps") == 0){
                        global_frame_time = 1000/atof(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-help") == 0){help(); exit(0);}
                if (strcmp(argv[i], "--help") == 0){help(); exit(0);}
                if (strcmp(argv[i], "-W") == 0){global_width = atoi(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-H") == 0){global_height = atoi(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-R") == 0){global_row = atoi(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-C") == 0){global_col = atoi(argv[i+1]);i++;continue;};
                if (strcmp(argv[i], "-Seed") == 0){global_SEED = atoi(argv[i+1]);i++;continue;};
        }
}


int main(int argc, char** argv) {
        global_SEED = time(NULL);
        // hamiltons param
        global_J1 = 1; global_J2 = 1; global_h = 0;
        global_H = ising2d_H(lat, global_J1, global_J2, global_h);
        global_dstep = 1e5;
        global_T = 1;
        global_frame_time=1000/60; // 60 fps

        // change temp with step.
        global_dF = 10;
        global_dT = 0;

        if (argc >= 2) {
                config_load(argc, argv);
        }
        global_beta = 1/global_T;
        lat = rand_islat2d(global_row, global_col);
        srand(global_SEED);

        // init opengl
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
        glutInitWindowSize(global_width, global_height);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("F_TERM");

        // clean and setup coord
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glColor3f(0.0, 1.0, 0.0);
        glPointSize(5.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, global_col, global_row, 0);

        glutDisplayFunc(render);
        glutTimerFunc(0, display, 0);
        glutMainLoop();
        free_islat2d(lat);
}
