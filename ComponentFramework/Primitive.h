#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Model.h"
#include "Shader.h"
#include "ObjLoader.h"
#include "VMath.h"

namespace GAME {
	class Primitive: public Model{
	public:
		//Constructor
		Primitive(GLenum drawMode_, std::vector<Vec3> &vertices_, std::vector<Vec3> &normals_, std::vector<Vec2> &uvs);
		Primitive(std::string filepath_);
		virtual ~Primitive();

		//Copy & Move Constructor Prevention 
		Primitive(const Primitive&) = delete;
		Primitive(Primitive&&) = delete;
		Primitive& operator = (const Primitive&) = delete;
		Primitive& operator = (Primitive&&) = delete;

		//Methods
		virtual bool OnCreate();
		virtual void OnDestroy();
		virtual void Update(const float deltaTime);
		virtual void Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix, const Matrix3& normalMatrix) const;
		virtual void Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix, const Matrix3& normalMatrix, const Vec4& color) const;

		//Custom Methods 
		virtual std::vector<Vec3> GetVerticies() { return verticies; }
		virtual Vec3 GetFathestPoint(const Vec3& direction);
		virtual Vec3 GetCenter();

	private:
		//Contains the verticies of an obj
		std::vector<Vec3> verticies;

		Shader *shader;
		GLuint vbo;
		GLuint vao;
		GLint verticiesID;
		GLint normalsID;
		GLint uvCoordsID;

		GLint projectionMatrixID;
		GLint modelViewMatrixID;
		GLint normalMatrixID;
		GLint colorVectorID;
		GLint lightPosID;
		Vec3 lightPos;
	};
}
#endif

