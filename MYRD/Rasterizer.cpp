#include "Rasterizer.h"
#include <algorithm>

Rasterizer::Rasterizer(int width, int height, unsigned char* fb) :_width(width), _height(height)
{
	//unsigned char* framebuf = static_cast<unsigned char*>(fb);
	if (fb != NULL) _frameBuffer = (BYTECOLOR*)fb;
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

void Rasterizer::setDepth(int x, int y, float depth)
{
	int index = (_height - y) * _width + x;
	_depthBuffer[index] = depth;
}

void Rasterizer::setPixel(int x, int y, BYTECOLOR color)
{
	int index = (_height - y) * _width + x;
	_frameBuffer[index] = color;
}

// x,y begin from the left bottom of the screen
int Rasterizer::getIndex(int x, int y)
{
	return (_height - y) * _width + x;
}

void Rasterizer::setPixelShader(std::function<Vector4f(PixelShaderVarying)> pixelShader)
{
	_pixelShader = pixelShader;
}

// Clear Buffer
void Rasterizer::clear(Buffers buffer)
{
	if (buffer == Buffers::Frame)
		for(int i = 0;i<_width;i++)
			for (int j = 1; j < _height; j++)
			{
				_frameBuffer[getIndex(i, j)] = _backgroundColor;
			}
	if (buffer == Buffers::Depth)
		fill(_depthBuffer.begin(), _depthBuffer.end(), numeric_limits<float>::infinity());
}


void Rasterizer::drawPrimitives(vector<Triangle>& triangleList)
{
	Matrix4f mv = _view * _model;
	Matrix4f mvp = _projection * mv;

	for (const auto& t : triangleList)
	{
		Triangle tri = t;

		/*tri.v[0] = vertexShader(t.v[0]);
		tri.v[1] = vertexShader(t.v[1]);
		tri.v[2] = vertexShader(t.v[2]);*/

		//for perspective adjustment
		vector<Vector4f> viewspacePos(3);

		viewspacePos[0] = mv * t.v[0].position;
		viewspacePos[1] = mv * t.v[1].position;
		viewspacePos[2] = mv * t.v[2].position;

		// view space normal
		Vector4f viewNormal = math::Normalize(mv * t.v[0].normal);
		tri.v[0].normal = viewNormal;
		tri.v[1].normal = viewNormal;
		tri.v[2].normal = viewNormal;

		// mvp transformation
		tri.v[0].position = mvp * t.v[0].position;
		tri.v[1].position = mvp * t.v[1].position;
		tri.v[2].position = mvp * t.v[2].position;
		// Homogeneous division
		tri.v[0].position.Homogeneous();
		tri.v[1].position.Homogeneous();
		tri.v[2].position.Homogeneous();
		// Viewprot transformaton
		for (auto& vert : tri.v)
		{
			vert.position.X = 0.5 * _width * (vert.position.X + 1.0f);
			vert.position.Y = 0.5 * _height * (vert.position.Y + 1.0f);
		}

		rasterizeTriangle(tri, viewspacePos);
	}
}

void Rasterizer::rasterizeTriangle(const Triangle& tri, const vector<Vector4f>& viewPos)
{
	// Calculate the AABB
	Vector4f v[3] = { tri.v[0].position,tri.v[1].position, tri.v[2].position };
	int left, right, top, bottom;
	left = (int)floor(v[0].X);
	right = (int)ceil(v[0].X);
	top = (int)ceil(v[0].Y);
	bottom = (int)floor(v[0].Y);
	for (int i = 1; i < 3; i++)
	{
		left = v[i].X < left ? (int)floor(v[i].X) : left;
		right = v[i].X > right ? (int)ceil(v[i].X) : right;
		top = v[i].Y > top ? (int)ceil(v[i].Y) : top;
		bottom = v[i].Y < bottom ? (int)floor(v[i].Y) : bottom;
	}

	// Rasterizing
	for(int i = left;i<=right;i++)
		for (int j = bottom; j <= top; j++)
		{
			if (algorithm::insideTriangle((float)i + 0.5, (float)j + 0.5, tri.v))
			{
				auto temp = algorithm::computeBaryCentric((float)i + 0.5, (float)j + 0.5, v);
				float alpha, beta, gamma;
				std::tie(alpha, beta, gamma) = temp;
				// perspective adjustment
				float zRevised = 1 / (alpha / viewPos[0].Z + beta / viewPos[1].Z + gamma / viewPos[2].Z);

				if (zRevised <= _depthBuffer[getIndex(i, j)])
				{		
					float alphaRevised = zRevised / viewPos[0].Z * alpha;
					float betaRevised = zRevised / viewPos[1].Z * beta;
					float gammaRevised = zRevised / viewPos[2].Z * gamma;

					auto interpolated_normal = alphaRevised * tri.v[0].normal + betaRevised * tri.v[1].normal + gammaRevised * tri.v[2].normal;
					auto interpolated_color = alphaRevised * tri.v[0].vertexColor + betaRevised * tri.v[1].vertexColor + gammaRevised * tri.v[2].vertexColor;
					auto interpolated_texcoords = alphaRevised * tri.v[0].textureCoordinate + betaRevised * tri.v[1].textureCoordinate + gammaRevised * tri.v[2].textureCoordinate;
					auto interpolated_viewPos = alphaRevised * viewPos[0] + betaRevised * viewPos[1] + gammaRevised + viewPos[2];
					
					PixelShaderVarying pixelData(interpolated_viewPos, interpolated_color, math::Normalize(interpolated_normal), interpolated_texcoords);
					auto pixelColor = _pixelShader(pixelData);
					setDepth(i, j, zRevised);
					setPixel(i, j, math::getBitColor(pixelColor));
				}
			}

		}
}