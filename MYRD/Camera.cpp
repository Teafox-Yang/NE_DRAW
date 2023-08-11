#include "Camera.h"

Camera::Camera(Vector4f eye, Vector4f target, float near, float far, float fovy, float aspect):
	_position(eye),_target(target),_near(near),_far(far), _fovy(fovy),_aspect(aspect)
{
	_distance = math::Magnitude(target - eye);
	_currDistance = _distance;
}

Matrix4f Camera::mtxLookAt()
{
	return algorithm::GetView(_position, _target, _up, _handness);
}

Matrix4f Camera::mtxProj()
{
	return algorithm::GetProjection(_fovy, _aspect, _near, _far, _handness);
}


void Camera::OrbitVertical(float theta)
{
	Vector4f dir = _position - _target;
	Vector4f axisY(0.0f, 1.0f, 0.0f);
	Vector4f rotateAxis = math::Cross(dir, axisY);
	Matrix4f rotation = algorithm::GetRotation(rotateAxis, theta);
	_position = rotation * _position;
}

void Camera::Scale(float rate)
{
	_currDistance = rate * _distance;
	if (_currDistance > 2.0f)
	{
		Vector4f dir = math::Normalize(_position - _target);
		_position = _target + rate * dir * _distance;
	}
}
