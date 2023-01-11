#pragma once
#ifndef OBJ_DRAWER
#define OBJ_DRAWER


//#include <GL/gl.h>
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "cyTriMesh.h"
#include "cyMatrix.h"
#include "cyGL.h"

using namespace cy;

class ObjDrawer
{
private:
	TriMesh obj;

	GLuint*  progID;

	unsigned int  VAO;
	unsigned int  VBO;
	unsigned int  tris;
	unsigned int  NB;
	unsigned int  TCB;
	unsigned int  Tan;
	unsigned int  Bitan;

	GLuint* v_loc;
	GLuint* f_loc;
	GLuint* n_loc;
	GLuint* tc_loc;
	GLuint* tan_loc;
	GLuint* bitan_loc;

	unsigned int  tex;
	unsigned int  disp_tex;

	Matrix4<float> mvp;
	bool isPerspect;
	float camerWidthScale;

public:
	ObjDrawer(char const* filename, bool loadMtl);
	void setProg(GLuint prog_id);
	void drawV();
	void drawTri();
	void drawPatches();
	void setAttrib();
	void setNormalTex(char const* nor_path);
	void setDisplacementTex(char const* nor_path);
	void setMV(float rotateX, float rotateY, float rotateZ, float scale, float transformZ);
	void setTesLevel(int level);
	void setPerspect(bool isPerspect);
	void resetGLProg();
	void setTexUnit(GLint u);
};

#endif
