#ifndef GL_H
#define GL_H
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "cyTriMesh.h"
#include "cyMatrix.h"
#include "cyGL.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#endif
namespace CC {
	const float PI = 3.145;
	class mat4 {
	public:
		float m[16];
		mat4();
		mat4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		mat4(const float* m);
		mat4(const mat4& other);
		mat4& operator=(mat4 other);
		mat4 operator*(const mat4& right);
	};

	class vec4 {
	public:
		float v[4];
		vec4(float x, float y, float z, float w);
		vec4(const vec4& other);
		vec4& operator=(vec4 other);
		vec4 operator*(const mat4& mat);
		vec4 operator+(const vec4& vec);
	};

	class vec2 {
	public:
		float x, y;
		vec2(float x = 0, float y = 0) : x(x), y(y) {}
		vec2(const vec2& other);
		vec2& operator=(vec2& other);
		vec2 operator+(const vec2& v);
		vec2 operator*(float s);
		float length();
	};

	class vec3 {
	public:
		float x, y, z;
		vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
		vec3(const vec3& other);
		vec3& operator=(vec3 other);
		vec3 operator+(const vec3& v);
		vec3 operator-(const vec3& v);
		vec3 operator*(float s);
		vec3 operator/(float s);
		float length();
		vec3 normalize();
		vec3 cross(const vec3& right);

	};

	//shader
	class Shader {
	protected:
		cy::GLSLProgram* shaderProgram;
	public:
		Shader();
		~Shader();
		void Use();
		virtual void UploadM(mat4& M) { }
		virtual void UploadInvM(mat4& InVM) { }
		virtual void UploadMVP(mat4& MVP) { }
		virtual void UploadVP(mat4& VP) { }
		virtual void UploadColor(vec4& color) { }
		virtual void UploadSamplerID() { }
		virtual void UploadMaterialAttributes(bool hasTexture, float specular_alpha, vec3 specular_color, vec3 diffuse_color) { }
		virtual void UploadLightAttributes(vec3 ambient, vec3 light_dir, vec3 light_color) { }
		virtual void UploadCameraPosition(vec3 camera_position) { }
		virtual void UploadSamplerCubeID() { }
		virtual void UploadViewDirMatrix(mat4& viewDirMatrix) { }
		virtual void UploadLightProjectionMatrix(mat4& LP) { }
		virtual void UploadShadowMapID() { }
	};

	//mesh shader
	class MeshShader : public Shader {
	public:
		MeshShader();
		void UploadSamplerID();
		//void UploadSamplerCubeID();
		void UploadM(mat4& M);
		void UploadInvM(mat4& InvM);
		void UploadMVP(mat4& MVP);
		void UploadLightProjectionMatrix(mat4& LP);
		void UploadMaterialAttributes(bool hasTexture, float specular_alpha, vec3 specular_color, vec3 diffuse_color);
		void UploadLightAttributes(vec3 ambient, vec3 light_dir, vec3 light_color);
		void UploadCameraPosition(vec3 camera_position);
		void UploadShadowMapID();
	};

	//reflective shader
	class ReflectiveShader : public Shader {
	public:
		ReflectiveShader();
		void UploadSamplerID();
		void UploadSamplerCubeID();
		void UploadM(mat4& M);
		void UploadInvM(mat4& InvM);
		void UploadMVP(mat4& MVP);
		void UploadMaterialAttributes(bool hasTexture, float specular_alpha, vec3 specular_color, vec3 diffuse_color);
		void UploadLightAttributes(vec3 ambient, vec3 light_dir, vec3 light_color);
		void UploadCameraPosition(vec3 camera_position);
		void UploadShadowMapID();
	};

	//shadow shader
	class ShadowShader : public Shader {
	public:
		ShadowShader();
		void UploadM(mat4& M);
		void UploadVP(mat4& VP);
		void UploadLightAttributes(vec3 ambient, vec3 light_dir, vec3 light_color);
		void UploadLightProjectionMatrix(mat4& LP);
	};

	//environment shader
	class EnvironmentShader : public Shader {
	public:
		EnvironmentShader();
		void UploadSamplerCubeID();
		void UploadViewDirMatrix(mat4& viewDirMatrix);
	};
	//Geo
	class Mesh
	{
	protected:
		unsigned int vao;
	public:
		Mesh();
		virtual void Draw() = 0;
	};
	class PolygonalMesh : public Mesh {
		float* vertex;
		float* texCoord;
		float* normal;
		int vertexCount = 0;
		unsigned int vbo[3];
	public:
		PolygonalMesh(const char* filename);
		~PolygonalMesh();

