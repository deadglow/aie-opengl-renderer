#include "Light.h"

Light::Light(glm::vec3 color_init, float intensity_init)
{
    color = color_init;
    intensity = intensity_init;
}

LightType Light::GetType()
{
    return LightType();
}

glm::vec4 Light::GetFinalColor()
{
    return glm::vec4(color * intensity, 1.0f);
}

LightShaderData Light::ConstructShaderData()
{
    return LightShaderData();
}

std::string Light::GetTypeName(LightType type)
{
	switch (type)
	{
	case LightType::Directional:
		return "Directional Light";
		break;
	case LightType::Point:
		return "Point Light";
		break;
	case LightType::Spot:
		return "Spot Light";
		break;
	default:
		return "";
		break;
	}
}
