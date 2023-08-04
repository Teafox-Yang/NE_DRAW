#pragma once

#include "Common.h"
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
	Rasterizer(int width, int height);

	void setModel(const Matrix4f& m);
	void setView(const Matrix4f& v);
	void setProjection(const Matrix4f& p);

	int getIndex(int x, int y);
	void setDepth();
	void setPixel();
	void clear(Buffers buffer);

	void draw(const vector<Triangle> &triangleList);


private:
	Matrix4f _model;
	Matrix4f _view;
	Matrix4f _projection;

	vector<BITCOLOR> _frameBuffer;
	vector<float> _depthBuffer;

	BITCOLOR _backgroundColor = BITCOLOR(0);

	int _width, _height;

	void rasterizeTriangle(const Triangle& t);

};

