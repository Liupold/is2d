#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include<GL/glut.h>
#include "ising-2d.h"

islat2d lat;
double global_H, global_J1, global_J2,
       global_h, global_T, global_beta, global_frame_time,
       global_dT;

int global_dstep, global_fc, global_dF;

int global_width = 1366, global_height = 768,
    global_row = 512, global_col = 512;

void render() {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_POINTS);
        int n = 0;

        // iterate y up to 2*pi, i.e., 360 degree
        // with small increment in angle as
        // glVertex2i just draws a point on specified co-ordinate
        for (int i = 0; i < lat.r; i ++) {
        for (int j = 0; j < lat.c; j ++) {
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

void display() {
        global_H = evolve_islat2d(lat, global_beta, global_dstep,
                        global_J1, global_J2, global_h, global_H);
        render();
        global_fc += 1;
        if ((global_fc == global_dF) && (global_dT != 0)) {
                global_T += global_dT; global_beta = (1/global_T);
                printf("Temp: %f\n\r", global_T);
                global_fc = 0;
        }
        glutTimerFunc(global_frame_time, display, 0);
}

void help() {
        printf("is2d [OPTIONS] ...\n");
        printf("\n\nSimulates ising model on a 2d grid. with help of C & OpenGL\n");
        printf("\nWriten By: Rohn Chatterjee  (https://github.com/liupold)\n\n");

        printf("-T <Float>\tSet simulation temperature. [default: 1]\n");
        printf("-j1 <Float>\tInteraction strength (left-right) [default: 1]\n");
        printf("-j2 <Float>\tInteraction strength (up-down) [default: 1]\n");
        printf("-h <Float>\tExternal Magnetic field interaction [default: 0]\n");
        printf("-dS <Integer>\tNumber of steps per frame [default: 1000000]\n");
        printf("-dT <Float>\tChange of temperature per dF [default: 0]\n");
        printf("-dF <Integer>\tFrame Spacing [default: 10]\n");
        printf("-fps <Float>\tFrames per second [default: 30]\n");
        printf("-W <Integer>\tWidth of the window [default: 1366]\n");
        printf("-H <Integer>\tHeight of the window [default: 768]\n");
        printf("-R <Integer>\tNumber of row in lattice [default: 512]\n");
        printf("-C <Integer>\tNumber of column in lattice [default: 512]\n");
}


void config_load(int argc, char** argv) {
        char* key;
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
        }
}


int main(int argc, char** argv) {
        lat = rand_islat2d(global_row, global_col);

        // hamiltons param
        global_J1 = 1; global_J2 = 1; global_h = 0;
        global_H = ising2d_H(lat, global_J1, global_J2, global_h);
        global_dstep = 1e6;
        global_T = 1;
        global_frame_time=1000/30; // 30 fps

        // change temp with step.
        global_fc = 0; global_dF = 10;
        global_dT = 0;

        if (argc >= 2) {
                config_load(argc, argv);
        }
        global_beta = 1/global_T;

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
        gluOrtho2D(0, global_row, 0, global_col);

        glutDisplayFunc(display);
        glutMainLoop();
}
