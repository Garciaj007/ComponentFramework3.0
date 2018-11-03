#version 330
in vec4 vVertex;
in vec4 vNormal;
in vec2 vUV;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat3 normalMatrix;

void main(){
	gl_Position = projectionMatrix * modelViewMatrix * vVertex;
}