#include <GL/glew.h>
#include <SDL.h>
#include "Scene0.h"
#include <iostream>
#include "MMath.h"
#include "QuadSphere.h"
#include "Model0.h"
#include "Trackball.h"
#include "Collider.h"
#include "Primitive.h"

using namespace GAME;
using namespace MATH;

Scene0::Scene0(class Window& windowRef):  Scene(windowRef) { 
	trackball = new Trackball();
	projectionMatrix.loadIdentity();
	viewMatrix.loadIdentity();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

Scene0::~Scene0(){ 
	OnDestroy();
}

bool Scene0::OnCreate() {
	OnResize(windowPtr->GetWidth(),windowPtr->GetHeight());

	/// Load Assets: as needed 
	color1 = Vec4(0.0f, 1.0f, 1.0f, 1.0f);
	color2 = Vec4(1.0f, 0.0f, 1.0f, 1.0f);
	prim1 = new Primitive("triangle1.obj");
	prim2 = new Primitive("triangle2.obj");
	prim1->SetPos(Vec3(-2.0f, -2.0f, 0.0f));
	prim2->SetPos(Vec3(-2.0f, -2.0f, 0.0f));
	return true;
}


void Scene0::OnResize(int w_, int h_){
	windowPtr->SetWindowSize(w_,h_);
	glViewport(0,0,windowPtr->GetWidth(),windowPtr->GetHeight());
	float aspect = float(windowPtr->GetWidth()) / float(windowPtr->GetHeight());
	
	projectionMatrix = MMath::perspective(45.0f, aspect, 1.0f, 100.0f);

	viewMatrix = MMath::lookAt(Vec3(0.0f, 0.0f, 10.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
}

void Scene0::OnDestroy(){
	/// Cleanup Assets
	if(trackball) delete trackball;
	trackball = nullptr;
	if (prim1) {
		delete prim1;
	}
	if (prim2) {
		delete prim2;
	}
	prim1 = nullptr;
	prim2 = nullptr;
}

void Scene0::Update(const float deltaTime){
	prim1->Update(deltaTime);
	prim2->Update(deltaTime);

	//Collision Handling
	if (Collider::Collided(prim1, prim2)) {
		Collider::HandleCollision(prim1, prim2);
	}
}

void Scene0::Render() const{
	/// Draw your scene here
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	prim1->Render(projectionMatrix, trackball->GetMatrix4() *  viewMatrix, trackball->GetMatrix3(), color1);
	prim2->Render(projectionMatrix, trackball->GetMatrix4() *  viewMatrix, trackball->GetMatrix3(), color2);
	SDL_GL_SwapWindow(windowPtr->getSDLWindow());
}

void Scene0::HandleEvents(const SDL_Event& SDLEvent){

	/*if(SDLEvent.type == SDL_EventType::SDL_MOUSEBUTTONDOWN){
		trackball->OnLeftMouseDown(SDLEvent.button.x,SDLEvent.button.y);
	}
	if (SDLEvent.type == SDL_EventType::SDL_MOUSEMOTION && 
		SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		trackball->OnMouseMove(SDLEvent.button.x,SDLEvent.button.y);
	}*/		
}