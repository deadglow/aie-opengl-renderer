#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 direction_init, glm::vec3 color_init, float intensity_init) : Light(color_init, intensity_init)
{
    direction = direction_init;
}

DirectionalLight::DirectionalLight(const DirectionalLight& dirLight)
{
    this->color = dirLight.color;
    this->intensity = dirLight.intensity;
    this->direction = dirLight.direction;
}

DirectionalLight::DirectionalLight()
{
    direction = glm::normalize(glm::vec3( 1, -1, -1 ));
    color = { 1.0f, 1.0f, 1.0f};
    intensity = 1.0f;
}

DirectionalLight::~DirectionalLight()
{
}

int DirectionalLight::GetType()
{
    return 0;
}

LightShaderData DirectionalLight::ConstructShaderData()
{
    LightShaderData shaderData;
    shaderData.type = GetType();
    shaderData.direction = direction;
    shaderData.color = GetFinalColor();

    return shaderData;
}
