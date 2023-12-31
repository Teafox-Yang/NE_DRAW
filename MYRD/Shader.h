#pragma once
#include "Common.h"
#include <vector>

using namespace std;

class PixelShaderVarying
{
public:
	PixelShaderVarying() = default;
	PixelShaderVarying(const Vector4f& viewPos, const Vector4f& col, const Vector4f& nor, const Vector2f& tc, const vector<Light>& li):
		viewPos(viewPos), color(col), normal(nor), texCoords(tc), lights(li){}
	Vector4f viewPos;
	Vector4f color;
	Vector4f normal;
	Vector2f texCoords;
	vector<Light> lights;
};

static Vector4f NormalPixelShader(const PixelShaderVarying& pixelData)
{
	Vector4f color = pixelData.normal;
	return color;
}

static Vector4f ColorPixelShader(const PixelShaderVarying& pixelData)
{
	Vector4f color = pixelData.color;
	return color;
}

static Vector4f PhongPixelShader(const PixelShaderVarying& pixelData)
{
	Vector4f ka = Vector4f(0.01f, 0.0f, 0.0f);
	Vector4f kd = pixelData.color;
	Vector4f ks = Vector4f(0.7937f);

	float ambientIntensity = 10.0f;
	Vector4f eyePos = Vector4f(0.0f);

	Vector4f color = pixelData.color;
	Vector4f viewPint = pixelData.viewPos;
	Vector4f normal = pixelData.normal;

	Vector4f resultColor = Vector4f(0);

	for (const Light& light : pixelData.lights)
	{
		Vector4f lightDir = light.position - viewPint;
		Vector4f viewDir = eyePos - viewPint;
		Vector4f hDir = math::Normalize(math::Normalize(lightDir) + math::Normalize(viewDir));
		float lightDis = pow(lightDir.X, 2) + pow(lightDir.Y, 2) + pow(lightDir.Z, 2);
		Vector4f lightIntensity = light.intensity / lightDis;

		float power = 50.0f;

		float nDotL = math::Dot(normal, math::Normalize(lightDir));
		float nDotH = math::Dot(normal, hDir);
		Vector4f lambert = lightIntensity * math::clamp(nDotL, 0.0f, 1.0f);
		Vector4f blinnPhong = lightIntensity * pow(math::clamp(nDotH, 0.0f, 1.0f), power);

		Vector4f diffuse = (Vector4f(kd.X * lambert.X, kd.Y * lambert.Y, kd.Z * lambert.Z));
		Vector4f specular = Vector4f(ks.X * blinnPhong.X, ks.Y * blinnPhong.Y, kd.Z * blinnPhong.Z);

		//Vector4f color = diffuse + specular + ambient;
		resultColor = resultColor + diffuse + specular;
	}
	Vector4f ambient = ambientIntensity * ka;
	resultColor = resultColor + ambient;
	return resultColor;
}


