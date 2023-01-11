#include "GL/freeglut.h"
#include <iostream>

float r = 0;
float g = 0;
float b = 0;
int framePast = 0;

int frameGap = 50;
float deltaR = 0.001;
float deltaG = 0.002;
float deltaB = 0.003;

void displayFunc() {
    //Clear the viewport
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //OpenGL draw calls
    
    //Swap buffers
    glutSwapBuffers();
}

void keyboardFunc(unsigned char key, int x, int y) {
    //Handle keyboard input here
    std::cout << frameGap;
    switch (key) {
        case 27: //ESC
            glutLeaveMainLoop();
            break;
        case 'w':
            frameGap--;
            break;
        case 's':
            frameGap++;
            break;
    }
}

void idleFunc() {
    //Handle animations here
    glClearColor(r, g, b, 0);
    framePast++;
    if (framePast > frameGap) {
        framePast = 0;
        if (r + deltaR < 1 && r + deltaR > 0) {
            r += deltaR;
        }
        else {
            deltaR = -deltaR;
        }
        if (g + deltaG < 1 && g + deltaG > 0) {
            g += deltaG;
        }
        else {
            deltaG = -deltaG;
        }
        if (b + deltaB < 1 && b + deltaB > 0) {
            b += deltaB;
        }
        else {
            deltaB = -deltaB;
        }
    }
    //Tell GLUT to redraw
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    //GLUT initialization
    glutInit(&argc, argv);

    //Create a window
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("Hello World!");

    //register callback function
    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    glutIdleFunc(idleFunc);

    //GL initialization
    glClearColor(0, 0, 0, 0);

    //call main loop
    glutMainLoop();
    return 0;
}