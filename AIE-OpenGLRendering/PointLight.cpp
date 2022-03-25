#include "PointLight.h"

PointLight::PointLight(glm::vec3 position_init, glm::vec3 color_init, float intensity_init) : Light(color_init, intensity_init)
{
    position = position_init;
}

PointLight::PointLight(const PointLight& pLight)
{
    this->color = pLight.color;
    this->intensity = pLight.intensity;
    this->position = pLight.position;
    this->radius = pLight.radius;
    this->constant = pLight.constant;
    this->linear = pLight.linear;
    this->quadratic = pLight.quadratic;
}

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

int PointLight::GetType()
{
    return 1;
}

LightShaderData PointLight::ConstructShaderData()
{
    LightShaderData shaderData;
    shaderData.type = GetType();
    shaderData.position = position;
    shaderData.color = GetFinalColor();
    shaderData.properties = GetProperties();

    return shaderData;
}


glm::vec4 PointLight::GetProperties()
{
    return glm::vec4(constant, linear, quadratic, radius);
}
