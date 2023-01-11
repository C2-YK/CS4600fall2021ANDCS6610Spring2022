#ifndef CCRender
#define CCRender
#include "render.h"
#endif
using namespace CC;

void getErrorInfo(unsigned int handle)
{
    int logLen;
    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0)
    {
        char* log = new char[logLen];
        int written;
        glGetShaderInfoLog(handle, logLen, &written, log);
        printf("Shader log:\n%s", log);
        delete log;
    }
}
void checkLinking(unsigned int program)
{
    int OK;
    glGetProgramiv(program, GL_LINK_STATUS, &OK);
    if (!OK)
    {
        printf("Failed to link shader program!\n");
        getErrorInfo(program);
    }
}

//mat4
mat4::mat4() {
    m[0] = 0; m[1] = 0; m[2] = 0; m[3] = 0;
    m[4] = 0; m[5] = 0; m[6] = 0; m[7] = 0;
    m[8] = 0; m[9] = 0; m[10] = 0; m[11] = 0;
    m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 0;
}

mat4::mat4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) {
    m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
    m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
    m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
    m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
}

mat4::mat4(const float* m) {
    for (unsigned int i = 0; i < 16; i++) {
        this->m[i] = m[i];
    }
}

mat4::mat4(const mat4& other) {
    for (unsigned int i = 0; i < 16; i++) {
        m[i] = other.m[i];
    }
}

mat4& mat4::operator=(mat4 other) {
    std::swap(m, other.m);
    return *this;
}

mat4 mat4::operator*(const mat4& right)
{
    mat4 result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int index = i * 4 + j;
            result.m[index] = 0;
            for (int k = 0; k < 4; k++) {
                int index_ik = i * 4 + k;
                int index_kj = k * 4 + j;
                result.m[index] += m[index_ik] * right.m[index_kj];
            }
        }
    }
    return result;
}

//vec4
vec4::vec4(float x = 0, float y = 0, float z = 0, float w = 1)
{
    v[0] = x; v[1] = y; v[2] = z; v[3] = w;
}
vec4::vec4(const vec4& other) {
    v[0] = other.v[0];
    v[1] = other.v[1];
    v[2] = other.v[2];
    v[3] = other.v[3];
}
vec4& vec4::operator=(vec4 other) {
    std::swap(v, other.v);
    return *this;
}

vec4 vec4::operator*(const mat4& mat)
{
    vec4 result;
    for (int j = 0; j < 4; j++)
    {
        result.v[j] = 0;
        for (int i = 0; i < 4; i++) {
            int index = i * 4 + j;
            result.v[j] += v[i] * mat.m[index];
        }
    }
    return result;
}

vec4 vec4::operator+(const vec4& vec)
{
    vec4 result(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2], v[3] + vec.v[3]);
    return result;
}

