#include "Rasterizer.h"
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
	int width = 800;
	int height = 600;

	float alpha = 1;
	float pos = 3.0;
	const TCHAR* title = _T("MYRD (software render) - ")
		_T("Left/Right: rotation, Up/Down: forward/backward");


	if (screen_init(width, height, title))
		return -1;

	//initialize rasterizer device
	Rasterizer rasDevice(width, height, screen_fb);
	Vector4f eye(pos, 3.0f,0.0f), at(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f);
	rasDevice.setModel(algorithm::GetIdentity());
	rasDevice.setView(algorithm::GetView(eye, at, up, Handness::Left));
    rasDevice.setProjection(algorithm::GetProjection(90, (float)width / height, 1.0f, 500.0f, Handness::Left));


	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		screen_dispatch();
		rasDevice.clear(Buffers::Frame);
		rasDevice.clear(Buffers::Depth);
		rasDevice.setView(algorithm::GetView(eye, at, up, Handness::Left));
		rasDevice.setPixelShader(PhongPixelShader);

		Matrix4f rotation = algorithm::GetRotation(Vector4f(0.0f, 1.0f, 0.0f), alpha);
		rasDevice.setModel(rotation);
		if (screen_keys[VK_UP]) eye.X -= 0.01f;
		if (screen_keys[VK_DOWN]) eye.X += 0.01f;
		if (screen_keys[VK_LEFT]) alpha += 1.0f;
		if (screen_keys[VK_RIGHT]) alpha -= 1.0f;

		rasDevice.drawTriangles(triList);
		screen_update();
		Sleep(1);
	}
	return 0;
}
