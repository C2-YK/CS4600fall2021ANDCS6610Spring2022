#include "cyVector.h"
#include "ObjDrawer.h"
#include <cmath>
#include "lodepng.h"
#include <string>

//debug
#include <iostream>

ObjDrawer::ObjDrawer(char const* filename, bool loadMtl) : v_loc(-1), isPerspect(true)
{
	obj.LoadFromFileObj(filename, loadMtl);

	int v_num = obj.NV();
	std::vector<Vec3<float>> vertices;
	std::vector<Vec2<float>> texC;
	std::vector<Vec3<float>> normals;
	std::vector<Vec3<float>> tangent;
	std::vector<Vec3<float>> bitangent;

	int f_num = obj.NF();
	for (int i = 0; i < f_num; ++i) {
		vertices.push_back(obj.V(obj.F(i).v[0]));
		vertices.push_back(obj.V(obj.F(i).v[1]));
		vertices.push_back(obj.V(obj.F(i).v[2]));
		normals.push_back(obj.VN(obj.F(i).v[0]));
		normals.push_back(obj.VN(obj.F(i).v[1]));
		normals.push_back(obj.VN(obj.F(i).v[2]));
		texC.push_back(Vec2<float>(obj.GetTexCoord(i, Vec3f(1, 0, 0))));
		texC.push_back(Vec2<float>(obj.GetTexCoord(i, Vec3f(0, 1, 0))));
		texC.push_back(Vec2<float>(obj.GetTexCoord(i, Vec3f(0, 0, 1))));

		Vec3<float> E1 = obj.V(obj.F(i).v[1]) - obj.V(obj.F(i).v[0]);
		Vec3<float> E2 = obj.V(obj.F(i).v[2]) - obj.V(obj.F(i).v[0]);
		Vec2<float> deltaUV1 = Vec2<float>(obj.GetTexCoord(i, Vec3f(0, 1, 0))) - Vec2<float>(obj.GetTexCoord(i, Vec3f(1, 0, 0)));
		Vec2<float> deltaUV2 = Vec2<float>(obj.GetTexCoord(i, Vec3f(0, 0, 1))) - Vec2<float>(obj.GetTexCoord(i, Vec3f(1, 0, 0)));
		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		Vec3f tan = (E1 * deltaUV2.y - E2 * deltaUV1.y) * r;
		Vec3f bitan = (E2 * deltaUV1.x - E1 * deltaUV2.x) * r;
		tan.Normalize();
		bitan.Normalize();

		tangent.push_back(tan);
		tangent.push_back(tan);
		tangent.push_back(tan);
		bitangent.push_back(bitan);
		bitangent.push_back(bitan);
		bitangent.push_back(bitan);
	}


	glUseProgram(progID);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vec3<float>), vertices.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &NB);
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vec3<float>), normals.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &TCB);
	glBindBuffer(GL_ARRAY_BUFFER, TCB);
	glBufferData(GL_ARRAY_BUFFER, texC.size() * sizeof(Vec2<float>), texC.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &Tan);
	glBindBuffer(GL_ARRAY_BUFFER, Tan);
	glBufferData(GL_ARRAY_BUFFER, tangent.size() * sizeof(Vec3<float>), tangent.data(), GL_STATIC_DRAW);
	glGenBuffers(1, &Bitan);
	glBindBuffer(GL_ARRAY_BUFFER, Bitan);
	glBufferData(GL_ARRAY_BUFFER, bitangent.size() * sizeof(Vec3<float>), bitangent.data(), GL_STATIC_DRAW);
}

void ObjDrawer::setTexUnit(GLint u)
{
	glUseProgram(progID);
	glUniform1i(glGetUniformLocation(progID, "tex"), u);
}

void ObjDrawer::setProg(GLuint prog_id)
{
	progID = prog_id;
}

