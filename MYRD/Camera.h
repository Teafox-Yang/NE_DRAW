#pragma once
#include "Common.h"

class Camera
{
public:
	Camera() = default;
	Camera(Vector4f eye, Vector4f target, float near, float far, float fovy, float aspect);

	Vector4f _position;
	Vector4f _target;
	Vector4f _up = Vector4f(0.0f, 1.0f, 0.0f);

	float _near, _far;
	float _fovy;
	float _aspect;
	float _distance;
	float _currDistance;

	Handness::Enum _handness = Handness::Left;

	Matrix4f mtxLookAt();
	Matrix4f mtxProj();
	void OrbitVertical(float theta = 0.0f);
	void Scale(float rate = 0.0f);

};

