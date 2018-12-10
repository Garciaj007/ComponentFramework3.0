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
	Sphere bounding;
	Matrix4 modelMatrix;
	Scene* parentScene;
	std::vector<Mesh*> meshes;
	Body* body;
public:
	Model(){ }
	virtual ~Model() { if (body) delete body; body = nullptr; }

	Model(const Model&) = delete;
	Model(Model&&) = delete;
	Model& operator = (const Model&) = delete;
	Model& operator = (Model&&) = delete;

	inline void SetPos(const Vec3& pos_) {
		if (body)
			body->SetPosition(pos_);
	}

	inline void SetVel(const Vec3& vel_) {
		if (body)
			body->SetVelocity(vel_);
	}

	inline Body* GetBody() {
		return body;
	}

	void Update(const float deltaTime) {
		if (body)
			body->Update(deltaTime);
	}

	virtual bool OnCreate() = 0;
	virtual void OnDestroy() = 0;
	virtual void Render(const Matrix4& projectionMatrix, const Matrix4& modelViewMatrix, const Matrix3& normalMatrix)const = 0;
};

} /// end of namespace

#endif