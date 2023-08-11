#include "Renderer.h"
#include "Screen.h"
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

Vector4f blue(0.0f, 0.0f, 1.0f), red(1.0f, 0.0f, 0.0f), green(0.0f, 1.0f, 0.0f);
Vector4f white(1.0f), black(0.0f), gray(0.8f, 0.8f, 0.8f);

Vertex vertexBuffer[8] = {
	{{-1.0f,-1.0f,1.0f},{},gray,{0,0}},
	{{1.0f,-1.0f,1.0f},{},gray,{0,1}},
	{{1.0f,1.0f,1.0f},{},gray,{1,1}},
	{{-1.0f,1.0f,1.0f},{},gray,{1,0}},

	{{-1.0f,-1.0f,-1.0f},{},gray,{0,0}},
	{{1.0f,-1.0f,-1.0f},{},gray,{0,1}},
	{{1.0f,1.0f,-1.0f},{},gray,{1,1}},
	{{-1.0f,1.0f,-1.0f},{},gray,{1,0}},
};

vector<Triangle> triList =
{
	GetTriangle(vertexBuffer,0,1,2),
	GetTriangle(vertexBuffer,0,3,2),

	GetTriangle(vertexBuffer,6,5,4),
	GetTriangle(vertexBuffer,6,7,4),

	GetTriangle(vertexBuffer,1,5,6),
	GetTriangle(vertexBuffer,1,2,6),

	GetTriangle(vertexBuffer,7,4,0),
	GetTriangle(vertexBuffer,7,3,0),

	GetTriangle(vertexBuffer,2,6,7),
	GetTriangle(vertexBuffer,2,3,7),

	GetTriangle(vertexBuffer,4,5,1),
	GetTriangle(vertexBuffer,4,0,1),
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, int nCmdShow)
{
	//initialize window
	int width = 800;
	int height = 600;

	const TCHAR* title = _T("MYRD (software render) - ")
		_T("Left/Right: rotation, Up/Down: forward/backward");

	if (screen_init(width, height, title))
		return -1;

	//initialize rasterizer device
	Renderer rasDevice(width, height, screen_fb);
	Vector4f eye(3.0f, 3.0f,0.0f), at(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f);
	Light l1 = Light(Vector4f(10.0f), Vector4f(500.0f));
	Light l2 = Light(Vector4f(-20.0f, 20.0f, 0.0f), Vector4f(500.0f));
	Camera camera(eye, at, 0.1f, 500.0f, 90.0f, (float)width / height);

    rasDevice.setProjection(camera.mtxProj());
	rasDevice.addLight(l1);
	rasDevice.addLight(l2);
	rasDevice.setPixelShader(PhongPixelShader);
	rasDevice.loadTexture("wall.jpg");

	float theta = 1.0f;
	float rate = 1.0f;
	float cameraTheta = 0.5f;
	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		screen_dispatch();
		rasDevice.clear(Buffers::Frame);
		rasDevice.clear(Buffers::Depth);

		// Camera distance
		if(mouse_delta > 0)
		{
			rate = math::clamp(rate - 0.05f,0.8f,1.5f);
			camera.Scale(rate);
			mouse_delta = 0;
		}
		if (mouse_delta < 0)
		{
			rate = math::clamp(rate + 0.05f, 0.8f, 1.5f);
			camera.Scale(rate);
			mouse_delta = 0;
		}
		// Camera orbit
		if (screen_keys[VK_UP]) camera.OrbitVertical(cameraTheta);
		if (screen_keys[VK_DOWN]) camera.OrbitVertical(-cameraTheta);

		// Cube rotation
		if (screen_keys[VK_LEFT]) theta += 1.0f;
		if (screen_keys[VK_RIGHT]) theta -= 1.0f;

		Matrix4f rotation = algorithm::GetRotation(Vector4f(0.0f, 1.0f, 0.0f), theta);
		rasDevice.setModel(rotation);
		rasDevice.setView(camera.mtxLookAt());
		rasDevice.updateMatrix();

		rasDevice.drawPlanes(triList);
		screen_update();
		Sleep(1);
	}
	return 0;
}
