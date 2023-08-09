#pragma once
#include "Common.h"
#include <vector>

using namespace std;

class PixelShaderVarying
{
public:
	PixelShaderVarying() = default;
	PixelShaderVarying(const Vector4f& viewPos, const Vector4f& col, const Vector4f& nor, const Vector2f& tc):
		viewPos(viewPos), color(col), normal(nor), texCoords(tc)	{}
	Vector4f viewPos;
	Vector4f color;
	Vector4f normal;
	Vector2f texCoords;
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

	Light l1 = Light(Vector4f(10.0f), Vector4f(500.0f));
	Light l2 = Light(Vector4f(-20.0f, 20.0f, 0.0f), Vector4f(500.0f));

	vector<Light> lights = { l1};

	float ambientIntensity = 10.0f;
	Vector4f eyePos = Vector4f(0, 0, 0);

	Vector4f color = pixelData.color;
	Vector4f point = pixelData.viewPos;
	//point = Vector4f(0);
	Vector4f normal = pixelData.normal;

	Vector4f resultColor = Vector4f(0);

	for (Light& light : lights)
	{
		Vector4f lightDir = light.position - point;
		Vector4f viewDir = eyePos - point;
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

static Vector4f LambertPixelShader(const PixelShaderVarying& pixelData)
{
	Vector4f normal = pixelData.normal;
	Vector4f lightDir = (1, 3, 1);
	float nDotL = math::Dot(normal, math::Normalize(lightDir));
	Vector4f kd = pixelData.color;
	float lambert = math::clamp(nDotL, 0.0f, 1.0f);
	return Vector4f(kd * lambert);

}

static Vector4f BlinnPhongPixelShader(const PixelShaderVarying& pixelData)
{
	Vector4f normal = pixelData.normal;
	Vector4f viewPos = pixelData.viewPos;
	Vector4f lightPos = (5, 10, 5);

	Vector4f lightDir = math::Normalize(lightPos - viewPos);
	float nDotL = math::Dot(normal, math::Normalize(lightDir));

	Vector4f kd = pixelData.color;
	float lambert = math::clamp(nDotL, 0.0f, 1.0f);
	return Vector4f(kd * lambert);

}
