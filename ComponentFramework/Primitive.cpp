#include "Primitive.h"
#include "MMath.h"
#include <map>

using namespace GAME;
using namespace MATH;

//Create Primitive
Primitive::Primitive(GLenum drawMode_, std::vector<Vec3> &vertices_, std::vector<Vec3> &normals_, std::vector<Vec2> & uvs_, Body *b)
{
	body = b;
	verticies = vertices_;
	meshes.push_back(new Mesh(drawMode_, vertices_, normals_, uvs_));
	OnCreate();
}

//Create Primitive from file path...
Primitive::Primitive(std::string filepath_, Body* b) {
	body = b;
	ObjLoader obj(filepath_.c_str());
	verticies = obj.vertices;
	meshes.push_back(new Mesh(GL_LINE_LOOP, obj.vertices, obj.normals, obj.uvCoords));
	OnCreate();
}

//Destructor
Primitive::~Primitive()
{
	OnDestroy();
}

bool Primitive::OnCreate() {
	body = new Body(1, -GetCenter());
	//New Shader Program
	shader = new Shader("baseVert.glsl", "baseFrag.glsl", 3, 0, "vVertex", 1, "vNormal", 2, "texCoords");
	//Get Shader program IDs 
	projectionMatrixID = glGetUniformLocation(shader->getProgram(), "projectionMatrix");
	modelViewMatrixID = glGetUniformLocation(shader->getProgram(), "modelViewMatrix");
	normalMatrixID = glGetUniformLocation(shader->getProgram(), "normalMatrix");
	colorVectorID = glGetUniformLocation(shader->getProgram(), "customColor");
	return true;
}

void Primitive::OnDestroy() {
	//Delete VAOs and VBOs
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	//Delete shader
	if (shader) {
		delete shader;
	}
	shader = nullptr;
	//Delete Meshes
	for (Mesh* m : meshes) {
		if (m) {
			delete m;
		}
		m = nullptr;
	}
}

void Primitive::Update(const float deltaTime) {
	//Set the meshes position to current Pos
	//Sets position to body's position
	body->Update(deltaTime);
	body->Print();
	Matrix4 translate = MMath::translate(body->pos.x, body->pos.y, body->pos.z);
	Matrix4 rotate = MMath::rotate(body->angle, 0.0f, 0.0f, 1.0f);
	modelMatrix = rotate * translate;
}

void Primitive::Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix, const Matrix3& normalMatrix) const {
	//Use Shader Program
	glUseProgram(shader->getProgram());
	//Apply Shader Params 
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, viewMatrix *  modelMatrix);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);
	glUniform4fv(colorVectorID, 1, Vec4(1, 1, 1, 1));
	//Render Meshes
	for (Mesh* mesh : meshes) {
		mesh->Render();
	}
	// Disable the VAO
	glEnableVertexAttribArray(0);
}

void Primitive::Render(const Matrix4& projectionMatrix, const Matrix4& viewMatrix, const Matrix3& normalMatrix, const Vec4& color) const {
	//Use Shader Program
	glUseProgram(shader->getProgram());
	//Apply Shader Params
	glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, projectionMatrix);
	glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, viewMatrix *  modelMatrix);
	glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, normalMatrix);
	glUniform4fv(colorVectorID, 1, color);
	//Render Meshes
	for (Mesh* mesh : meshes) {
		mesh->Render();
	}
	// Disable the VAO
	glEnableVertexAttribArray(0);
}