void ObjDrawer::setAttrib()
{
	glUseProgram(progID);
	v_loc = glGetAttribLocation(progID, "m_pos");
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(v_loc);
	glVertexAttribPointer(v_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	tc_loc = glGetAttribLocation(progID, "texc");
	glBindBuffer(GL_ARRAY_BUFFER, TCB);
	glEnableVertexAttribArray(tc_loc);
	glVertexAttribPointer(tc_loc, 2, GL_FLOAT, GL_FALSE, 0, 0);

	n_loc = glGetAttribLocation(progID, "m_normal");
	glBindBuffer(GL_ARRAY_BUFFER, NB);
	glEnableVertexAttribArray(n_loc);
	glVertexAttribPointer(n_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	tan_loc = glGetAttribLocation(progID, "m_tangent");
	glBindBuffer(GL_ARRAY_BUFFER, Tan);
	glEnableVertexAttribArray(tan_loc);
	glVertexAttribPointer(tan_loc , 3, GL_FLOAT, GL_FALSE, 0, 0);

	bitan_loc = glGetAttribLocation(progID, "m_bitangent");
	glBindBuffer(GL_ARRAY_BUFFER, Bitan);
	glEnableVertexAttribArray(bitan_loc);
	glVertexAttribPointer(bitan_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void ObjDrawer::setNormalTex(char const* nor_path)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, nor_path);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLuint sampler = glGetUniformLocation(progID, "normal_tex");
	glUseProgram(progID);
	glUniform1i(sampler, 0);
}

void ObjDrawer::setDisplacementTex(char const* disp_path)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, disp_path);
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &disp_tex);
	glBindTexture(GL_TEXTURE_2D, disp_tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)image.data());
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	GLuint sampler = glGetUniformLocation(progID, "disp_tex");
	glUseProgram(this->progID);
	glUniform1i(sampler, 1);
}

void ObjDrawer::setMV(float rotateX, float rotateY, float rotateZ, float scale, float transformZ)
{
	glUseProgram(progID);

	Matrix4<float> m_model;
	m_model.SetRotationX(-1.57);
	Matrix4<float> m_view;
	m_view.SetRotationZYX(rotateX, rotateY, rotateZ);
	Matrix4<float> v_trans;
	v_trans.SetTranslation(Vec3f(0., 0., -50 - 10 * transformZ));
	m_view = v_trans * m_view;

	Matrix4<float> m_pres;
	m_pres.SetPerspective(1.f, 1920. / 1080., 0.000001f, 100.f);

	Matrix4<float> mv = m_view * m_model;
	Matrix4<float> v = m_view;
	mvp = m_pres * m_view * m_model;
	Matrix4<float> m = m_model;

	float sending[16];
	mvp.Get(sending);
	float mv_sending[16];
	mv.Get(mv_sending);

	float v_sending[16];
	v.Get(v_sending);

	float m_sending[16];
	m.Get(m_sending);

	float p_sending[16];
	m_pres.Get(p_sending);


	GLint m_pos = glGetUniformLocation(progID, "m");
	glUniformMatrix4fv(m_pos, 1, false, m_sending);

	GLint mv_pos = glGetUniformLocation(progID, "mv");
	glUniformMatrix4fv(mv_pos, 1, false, mv_sending);

	GLint v_pos = glGetUniformLocation(progID, "v");
	glUniformMatrix4fv(v_pos, 1, false, v_sending);

	GLint mvp_pos = glGetUniformLocation(progID, "mvp");
	glUniformMatrix4fv(mvp_pos, 1, false, sending);

	GLint p_pos = glGetUniformLocation(progID, "p");
	glUniformMatrix4fv(p_pos, 1, false, p_sending);
}

void ObjDrawer::setTesLevel(int level)
{
	GLuint tesl_pos = glGetUniformLocation(progID, "tes_level");
	glUseProgram(progID);
	glUniform1i(tesl_pos, level);
}

void ObjDrawer::setPerspect(bool isPerspect)
{
	this->isPerspect = isPerspect;
}

void ObjDrawer::resetGLProg()
{
	glUseProgram(progID);
	glDeleteProgram(progID);
}



void ObjDrawer::drawV()
{
	glUseProgram(progID);
	glDrawArrays(GL_POINTS, 0, obj.NV());
}

void ObjDrawer::drawTri()
{
	glUseProgram(progID);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(v_loc);
	glEnableVertexAttribArray(n_loc);
	glEnableVertexAttribArray(tc_loc);
	glEnableVertexAttribArray(tan_loc);
	glEnableVertexAttribArray(bitan_loc);
	glDrawArrays(GL_TRIANGLES, 0, obj.NF() * 3 * sizeof(Vec3f));
	glDisableVertexAttribArray(v_loc);
	glDisableVertexAttribArray(n_loc);
	glDisableVertexAttribArray(tc_loc);
	glDisableVertexAttribArray(tan_loc);
	glDisableVertexAttribArray(bitan_loc);
}

void ObjDrawer::drawPatches()
{
	glUseProgram(progID);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(v_loc);
	glEnableVertexAttribArray(n_loc);
	glEnableVertexAttribArray(tc_loc);
	glEnableVertexAttribArray(tan_loc);
	glEnableVertexAttribArray(bitan_loc);
	glDrawArrays(GL_PATCHES, 0, obj.NF() * 3 * sizeof(Vec3f));
	glDisableVertexAttribArray(v_loc);
	glDisableVertexAttribArray(n_loc);
	glDisableVertexAttribArray(tc_loc);
	glDisableVertexAttribArray(tan_loc);
	glDisableVertexAttribArray(bitan_loc);
}
