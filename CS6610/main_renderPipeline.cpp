#include "GL/glew.h"
#include "GL/freeglut.h"
#include "cyTriMesh.h"
#include "cyMatrix.h"
#include "cyGL.h"
#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <vector>


GLuint vao;
cy::GLSLProgram program;

cy::Matrix4f perspectiveMatix;
float rotX = 0, rotY = 0, transZ = 3, autorot = 0;
const int w = 1024;
const int h = 1024;
int _x = 0;
int _y = 0;
int totalV = 0;
bool leftButtonHold = false;
bool rightButtonHold = false;
float zdiff = 1.7;
bool test = true;

struct Buffer
{
private:
    int vertexLen;
    int texLen;
    int nLen;
    float* vertex;
    float* texCoord;
    float* normal;
    GLuint getNormalBuffer() {
        GLuint normalBuffer;
        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER,
            getSizeOfN(),
            normal,
            GL_STATIC_DRAW);
        return normalBuffer;
    }
    GLuint getVertexBuffer() {
        GLuint vertexBuffer;
        glGenBuffers(1, &vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,
            getSizeOfVertex(),
            vertex,
            GL_STATIC_DRAW);
        return vertexBuffer;
    }
    GLuint getTextureCoordinatesBuffer() {
        GLuint txcBuffer;
        glGenBuffers(1, &txcBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, txcBuffer);
        glBufferData(GL_ARRAY_BUFFER,
            getSizeOfTex(),
            texCoord,
            GL_STATIC_DRAW);
        return txcBuffer;

    }
public:
    GLuint n;
    GLuint vert;
    GLuint txc;
    Buffer(cyTriMesh mesh) {
        mesh.ComputeBoundingBox();
        cy::Vec3f max = mesh.GetBoundMax();
        cy::Vec3f min = mesh.GetBoundMin();
        cy::Vec3f shift({ -(min[0] + max[0]) / 2, -(min[1] + max[1]) / 2 , -(min[2] + max[2]) / 2 });
        cy::Vec3f size({ (max[0] - min[0]) / 2, (max[1] - min[1]) / 2, (max[2] - min[2]) / 2 });
        float maxsize = std::max(std::max(size[0], size[1]), size[2]);
        float scale = 1 / maxsize;
        std::vector<float>* vbuffer = new std::vector<float>;
        std::vector<float>* tbuffer = new std::vector<float>;
        std::vector<float>* nbuffer = new std::vector<float>;
        for (unsigned int i = 0; i < mesh.NF(); i++) {
            cy::TriMesh::TriFace f = mesh.F(i);
            cy::TriMesh::TriFace fn = mesh.FN(i);
            cy::TriMesh::TriFace ft = mesh.FT(i);
            for (unsigned int j = 0; j < 3; j++) {
                int vid = f.v[j];
                cy::Vec3f v = mesh.V(vid);
                for (unsigned int k = 0; k < 3; k++) {
                    vbuffer->push_back((v[k]+shift[k])*scale);
                }
                vid = fn.v[j];
                cy::Vec3f vn = mesh.VN(vid);
                for (unsigned int k = 0; k < 3; k++) {
                    nbuffer->push_back(vn[k]);
                }
                vid = ft.v[j];
                cy::Vec3f vt = mesh.VT(vid);
                for (unsigned int k = 0; k < 2; k++) {
                    tbuffer->push_back(vt[k]);
                }
            }
        }
        vertex = new float[vbuffer->size()];
        std::copy(vbuffer->begin(), vbuffer->end(), vertex);
        texCoord = new float[tbuffer->size()];
        std::copy(tbuffer->begin(), tbuffer->end(), texCoord);
        normal = new float[nbuffer->size()];
        std::copy(nbuffer->begin(), nbuffer->end(), normal);
        vertexLen = vbuffer->size();
        texLen = tbuffer->size();
        nLen = nbuffer->size();
        delete vbuffer;
        delete tbuffer;
        delete nbuffer;

        vert = getVertexBuffer();
        n = getNormalBuffer();
        txc = getTextureCoordinatesBuffer();
    }

    int getSizeOfVertex() {
        return vertexLen * sizeof(float);
    }

    int getSizeOfTex() {
        return texLen * sizeof(float);
    }

    int getSizeOfN() {
        return nLen * sizeof(float);
    }

    int getVertexCount() {
        return vertexLen / 3;
    }

};

