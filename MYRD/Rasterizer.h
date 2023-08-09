#pragma once

#include "Common.h"
#include "Shader.h"
#include <functional>
#include <vector>

using namespace std;

enum class Primitive
{
	Line,
	Triangle
};

enum class Buffers
{
	Frame = 1,
	Depth = 2
};
class Rasterizer
{
public:
	Rasterizer(int width, int height, unsigned char* fb);

	void setModel(const Matrix4f& m);
	void setView(const Matrix4f& v);
	void setProjection(const Matrix4f& p);

	int getIndex(int x, int y);
	void setDepth(int x, int y, float depth);
	void setPixel(int x, int y, BYTECOLOR color);
	void clear(Buffers buffer);

	void setPixelShader(std::function<Vector4f(PixelShaderVarying)> pixelShader);

	void drawPlanes(vector<Triangle>& triangleList);
	void drawTriangles(vector<Triangle>& triangleList);


private:
	Matrix4f _model;
	Matrix4f _view;
	Matrix4f _projection;

	function<Vector4f(PixelShaderVarying)> _pixelShader;
	function<Vertex(Vertex)> _vertexShader;

	BYTECOLOR* _frameBuffer;
	vector<float> _depthBuffer;

	BYTECOLOR _backgroundColor = BYTECOLOR(0);

	int _width, _height;

	void rasterizeTriangle(const Triangle& tri, const vector<Vector4f>& viewPos);

};