//vec2
vec2::vec2(const vec2& other) {
    x = other.x;
    y = other.y;
}
vec2& vec2::operator=(vec2& other) {
    std::swap(x, other.x);
    std::swap(y, other.y);
    return *this;
}
vec2 vec2::operator+(const vec2& v)
{
    return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator*(float s)
{
    return vec2(x * s, y * s);
}

float vec2::length() { return sqrt(x * x + y * y); }

//vec3
vec3::vec3(const vec3& other) {
    x = other.x;
    y = other.y;
    z = other.z;
}
vec3& vec3::operator=(vec3 other) {
    std::swap(x, other.x);
    std::swap(y, other.y);
    std::swap(z, other.z);
    return *this;
}
vec3 vec3::operator+(const vec3& v) { return vec3(x + v.x, y + v.y, z + v.z); }

vec3 vec3::operator-(const vec3& v) { return vec3(x - v.x, y - v.y, z - v.z); }

vec3 vec3::operator*(float s) { return vec3(x * s, y * s, z * s); }

vec3 vec3::operator/(float s) { return vec3(x / s, y / s, z / s); }

float vec3::length() { return sqrt(x * x + y * y + z * z); }

vec3 vec3::normalize() { return *this / length(); }

vec3 vec3::cross(const vec3& right) {
    return vec3(this->y * right.z - this->z * right.y, this->z * right.x - this->x * right.z, this->x * right.y - this->y * right.x);
}

Camera::Camera() {
    position = vec3(0.0, 0.0, 2.0);
    target = vec3(0.0, 0.0, 0.0);
    up = vec3(0.0, 1.0, 0.0);
    fov = PI / 4.0; asp = 1.0; n = 0.01; f = 10.0;
}

mat4 Camera::getViewMatrix() {
    vec3 zAxis = (position - target).normalize();
    vec3 xAxis = up.cross(zAxis).normalize();
    vec3 yAxis = zAxis.cross(xAxis);
    //return translation * orientation matrix
    return
        mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            -position.x, -position.y, -position.z, 1.0f) *
        mat4(
            xAxis.x, yAxis.x, zAxis.x, 0.0f,
            xAxis.y, yAxis.y, zAxis.y, 0.0f,
            xAxis.z, yAxis.z, zAxis.z, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
}
mat4 Camera::getProjectionMatrix(){
    float sy = 1 / tan(fov / 2);
    return mat4(
                sy / asp, 0.0f, 0.0f, 0.0f,
                0.0f, sy, 0.0f, 0.0f,
                0.0f, 0.0f, -(n + f) / (f - n), -1.0f,
                0.0f, 0.0f, -2 * n * f / (f - n), 0.0f);
}
mat4 Camera::getInverseViewMatix() {
    vec3 zAxis = (position - target).normalize();
    vec3 xAxis = up.cross(zAxis).normalize();
    vec3 yAxis = zAxis.cross(xAxis);
    return mat4(
        xAxis.x, xAxis.y, xAxis.z, 0.0f,
        yAxis.x, yAxis.y, yAxis.z, 0.0f,
        zAxis.x, zAxis.y, zAxis.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}
mat4 Camera::getInverseProjectionMatrix() {
    float sy = 1 / tan(fov / 2);
    return mat4(
        asp / sy, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0 / sy, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, (n - f) / 2.0 / n / f,
        0.0f, 0.0f, -1.0f, (n + f) / 2.0 / n / f);
}
vec3 Camera::getCameraDirection() {
    return (target - position).normalize();
}

void Camera::cameraDirectionMove(float delta) {
    vec3 w = getCameraDirection();

    position = position + w * delta;
    target = target + w * delta;
}

void Camera::cameraHorizontalMove(float delta) {
    vec3 w = getCameraDirection();
    vec3 v = up.normalize();
    vec3 u = w.cross(v);
    position = position + u * delta;
    target = target + u * delta;
}

void Camera::horizontalRotate(float delta) {
    vec3 w = (target - position);
    float l = w.length();
    w = w.normalize();
    vec3 v = up.normalize();
    vec3 u = w.cross(v);

    float alpha = delta * PI / 180.0;

    target = position + (w * cos(alpha) + u * sin(alpha)) * l;
}

void Camera::verticalRotate(float delta) {
    vec3 w = (target - position);
    float l = w.length();
    w = w.normalize();
    vec3 v = up.normalize();
    vec3 u = w.cross(v);
    u = vec3(u.x, 0.0, u.z).normalize();
    float alpha = delta * PI / 180.0;
    target = position + (w * cos(alpha) + v * sin(alpha)) * l;
    up = u.cross(getCameraDirection());
    std::cout << up.x << " " << up.y << " " << up.z << "\n";
}

void Camera::UploadAttributes(Shader* shader) {
    shader->UploadCameraPosition(position);
}

//shader
Shader::Shader() {
    shaderProgram = new cy::GLSLProgram();
}
Shader::~Shader() {
    if (shaderProgram) {
        shaderProgram->Delete();
    }
}
void Shader::Use() {
    if (shaderProgram) {
        glUseProgram(shaderProgram->GetID());
    }
}
MeshShader::MeshShader() {
    shaderProgram->CreateProgram();
    cy::GLSLShader vs;
    vs.CompileFile("MeshShader.vert", GL_VERTEX_SHADER);
    cy::GLSLShader fs;
    fs.CompileFile("MeshShader.frag", GL_FRAGMENT_SHADER);
    shaderProgram->AttachShader(vs);
    shaderProgram->AttachShader(fs);
    shaderProgram->Link();
}
void MeshShader::UploadSamplerID()
{
    int samplerUnit = 0;
    int location = glGetUniformLocation(shaderProgram->GetID(), "textureMap");
    glUniform1i(location, samplerUnit);
    glActiveTexture(GL_TEXTURE0 + samplerUnit);
}


void MeshShader::UploadShadowMapID() {
    int samplerUnit = 2;
    int location = glGetUniformLocation(shaderProgram->GetID(), "shadowMap");
    glUniform1i(location, samplerUnit);
    glActiveTexture(GL_TEXTURE0 + samplerUnit);
}
/*
void MeshShader::UploadSamplerCubeID() {
    int samplerCube = 1;
    int location = glGetUniformLocation(shaderProgram->GetID(), "environmentMap");
    glUniform1i(location, samplerCube);
    glActiveTexture(GL_TEXTURE0 + samplerCube);
}
*/
void MeshShader::UploadM(mat4& M) {
    shaderProgram->SetUniformMatrix4("m", M.m);
}
void MeshShader::UploadInvM(mat4& InvM) {
    shaderProgram->SetUniformMatrix4("invM", InvM.m);
}
void MeshShader::UploadMVP(mat4& MVP) {
    shaderProgram->SetUniformMatrix4("mvp", MVP.m);
}
void MeshShader::UploadLightProjectionMatrix(mat4& LP) {
    shaderProgram->SetUniformMatrix4("lp", LP.m);
}
void MeshShader::UploadMaterialAttributes(bool hasTexture, float specular_alpha, vec3 specular_color, vec3 diffuse_color) {
    hasTexture ? shaderProgram->SetUniform("hasTexture", (float)1.0) : shaderProgram->SetUniform("hasTexture", (float)-1.0);
    shaderProgram->SetUniform("specular_alpha", specular_alpha);
    shaderProgram->SetUniform("specular_color", specular_color.x, specular_color.y, specular_color.z);
    shaderProgram->SetUniform("diffuse_color", diffuse_color.x, diffuse_color.y, diffuse_color.z);
}
void MeshShader::UploadLightAttributes(vec3 ambient, vec3 light_pos, vec3 light_color) {
    shaderProgram->SetUniform("ambient", ambient.x, ambient.y, ambient.z);
    shaderProgram->SetUniform("light_pos", light_pos.x, light_pos.y, light_pos.z);
    shaderProgram->SetUniform("light_color", light_color.x, light_color.y, light_color.z);
}
void MeshShader::UploadCameraPosition(vec3 camera_position) {
    shaderProgram->SetUniform("camera_position", camera_position.x, camera_position.y, camera_position.z);
}

ReflectiveShader::ReflectiveShader() {
    shaderProgram->CreateProgram();
    cy::GLSLShader vs;
    vs.CompileFile("ReflectiveShader.vert", GL_VERTEX_SHADER);
    cy::GLSLShader fs;
    fs.CompileFile("ReflectiveShader.frag", GL_FRAGMENT_SHADER);
    shaderProgram->AttachShader(vs);
    shaderProgram->AttachShader(fs);
    shaderProgram->Link();
}
void ReflectiveShader::UploadSamplerID() {
    int samplerUnit = 0;
    int location = glGetUniformLocation(shaderProgram->GetID(), "textureMap");
    glUniform1i(location, samplerUnit);
    glActiveTexture(GL_TEXTURE0 + samplerUnit);
}
void ReflectiveShader::UploadSamplerCubeID() {
    int samplerCube = 1;
    int location = glGetUniformLocation(shaderProgram->GetID(), "environmentMap");
    glUniform1i(location, samplerCube);
    glActiveTexture(GL_TEXTURE0 + samplerCube);
}
void ReflectiveShader::UploadShadowMapID() {
    int samplerUnit = 2;
    int location = glGetUniformLocation(shaderProgram->GetID(), "shadowMap");
    glUniform1i(location, samplerUnit);
    glActiveTexture(GL_TEXTURE0 + samplerUnit);
}
void ReflectiveShader::UploadM(mat4& M) {
    shaderProgram->SetUniformMatrix4("m", M.m);
}
void ReflectiveShader::UploadInvM(mat4& InvM) {
    shaderProgram->SetUniformMatrix4("invM", InvM.m);
}
void ReflectiveShader::UploadMVP(mat4& MVP) {
    shaderProgram->SetUniformMatrix4("mvp", MVP.m);
}
void ReflectiveShader::UploadMaterialAttributes(bool hasTexture, float specular_alpha, vec3 specular_color, vec3 diffuse_color) {
    hasTexture ? shaderProgram->SetUniform("hasTexture", (float)1.0) : shaderProgram->SetUniform("hasTexture", (float)-1.0);
    shaderProgram->SetUniform("specular_alpha", specular_alpha);
    shaderProgram->SetUniform("specular_color", specular_color.x, specular_color.y, specular_color.z);
    shaderProgram->SetUniform("diffuse_color", diffuse_color.x, diffuse_color.y, diffuse_color.z);
}
void ReflectiveShader::UploadLightAttributes(vec3 ambient, vec3 light_pos, vec3 light_color) {
    shaderProgram->SetUniform("ambient", ambient.x, ambient.y, ambient.z);
    shaderProgram->SetUniform("light_pos", light_pos.x, light_pos.y, light_pos.z);
    shaderProgram->SetUniform("light_color", light_color.x, light_color.y, light_color.z);
}
void ReflectiveShader::UploadCameraPosition(vec3 camera_position) {
    shaderProgram->SetUniform("camera_position", camera_position.x, camera_position.y, camera_position.z);
}

ShadowShader::ShadowShader() {
    shaderProgram->CreateProgram();
    cy::GLSLShader vs;
    vs.CompileFile("ShadowShader.vert", GL_VERTEX_SHADER);
    cy::GLSLShader fs;
    fs.CompileFile("ShadowShader.frag", GL_FRAGMENT_SHADER);
    shaderProgram->AttachShader(vs);
    shaderProgram->AttachShader(fs);
    shaderProgram->Link();
}

void ShadowShader::UploadM(mat4& M)
{
    shaderProgram->SetUniformMatrix4("m", M.m);
}

void ShadowShader::UploadVP(mat4& VP)
{
    shaderProgram->SetUniformMatrix4("vp", VP.m);
}

void ShadowShader::UploadLightAttributes(vec3 ambient, vec3 light_pos, vec3 light_color)
{
    shaderProgram->SetUniform("ambient", ambient.x, ambient.y, ambient.z);
    shaderProgram->SetUniform("light_pos", light_pos.x, light_pos.y, light_pos.z);
    shaderProgram->SetUniform("light_color", light_color.x, light_color.y, light_color.z);
}

void ShadowShader::UploadLightProjectionMatrix(mat4& LP) {
    shaderProgram->SetUniformMatrix4("lp", LP.m);
}

EnvironmentShader::EnvironmentShader() {
    shaderProgram->CreateProgram();
    cy::GLSLShader vs;
    vs.CompileFile("EnvironmentShader.vert", GL_VERTEX_SHADER);
    cy::GLSLShader fs;
    fs.CompileFile("EnvironmentShader.frag", GL_FRAGMENT_SHADER);
    shaderProgram->AttachShader(vs);
    shaderProgram->AttachShader(fs);
    shaderProgram->Link();
}

void EnvironmentShader::UploadSamplerCubeID() {
    int samplerCube = 1;
    int location = glGetUniformLocation(shaderProgram->GetID(), "environmentMap");
    glUniform1i(location, samplerCube);
    glActiveTexture(GL_TEXTURE0 + samplerCube);
}

void EnvironmentShader::UploadViewDirMatrix(mat4& viewDirMatrix) {
    shaderProgram->SetUniformMatrix4("viewDirMatrix", viewDirMatrix.m);
}

Texture::Texture(const std::string& inputFileName) {
    std::vector<unsigned char> data;
    unsigned width, height;
    unsigned error = lodepng::decode(data, width, height, inputFileName);
    if (error != 0) {
        std::cout << "load texture error " << error << ": " << lodepng_error_text(error) << std::endl;
        exit(1);
    }
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, textureId);
}
TextureCube::TextureCube(
    const std::string& inputFileName0, const std::string& inputFileName1, const std::string& inputFileName2,
    const std::string& inputFileName3, const std::string& inputFileName4, const std::string& inputFileName5) {
    std::string filename[6];
    filename[0] = inputFileName0; filename[1] = inputFileName1; filename[2] = inputFileName2;
    filename[3] = inputFileName3; filename[4] = inputFileName4; filename[5] = inputFileName5;
    glGenTextures(1, &textureId); glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
    for (int i = 0; i < 6; i++) {
        std::vector<unsigned char> data;
        unsigned width, height;
        unsigned error = lodepng::decode(data, width, height, filename[i]);
        if (error != 0) {
            std::cout << "load texture cube error " << error << ": " << lodepng_error_text(error) << std::endl;
            exit(1);
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void TextureCube::Bind() { 
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId); 
}

ShadowMap::ShadowMap() {
    depthBuffer.Initialize();
}

void ShadowMap::BindBuffer() {
    depthBuffer.Bind();
}
void ShadowMap::UnbindBuffer() {
    depthBuffer.Unbind();
}
void ShadowMap::Bind() {
    glBindTexture(GL_TEXTURE_2D, depthBuffer.GetTextureID());
    //depthBuffer.BindTexture();
}

Material::Material(Shader* shader, float specular_alpha, vec3 specular_color, vec3 diffuse_color, Texture* texture, TextureCube* environmentMap) {
    this->shader = shader;
    this->specular_alpha = specular_alpha;
    this->specular_color = specular_color;
    this->diffuse_color = diffuse_color;
    this->texture = texture;
    this->environmentMap = environmentMap;

}

void Material::UploadAttributes() {
    if (texture) {
        shader->UploadSamplerID();
        texture->Bind();
        shader->UploadMaterialAttributes(true, specular_alpha, specular_color, diffuse_color);
    }
    else {
        shader->UploadMaterialAttributes(false, specular_alpha, specular_color, diffuse_color);
    }
    if (environmentMap) {
        shader->UploadSamplerCubeID();
        environmentMap->Bind();
    }
    if (shadowMap) {
        shader->UploadShadowMapID();
        shadowMap->Bind();
    }
}

Mesh::Mesh() {
	glGenVertexArrays(1, &vao);
}

PolygonalMesh::PolygonalMesh(const char* filename)
{
    cyTriMesh mesh;
    mesh.LoadFromFileObj(filename);
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
                vbuffer->push_back((v[k] + shift[k]) * scale);
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
    int vertexLen = vbuffer->size();
    int texLen = tbuffer->size();
    int nLen = nbuffer->size();
    delete vbuffer;
    delete tbuffer;
    delete nbuffer;

    glBindVertexArray(vao);
    glGenBuffers(3, &vbo[0]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexLen * sizeof(float), vertex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, texLen * sizeof(float), texCoord, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nLen * sizeof(float), normal, GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    vertexCount = vertexLen/3;
}


void PolygonalMesh::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount * 3);
    glDisable(GL_DEPTH_TEST);
}


PolygonalMesh::~PolygonalMesh()
{
    delete vertex;
    delete texCoord;
    delete normal;
}

RenderObject::RenderObject(Mesh* mesh, Material* material) {
    this->mesh = mesh;
    this->material = material;
}

void RenderObject::Draw() {
    material->UploadAttributes();
    mesh->Draw();
}

Environment::Environment(EnvironmentShader* s, TextureCube* e, Camera* c) {
    shader = s;
    environmentMap = e;
    camera = c;

    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    static float vertexCoords[] = {
        -1,    -1,     0,     1,
        1,     -1,     0,     1,
        -1,      1,     0,     1,
        1,      1,     0,     1 };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
}

void Environment::Draw() {
    shader->Use();
    environmentMap->Bind();
    mat4 viewDirMatrix = camera->getInverseProjectionMatrix() * camera->getInverseViewMatix();
    shader->UploadViewDirMatrix(viewDirMatrix);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Light::Light(vec3 ambient, vec3 light_pos, vec3 light_color) {
    this->ambient = ambient;
    this->light_color = light_color;
    setPosition(light_pos);
}

void Light::UploadAttributes(Shader* shader) {
    shader->UploadLightAttributes(ambient, light_pos, light_color);
    shader->UploadLightProjectionMatrix(lightProjectionMatrix);
}

void Light::setPosition(vec3& pos) {
    light_pos = vec3(pos.x, pos.y, pos.z);
    glm::mat4 orthgonalProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    glm::vec3 lightPos = glm::vec3(light_pos.x, light_pos.y, light_pos.z);
    glm::mat4 lightView = glm::lookAt(20.0f * lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lp = orthgonalProjection * lightView;
    const float* v = (const float*)glm::value_ptr(lp);
    lightProjectionMatrix = mat4(v);
}

WorldObject::WorldObject(RenderObject* renderObject, Light* light, Camera* camera, vec3 position, vec3 scaling = vec3(1.0, 1.0, 1.0), float orientation = 0.0) {
    this->renderObject = renderObject;
    this->light = light;
    this->camera = camera;
    this->position = position;
    this->scaling = scaling;
    this->orientation = orientation;
    this->shader = renderObject->GetShader();
}

void WorldObject::Draw() {
    shader->Use();
    UploadAttributes(shader);
    light->UploadAttributes(shader);
    camera->UploadAttributes(shader);
    renderObject->Draw();
}

void WorldObject::DrawShadow(Shader* shadowShader) {
    shadowShader->Use();

    UploadAttributes(shadowShader);

    light->UploadAttributes(shadowShader);

    renderObject->Draw();
}

void WorldObject::SetShadowMap(ShadowMap* shadowmap) {
    renderObject->GetMaterial()->SetShadowMap(shadowmap);
}

void WorldObject::UploadAttributes(Shader* s) {
    mat4 T = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        position.x, position.y, position.z, 1.0);

    mat4 InvT = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        -position.x, -position.y, -position.z, 1.0);

    mat4 S = mat4(
        scaling.x, 0.0, 0.0, 0.0,
        0.0, scaling.y, 0.0, 0.0,
        0.0, 0.0, scaling.z, 0.0,
        0.0, 0.0, 0.0, 1.0);

    mat4 InvS = mat4(
        1.0 / scaling.x, 0.0, 0.0, 0.0,
        0.0, 1.0 / scaling.y, 0.0, 0.0,
        0.0, 0.0, 1.0 / scaling.z, 0.0,
        0.0, 0.0, 0.0, 1.0);

    float alpha = orientation / 180.0 * PI;

    mat4 R = mat4(
        cos(alpha), 0.0, sin(alpha), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(alpha), 0.0, cos(alpha), 0.0,
        0.0, 0.0, 0.0, 1.0);

    mat4 InvR = mat4(
        cos(alpha), 0.0, -sin(alpha), 0.0,
        0.0, 1.0, 0.0, 0.0,
        sin(alpha), 0.0, cos(alpha), 0.0,
        0.0, 0.0, 0.0, 1.0);

    mat4 M = S * R * T;
    mat4 InvM = InvT * InvR * InvS;

    mat4 MVP = M * camera->getViewMatrix() * camera->getProjectionMatrix();
    mat4 VP = camera->getViewMatrix() * camera->getProjectionMatrix();

    s->UploadM(M);
    s->UploadInvM(InvM);
    s->UploadMVP(MVP);
    s->UploadVP(VP);
}

Scene::Scene(std::vector<WorldObject*>* worldObjects, TextureCube* environmentMap, Camera* camera) {
    this->environmentMap = environmentMap;
    this->shadowShader = new ShadowShader();
    this->shadowmap = new ShadowMap();
    this->worldObjects = worldObjects;
    this->camera = camera;
    if (environmentMap) {
        environmentShader = new EnvironmentShader();
        environment = new Environment(environmentShader, environmentMap, camera);
    }
    DrawShadowmap();
    for (unsigned int i = 0; i < worldObjects->size(); i++) {
        (*worldObjects)[i]->SetShadowMap(shadowmap);
    }
}

void Scene::Draw() {
    //environment depth test is disabled, so it has to call before world objs.
    if (environment) {
        environment->Draw();
    }
    for (unsigned int i = 0; i < worldObjects->size(); i++) {
        (*worldObjects)[i]->Draw();
    }
}

void Scene::DrawShadowmap() {
    shadowmap->BindBuffer();
    glClear(GL_DEPTH_BUFFER_BIT);
    for (unsigned int i = 0; i < worldObjects->size(); i++) {
        (*worldObjects)[i]->Draw();
    }
    shadowmap->UnbindBuffer();
}