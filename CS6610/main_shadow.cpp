#ifndef CCRender
#define CCRender
#include "render.h"
#endif
#ifndef GL_H
#define GL_H
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "cyTriMesh.h"
#include "cyMatrix.h"
#include "cyGL.h"
#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <vector>
#endif

CC::Scene* scene = nullptr;
CC::Camera* camera = nullptr;
std::vector<CC::WorldObject*>* worldObjects = nullptr;
CC::TextureCube* environmentMap = nullptr;
int w = 1024;
int h = 1024;
bool leftButtonHold = false;
bool rightButtonHold = false;
int _x = 0;
int _y = 0;

void displayFunc() {
    if (!scene) {
        std::cout << "Display Error: scene not found";
        exit(1);
    }
    //Clear the viewport
    glClearColor(0.5, 0.5, 0.5, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    scene->Draw();
    glutSwapBuffers();
    
}

void keyboardFunc(unsigned char key, int x, int y) {
    //Handle keyboard input here
    CC::vec3 pos;
    switch (key) {
        case 27: //ESC
            glutLeaveMainLoop();
            break;
        case 'w':
            camera->cameraDirectionMove(0.05);
            pos = camera->getPosition();
            std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
            break;
        case 's':
            camera->cameraDirectionMove(-0.05);
            pos = camera->getPosition();
            std::cout << pos.x << " " << pos.y << " " << pos.z << "\n";
            break;
        case 'd':
            camera->cameraHorizontalMove(0.05);
            break;
        case 'a':
            camera->cameraHorizontalMove(-0.05);
            break;
    }
}

void mouseFunc(int button, int state, int x, int y) {
    
    switch (button) {
        case 2://right
            if (!leftButtonHold) {
                rightButtonHold = (state == 0);
            }
            break;
        case 0://left
            if (!rightButtonHold) {
                leftButtonHold = (state == 0);
            }
            break;
    }
    
}

void mouseMotionFunc(int x, int y) {
    
    if (_x != x) {
        if (rightButtonHold) {
            float delta = _x > x ? -0.2 : 0.2;
            

        }
        else if (leftButtonHold) {
            float deltaY = _x > x ? -0.2 : 0.2;
            //rotY += deltaY;
            camera->horizontalRotate(deltaY);
        }
        _x = x;
    }
    else if (_y != y) {
        if (leftButtonHold) {
            float deltaX = _y > y ? 0.05 : -0.05;
            //rotX += deltaX;
            camera->verticalRotate(deltaX);
        }
        _y = y;
    }
    
}

void idleFunc() {
    //Handle animations here
    //Tell GLUT to redraw
    glutPostRedisplay();
}

void createWindow() {
    //Create a window
    glutInitWindowSize(w, h);
    glutInitWindowPosition(100, 100);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow("CS5610");
}

void registerCallback() {
    //register callback function
    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseMotionFunc);
    glutIdleFunc(idleFunc);
}

void initializeScene() {
    camera = new CC::Camera();
    CC::MeshShader* meshShader = new CC::MeshShader();
    environmentMap = new CC::TextureCube("cubemap_posx.png", "cubemap_negx.png", "cubemap_posy.png", "cubemap_negy.png", "cubemap_posz.png", "cubemap_negz.png");
    worldObjects = new std::vector<CC::WorldObject*>();
    CC::PolygonalMesh* teapot = new CC::PolygonalMesh("teapot.obj");
    CC::Texture* bricks = new CC::Texture("bricks.png");
    CC::Light* light = new CC::Light(CC::vec3(0.2, 0.2, 0.2), CC::vec3(10, 10, 10), CC::vec3(0.9, 0.9, 0.9));
    CC::Material* defaultMaterial = new CC::Material(meshShader, 500, CC::vec3(0.95,0.95,0.95), CC::vec3(0.5, 0.5, 0.5));
    CC::RenderObject* teapotRO = new CC::RenderObject(teapot, defaultMaterial);
    CC::WorldObject* Obj1 = new CC::WorldObject(teapotRO, light, camera, CC::vec3(0,0,0), CC::vec3(1, 1, 1), 0);
    worldObjects->push_back(Obj1);
    CC::PolygonalMesh* plate = new CC::PolygonalMesh("plate.obj");
    CC::RenderObject* plateRO = new CC::RenderObject(plate, defaultMaterial);
    CC::WorldObject* Obj2 = new CC::WorldObject(plateRO, light, camera, CC::vec3(0, -0.5, 0), CC::vec3(4, 4, 4), 0);
    worldObjects->push_back(Obj2);
    scene = new CC::Scene(worldObjects, environmentMap, camera);
}

int main(int argc, char** argv) {
    //GLUT initialization
    glutInit(&argc, argv);
    createWindow();
    registerCallback();
    glewInit();//Init after create window
    std::cout << "Start Initialize Scene";
    initializeScene();
    std::cout << "Start Main Loop";
    //call main loop
    glutMainLoop();
    return 0;
}