		void Draw();
	};

	//Texture
	class Texture {
	public:
		unsigned int textureId;
		Texture(const std::string& inputFileName);
		void Bind();
	};

	//Texture Cube
	class TextureCube {
	public:
		unsigned int textureId;
		TextureCube(
			const std::string& inputFileName0, const std::string& inputFileName1, const std::string& inputFileName2,
			const std::string& inputFileName3, const std::string& inputFileName4, const std::string& inputFileName5);
		void Bind();
	};

	class ShadowMap {
	public:
		cy::GLRenderDepth2D depthBuffer;
		ShadowMap();
		void BindBuffer();
		void UnbindBuffer();
		void Bind();
	};

	//Material
	class Material {
	public:
		Shader* shader;
		Texture* texture;
		vec3 diffuse_color;
		float specular_alpha;
		vec3 specular_color;
		TextureCube* environmentMap;
		ShadowMap* shadowMap = 0;
		Material(Shader* shader, float specular_alpha = 500, vec3 specular_color = vec3(1,1,1), vec3 diffuse_color = vec3(0.3, 0.8,0.4), Texture* texture = 0, TextureCube* e = 0);
		void SetShadowMap(ShadowMap* shadowMap) { this->shadowMap = shadowMap; }
		Shader* GetShader() { return shader; }
		void UploadAttributes();
	};

	class RenderObject {
		Mesh* mesh;
		Material* material;
	public:
		RenderObject(Mesh* mesh, Material* material);

		Shader* GetShader() { return material->GetShader(); }
		Material* GetMaterial() { return material; }
		void Draw();
	};

	//camera
	class Camera {
	public:
		vec3  position = vec3(0.0, 0.0, 2.0);
		vec3 target = vec3(0.0, 0.0, 0.0);
		vec3 up = vec3(0.0, 1.0, 0.0);
		float fov, asp, n, f;
		Camera();
		void SetAspectRatio(float a) { asp = a; };
		vec3 getPosition() { return position; };
		mat4 getViewMatrix();
		mat4 getProjectionMatrix();
		mat4 getInverseViewMatix();
		mat4 getInverseProjectionMatrix();
		vec3 getCameraDirection();
		void cameraDirectionMove(float delta);
		void cameraHorizontalMove(float delta);
		void horizontalRotate(float delta);
		void verticalRotate(float delta);
		void UploadAttributes(Shader* shader);
	};

	class Environment : public Mesh {
	public:
		unsigned int vbo;
		EnvironmentShader* shader;
		TextureCube* environmentMap;
		Camera* camera;
		Environment(EnvironmentShader* s, TextureCube* e, Camera* c);
		void Draw();
	};

	class Light {
		vec3 ambient, light_pos, light_color;
		mat4 lightProjectionMatrix;

	public:
		Light(vec3 ambient, vec3 light_pos, vec3 light_color);
		void UploadAttributes(Shader* shader);
		void setPosition(vec3& pos);
	};

	class WorldObject {
		Shader* shader;
		RenderObject* renderObject; 
		Light* light; 
		Camera* camera;
		vec3 position;
		vec3 scaling;
		float orientation;
	public:
		WorldObject(RenderObject* renderObject, Light* light, Camera* camera, vec3 position, vec3 scaling, float orientation);
		virtual void UploadAttributes(Shader* s);
		virtual vec3& GetPosition() { return position; }
		virtual float GetOrientation() { return orientation; }
		virtual void Move(float dt) {}
		virtual void PushedBy(float dt, WorldObject* o) {}
		virtual void DrawSpotlight() {}
		virtual void Roll(float dt, WorldObject* o, int index) {}
		virtual void SetAheadOrientation(float orient) {}
		void Draw();
		void DrawShadow(Shader* shadowShader);
		void SetShadowMap(ShadowMap* shadowmap);
	};

	class Scene {
		MeshShader* meshShader = 0;
		ShadowShader* shadowShader = 0;
		TextureCube* environmentMap = 0;
		EnvironmentShader* environmentShader = 0;
		std::vector<WorldObject*>* worldObjects = 0;
		Environment* environment = 0;
		Camera* camera = 0;
		ShadowMap* shadowmap = 0;
	public:
		Scene(std::vector<WorldObject*>* worldObjects, TextureCube* environmentMap, Camera* camera);
		void Draw();
		void DrawShadowmap();
	};
}


