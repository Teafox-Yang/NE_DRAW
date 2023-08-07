#pragma once
#include "Common.h"

struct PixelShaderVarying
{
	PixelShaderVarying() = default;
	PixelShaderVarying(const Vector4f& viewPos, const Vector4f& col, const Vector4f& nor, const Vector2f& tc):
		color(col), normal(nor), texCoords(tc)	{}
	Vector4f viewPos;
	Vector4f color;
	Vector4f normal;
	Vector2f texCoords;
};

Vector4f normalPixelShader(const PixelShaderVarying& pixelData)
{
	Vector4f color = pixelData.normal + Vector4f(1.0f, 1.0f, 1.0f);
	return color;
}

Vector4f phongFragmentShader(const PixelShaderVarying& pixelData)
{
	Vector4f ka = Vector4f(0.005f);
	Vector4f kd = pixelData.color;
	Vector4f ks = Vector4f(0.7937f);

	Light l1 = Light(Vector4f(20.0f), Vector4f(500.0f));
	Light l2 = Light(Vector4f(-20.0f, 20.0f, 0.0f), Vector4f(500.0f));

	vector<Light> lights = { l1,l2 };

	float ambientIntensity = 10.0f;
	Vector4f eyePos = Vector4f(0, 0, 10);

	auto color = pixelData.color;
	auto point = pixelData.viewPos;
	auto normal = pixelData.normal;

	for (auto& light : lights)
	{
		auto lightDir = light.position - point;
		auto viewDir = eyePos - point;
		auto hDir = math::Normalize(math::Normalize(lightDir) + math::Normalize(viewDir));
		auto lightDis = pow(lightDir.X, 2) + pow(lightDir.Y, 2) + pow(lightDir.Z, 2);
		auto lightIntensity = light.intensity / lightDis;

		float power = 100.0f;

		auto nDotL = math::Dot(normal, math::Normalize(lightDir));
		auto nDotH = math::Dot(normal, hDir);
		auto lambert = lightIntensity * math::clamp(nDotL, 0.0f, 1.0f);
		auto blinnPhong = lightIntensity * pow(math::clamp(nDotH, 0.0f, 1.0f), power);

		auto diffuse = Vector4f(kd.X * lambert.X, kd.Y * lambert.Y, kd.Z * lambert.Z);
		auto specular = Vector4f(ks.X * blinnPhong.X, ks.Y * blinnPhong.Y, kd.Z * blinnPhong.Z);
		auto ambient = ambientIntensity * ka;

		auto color = diffuse + specular + ambient;
	}
}