cy::Matrix4f getMVP() {
    float rotZ = 0;
    cy::Matrix4f rotation({
        std::cos(rotZ) * std::cos(rotY), std::cos(rotZ) * std::sin(rotY) * std::sin(rotX) - std::sin(rotZ) * std::cos(rotX), std::cos(rotZ) * std::sin(rotY) * std::cos(rotX) + std::sin(rotZ) * std::sin(rotX), 0,
            std::sin(rotZ) * std::cos(rotY), std::sin(rotZ) * std::sin(rotY) * std::sin(rotX) + std::cos(rotZ) * std::cos(rotX), std::sin(rotZ) * std::sin(rotY) * std::cos(rotX) - std::cos(rotZ) * std::sin(rotX), 0,
            -std::sin(rotY), std::cos(rotY) * std::sin(rotX), std::cos(rotY) * std::cos(rotX), 0,
            0, 0, 0, 1
        });
    cy::Matrix4f scale;
    scale.SetScale(cy::Vec3f(transZ/3,transZ/3,transZ/3));
    return rotation*scale;
}

void displayFunc() {
    
    //Clear the viewport
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //OpenGL draw calls
    float mat[16];
    float nrm[9];
    getMVP().Get(mat);
    nrm[0] = mat[0];
    nrm[1] = mat[1];
    nrm[2] = mat[2];
    nrm[3] = mat[4];
    nrm[4] = mat[5];
    nrm[5] = mat[6];
    nrm[6] = mat[8];
    nrm[7] = mat[9];
    nrm[8] = mat[10];
    program.SetUniformMatrix4("mvp", mat);
    program.SetUniformMatrix3("normal", nrm);
    program.SetUniformMatrix4("mv", mat);
    program["textureMap"] = 0;
    glUseProgram(program.GetID());
    glDrawArrays(GL_TRIANGLES, 0, totalV);

    //Swap buffers
    glutSwapBuffers();
    
}

void keyboardFunc(unsigned char key, int x, int y) {
    //Handle keyboard input here
    switch (key) {
        case 27: //ESC
            glutLeaveMainLoop();
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
            float delta = _x > x ? -0.001 : 0.001;
            transZ *= delta + 1;
        }
        else if (leftButtonHold) {
            float deltaY = _x > x ? -0.01 : 0.01;
            rotY += deltaY;
        }
        _x = x;
    }
    else if (_y != y) {
        if (leftButtonHold) {
            float deltaX = _y > y ? -0.01 : 0.01;
            rotX += deltaX;
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

int main(int argc, char** argv) {
    //GLUT initialization
    glutInit(&argc, argv);
    createWindow();
    registerCallback();
    glewInit();//Init after create window
    //GL initialization
    glClearColor(0.3, 0.3, 0.3, 0);
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //load obj file
    cy::TriMesh mesh;
    mesh.LoadFromFileObj(argv[1]);
    Buffer buffer(mesh);
    totalV = buffer.getVertexCount();
    
    //load png file
    const char* pngFile = argv[2];
    // Load file and decode image.
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, pngFile);
    // If there's an error, display it.
    if (error != 0) {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
        return 1;
    }
    
    cyGLTexture2D tex;
    tex.Initialize();
    tex.SetImage(&image[0], 4, width, height);
    tex.BuildMipmaps();
    tex.Bind(0);
    

    program.CreateProgram();

    cy::GLSLShader vs;
    vs.CompileFile("shader.vert", GL_VERTEX_SHADER);

    cy::GLSLShader fs;
    fs.CompileFile("shader.frag", GL_FRAGMENT_SHADER);

    program.AttachShader(vs);
    program.AttachShader(fs);
    program.Link();
    program.SetAttribBuffer("vert_pos", buffer.vert, 3);
    program.SetAttribBuffer("vert_n", buffer.n, 3);
    program.SetAttribBuffer("vert_txc", buffer.txc, 2);
    //call main loop
    glutMainLoop();
    return 0;
}