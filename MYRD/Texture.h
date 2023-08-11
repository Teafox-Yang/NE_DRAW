#pragma once
#include "Common.h"
#include <vector>
using namespace std;

class Texture
{
public:
	Texture() = default;
	Texture(const char* file);
	Vector4f Tex2D(float u, float v);
	bool isLoaded();
private:
	int _width, _height;
	bool _loaded = false;
	vector<Vector4f> _texture;

	Vector4f _getColor(int u, int v);
};

