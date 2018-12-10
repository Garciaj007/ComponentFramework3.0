#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include "Mesh.h"
#include "Body.h"

using namespace MATH;

class Scene;

namespace GAME {

class Model {
protected:
	Vec3 orientation;
	Vec3 gravity;
	Sphere bounding;
	Matrix4 modelMatrix;
	Scene* parentScene;
	std::vector<Mesh*> meshes;
public:
	Model(){ }
	virtual ~Model() { }

	Model(const Model&) = delete;
	Model(Model&&) = delete;
	Model& operator = (const Model&) = delete;
	Model& operator = (Model&&) = delete;

	Body* body;

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Render(const Matrix4& projectionMatrix, const Matrix4& modelViewMatrix, const Matrix3& normalMatrix)const = 0;
};

} /// end of namespace

#endif