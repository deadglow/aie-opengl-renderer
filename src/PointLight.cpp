#include "PointLight.h"

PointLight::PointLight(glm::vec3 position_init, glm::vec3 color_init, float intensity_init) : Light(color_init, intensity_init)
{
    transform.SetPosition(position_init);
}

LightType PointLight::GetType()
{
    return LightType::Point;
}

LightShaderData PointLight::ConstructShaderData()
{
    LightShaderData shaderData;
    shaderData.type = (int)GetType();
    shaderData.position = transform.GetPosition();
    shaderData.color = GetFinalColor();
    shaderData.properties = GetProperties();

    return shaderData;
}


glm::vec4 PointLight::GetProperties()
{
    return glm::vec4(constant, linear, quadratic, radius);
}
