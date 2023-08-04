#include "Rasterizer.h"
#include <algorithm>

Rasterizer::Rasterizer(int width, int height) :_width(width), _height(height)
{
	_frameBuffer.resize(width * height);
	_depthBuffer.resize(width * height);
}


void Rasterizer::setModel(const Matrix4f& m)
{
	_model = m;
}

void Rasterizer::setView(const Matrix4f& v)
{
	_view = v;
}

void Rasterizer::setProjection(const Matrix4f& p)
{
	_projection = p;
}

// x,y begin from the left bottom of the screen
int Rasterizer::getIndex(int x, int y)
{
	return (_height - y) * _width + x;
}

// Clear Buffer
void Rasterizer::clear(Buffers buffer)
{
	if (buffer == Buffers::Frame)
		fill(_frameBuffer.begin(), _frameBuffer.end(), BITCOLOR(0));
	if (buffer == Buffers::Depth)
		fill(_depthBuffer.begin(), _depthBuffer.end(), numeric_limits<float>::infinity());
}

// is the point on the right side of each edge(clockwize)
static bool insideTriangle(int x, int y, const Vertex* v)
{
	Vector2f P = Vector2f(x, y);
	Vector2f A = Vector2f(v[0].Position.X, v[0].Position.Y);
	Vector2f B = Vector2f(v[1].Position.X, v[1].Position.Y);
	Vector2f C = Vector2f(v[2].Position.X, v[2].Position.Y);

	Vector2f AB = B - A;
	Vector2f BC = C - B;
	Vector2f CA = A - C;

	Vector2f AP = P - A;
	Vector2f BP = P - B;
	Vector2f CP = P - C;

	return AB.X * AP.Y - AB.Y * AP.X > 0
		&& BC.X * BP.Y - BC.Y * BP.X > 0
		&& CA.X * CP.Y - CA.Y * CP.X > 0;
}