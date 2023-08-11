#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(const char* file)
{
	int channels;
	unsigned char* data = stbi_load(file, &_width, &_height, &channels, 0);
	if (data != NULL)
	{
		_loaded = true;
		_texture.resize(_width * _height);
		for (int i = 0; i < _height; i++)
			for (int j = 0; j < _width; j++)
			{
				int index = i * _width + j;
				BYTECOLOR r = static_cast<BYTECOLOR>(data[index * 3]);
				BYTECOLOR g = static_cast<BYTECOLOR>(data[index * 3 + 1]);
				BYTECOLOR b = static_cast<BYTECOLOR>(data[index * 3 + 2]);

				_texture[index] = Vector4f(r / 256.0, g / 256.0, b / 256.0);
			}
	}

}

// the (0,0) is at the left top of the image
// the uv(0,0) is at the left bottom of the texcoord

Vector4f Texture::Tex2D(float u, float v)
{
	u = math::clamp(u, 0.0f, 1.0f);
	v = math::clamp(v, 0.0f, 1.0f);
	float uTex = u * (_width - 1.0f);
	float vTex = (1.0f - v) * (_height - 1.0f);

	return _getColor((int)uTex, (int)vTex);
}

bool Texture::isLoaded()
{
	return _loaded;
}

Vector4f Texture::_getColor(int u, int v)
{
	return _texture[v * _width + u];
}
